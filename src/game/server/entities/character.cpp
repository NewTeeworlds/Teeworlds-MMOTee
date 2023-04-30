/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#include <base/math.h>
#include <base/vmath.h>
#include <new>
#include <engine/shared/config.h>
#include <engine/server/mapconverter.h>
#include <game/server/gamecontext.h>
#include <game/mapitems.h>

#include "character.h"
#include "engine/server.h"
#include "projectile.h"
#include "flyingpoint.h"
#include "biologist-laser.h"
#include "biologist-mine.h"
#include "healer-health.h"
#include "bouncing-bullet.h"

#include "snapfullproj.h"
#include "snapfullpickup.h"
#include "hero-flag.h"
#include "slug-slime.h"
#include "items/bonus.h"
#include "items/dropitem.h"
#include "sword/sword.h"
#include "buffs/buff.h"
#include "items/pickup.h"
#include "sword/pizdamet.h"

//input count
struct CInputCount
{
	int m_Presses;
	int m_Releases;
};

CInputCount CountInput(int Prev, int Cur)
{
	CInputCount c = {0, 0};
	Prev &= INPUT_STATE_MASK;
	Cur &= INPUT_STATE_MASK;
	int i = Prev;

	while(i != Cur)
	{
		i = (i+1)&INPUT_STATE_MASK;
		if(i&1)
			c.m_Presses++;
		else
			c.m_Releases++;
	}

	return c;
}


MACRO_ALLOC_POOL_ID_IMPL(CCharacter, MAX_CLIENTS)

// Character, "physical" player's part
CCharacter::CCharacter(CGameWorld *pWorld)
: CEntity(pWorld, CGameWorld::ENTTYPE_CHARACTER)
{
	m_ProximityRadius = ms_PhysSize;
	m_Health = 0;
	m_Armor = 0;
	
	m_AirJumpCounter = 0;
	m_HeartID = Server()->SnapNewID();
	m_AntiFireTick = 0;
	m_IsFrozen = false;
	m_FrozenTime = -1;
	m_DartLifeSpan = -1;
	m_IsInvisible = false;
	m_InvisibleTick = 0;
	m_PositionLockTick = -Server()->TickSpeed()*10;
	m_PositionLocked = false;
	m_PositionLockAvailable = false;
	m_PoisonTick = 0;
	m_HealTick = 0;
	m_InAirTick = 0;
	m_InWater = 0;
	m_BonusTick = 0;
	m_WaterJumpLifeSpan = 0;
	m_NinjaVelocityBuff = 0;
	m_NinjaStrengthBuff = 0;
	m_NinjaAmmoBuff = 0;
}

bool CCharacter::FindPortalPosition(vec2 Pos, vec2& Res)
{
	vec2 PortalShift = Pos - m_Pos;
	vec2 PortalDir = normalize(PortalShift);
	if(length(PortalShift) > 500.0f)
		PortalShift = PortalDir * 500.0f;
	
	float Iterator = length(PortalShift);
	while(Iterator > 0.0f)
	{
		PortalShift = PortalDir * Iterator;
		vec2 PortalPos = m_Pos + PortalShift;
	
		Res = PortalPos;
		return true;
		
		Iterator -= 4.0f;
	}
	
	return false;
}

void CCharacter::Reset()
{	
	Destroy();
}

bool CCharacter::Spawn(CPlayer *pPlayer, vec2 Pos)
{
	m_EmoteStop = -1;
	m_LastAction = -1;
	m_LastNoAmmoSound = -1;
	m_ActiveWeapon = WEAPON_GUN;
	m_LastWeapon = WEAPON_HAMMER;
	m_QueuedWeapon = -1;

	m_pPlayer = pPlayer;
	m_Pos = Pos;
	
	if(m_pPlayer->IsBot())
		LockBotPos = m_Pos;
	
	if(m_pPlayer->AccData.Jail)
	{
		if(m_pPlayer->AccData.IsJailed && m_pPlayer->AccData.JailLength > 0)
		{
			m_pPlayer->m_JailTick = Server()->TickSpeed()*m_pPlayer->AccData.JailLength;
		}
		else
		{
			m_pPlayer->m_JailTick = Server()->TickSpeed()*360;
		}
	}
	
	m_Core.Reset();
	m_Core.Init(&GameServer()->m_World.m_Core, GameServer()->Collision());
	m_Core.m_Pos = m_Pos;
	GameServer()->m_World.m_Core.m_apCharacters[m_pPlayer->GetCID()] = &m_Core;

	m_ReckoningTick = 0;
	mem_zero(&m_SendCore, sizeof(m_SendCore));
	mem_zero(&m_ReckoningCore, sizeof(m_ReckoningCore));

	GameServer()->m_World.InsertEntity(this);
	m_Alive = true;

	GameServer()->m_pController->OnCharacterSpawn(this);

	if(!m_pPlayer->IsBot())
	{
		m_pPlayer->m_TeeInfos.m_UseCustomColor = true;
		GameServer()->m_pController->OnPlayerInfoChange(m_pPlayer);
	}
	m_AntiFireTick = Server()->Tick();
	m_IsFrozen = false;
	m_FrozenTime = -1;
	m_SlipperyTick = -1;
	m_PositionLockTick = -Server()->TickSpeed()*10;
	m_PositionLocked = false;
	m_PositionLockAvailable = false;
	m_InCrafted = false;
	m_InQuest = false;
	InWork = false;
	
	// FIXED BUG sry in Price, Initialized
	if(Server()->GetItemPrice(m_pPlayer->GetCID(), IGUN, 0) <= 0)
	{
		InShop = true;
		GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
		InShop = false;
	}
	GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);

	m_Poison = 0;
	
	ClassSpawnAttributes();
	DestroyChildEntities();
	if(GetClass() == PLAYERCLASS_NONE)
	{
		OpenClassChooser();
	}
	m_pPlayer->m_HealthStart = m_Health;
	m_pPlayer->m_Mana = 0;
	return true;
}

void CCharacter::Destroy()
{	
	DestroyChildEntities();
	if(m_HeartID >= 0)
	{
		Server()->SnapFreeID(m_HeartID);
		m_HeartID = -1;
	}
	GameServer()->m_World.m_Core.m_apCharacters[m_pPlayer->GetCID()] = 0;
	m_Alive = false;
}

void CCharacter::SetWeapon(int W)
{
	if(W == m_ActiveWeapon)
		return;

	m_LastWeapon = m_ActiveWeapon;
	m_QueuedWeapon = -1;
	m_ActiveWeapon = W;
	GameServer()->CreateSound(m_Pos, SOUND_WEAPON_SWITCH);

	if(m_ActiveWeapon < 0 || m_ActiveWeapon >= NUM_WEAPONS)
		m_ActiveWeapon = 0;
}

bool CCharacter::IsGrounded()
{
	if(GameServer()->Collision()->CheckPoint(m_Pos.x+m_ProximityRadius/2, m_Pos.y+m_ProximityRadius/2+5))
		return true;
	if(GameServer()->Collision()->CheckPoint(m_Pos.x-m_ProximityRadius/2, m_Pos.y+m_ProximityRadius/2+5))
		return true;
	return false;
}

void CCharacter::HandleNinja()
{
	if(GetInfWeaponID(m_ActiveWeapon) != INFWEAPON_NINJA)
		return;

	m_DartLifeSpan--;

	if (m_DartLifeSpan == 0)
	{
		// reset velocity
		m_Core.m_Vel = m_DartDir*m_DartOldVelAmount;
	}

	if (m_DartLifeSpan > 0)
	{
		// Set velocity
		float VelocityBuff = min(1.0f + static_cast<float>(m_NinjaVelocityBuff)/10.0f, 2.0f);
		m_Core.m_Vel = m_DartDir * g_pData->m_Weapons.m_Ninja.m_Velocity * VelocityBuff;
		vec2 OldPos = m_Pos;
		GameServer()->Collision()->MoveBox(&m_Core.m_Pos, &m_Core.m_Vel, vec2(m_ProximityRadius, m_ProximityRadius), 0.f);

		// reset velocity so the client doesn't predict stuff
		m_Core.m_Vel = vec2(0.f, 0.f);


		// check if we Hit anything along the way
		{
			CCharacter *aEnts[MAX_CLIENTS];
			vec2 Dir = m_Pos - OldPos;
			float Radius = m_ProximityRadius * 2.0f;
			vec2 Center = OldPos + Dir * 0.5f;
			int Num = GameServer()->m_World.FindEntities(Center, Radius, (CEntity**)aEnts, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);

			for (int i = 0; i < Num; ++i)
			{
				if (aEnts[i] == this)
					continue;

				// make sure we haven't Hit this object before
				bool bAlreadyHit = false;
				for (int j = 0; j < m_NumObjectsHit; j++)
				{
					if (m_apHitObjects[j] == aEnts[i])
						bAlreadyHit = true;
				}
				if (bAlreadyHit)
					continue;

				// check so we are sufficiently close
				if (distance(aEnts[i]->m_Pos, m_Pos) > (m_ProximityRadius * 2.0f))
					continue;

				// Hit a player, give him damage and stuffs...
				GameServer()->CreateSound(aEnts[i]->m_Pos, SOUND_NINJA_HIT);
				// set his velocity to fast upward (for now)
				if(m_NumObjectsHit < 10)
					m_apHitObjects[m_NumObjectsHit++] = aEnts[i];

				aEnts[i]->TakeDamage(vec2(0, -10.0f), min(g_pData->m_Weapons.m_Ninja.m_pBase->m_Damage + m_NinjaStrengthBuff, 20), m_pPlayer->GetCID(), WEAPON_NINJA, TAKEDAMAGEMODE_NOINFECTION);
			}
		}
	}
}


void CCharacter::DoWeaponSwitch()
{
/* INFECTION MODIFICATION START ***************************************/
	if(m_ReloadTimer != 0 || m_QueuedWeapon == -1)
		return;
/* INFECTION MODIFICATION END *****************************************/

	// switch Weapon
	SetWeapon(m_QueuedWeapon);
}

void CCharacter::HandleWeaponSwitch()
{
	// select Weapon
	int Next = CountInput(m_LatestPrevInput.m_NextWeapon, m_LatestInput.m_NextWeapon).m_Presses;
	int Prev = CountInput(m_LatestPrevInput.m_PrevWeapon, m_LatestInput.m_PrevWeapon).m_Presses;

	int WantedWeapon = m_ActiveWeapon;
	if(m_QueuedWeapon != -1)
		WantedWeapon = m_QueuedWeapon;
	
	if(Next < 128) // make sure we only try sane stuff
	{
		while(Next) // Next Weapon selection
		{
			WantedWeapon = (WantedWeapon+1)%NUM_WEAPONS;
			if(m_aWeapons[WantedWeapon].m_Got)
				Next--;
		}
	}

	if(Prev < 128) // make sure we only try sane stuff
	{
		while(Prev) // Prev Weapon selection
		{
			WantedWeapon = (WantedWeapon-1)<0?NUM_WEAPONS-1:WantedWeapon-1;
			if(m_aWeapons[WantedWeapon].m_Got)
				Prev--;
		}
	}

	// Direct Weapon selection
	if(m_LatestInput.m_WantedWeapon)
		WantedWeapon = m_Input.m_WantedWeapon-1;

	// check for insane values
	if(WantedWeapon >= 0 && WantedWeapon < NUM_WEAPONS && WantedWeapon != m_ActiveWeapon && m_aWeapons[WantedWeapon].m_Got)
		m_QueuedWeapon = WantedWeapon;

	DoWeaponSwitch();
}

void CCharacter::UpdateTuningParam()
{
	CTuningParams* pTuningParams = &m_pPlayer->m_NextTuningParams;
	
	bool NoActions = false;
	bool FixedPosition = false;

	if(m_PositionLocked)
	{
		NoActions = true;
		FixedPosition = true;
	}
	
	if(m_IsFrozen)
		NoActions = true;
	
	if(m_HookMode == 1)
	{
		pTuningParams->m_HookDragSpeed = 0.0f;
		pTuningParams->m_HookDragAccel = 1.0f;
	}
	if(m_InWater == 1)
	{
		pTuningParams->m_Gravity = -0.05f;
		pTuningParams->m_GroundFriction = 0.95f;
		pTuningParams->m_GroundControlSpeed = 250.0f / Server()->TickSpeed();
		pTuningParams->m_GroundControlAccel = 1.5f;
		//pTuningParams->m_GroundJumpImpulse = 0.0f;
		pTuningParams->m_AirFriction = 0.95f;
		pTuningParams->m_AirControlSpeed = 250.0f / Server()->TickSpeed();
		pTuningParams->m_AirControlAccel = 1.5f;
		//pTuningParams->m_AirJumpImpulse = 0.0f;
	}
	if(m_SlipperyTick > 0)
	{
		pTuningParams->m_GroundFriction = 1.0f;
	}
	
	if(NoActions)
	{
		pTuningParams->m_GroundControlAccel = 0.0f;
		pTuningParams->m_GroundJumpImpulse = 0.0f;
		pTuningParams->m_AirJumpImpulse = 0.0f;
		pTuningParams->m_AirControlAccel = 0.0f;
		pTuningParams->m_HookLength = 0.0f;
	}
	if(FixedPosition)
	{
		pTuningParams->m_Gravity = 0.0f;
	}
	
	if(Server()->GetItemSettings(m_pPlayer->GetCID(), JUMPIMPULS))
	{
		pTuningParams->m_GroundJumpImpulse = 20.0f;
	}

	if(m_pPlayer->GetBotType() == BOT_L3MONSTER)
	{
		pTuningParams->m_Gravity = 0.0f;
	}
}

void CCharacter::FireWeapon()
{
	//Wait 1 second after spawning
	if(Server()->Tick() - m_AntiFireTick < Server()->TickSpeed())
		return;

	if(m_pPlayer->m_JailTick > 0)
		return;

	if(InShop)
		return;
	
	if(m_ReloadTimer != 0)
		return;
	
	if(IsFrozen())
		return;

	if(!m_pPlayer->IsBot() && GetClass() == PLAYERCLASS_NONE)
		return;

	DoWeaponSwitch();
	vec2 Direction = normalize(vec2(m_LatestInput.m_TargetX, m_LatestInput.m_TargetY));

	bool FullAuto = false;
	if(m_ActiveWeapon == WEAPON_GRENADE || m_ActiveWeapon == WEAPON_SHOTGUN || m_ActiveWeapon == WEAPON_RIFLE)
		FullAuto = true;

	if((m_ActiveWeapon == WEAPON_HAMMER && Server()->GetItemSettings(m_pPlayer->GetCID(), HAMMERAUTO)) || 
		(m_ActiveWeapon == WEAPON_GUN && Server()->GetItemSettings(m_pPlayer->GetCID(), GUNAUTO)))
		FullAuto = true;
	
	// check if we gonna fire
	bool WillFire = false;
	if(CountInput(m_LatestPrevInput.m_Fire, m_LatestInput.m_Fire).m_Presses)
		WillFire = true;
	
	if(FullAuto && (m_LatestInput.m_Fire&1) && m_aWeapons[m_ActiveWeapon].m_Ammo)
		WillFire = true;
	// 不检查锤子的弹药
	if(FullAuto && (m_LatestInput.m_Fire&1) && m_ActiveWeapon == WEAPON_HAMMER)
		WillFire = true;

	if(!WillFire)
		return;

	// check for ammo
	if(!m_aWeapons[m_ActiveWeapon].m_Ammo && m_ActiveWeapon != WEAPON_HAMMER)
	{
		// 125ms is a magical limit of how fast a human can click
		m_ReloadTimer = 125 * Server()->TickSpeed() / 1000;
		if(m_LastNoAmmoSound+Server()->TickSpeed() <= Server()->Tick())
		{
			GameServer()->CreateSound(m_Pos, SOUND_WEAPON_NOAMMO);
			m_LastNoAmmoSound = Server()->Tick();
		}
		return;
	}

	vec2 ProjStartPos = m_Pos+Direction*m_ProximityRadius*0.75f;

	switch(m_ActiveWeapon)
	{
		case WEAPON_HAMMER:
		{
			for(auto *pPick = (CPickup*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_PICKUP); pPick; pPick = (CPickup*) pPick->TypeNext())
			{
				if(distance(pPick->m_Pos, m_Pos) < 30.0f && pPick->m_SpawnTick == -1)
				{
					pPick->StartFarm(m_pPlayer->GetCID());
					m_ReloadTimer = Server()->TickSpeed();
					return;
				}
			}

			// ---------- 检查玩家职业是 Berserk(狂战士) 还是 Assasins(刺客)
			int Range = 0;
			if(m_pPlayer->AccData.Class == PLAYERCLASS_BERSERK)	Range = m_pPlayer->AccUpgrade.HammerRange*20;
			else if(m_pPlayer->AccData.Class == PLAYERCLASS_ASSASINS) Range = 100;
		
			// reset objects Hit
			m_NumObjectsHit = 0;
			GameServer()->CreateSound(m_Pos, SOUND_HAMMER_FIRE);
			
			CCharacter *apEnts[MAX_CLIENTS];
			int Hits = 0;
			
			if(Server()->GetItemSettings(m_pPlayer->GetCID(), LAMPHAMMER))
			{
				int Num = GameServer()->m_World.FindEntities(ProjStartPos, m_ProximityRadius*10.0f+Range, (CEntity**)apEnts, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
				for (int i = 0; i < Num; ++i)
				{
					CCharacter *pTarget = apEnts[i];
					if ((pTarget == this) || GameServer()->Collision()->IntersectLine(ProjStartPos, pTarget->m_Pos, NULL, NULL))
						continue;

					new CFlyingPoint(GameWorld(), m_Pos, pTarget->GetPlayer()->GetCID(), m_pPlayer->GetCID(), m_Core.m_Vel);
				}
			}

			int Num = GameServer()->m_World.FindEntities(ProjStartPos, m_ProximityRadius*2.0f+Range, (CEntity**)apEnts, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
			for (int i = 0; i < Num; ++i)
			{
				CCharacter *pTarget = apEnts[i];

				if ((pTarget == this) || GameServer()->Collision()->IntersectLine(ProjStartPos, pTarget->m_Pos, NULL, NULL))
					continue;

				// set his velocity to fast upward (for now)
				if(length(pTarget->m_Pos-ProjStartPos) > 0.0f)
					GameServer()->CreateHammerHit(pTarget->m_Pos-normalize(pTarget->m_Pos-ProjStartPos)*m_ProximityRadius*0.5f);
				else
					GameServer()->CreateHammerHit(ProjStartPos);

				vec2 Dir;
				if (length(pTarget->m_Pos - m_Pos) > 0.0f)
					Dir = normalize(pTarget->m_Pos - m_Pos);
				else
					Dir = vec2(0.f, -1.f);

				pTarget->TakeDamage(vec2(0.f, -1.f) + normalize(Dir + vec2(0.f, -1.1f)) * 10.0f, m_pPlayer->m_InArea ? 0 : g_pData->m_Weapons.m_Hammer.m_pBase->m_Damage,
					m_pPlayer->GetCID(), m_ActiveWeapon, 0);
				Hits++;
			}

			if(!Server()->GetItemSettings(m_pPlayer->GetCID(), SDROP))
				TakeItemChar(m_pPlayer->GetCID());

			if(Hits)
				m_ReloadTimer = Server()->TickSpeed()/3;

		} break;

		case WEAPON_GUN:
		{
			bool Explode = Server()->GetItemSettings(m_pPlayer->GetCID(), EXGUN) != 0;
			
			if(Server()->GetItemSettings(m_pPlayer->GetCID(), GUNBOUNCE))
				new CBouncingBullet(GameWorld(), m_pPlayer->GetCID(), ProjStartPos, Direction, Explode, WEAPON_GUN, 80);
			else
				new CProjectile(GameWorld(), WEAPON_GUN,
					m_pPlayer->GetCID(),
					ProjStartPos,
					Direction,
					(int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GunLifetime),
					g_pData->m_Weapons.m_Gun.m_pBase->m_Damage, Explode, 10, -1, WEAPON_GUN);

			GameServer()->CreateSound(m_Pos, SOUND_GUN_FIRE);
		} break;

		case WEAPON_SHOTGUN: 
		{
			bool Explode = Server()->GetItemSettings(m_pPlayer->GetCID(), EXSHOTGUN) != 0;
			
			int ShotSpread = 5 + m_pPlayer->AccUpgrade.Spray;
			if(ShotSpread > 36)
				ShotSpread = 36;

			if(m_pPlayer->GetBotType() == BOT_BOSSSLIME)
				ShotSpread = 40;

			float Spreading[20 * 2 + 1];
			for (int i = 0; i < 20 * 2 + 1; i++)
				Spreading[i] = -1.2f + 0.06f * i;

			CMsgPacker Msg(NETMSGTYPE_SV_EXTRAPROJECTILE);
			Msg.AddInt(ShotSpread / 2 * 2 + 1);
			

			for (int i = -ShotSpread / 2; i <= ShotSpread / 2; ++i)
			{
				float a = GetAngle(Direction);
				a += Spreading[i + 20];
				float v = 1 - (absolute(i) / (float)ShotSpread) / 2;
				float Speed = m_pPlayer->AccUpgrade.Spray > 0 ? 1.0f : mix((float)GameServer()->Tuning()->m_ShotgunSpeeddiff, 1.2f, v);
				
				if(Server()->GetItemSettings(m_pPlayer->GetCID(), HYBRIDSG))
					new CBouncingBullet(GameWorld(), m_pPlayer->GetCID(), ProjStartPos, vec2(cosf(a), sinf(a))*Speed, Explode, WEAPON_GUN, 12);

				if(m_pPlayer->GetBotType() == BOT_BOSSSLIME || Server()->GetItemSettings(m_pPlayer->GetCID(), MODULESHOTGUNSLIME))
					new CBouncingBullet(GameWorld(), m_pPlayer->GetCID(), ProjStartPos, vec2(cosf(a), sinf(a))*Speed, Explode, WEAPON_SHOTGUN, 16);
				else
				{
					new CProjectile(GameWorld(), WEAPON_SHOTGUN, 
					m_pPlayer->GetCID(), 
					ProjStartPos, 
					vec2(cosf(a), sinf(a))*Speed, 
					(int)(Server()->TickSpeed()*GameServer()->Tuning()->m_ShotgunLifetime*2), 20, Explode, 10, -1, WEAPON_SHOTGUN);
				}
			}
			Server()->SendMsg(&Msg, 0, m_pPlayer->GetCID());
			GameServer()->CreateSound(m_Pos, SOUND_SHOTGUN_FIRE);
		} break;

		case WEAPON_GRENADE:
		{
			if(Server()->GetItemSettings(m_pPlayer->GetCID(), PIZDAMET))
			{
				if(random_prob(0.5f))
					m_pPlayer->m_Mana--;

				m_aWeapons[m_ActiveWeapon].m_Ammo++;
				new CPizdamet(GameWorld(), m_Pos, m_pPlayer->GetCID());
			}
			else
			{
				int ShotSpread = 2 + m_pPlayer->AccUpgrade.Spray/3;
				if(ShotSpread > 10)
					ShotSpread = 10;

				if(m_pPlayer->GetBotType() == BOT_BOSSSLIME)
					ShotSpread = 15;

	
				float Spreading[20 * 2 + 1];
				for (int i = 0; i < 20 * 2 + 1; i++)
					Spreading[i] = -1.2f + 0.06f * i;

				CMsgPacker Msg(NETMSGTYPE_SV_EXTRAPROJECTILE);
				Msg.AddInt(ShotSpread * 2 + 1);

				for (int i = 1; i < ShotSpread; ++i)
				{
					float a = GetAngle(Direction);
					a += Spreading[i + 20-ShotSpread/2];
					float v = 1 - (absolute(i) / (float)ShotSpread) / 20;
					float Speed = mix((float)GameServer()->Tuning()->m_ShotgunSpeeddiff, 1.2f, v);
					
					if(Server()->GetItemSettings(m_pPlayer->GetCID(), GRENADEBOUNCE))
						new CBouncingBullet(GameWorld(), m_pPlayer->GetCID(), ProjStartPos, vec2(cosf(a), sinf(a))*Speed, true, WEAPON_GRENADE, 100);
					else
						new CProjectile(GameWorld(), WEAPON_GRENADE, 
						m_pPlayer->GetCID(), 
						ProjStartPos, 
						vec2(cosf(a), sinf(a))*Speed, 
						(int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GrenadeLifetime), 
						g_pData->m_Weapons.m_Grenade.m_pBase->m_Damage, true, 0, SOUND_GRENADE_EXPLODE, WEAPON_GRENADE);
				}
				Server()->SendMsg(&Msg, 0, m_pPlayer->GetCID());
			}
			GameServer()->CreateSound(m_Pos, SOUND_GRENADE_FIRE);
		} break;

		case WEAPON_RIFLE:
		{
			bool Explode = Server()->GetItemSettings(m_pPlayer->GetCID(), EXLASER) != 0;
						
			int ShotSpread = m_pPlayer->m_InArea ? 2 : 2 + m_pPlayer->AccUpgrade.Spray/3;
			if(ShotSpread > 10)
				ShotSpread = 10;

			CMsgPacker Msg(NETMSGTYPE_SV_EXTRAPROJECTILE);
			Msg.AddInt(ShotSpread / 2 * 2 + 1);

			float Spreading[20 * 2 + 1];
			for (int i = 0; i < 20 * 2 + 1; i++)
				Spreading[i] = -1.2f + 0.06f * i;

			for (int i = 1; i < ShotSpread; ++i)
			{
				float a = GetAngle(Direction);
				a += Spreading[i + 20-ShotSpread/2];
				float v = 1 - (absolute(i) / (float)ShotSpread) / 20;
				float Speed = mix((float)GameServer()->Tuning()->m_ShotgunSpeeddiff, 1.2f, v);
				
				new CBiologistLaser(GameWorld(), m_Pos, vec2(cosf(a), sinf(a))*Speed, m_pPlayer->GetCID(), 3, Explode);
			}
			Server()->SendMsg(&Msg, 0, m_pPlayer->GetCID());
			GameServer()->CreateSound(m_Pos, SOUND_RIFLE_FIRE);
		} break;

		case WEAPON_NINJA:
		{
			m_NumObjectsHit = 0;
			GameServer()->CreateSound(m_Pos, SOUND_NINJA_FIRE);
		} break;

	}

	m_AttackTick = Server()->Tick();
	if(m_aWeapons[m_ActiveWeapon].m_Ammo > 0) // -1 == unlimited
		m_aWeapons[m_ActiveWeapon].m_Ammo--;

	if(!m_ReloadTimer)
	{
		// 125ms is a magical limit of how fast a human can click
		int InfWID = GetInfWeaponID(m_ActiveWeapon);
		int ReloadTime = Server()->GetFireDelay(m_pPlayer->GetCID(), InfWID);
			
		m_ReloadTimer = g_pData->m_Weapons.m_aId[m_ActiveWeapon].m_Firedelay * Server()->TickSpeed() / ReloadTime;
	}
}

void CCharacter::SaturateVelocity(vec2 Force, float MaxSpeed)
{
	if(length(Force) < 0.00001)
		return;
	
	float Speed = length(m_Core.m_Vel);
	vec2 VelDir = normalize(m_Core.m_Vel);
	if(Speed < 0.00001)
	{
		VelDir = normalize(Force);
	}
	vec2 OrthoVelDir = vec2(-VelDir.y, VelDir.x);
	float VelDirFactor = dot(Force, VelDir);
	float OrthoVelDirFactor = dot(Force, OrthoVelDir);
	
	vec2 NewVel = m_Core.m_Vel;
	if(Speed < MaxSpeed || VelDirFactor < 0.0f)
	{
		NewVel += VelDir*VelDirFactor;
		float NewSpeed = length(NewVel);
		if(NewSpeed > MaxSpeed)
		{
			if(VelDirFactor > 0.f)
				NewVel = VelDir*MaxSpeed;
			else
				NewVel = -VelDir*MaxSpeed;
		}
	}
	
	NewVel += OrthoVelDir * OrthoVelDirFactor;
	
	m_Core.m_Vel = NewVel;
}

void CCharacter::HandleWeapons()
{
	if(IsFrozen())
		return;
		
	//ninja
	HandleNinja();

	// check reload timer
	if(m_ReloadTimer)
	{
		m_ReloadTimer--;
		return;
	}

	// fire Weapon, if wanted
	FireWeapon();

	// ammo regen

	for(int i=WEAPON_GUN; i<=WEAPON_RIFLE; i++)
	{
		int InfWID = GetInfWeaponID(i);
		int AmmoRegenTime = Server()->GetAmmoRegenTime(m_pPlayer->GetCID(), InfWID);
		int MaxAmmo = Server()->GetMaxAmmo(m_pPlayer->GetCID(), GetInfWeaponID(i));
		
		if(AmmoRegenTime)
		{
			if(m_ReloadTimer <= 0)
			{
				if (m_aWeapons[i].m_AmmoRegenStart < 0)
					m_aWeapons[i].m_AmmoRegenStart = Server()->Tick();

				if ((Server()->Tick() - m_aWeapons[i].m_AmmoRegenStart) >= AmmoRegenTime * Server()->TickSpeed() / 1000)
				{
					// Add some ammo
					m_aWeapons[i].m_Ammo = min(m_aWeapons[i].m_Ammo + 1, MaxAmmo);
					m_aWeapons[i].m_AmmoRegenStart = -1;
				}
			}
		}
	}
	return;
}

void CCharacter::RemoveGun(int WEAPONID)
{
	m_aWeapons[WEAPONID].m_Got = false;
	m_aWeapons[WEAPONID].m_Ammo = 0;
	m_ActiveWeapon = WEAPON_HAMMER;
}

void CCharacter::RemoveAllGun()
{
	m_aWeapons[WEAPON_GUN].m_Got = false;
	m_aWeapons[WEAPON_GUN].m_Ammo = 0;
	m_aWeapons[WEAPON_RIFLE].m_Got = false;
	m_aWeapons[WEAPON_RIFLE].m_Ammo = 0;
	m_aWeapons[WEAPON_GRENADE].m_Got = false;
	m_aWeapons[WEAPON_GRENADE].m_Ammo = 0;
	m_aWeapons[WEAPON_SHOTGUN].m_Got = false;
	m_aWeapons[WEAPON_SHOTGUN].m_Ammo = 0;
}

bool CCharacter::GiveWeapon(int Weapon, int Ammo, bool GetAmmo)
{
	int InfWID = GetInfWeaponID(Weapon);
	int MaxAmmo = Server()->GetMaxAmmo(m_pPlayer->GetCID(), InfWID);
	
	if(Ammo < 0)
		Ammo = MaxAmmo;
	
	if(m_aWeapons[Weapon].m_Ammo < MaxAmmo || !m_aWeapons[Weapon].m_Got)
	{
		m_aWeapons[Weapon].m_Got = true;
		if(GetAmmo)
			m_aWeapons[Weapon].m_Ammo += min(MaxAmmo, Ammo);
		else
			m_aWeapons[Weapon].m_Ammo = min(MaxAmmo, Ammo);
		return true;
	}
	return false;
}

void CCharacter::SetEmote(int Emote, int Tick)
{
	m_EmoteType = Emote;
	m_EmoteStop = Tick;
}

void CCharacter::OnPredictedInput(CNetObj_PlayerInput *pNewInput)
{
	// check for changes
	if(mem_comp(&m_Input, pNewInput, sizeof(CNetObj_PlayerInput)) != 0)
		m_LastAction = Server()->Tick();

	// copy new input
	mem_copy(&m_Input, pNewInput, sizeof(m_Input));
	m_NumInputs++;

	// it is not allowed to aim in the center
	if(m_Input.m_TargetX == 0 && m_Input.m_TargetY == 0)
		m_Input.m_TargetY = -1;
}

void CCharacter::OnDirectInput(CNetObj_PlayerInput *pNewInput)
{
	mem_copy(&m_LatestPrevInput, &m_LatestInput, sizeof(m_LatestInput));
	mem_copy(&m_LatestInput, pNewInput, sizeof(m_LatestInput));

	// it is not allowed to aim in the center
	if(m_LatestInput.m_TargetX == 0 && m_LatestInput.m_TargetY == 0)
		m_LatestInput.m_TargetY = -1;

	if(m_NumInputs > 2 && m_pPlayer->GetTeam() != TEAM_SPECTATORS)
	{
		HandleWeaponSwitch();
		FireWeapon();
	}

	mem_copy(&m_LatestPrevInput, &m_LatestInput, sizeof(m_LatestInput));
} 

void CCharacter::ResetInput()
{
	m_Input.m_Direction = 0;
	m_Input.m_Hook = 0;
	// simulate releasing the fire button
	if((m_Input.m_Fire&1) != 0)
		m_Input.m_Fire++;
	m_Input.m_Fire &= INPUT_STATE_MASK;
	m_Input.m_Jump = 0;
	m_LatestPrevInput = m_LatestInput = m_Input;
}

void CCharacter::Tick()
{
	vec2 PrevPos = m_Core.m_Pos;
	if(IsAlive())
	{	
		if(m_ReloadOther)
			m_ReloadOther--;

		// 通过计数初始化boss的生命值
		// 计算方法：玩家等级之和*500
		if(m_pPlayer->GetBotType() == BOT_BOSSSLIME && !GameServer()->m_BossStart)
		{
			if(!g_Config.m_SvCityStart) m_Health = 10+GameServer()->GetBossLeveling()*500;
			else if(g_Config.m_SvCityStart == 1) m_Health = 10+GameServer()->GetBossLeveling()*1000;

			m_pPlayer->m_HealthStart = m_Health;
		}
		
		// 生命值恢复
		if(m_pPlayer->AccUpgrade.HPRegen && m_pPlayer->m_Health < m_pPlayer->m_HealthStart)
		{
			if(!HPRegenTick) HPRegenTick = 900-m_pPlayer->AccUpgrade.HPRegen*3;
			else
			{
				HPRegenTick--;
				if(HPRegenTick == 1) m_Health += 50;
			}
		}

		if((m_pPlayer->m_AngryWroth > 120 || Server()->GetItemCount(m_pPlayer->GetCID(), SPECSNAPDRAW)) && Server()->Tick() % (1 * Server()->TickSpeed()) == 0)
			GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());

		m_pPlayer->m_Health = m_Health;
		
		int PlayerPos = GameServer()->Collision()->GetZoneValueAt(GameServer()->m_ZoneHandle_Bonus, m_Pos.x, m_Pos.y);
		switch (PlayerPos)
		{
		// 公会大门
		case ZONE_INCLAN1:
			if(!Server()->GetTopHouse(0))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间房屋还没有公会入驻,暂不开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}

			if(!Server()->GetOpenHouse(0) && Server()->GetClanID(m_pPlayer->GetCID()) != Server()->GetTopHouse(0))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间公会房不对外开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}
			break;
		case ZONE_INCLAN2:
			if(!Server()->GetTopHouse(1))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间房屋还没有公会入驻,暂不开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}

			if(!Server()->GetOpenHouse(1) && Server()->GetClanID(m_pPlayer->GetCID()) != Server()->GetTopHouse(1))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间公会房不对外开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}
			break;
		case ZONE_INCLAN3:
			if(!Server()->GetTopHouse(2))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间房屋还没有公会入驻,暂不开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}

			if(!Server()->GetOpenHouse(2) && Server()->GetClanID(m_pPlayer->GetCID()) != Server()->GetTopHouse(1))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间公会房不对外开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}
			break;
		case ZONE_INCLAN4:
			if(!Server()->GetTopHouse(3))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间房屋还没有公会入驻,暂不开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}

			if(!Server()->GetOpenHouse(3) && Server()->GetClanID(m_pPlayer->GetCID()) != Server()->GetTopHouse(1))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间公会房不对外开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}
			break;
		case ZONE_INCLAN5:
			if(!Server()->GetTopHouse(4))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间房屋还没有公会入驻,暂不开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}

			if(!Server()->GetOpenHouse(4) && Server()->GetClanID(m_pPlayer->GetCID()) != Server()->GetTopHouse(1))
			{
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), -1, _("这间公会房不对外开放"), NULL);
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);	
			}
			break;
		// 公会座椅
		case ZONE_CHAIRCLAN1:
			if(!m_ReloadOther)
			{
				m_ReloadOther = Server()->TickSpeed();

				int Exp = 20+Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(0));
				int Money = 500+(Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(0))*50);

				unsigned long int LegalExp = m_pPlayer->AccData.Exp + Exp;
				int LegalMoney = m_pPlayer->AccData.Money + Money;

				m_pPlayer->AccData.Exp += Exp;
				m_pPlayer->AccData.Money += Money;

				GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);

				if(m_pPlayer->AccData.Exp > LegalExp || m_pPlayer->AccData.Money > LegalMoney)
				{
					Server()->Kick(m_pPlayer->GetCID(), "You pidor");
					return;
				}
			}
			break;
		case ZONE_CHAIRCLAN2:
			if(!m_ReloadOther)
			{
				m_ReloadOther = Server()->TickSpeed();

				int Exp = 20+Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(1));
				int Money = 500+(Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(1))*50);

				unsigned long int LegalExp = m_pPlayer->AccData.Exp + Exp;
				int LegalMoney = m_pPlayer->AccData.Money + Money;

				m_pPlayer->AccData.Exp += Exp;
				m_pPlayer->AccData.Money += Money;

				GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);

				if(m_pPlayer->AccData.Exp > LegalExp || m_pPlayer->AccData.Money > LegalMoney)
				{
					Server()->Kick(m_pPlayer->GetCID(), "You pidor");
					return;
				}
			}
			break;
		case ZONE_CHAIRCLAN3:
			if(!m_ReloadOther)
			{
				m_ReloadOther = Server()->TickSpeed();

				int Exp = 20+Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(2));
				int Money = 500+(Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(2))*50);

				unsigned long int LegalExp = m_pPlayer->AccData.Exp + Exp;
				int LegalMoney = m_pPlayer->AccData.Money + Money;

				m_pPlayer->AccData.Exp += Exp;
				m_pPlayer->AccData.Money += Money;

				GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);

				if(m_pPlayer->AccData.Exp > LegalExp || m_pPlayer->AccData.Money > LegalMoney)
				{
					Server()->Kick(m_pPlayer->GetCID(), "You pidor");
					return;
				}
			}
			break;
		case ZONE_CHAIRCLAN4:
			if(!m_ReloadOther)
			{
				m_ReloadOther = Server()->TickSpeed();

				int Exp = 20+Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(3));
				int Money = 500+(Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(3))*50);

				unsigned long int LegalExp = m_pPlayer->AccData.Exp + Exp;
				int LegalMoney = m_pPlayer->AccData.Money + Money;

				m_pPlayer->AccData.Exp += Exp;
				m_pPlayer->AccData.Money += Money;

				GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);

				if(m_pPlayer->AccData.Exp > LegalExp || m_pPlayer->AccData.Money > LegalMoney)
				{
					Server()->Kick(m_pPlayer->GetCID(), "You pidor");
					return;
				}
			}
			break;
		case ZONE_CHAIRCLAN5:
			if(!m_ReloadOther)
			{
				m_ReloadOther = Server()->TickSpeed();

				int Exp = 20+Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(4));
				int Money = 500+(Server()->GetClan(Clan::ChairLevel, Server()->GetTopHouse(4))*50);

				unsigned long int LegalExp = m_pPlayer->AccData.Exp + Exp;
				int LegalMoney = m_pPlayer->AccData.Money + Money;

				m_pPlayer->AccData.Exp += Exp;
				m_pPlayer->AccData.Money += Money;

				GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);

				if(m_pPlayer->AccData.Exp > LegalExp || m_pPlayer->AccData.Money > LegalMoney)
				{
					Server()->Kick(m_pPlayer->GetCID(), "You pidor");
					return;
				}
			}
			break;
		// 普通座椅
		case ZONE_SEAT1:
			m_pPlayer->m_ActiveChair = true;
			if(!m_ReloadOther)
			{
				m_ReloadOther = Server()->TickSpeed();

				int Exp = 20;
				int Money = 600;

				unsigned long int LegalExp = m_pPlayer->AccData.Exp + Exp;
				int LegalMoney = m_pPlayer->AccData.Money + Money;

				if(g_Config.m_SvCityStart == 1)
				{
					m_pPlayer->AccData.Exp += Exp;
					m_pPlayer->AccData.Money += Money;
					GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);
				}
				else
				{
					Exp = 10;
					Money = 200;

					LegalExp = m_pPlayer->AccData.Exp + Exp;
					LegalMoney = m_pPlayer->AccData.Money + Money;

					m_pPlayer->AccData.Exp += Exp;
					m_pPlayer->AccData.Money += Money;
					GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);
				}

				if(m_pPlayer->AccData.Exp > LegalExp || m_pPlayer->AccData.Money > LegalMoney)
				{
					Server()->Kick(m_pPlayer->GetCID(), "You pidor");
					return;
				}
			}
			break;
		case ZONE_SEAT2:
			m_pPlayer->m_ActiveChair = true;
			if(!m_ReloadOther)
			{
				m_ReloadOther = Server()->TickSpeed();

				int Exp = 30;
				int Money = 800;

				unsigned long int LegalExp = m_pPlayer->AccData.Exp + Exp;
				int LegalMoney = m_pPlayer->AccData.Money + Money;
			
				if(g_Config.m_SvCityStart == 1)
				{
					m_pPlayer->AccData.Exp += Exp;
					m_pPlayer->AccData.Money += Money;					
					GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);
				}
				else
				{
					Exp = 15; // 白房间的座位
					Money = 400;

					LegalExp = m_pPlayer->AccData.Exp + Exp;
					LegalMoney = m_pPlayer->AccData.Money + Money;

					m_pPlayer->AccData.Exp += Exp;
					m_pPlayer->AccData.Money += Money;
					GameServer()->SendBroadcast_LChair(m_pPlayer->GetCID(), Exp, Money);
				}

				if(m_pPlayer->AccData.Exp > LegalExp || m_pPlayer->AccData.Money > LegalMoney)
				{
					Server()->Kick(m_pPlayer->GetCID(), "You pidor");
					return;
				}
			}
			break;
		case ZONE_BOSS:
			GameServer()->EnterBoss(m_pPlayer->GetCID(), BOT_BOSSSLIME);
			break;
		case ZONE_GAMEROOM:
			GameServer()->EnterArea(m_pPlayer->GetCID());
			break;
		case ZONE_WHITEROOM:
			if(!Server()->GetItemCount(m_pPlayer->GetCID(), WHITETICKET))
			{
				Die(m_pPlayer->GetCID(), WEAPON_WORLD);
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), 200, ("你没有进入白房间的门票，请前往商店购买门票!"), NULL);
			}
			else
			{
				GameServer()->SendBroadcast_Localization(m_pPlayer->GetCID(), 200, 100, _("欢迎来到白房间."), NULL);
			}
			break;
		case ZONE_DEATH:
			Die(m_pPlayer->GetCID(), WEAPON_WORLD);
			break;
		default:
			if(m_pPlayer->m_ActiveChair)
			{
				GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 106, 20, -1);
				m_pPlayer->m_ActiveChair = false;
			}
			// ------------------- 功能区 & 商店
			if (PlayerPos == ZONE_SHOP && !InShop)
			{
				InShop = true;
				GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 100, INSHOP);
				GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
				break;
			}
			else if (PlayerPos != ZONE_SHOP && InShop)
			{
				InShop = false;
				GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 50, EXITSHOP);
				GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
				break;
			}
			if (PlayerPos == ZONE_CRAFT && !m_InCrafted)
			{
				m_InCrafted = true;
				GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 50, INCRAFT);
				GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
				break;
			}
			else if (PlayerPos != ZONE_CRAFT && m_InCrafted)
			{
				m_InCrafted = false;
				GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 50, EXITSHOP);
				GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
				break;
			}
			if (PlayerPos == ZONE_QUESTROOM && !m_InQuest)
			{
				m_InQuest = true;
				GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 50, INQUEST);
				GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
				break;
			}
			else if (PlayerPos != ZONE_QUESTROOM && m_InQuest)
			{
				m_InQuest = false;
				GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 50, EXITSHOP);
				GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
				break;
			}

			if (PlayerPos == ZONE_WATER && !m_InWater)
			{
				GameServer()->CreateSound(m_Pos, 11);
				GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());
				m_InWater = true;
				break;
			}
			else if (PlayerPos != ZONE_WATER && m_InWater)
			{
				GameServer()->CreateSound(m_Pos, 11);
				GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());
				m_InWater = false;
				break;
			}
			break;
		}

		// ------------------- PvP 区域伤害开关
		if(PlayerPos == ZONE_ANTIPVP && !m_AntiPVP) {
			m_AntiPVP = true;
			GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 100, INANTIPVP);
		}
		
		if(PlayerPos == ZONE_PVP && m_AntiPVP){
			m_AntiPVP = false;
			GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 101, 50, EXITANTIPVP);
		}
		// 防止机器人(Pig, Kwah, Boomer等怪物)进入 non-PvP 区域
		if(PlayerPos == ZONE_PVP && m_pPlayer->IsBot() && m_pPlayer->GetBotType() != BOT_GUARD)
		{
			Die_Bot();
		}
	}
	
	if(m_PositionLockTick > 0)
	{
		--m_PositionLockTick;
		if(m_PositionLockTick <= 0)
			m_PositionLocked = false;
	}
	

	if(m_IsFrozen)
	{
		--m_FrozenTime;
		if(m_FrozenTime <= 0)
			Unfreeze();
		else
		{
			if (m_FrozenTime % Server()->TickSpeed() == Server()->TickSpeed() - 1)
				GameServer()->CreateDamageInd(m_Pos, 0, (m_FrozenTime + 1) / Server()->TickSpeed());		
		}
	}
	
	if(m_SlipperyTick > 0)
		--m_SlipperyTick;
	
	if(m_Poison)
	{
		if(m_PoisonTick == 0)
		{
			m_Poison--;
			if(GameServer()->m_apPlayers[m_PoisonFrom] && GameServer()->m_apPlayers[m_PoisonFrom]->GetCharacter())
			{
				GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());
				TakeDamage(vec2(0.0f, 0.0f), 1, m_PoisonFrom, WEAPON_HAMMER, TAKEDAMAGEMODE_NOINFECTION);
				GameServer()->SendEmoticon(m_pPlayer->GetCID(), EMOTICON_SORRY);
			}
			else 
				m_Poison = 0;

			if(m_Poison > 0)
				m_PoisonTick = Server()->TickSpeed()/2;
		}
		else
		{
			m_PoisonTick--;
		}
	}
	
	if(!m_InWater && !IsGrounded() && (m_Core.m_HookState != HOOK_GRABBED || m_Core.m_HookedPlayer != -1))
	{
		m_InAirTick++;
	}
	else
	{
		m_InAirTick = 0;
	}
	
	if(GetClass() == PLAYERCLASS_ASSASINS && IsGrounded() && m_DartLifeSpan <= 0)
	{
		m_DartLeft = g_Config.m_InfNinjaJump;
	}
	if(GetClass() == PLAYERCLASS_ASSASINS && m_InAirTick <= Server()->TickSpeed())
	{
		m_PositionLockAvailable = true;
	}
	
	if(m_IsFrozen || (GetClass() == PLAYERCLASS_ASSASINS && m_PositionLocked))
	{
		m_Input.m_Jump = 0;
		m_Input.m_Direction = 0;
		m_Input.m_Hook = 0;
	}
	
	m_pPlayer->m_Health = m_Health; 
	
	UpdateTuningParam();

	m_Core.m_Input = m_Input;
	
	CCharacterCore::CParams CoreTickParams(&m_pPlayer->m_NextTuningParams);
	CoreTickParams.m_HookMode = m_HookMode;
	
	m_Core.Tick(true, &CoreTickParams);

	if(m_pPlayer->GetBotType() == BOT_NPCW || m_pPlayer->GetBotType() == BOT_FARMER)
	{
		m_Core.m_Vel = vec2(0.0f, 1.0f);
		m_Core.m_Pos = PrevPos;
	}
	
	// Hook protection
	// 钩子保护
	if(m_Core.m_HookedPlayer >= 0)
	{
		if(GameServer()->m_apPlayers[m_Core.m_HookedPlayer] && GameServer()->m_apPlayers[m_Core.m_HookedPlayer]->GetCharacter())
		{
			if(m_pPlayer && Server()->GetItemSettings(m_pPlayer->GetCID(), HOOKDAMAGE) && m_HookDmgTick + Server()->TickSpeed()*0.50f < Server()->Tick())
			{
				m_HookDmgTick = Server()->Tick();
				if(Server()->GetItemSettings(m_pPlayer->GetCID(), MODULEHOOKEXPLODE))
					GameServer()->CreateExplosion(GameServer()->m_apPlayers[m_Core.m_HookedPlayer]->GetCharacter()->m_Pos, m_pPlayer->GetCID(), WEAPON_WORLD, false, 0);
				else
					GameServer()->m_apPlayers[m_Core.m_HookedPlayer]->GetCharacter()->TakeDamage(vec2(0,0), 2, m_pPlayer->GetCID(), WEAPON_WORLD, false);
			}
	
			// Если хукаешь НПС
			// 如果玩家勾住 NPC
			if((GameServer()->m_apPlayers[m_Core.m_HookedPlayer]->GetBotType() == BOT_GUARD 
					&& GameServer()->m_apPlayers[m_Core.m_HookedPlayer]->IsBot()) || GameServer()->m_apPlayers[m_Core.m_HookedPlayer]->m_ActiveChair)
			{
				m_Core.m_HookedPlayer = -1;
				m_Core.m_HookState = HOOK_RETRACTED;
				m_Core.m_HookPos = m_Pos;
			}
		}
	}

	HandleWeapons();
	
	if(!m_pPlayer->IsBot())
	{		
		if(m_pPlayer->MapMenu() == 1)
		{
			if(GetClass() != PLAYERCLASS_NONE)
			{
				m_AntiFireTick = Server()->Tick();
				m_pPlayer->CloseMapMenu();
			}
			else
			{
				vec2 CursorPos = vec2(m_Input.m_TargetX, m_Input.m_TargetY);
				bool Broadcast = false;

				if(length(CursorPos) > 50.0f)
				{
					float Angle = 2.0f*pi+atan2(CursorPos.x, -CursorPos.y);
					float AngleStep = 2.0f*pi/static_cast<float>(CMapConverter::NUM_MENUCLASS);
					m_pPlayer->m_MapMenuItem = ((int)((Angle+AngleStep/2.0f)/AngleStep))%CMapConverter::NUM_MENUCLASS;
					
					switch(m_pPlayer->m_MapMenuItem)
					{
						case CMapConverter::MENUCLASS_ASSASINS:
							GameServer()->SendBroadcast_Localization(m_pPlayer->GetCID(), BROADCAST_PRIORITY_INTERFACE, BROADCAST_DURATION_REALTIME, _("刺客"), NULL);
							Broadcast = true;	
							break;
						case CMapConverter::MENUCLASS_BERSERK:
							GameServer()->SendBroadcast_Localization(m_pPlayer->GetCID(), BROADCAST_PRIORITY_INTERFACE, BROADCAST_DURATION_REALTIME, _("战士"), NULL);
							Broadcast = true;
							break;
						case CMapConverter::MENUCLASS_HEALER:
							GameServer()->SendBroadcast_Localization(m_pPlayer->GetCID(), BROADCAST_PRIORITY_INTERFACE, BROADCAST_DURATION_REALTIME, _("医师"), NULL);
							Broadcast = true;
							break;
						default:
							m_pPlayer->m_MapMenuItem = -1;
							GameServer()->SendBroadcast_Localization(m_pPlayer->GetCID(), BROADCAST_PRIORITY_INTERFACE, BROADCAST_DURATION_REALTIME, _("你需要选择一个职业."), NULL);
							break;
					}
				}
				
				if(m_Input.m_Fire&1 && m_pPlayer->m_MapMenuItem >= 0)
				{
					int NewClass = -1;
					switch(m_pPlayer->m_MapMenuItem)
					{
						case CMapConverter::MENUCLASS_ASSASINS:
							NewClass = m_pPlayer->AccData.Class = PLAYERCLASS_ASSASINS;
							break;
						case CMapConverter::MENUCLASS_BERSERK:
							NewClass = m_pPlayer->AccData.Class = PLAYERCLASS_BERSERK;
							break;
						case CMapConverter::MENUCLASS_HEALER:
							NewClass = m_pPlayer->AccData.Class = PLAYERCLASS_HEALER;
							break;
					}
					
					if(NewClass >= 0)
					{
						m_AntiFireTick = Server()->Tick();
						m_pPlayer->m_MapMenuItem = 0;
						m_pPlayer->SetClass(NewClass);
						GameServer()->UpdateStats(m_pPlayer->GetCID());
						Die(m_pPlayer->GetCID(), WEAPON_WORLD);
						m_pPlayer->SetClassSkin(m_pPlayer->AccData.Class);
						GameServer()->ResetVotes(m_pPlayer->GetCID(), AUTH);
						GameServer()->GiveItem(m_pPlayer->GetCID(), MONEYBAG, 1);
					}
				}
			}
		}

		if(Server()->GetItemCount(m_pPlayer->GetCID(), EARRINGSKWAH))
		{
			int Jump = 1+Server()->GetItemCount(m_pPlayer->GetCID(), EARRINGSKWAH);
			if(IsGrounded()) m_AirJumpCounter = 0;
			if(m_Core.m_TriggeredEvents&COREEVENT_AIR_JUMP && m_AirJumpCounter < Jump)
			{
				m_Core.m_Jumped &= ~2;
				m_AirJumpCounter++;
			}
		}
	}
	m_PrevInput = m_Input;
	return;
}

void CCharacter::TickDefered()
{
	// advance the dummy
	{
		CCharacterCore::CParams CoreTickParams(&GameWorld()->m_Core.m_Tuning);
		CWorldCore TempWorld;
		m_ReckoningCore.Init(&TempWorld, GameServer()->Collision());
		m_ReckoningCore.Tick(false, &CoreTickParams);
		m_ReckoningCore.Move(&CoreTickParams);
		m_ReckoningCore.Quantize();
	}

	CCharacterCore::CParams CoreTickParams(&m_pPlayer->m_NextTuningParams);
	
	//lastsentcore
	vec2 StartPos = m_Core.m_Pos;
	vec2 StartVel = m_Core.m_Vel;
	bool StuckBefore = GameServer()->Collision()->TestBox(m_Core.m_Pos, vec2(28.0f, 28.0f));

	m_Core.Move(&CoreTickParams);
	bool StuckAfterMove = GameServer()->Collision()->TestBox(m_Core.m_Pos, vec2(28.0f, 28.0f));
	m_Core.Quantize();
	bool StuckAfterQuant = GameServer()->Collision()->TestBox(m_Core.m_Pos, vec2(28.0f, 28.0f));
	m_Pos = m_Core.m_Pos;

	if(!StuckBefore && (StuckAfterMove || StuckAfterQuant))
	{
		// Hackish solution to get rid of strict-aliasing warning
		union
		{
			float f;
			unsigned u;
		}StartPosX, StartPosY, StartVelX, StartVelY;

		StartPosX.f = StartPos.x;
		StartPosY.f = StartPos.y;
		StartVelX.f = StartVel.x;
		StartVelY.f = StartVel.y;

		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "STUCK!!! %d %d %d %f %f %f %f %x %x %x %x",
			StuckBefore,
			StuckAfterMove,
			StuckAfterQuant,
			StartPos.x, StartPos.y,
			StartVel.x, StartVel.y,
			StartPosX.u, StartPosY.u,
			StartVelX.u, StartVelY.u);
		GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);
	}

	int Events = m_Core.m_TriggeredEvents;

	if(Events&COREEVENT_HOOK_ATTACH_PLAYER) GameServer()->CreateSound(m_Pos, SOUND_HOOK_ATTACH_PLAYER, CmaskAll());

	if(m_pPlayer->GetTeam() == TEAM_SPECTATORS)
	{
		m_Pos.x = m_Input.m_TargetX;
		m_Pos.y = m_Input.m_TargetY;
	}

	// update the m_SendCore if needed
	{
		CNetObj_Character Predicted;
		CNetObj_Character Current;
		mem_zero(&Predicted, sizeof(Predicted));
		mem_zero(&Current, sizeof(Current));
		m_ReckoningCore.Write(&Predicted);
		m_Core.Write(&Current);

		// only allow dead reackoning for a top of 3 seconds
		if(m_ReckoningTick+Server()->TickSpeed()*3 < Server()->Tick() || mem_comp(&Predicted, &Current, sizeof(CNetObj_Character)) != 0)
		{
			m_ReckoningTick = Server()->Tick();
			m_SendCore = m_Core;
			m_ReckoningCore = m_Core;
		}
	}
}

void CCharacter::TickPaused()
{
	++m_AttackTick;
	++m_DamageTakenTick;
	++m_ReckoningTick;
	if(m_LastAction != -1)
		++m_LastAction;
	if(m_aWeapons[m_ActiveWeapon].m_AmmoRegenStart > -1)
		++m_aWeapons[m_ActiveWeapon].m_AmmoRegenStart;
	if(m_EmoteStop > -1)
		++m_EmoteStop;
		
	++m_HookDmgTick;
}

bool CCharacter::IncreaseHealth(int Amount)
{
	if(m_Health >= 10)
		return false;
	m_Health = clamp(m_Health+Amount, 0, 10);
	return true;
}

bool CCharacter::IncreaseArmor(int Amount)
{
	if(m_Armor >= 10)
		return false;
	m_Armor = clamp(m_Armor+Amount, 0, 10);
	return true;
}

bool CCharacter::IncreaseOverallHp(int Amount)
{
	bool success = false;
	if(m_Health < 10)
	{
		int healthDiff = 10-m_Health;
		IncreaseHealth(Amount);
		success = true;
		Amount = Amount - healthDiff;
	}
	if(Amount > 0)
	{
		if (IncreaseArmor(Amount)) 
			success = true;
	}
	return success;
}

void CCharacter::Die(int Killer, int Weapon)
{
	DestroyChildEntities();

	// we got to wait 0.5 secs before respawning
	m_pPlayer->m_RespawnTick = Server()->Tick()+Server()->TickSpeed()/2;
	int ModeSpecial = GameServer()->m_pController->OnCharacterDeath(this, GameServer()->m_apPlayers[Killer], Weapon);
	if(!m_pPlayer->IsBot()){
		// send the kill message
		CNetMsg_Sv_KillMsg Msg;
		Msg.m_Killer = Killer;
		Msg.m_Victim = m_pPlayer->GetCID();
		Msg.m_Weapon = Weapon;
		Msg.m_ModeSpecial = ModeSpecial;
		Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, -1);
	}
	// a nice sound
	GameServer()->CreateSound(m_Pos, SOUND_PLAYER_DIE);

	// 因为boss/玩家的死亡导致的boss战结束
	// вся хуйня когда мрут боссы и игроки
	if(GameServer()->m_BossStartTick < 10 && GameServer()->m_BossStart)
	{
		// 打败了boss
		// 原注释：если босс проебал то отсосите все пидорасы ебаные ебал всех
		if(m_pPlayer->GetBotType() == BOT_BOSSSLIME && !GameServer()->m_WinWaitBoss)
		{
			int CountWin = GameServer()->GetBossCount();
			GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("Boss {str:bossn} 被{int:cwin}个玩家击败."), "bossn", GameServer()->GetBossName(GameServer()->m_BossType), "cwin", &CountWin, NULL);			
			
			GameServer()->m_WinWaitBoss = 1000;
		}
		
		// 如果玩家死了，就会离开boss房间
		if(m_pPlayer->m_InBossed)
		{	
			m_pPlayer->m_InBossed = false;
			GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("你被 Boss {str:name}击败."), "name", GameServer()->GetBossName(GameServer()->m_BossType), NULL);
		}
	}

	// 竞技场
	if (GameServer()->m_AreaEndGame && m_pPlayer->m_InArea)
	{
		// если игрок погиб он уже окончательно вышел с арены
		// 如果玩家死亡，他就会离开竞技场
		m_pPlayer->m_InArea = false;
		GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("你被击败了."), NULL);
	}

	// this is for auto respawn after 3 secs
	m_pPlayer->m_DieTick = Server()->Tick();

	m_Alive = false;
	GameServer()->m_World.RemoveEntity(this);
	GameServer()->m_World.m_Core.m_apCharacters[m_pPlayer->GetCID()] = 0;
	GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());
	
	if(Killer >=0 && Killer < MAX_CLIENTS)
	{
		CPlayer* pKillerPlayer = GameServer()->m_apPlayers[Killer];
		pKillerPlayer->AccData.Kill++;
		
		if(pKillerPlayer && !pKillerPlayer->IsBot() && !m_pPlayer->IsBot()
			&& Killer != m_pPlayer->GetCID() && !pKillerPlayer->m_InArea)
		{
			int get = 300;
			if(Server()->GetItemCount(Killer, RELRINGS))
			{
				get = 300-Server()->GetItemCount(Killer, RELRINGS)*10;
				if(get < 50)
					get = 50;
			}
			
			pKillerPlayer->AccData.Rel += get;
			GameServer()->SendChatTarget_Localization(Killer, CHATCATEGORY_DEFAULT, _("交际愤怒值: {int:rel}"), "rel", &pKillerPlayer->AccData.Rel, NULL);
		}
		
		if(m_pPlayer->m_Search)
		{
			if(pKillerPlayer && Killer != m_pPlayer->GetCID() 
				&& (!pKillerPlayer->IsBot() || pKillerPlayer->GetBotType() == BOT_GUARD))
			{
				m_pPlayer->m_Search = false;
				GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("玩家 {str:name}, 击败玩家 {str:name1}, 并将其打入大牢"), "name", Server()->ClientName(Killer), "name1", Server()->ClientName(m_pPlayer->GetCID()), NULL);
						
				m_pPlayer->AccData.Jail = true;
				m_pPlayer->AccData.Rel = 0;
				GameServer()->UpdateStats(m_pPlayer->GetCID());
				
				if(!pKillerPlayer->IsBot())
				{
					pKillerPlayer->MoneyAdd(m_pPlayer->AccData.Level*1000, false, true);
					GameServer()->UpdateStats(Killer);
					GameServer()->ResetVotes(Killer, AUTH);
				}
			}
		}
	}
}


void CCharacter::Die_Bot() //机器人(如 Pig)因为进入 non-PvP 区域而判定死亡，不在 HUD 中显示
{
	DestroyChildEntities();

	// we got to wait 0.5 secs before respawning
	m_pPlayer->m_RespawnTick = Server()->Tick()+Server()->TickSpeed()/2;

	// a nice sound
	GameServer()->CreateSound(m_Pos, SOUND_PLAYER_DIE);

	

	// this is for auto respawn after 3 secs
	m_pPlayer->m_DieTick = Server()->Tick();

	m_Alive = false;
	GameServer()->m_World.RemoveEntity(this);
	GameServer()->m_World.m_Core.m_apCharacters[m_pPlayer->GetCID()] = 0;
	GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());
	

}


int CCharacter::SendToJail(int PlayerID, int JailLength) //手动送某人进监狱:)
{
	DestroyChildEntities();

	// we got to wait 0.5 secs before respawning
	m_pPlayer->m_RespawnTick = Server()->Tick()+Server()->TickSpeed()/2;
	int ModeSpecial = GameServer()->m_pController->OnCharacterDeath(this, GameServer()->m_apPlayers[PlayerID], WEAPON_WORLD);

	// send the kill message
	CNetMsg_Sv_KillMsg Msg;
	Msg.m_Killer = PlayerID;
	Msg.m_Victim = PlayerID;
	Msg.m_Weapon = WEAPON_WORLD;
	Msg.m_ModeSpecial = ModeSpecial;
	Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, -1);
	
	// a nice sound
	GameServer()->CreateSound(m_Pos, SOUND_PLAYER_DIE);

	// this is for auto respawn after 3 secs
	m_pPlayer->m_DieTick = Server()->Tick();

	m_Alive = false;
	GameServer()->m_World.RemoveEntity(this);
	GameServer()->m_World.m_Core.m_apCharacters[m_pPlayer->GetCID()] = 0;
	GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());
	
	CPlayer* pKillerPlayer = GameServer()->m_apPlayers[PlayerID];
	pKillerPlayer->AccData.Kill++;
	
	m_pPlayer->m_Search = false;
	GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("玩家 {str:name} 被捕了!"), "name", Server()->ClientName(m_pPlayer->GetCID()), NULL);
					
	m_pPlayer->AccData.Jail = true;
	m_pPlayer->AccData.Rel = 0;
	m_pPlayer->AccData.IsJailed = true;
	m_pPlayer->AccData.JailLength = JailLength;
	GameServer()->UpdateStats(m_pPlayer->GetCID());
	return 0;
	
}

int CCharacter::Unjail(int PlayerID) //手动救某人出监狱
{
	DestroyChildEntities();

	// we got to wait 0.5 secs before respawning
	m_pPlayer->m_RespawnTick = Server()->Tick()+Server()->TickSpeed()/2;
	int ModeSpecial = GameServer()->m_pController->OnCharacterDeath(this, GameServer()->m_apPlayers[PlayerID], WEAPON_WORLD);

	// send the kill message
	CNetMsg_Sv_KillMsg Msg;
	Msg.m_Killer = PlayerID;
	Msg.m_Victim = PlayerID;
	Msg.m_Weapon = WEAPON_WORLD;
	Msg.m_ModeSpecial = ModeSpecial;
	Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, -1);
	
	// a nice sound
	GameServer()->CreateSound(m_Pos, SOUND_PLAYER_DIE);
	
	// this is for auto respawn after 3 secs
	m_pPlayer->m_DieTick = Server()->Tick();

	m_Alive = false;
	GameServer()->m_World.RemoveEntity(this);
	GameServer()->m_World.m_Core.m_apCharacters[m_pPlayer->GetCID()] = 0;
	GameServer()->CreateDeath(m_Pos, m_pPlayer->GetCID());
	
	m_pPlayer->m_Search = false;
	GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("玩家 {str:name} 出监狱了!"), "name", Server()->ClientName(m_pPlayer->GetCID()), NULL);
					
	m_pPlayer->AccData.Jail = false;
	m_pPlayer->AccData.Rel = 0;
	m_pPlayer->AccData.IsJailed = false;
	GameServer()->UpdateStats(m_pPlayer->GetCID());
	return 0;
	
}


bool CCharacter::TakeDamage(vec2 Force, int Dmg, int From, int Weapon, int Mode)
{
	CPlayer *pFrom = GameServer()->m_apPlayers[From];
	CCharacter *pChr = GameServer()->m_apPlayers[From]->GetCharacter();

	if(pFrom && pChr)
	{
		GameServer()->SendBroadcast_LStat(m_pPlayer->GetCID(), 106, 50, -1);
		
		// Боты
		// 机器人
		if(m_pPlayer->GetBotType() == BOT_NPCW || m_pPlayer->GetBotType() == BOT_FARMER)
			return true; 
	
		// Антипвп в городе
		// 城市中的 anti-PvP
		if(pChr->m_AntiPVP || m_AntiPVP)
			return true;

		// Кольцо антиурона
		// 防止伤到自己的 Ring Selfine 
		if(From == m_pPlayer->GetCID() && Server()->GetItemSettings(From, RINGNOSELFDMG))
			return true;

		// Если не арена
		// 如果不是竞技场
		if(!m_pPlayer->m_InArea && !pFrom->m_InArea)
		{
			// Тюрьма
			// 监狱
			if(m_pPlayer->AccData.Jail)
				return true;

			// АнтиПВП вся хуня
			// AntiPVP
			if((Server()->GetItemSettings(m_pPlayer->GetCID(), SANTIPVP) || Server()->GetItemSettings(From, SANTIPVP) || 
				m_pPlayer->m_AntiPvpSmall) && !m_pPlayer->IsBot() && !pFrom->IsBot() && m_pPlayer->GetCID() != From)
				return true;
			
			// Боссецкий
			// (咱也布吉岛这是什么
			if((pFrom->m_InBossed || (m_pPlayer->m_InBossed && m_pPlayer->GetCID() != From)) && !pFrom->IsBot() && !m_pPlayer->IsBot())
				return true;

			// Бот бота не бьет
			// 机器人不打机器人 (除了守卫)
			if(pFrom->GetBotType() == m_pPlayer->GetBotType() && pFrom->GetBotType() >= 0)
				return true;
		
			// Сокланы
			// (咱也布吉岛这是什么
			if(Server()->GetClanID(From) && Server()->GetClanID(m_pPlayer->GetCID()) == Server()->GetClanID(From)
				&& m_pPlayer->GetCID() != From)
				return true;
				
			// Агрессия
			// 守卫愤怒值
			if(m_pPlayer->GetBotType() == BOT_GUARD)
			{
				pFrom->AccData.Rel += 10;
				GameServer()->SendChatTarget_Localization(From, CHATCATEGORY_DEFAULT, _("交际愤怒值: {int:rel}"), "rel", &pFrom->AccData.Rel, NULL);
			}
		}
		// Арена
		// 激光 PvP 竞技场
		if((m_pPlayer->m_InArea && pFrom->m_InArea && GameServer()->m_AreaStartTick < 500 && GameServer()->m_AreaStartTick > 1) 
			|| (m_pPlayer->m_InArea && GameServer()->m_AreaType == 2))
			return true;

		// Отталкивание 
		// 霰弹枪的击退作用
		if(m_pPlayer->GetBotType() != BOT_GUARD)
		{
			if(m_ActiveWeapon == WEAPON_SHOTGUN)
				m_Core.m_Vel += Force/100;
			else
				m_Core.m_Vel += Force;	
		}

		// Кольцо анти урона по себе
		// 自身抗伤害之戒 (Slime 的戒指)
		if(Server()->GetItemCount(From, RARERINGSLIME) && From == m_pPlayer->GetCID())
			return true;
	}

	// Тюрьма 
	// 监狱
	if(From >= 0 && pFrom->GetBotType() == BOT_GUARD && !m_pPlayer->IsBot())
	{
		if(m_pPlayer->m_Search)
		{
			Die(From, Weapon);
			return true;
		}
		else
			return true;
	}
	
	if(From == m_pPlayer->GetCID())
		Dmg = max(1, Dmg/2);

	m_DamageTaken++;
	
	if(From >= 0 && pFrom && pFrom->GetCharacter())
	{
		//TODO
		if(Server()->GetItemCount(From, FREEAZER))
		{
			auto probability = (float)(200-Server()->GetItemCount(From, FREEAZER)*5);
			if (probability <= 5.0f) probability = 5.0f;
			if(random_prob(1.0f/probability))
			{
				if(m_pPlayer->GetBotType() == BOT_BOSSSLIME) Freeze(2);
				else Freeze(1);
			}
		}
		
		if(m_pPlayer->AccData.Class == PLAYERCLASS_HEALER && m_pPlayer->AccUpgrade.Pasive2)
		{
			auto RandProc = (float)(100-m_pPlayer->AccUpgrade.Pasive2*2);
			if(random_prob(1.0f/RandProc))
			{
				if(!Server()->GetItemSettings(m_pPlayer->GetCID(), SCHAT)) 
					GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("Passive skill don't get damage"), NULL);
				return true;
			}
		}
		
		auto getcount = (float)(pFrom->AccData.Class == PLAYERCLASS_ASSASINS ? 15-pFrom->AccUpgrade.HammerRange : 15.0f);
		if(random_prob(1.0f/getcount))
		{
			int CritDamage = Dmg+pFrom->AccUpgrade.Damage*2+random_int(0, 50);
			if(pFrom->AccData.Class == PLAYERCLASS_ASSASINS)
				CritDamage += (CritDamage/100)*pFrom->AccUpgrade.Pasive2*3;
			
			Dmg = CritDamage;
			if(pFrom->GetCharacter()->m_ActiveWeapon == WEAPON_SHOTGUN)
				Dmg = (int)(CritDamage/2);
			
			if(Server()->GetItemSettings(From, TITLE_DONATE_BAOJI50))
				Dmg *= 50;

			if(!Server()->GetItemSettings(From, SCHAT))
				GameServer()->SendChatTarget_Localization(From, CHATCATEGORY_BERSERK, _("暴击伤害 {int:crit}"), "crit", &Dmg, NULL);
		}
		else
		{
			int DamageProc = Dmg+pFrom->AccUpgrade.Damage;
			if(pFrom->AccData.Class == PLAYERCLASS_BERSERK)
				DamageProc += (DamageProc/100)*pFrom->AccUpgrade.Pasive2*3;
			
			Dmg = DamageProc;
			if(pFrom->GetCharacter()->m_ActiveWeapon == WEAPON_SHOTGUN)
				Dmg = (int)DamageProc/2;
		}

		if(pFrom->m_AngryWroth)
			Dmg += pFrom->m_AngryWroth/2;
	}
	else Dmg = 1;

	if(Server()->Tick() < m_DamageTakenTick+25)
	{
		GameServer()->CreateDamageInd(m_Pos, m_DamageTaken*0.25f, Dmg/10);
	}
	else
	{
		m_DamageTaken = 0;
		GameServer()->CreateDamageInd(m_Pos, 0, Dmg/10);
	}

	if(Dmg)
	{
		if(Server()->GetItemEnquip(m_pPlayer->GetCID(), 17))
			Dmg += Server()->GetBonusEnchant(m_pPlayer->GetCID(), Server()->GetItemEnquip(m_pPlayer->GetCID(), 17), 17);

		if(m_Armor)
		{
			if(Dmg <= m_Armor)
			{
				m_Armor -= Dmg;
				Dmg = 0;
			}
			else
			{
				Dmg -= m_Armor;
				m_Armor = 0;
			}
		}

		if(From >= 0)
			m_Health -= Dmg;
			
		// do damage Hit sound
		if(From >= 0 && From != m_pPlayer->GetCID() && GameServer()->m_apPlayers[From])
		{
			int64_t Mask = CmaskOne(From);
			for(int i = 0; i < MAX_NOBOT; i++)
			{
				if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() == TEAM_SPECTATORS && GameServer()->m_apPlayers[i]->m_SpectatorID == From)
					Mask |= CmaskOne(i);
			}
			GameServer()->CreateSound(GameServer()->m_apPlayers[From]->m_ViewPos, SOUND_HIT, Mask);
		}
	}

	m_DamageTakenTick = Server()->Tick();
	m_InvisibleTick = Server()->Tick();

	// check for death
	if(m_Health <= 0)
	{
		int randforce = random_int(0, 30);
		if(pFrom)
		{
			if(g_Config.m_SvEventHammer)
				CreateDropRandom(EVENTBOX, 1, 40, From, Force/(50+randforce));

			if(g_Config.m_SvEventSummer)
				CreateDropRandom(ESUMMER, 1, 45, From, Force/(50+randforce));

			if(m_pPlayer->GetBotType() == BOT_L1MONSTER)
			{
				if(!g_Config.m_SvCityStart)
				{
					CreateDropRandom(AHAPPY, 1, 2000, From, Force/(50+randforce));
					CreateDropRandom(AEVIL, 1, 2000, From, Force/(50+randforce));
					CreateDropRandom(APAIN, 1, 2000, From, Force/(50+randforce));
					CreateDropRandom(ABLINK, 1, 2000, From, Force/(50+randforce));
					CreateDropRandom(ASUPRRISE, 1, 2000, From, Force/(50+randforce));
					CreateDropRandom(PIGPORNO, 1, 40, From, Force/(50+randforce));
					CreateDropRandom(LEATHER, 1, 60, From, Force/(50+randforce));
				}
				else if(g_Config.m_SvCityStart == 1)
				{
					CreateDropRandom(ZOMBIEBRAIN, 1, 40, From, Force/(50+randforce));
					CreateDropRandom(ZOMBIEEYE, 1, 40, From, Force/(50+randforce));
				}
			}
		
			if(pFrom && m_pPlayer->GetBotType() == BOT_L2MONSTER)
			{
				if(!g_Config.m_SvCityStart)
				{
					CreateDropRandom(KWAHGANDON, 1, 44, From, Force/(50+randforce));
					CreateDropRandom(FOOTKWAH, 1, 44, From, Force/(40+randforce));
				}
				else if(g_Config.m_SvCityStart == 1)
				{
					CreateDropRandom(SKELETSBONE, 1, 45, From, Force/(50+randforce));
					CreateDropRandom(SKELETSKULL, 1, 45, From, Force/(50+randforce));
				}
			}
			if(pFrom && m_pPlayer->GetBotType() == BOT_L3MONSTER)
			{
				if(!g_Config.m_SvCityStart)
				{
					CreateDropRandom(HEADBOOMER, 1, 42, From, Force/(50+randforce));
				}
				else if(g_Config.m_SvCityStart == 1)
				{
					CreateDropRandom(NIMFHEART, 1, 40, From, Force/(50+randforce));
					CreateDropRandom(NIMFEARS, 1, 40, From, Force/(50+randforce));
					CreateDropRandom(NIMFBODY, 1, 40, From, Force/(50+randforce));
				}
			}
		}

		if(m_pPlayer->GetBotType() == BOT_BOSSSLIME)
		{
			for(int i = 0; i < MAX_NOBOT; ++i)
			{
				randforce = random_int(0, 80);
				if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetCharacter())
				{
					if(GameServer()->m_apPlayers[i]->m_InBossed)
					{
						if(!Server()->GetItemCount(i, BOSSDIE))
							GameServer()->GiveItem(i, BOSSDIE, 1);

						if(Server()->GetItemSettings(m_pPlayer->GetCID(), SCHAT) != 2) 
							GameServer()->SendChatTarget_Localization(i, CHATCATEGORY_DEFAULT, _("玩家之间分享掉落的物品"), NULL);		
						
						if(!g_Config.m_SvCityStart)
						{
							CreateDropRandom(MONEYBAG, 100+random_int(0, 200), false, i, Force/(50+randforce));
							CreateDropRandom(RARESLIMEDIRT, 1, 90, i, Force/(45+randforce));
							CreateDropRandom(FORMULAFORRING, 1, 90, i, Force/(40+randforce));
							CreateDropRandom(FORMULAEARRINGS, 1, 90, i, Force/(35+randforce));
							CreateDropRandom(FORMULAWEAPON, 1, 90, i, Force/(40+randforce));
							CreateDropRandom(RANDOMCRAFTITEM, 1, 15, i, Force/(45+randforce));
						}
						else if(g_Config.m_SvCityStart == 1)
						{
							CreateDropRandom(MONEYBAG, 300+random_int(0, 200), false, i, Force/(50+randforce));
							CreateDropRandom(BOOKEXPMIN, 1, 15, i, Force/(45+randforce));
							CreateDropRandom(BOOKMONEYMIN, 1, 80, i, Force/(45+randforce));
							CreateDropRandom(CLANBOXEXP, 1, 50, i, Force/(45+randforce));
						}
					}  
				}
			}
		}
		if(!m_pPlayer->m_InArea)
		{
			for(int i = 0; i < g_Config.m_SvDropDance; ++i)
			{
				switch(random_int(0, 10))
				{
					case 1: new CBonus(GameWorld(), m_Pos, Force/(50+random_int(0, 30)), 2, -1); break;
					case 4: new CBonus(GameWorld(), m_Pos, Force/(50+random_int(0, 30)), 3, -1); break;
					case 8: new CBonus(GameWorld(), m_Pos, Force/(50+random_int(0, 30)), 4, -1);
				}
				if(random_int(0, 3) == 1)
					new CBonus(GameWorld(), m_Pos, Force/(40+random_int(0, 30)), 1, m_pPlayer->GetCID());
					
				new CBonus(GameWorld(), m_Pos, Force/(30+random_int(0, 30)), 0, m_pPlayer->GetCID());
			}
		}
		
		if(pFrom) new CFlyingPoint(GameWorld(), m_Pos, From, From, m_Core.m_Vel);

		Die(From, Weapon);
		if (From >= 0 && From != m_pPlayer->GetCID() && GameServer()->m_apPlayers[From])
		{
			CCharacter *pChr = GameServer()->m_apPlayers[From]->GetCharacter();
			if (pChr)
			{
				pChr->m_EmoteType = EMOTE_HAPPY;
				pChr->m_EmoteStop = Server()->Tick() + Server()->TickSpeed();
			}
		}

		return false;
	}

	if (Dmg > 2)
		GameServer()->CreateSound(m_Pos, SOUND_PLAYER_PAIN_LONG);
	else
		GameServer()->CreateSound(m_Pos, SOUND_PLAYER_PAIN_SHORT);

	m_EmoteType = EMOTE_PAIN;
	m_EmoteStop = Server()->Tick() + 500 * Server()->TickSpeed() / 1000;

	return true;
}

void CCharacter::Snap(int SnappingClient)
{
	int id = m_pPlayer->GetCID();

	if (!Server()->Translate(id, SnappingClient))
		return;

	if(NetworkClipped(SnappingClient))
		return;
	
	CPlayer* pClient = GameServer()->m_apPlayers[SnappingClient];
	if(SnappingClient != m_pPlayer->GetCID() && !m_pPlayer->IsBot())
	{
		if(pClient)
		{
			CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_HeartID, sizeof(CNetObj_Pickup)));
			if(!pP)
				return;

			pP->m_X = (int)m_Pos.x;
			pP->m_Y = (int)m_Pos.y - 60.0;
			if(m_Health < m_pPlayer->m_HealthStart)
				pP->m_Type = POWERUP_HEALTH;
			else
				pP->m_Type = POWERUP_ARMOR;
			pP->m_Subtype = 0;
		}
	}

	// РИСУЕМ ИГРОКА
	CNetObj_Character *pCharacter = static_cast<CNetObj_Character *>(Server()->SnapNewItem(NETOBJTYPE_CHARACTER, id, sizeof(CNetObj_Character)));
	if(!pCharacter)
		return;
		
	// 表情
	if(m_pPlayer && m_pPlayer->GetBotType() <= 0)
	{
		EmoteNormal = EMOTE_NORMAL;
		if(Server()->GetItemCount(m_pPlayer->GetCID(), MODULEEMOTE))
		{
			EmoteNormal = Server()->GetItemSettings(m_pPlayer->GetCID(), MODULEEMOTE);
		}
		else
		{
			if(m_pPlayer->m_ActiveChair) EmoteNormal = EMOTE_HAPPY;
			if(Server()->GetItemCount(m_pPlayer->GetCID(), AHAPPY))
				EmoteNormal = EMOTE_HAPPY;
			else if(Server()->GetItemCount(m_pPlayer->GetCID(), APAIN))
				EmoteNormal = EMOTE_PAIN;
			else if(Server()->GetItemCount(m_pPlayer->GetCID(), ASUPRRISE))
				EmoteNormal = EMOTE_SURPRISE;
			else if(Server()->GetItemCount(m_pPlayer->GetCID(), ABLINK))
				EmoteNormal = EMOTE_BLINK;
			else if(Server()->GetItemCount(m_pPlayer->GetCID(), AEVIL))
				EmoteNormal = EMOTE_ANGRY;
		}
		if(m_InWater)
			EmoteNormal = EMOTE_BLINK;
	}

	//TODO
	// РИСУЕМ ИГРОКА ПРИ ПАУЗЕ
	if(!m_ReckoningTick || GameServer()->m_World.m_Paused)
	{
		pCharacter->m_Tick = 0;
		m_Core.Write(pCharacter);
	}
	else
	{
		pCharacter->m_Tick = m_ReckoningTick;
		m_SendCore.Write(pCharacter);
	}

	// 安装表情(?)
	// 原注释：УСТАНОВКА ЭМОЦИИ
	if (m_EmoteStop < Server()->Tick())
	{
		m_EmoteType = EmoteNormal;
		m_EmoteStop = -1;
	}

	if (pCharacter->m_HookedPlayer != -1)
	{
		if (!Server()->Translate(pCharacter->m_HookedPlayer, SnappingClient))
			pCharacter->m_HookedPlayer = -1;
	}
	pCharacter->m_Emote = m_EmoteType;

	pCharacter->m_AmmoCount = 0;
	pCharacter->m_Health = 0;
	pCharacter->m_Armor = 0;
	pCharacter->m_Weapon = m_ActiveWeapon;

	// 武器：
	// 在玩家被冻结时，或者玩家职业为 Assasins(刺客)且切换到锤子武器时，将玩家武器（外观）设置为 Ninja(忍者)
	if((GetInfWeaponID(m_ActiveWeapon) == INFWEAPON_HAMMER && m_pPlayer && m_pPlayer->AccData.Class == PLAYERCLASS_ASSASINS && !m_pPlayer->m_InArea) || m_IsFrozen)
	{
		if(m_IsFrozen)
			pCharacter->m_Emote = EMOTE_BLINK;

		pCharacter->m_Weapon = WEAPON_NINJA;
	}
	
	pCharacter->m_AttackTick = m_AttackTick;
	pCharacter->m_Direction = m_Input.m_Direction;

	if(m_pPlayer->GetCID() == SnappingClient || SnappingClient == -1 ||
		(!g_Config.m_SvStrictSpectateMode && m_pPlayer->GetCID() == GameServer()->m_apPlayers[SnappingClient]->m_SpectatorID))
	{
		pCharacter->m_Health = m_Health;
		pCharacter->m_Armor = m_Armor;
		if(m_aWeapons[m_ActiveWeapon].m_Ammo > 0)
			pCharacter->m_AmmoCount = m_aWeapons[m_ActiveWeapon].m_Ammo;
	}

	if(pCharacter->m_Emote == EmoteNormal)
	{
		if(250 - ((Server()->Tick() - m_LastAction)%(250)) < 5)
			pCharacter->m_Emote = EMOTE_BLINK;
	}

	pCharacter->m_PlayerFlags = GetPlayer()->m_PlayerFlags;
}

// 获得职业
void CCharacter::OpenClassChooser()
{
	m_pPlayer->OpenMapMenu(1);
}

// 
int CCharacter::GetClass()
{
	if(m_pPlayer)
		return m_pPlayer->GetClass();

	return PLAYERCLASS_NONE;
}

bool CCharacter::InCrafted()
{
	if(m_InCrafted) 
		return true;
	
	return false;
}

bool CCharacter::InQuest()
{
	if(m_InQuest) 
		return true;
	
	return false;
}

// InfClass 的功能,这边用不上
// 给 Ninja(忍者) Buff(加成)
/*
void CCharacter::GiveNinjaBuf()
{
	if(GetClass() != PLAYERCLASS_ASSASINS)
		return;
	
	switch(random_int(0, 2))
	{
		case 0: //速度增益
			m_NinjaVelocityBuff++;
			GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("Sword velocity increased"), NULL);
			break;
		case 1: //力量增益
			m_NinjaStrengthBuff++;
			GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("Sword strength increased"), NULL);
			break;
		case 2: //弹药增益
			m_NinjaAmmoBuff++;
			GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("Grenade limit increased"), NULL);
			break;
	}
}
*/

// 空间属性
void CCharacter::ClassSpawnAttributes()
{			
	if(!Server()->GetItemSettings(m_pPlayer->GetCID(), SCHAT))
		GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("注意!所有的设置项都在投票选项里面"), NULL);

	if(m_pPlayer->m_InArea)
	{
		m_aWeapons[WEAPON_HAMMER].m_Got = false;
		
		Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_RIFLE, 100);
		Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_RIFLE, 1000);
		Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_RIFLE, 1);
		
		if(GameServer()->m_AreaType == 2)
		{
			m_aWeapons[WEAPON_HAMMER].m_Got = true;
			Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_HAMMER, 10000);// 我讨厌卡锤
			Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_HAMMER, 1000);
			Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_HAMMER, 0);
		}

		m_aWeapons[WEAPON_RIFLE].m_Got = true;
		GiveWeapon(WEAPON_RIFLE, 10000);
		m_ActiveWeapon = WEAPON_RIFLE;
		m_Health = 1;
		return;
	}
	// 初始化职业属性
	// 1.Healer: 每个生命升级点增加 50 血量; 每 4 点散射增加 1% 血量
	// 2.Berserk: 无
	// 3.Assasins: 初始生命值为 5
	switch(GetClass())
	{
		case PLAYERCLASS_HEALER:
			RemoveAllGun();
			m_Health = 10+m_pPlayer->AccUpgrade.Health*50;
			if(m_pPlayer->AccUpgrade.HammerRange)
			{
				int Proc = (m_Health / 100)*m_pPlayer->AccUpgrade.HammerRange*4;
				m_Health = 10+m_pPlayer->AccUpgrade.Health*50+Proc;
			}
			if(!m_pPlayer->IsKownClass(PLAYERCLASS_HEALER))
			{
				m_pPlayer->m_knownClass[PLAYERCLASS_HEALER] = true;
			}
			break;
		case PLAYERCLASS_BERSERK:
			RemoveAllGun();
			m_Health = 10+m_pPlayer->AccUpgrade.Health*40;
			if(!m_pPlayer->IsKownClass(PLAYERCLASS_BERSERK))
			{
				m_pPlayer->m_knownClass[PLAYERCLASS_BERSERK] = true;
			}
			break;
		case PLAYERCLASS_ASSASINS:
			RemoveAllGun();
			m_Health = 5+m_pPlayer->AccUpgrade.Health*40;
			if(!m_pPlayer->IsKownClass(PLAYERCLASS_ASSASINS))
			{
				m_pPlayer->m_knownClass[PLAYERCLASS_ASSASINS] = true;
			}
			break;
		case PLAYERCLASS_NONE:
			m_Health = 10;
			break;
	}

	if(Server()->GetItemEnquip(m_pPlayer->GetCID(), 15))
	{
		m_Health += Server()->GetBonusEnchant(m_pPlayer->GetCID(), Server()->GetItemEnquip(m_pPlayer->GetCID(), 15), 15);
		m_Armor += Server()->GetBonusEnchant(m_pPlayer->GetCID(), Server()->GetItemEnquip(m_pPlayer->GetCID(), 15), 15);
	}

	if(Server()->GetItemEnquip(m_pPlayer->GetCID(), 16))
	{
		m_Health += Server()->GetBonusEnchant(m_pPlayer->GetCID(), Server()->GetItemEnquip(m_pPlayer->GetCID(), 16), 16);
		m_Armor += Server()->GetBonusEnchant(m_pPlayer->GetCID(), Server()->GetItemEnquip(m_pPlayer->GetCID(), 16), 16);
	}

	if(Server()->GetItemSettings(m_pPlayer->GetCID(), BOSSDIE))
		m_Health += 1000;
	
	if(Server()->GetItemSettings(m_pPlayer->GetCID(), TITLEQUESTS))
	{
		m_Health += 1500;
		m_Armor += 500;
	}

	if(Server()->GetItemSettings(m_pPlayer->GetCID(), TITLEENCHANT))
	{
		m_Health += 1000;
		m_Armor += 1000;
	}

	if (Server()->GetItemSettings(m_pPlayer->GetCID(), TITLE_DONATE_SHENGMIN70))
		m_Health *= 70;

	/*if(Server()->GetItemSettings(m_pPlayer->GetCID(), TITLEMOON))
	{
		GameServer()->m_apPlayers[m_pPlayer->GetCID()]->AccUpgrade.Speed += 10;
	}*/
	// 新手保护，禁用 PvP
	m_pPlayer->m_AntiPvpSmall = false;
	if(m_pPlayer->AccData.Level < 20)
	{
		m_pPlayer->m_AntiPvpSmall = true;

		if(Server()->GetItemSettings(m_pPlayer->GetCID(), SCHAT) != 2)
			GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("PVP模式 -- 新手保护已开启."), NULL);	
	}
	//TODO
	// книги инфа
	if(m_pPlayer->m_MoneyAdd)
		GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("You have an active {str:name}."), "name", Server()->GetItemName(m_pPlayer->GetCID(), BOOKMONEYMIN), NULL);		
	if(m_pPlayer->m_ExperienceAdd)
		GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("You have an active {str:name}."), "name", Server()->GetItemName(m_pPlayer->GetCID(), BOOKEXPMIN), NULL);		

	if(m_pPlayer->IsBot())
		m_Health = 10+m_pPlayer->AccUpgrade.Health*10;

	// 佩戴 Ring Boomer 生命值增加 5%
	if(Server()->GetItemCount(m_pPlayer->GetCID(), RINGBOOMER))
		m_Health += (m_Health/100)*5*Server()->GetItemCount(m_pPlayer->GetCID(), RINGBOOMER);		

	// 武器属性设置
	int geta = (int)(5+m_pPlayer->AccUpgrade.Ammo);// 弹药数量
	int getsp = 1000+m_pPlayer->AccUpgrade.Speed*20;// 射速
	int getspg = 1000+m_pPlayer->AccUpgrade.Speed*8;// Grenade（火箭炮）射速
	int getar = 0;									// 子弹回复速度
	if(m_pPlayer->AccUpgrade.AmmoRegen > 0) 
		getar= (int)(650-m_pPlayer->AccUpgrade.AmmoRegen*2);
		
	// 按照弹夹数量添加弹药
	if(Server()->GetItemCount(m_pPlayer->GetCID(), WEAPONPRESSED))
		geta += Server()->GetItemCount(m_pPlayer->GetCID(), WEAPONPRESSED)*5;
		
	if(m_pPlayer->GetCharacter())
	{
		// 给boss武器 
		if(m_pPlayer->GetBotType() == BOT_BOSSSLIME)
		{
			Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_SHOTGUN, 99999);
			Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_SHOTGUN, 10);
			Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_SHOTGUN, 4000);

			Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_GRENADE, 99999);
			Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_GRENADE, 10);
			Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_GRENADE, 4000);

			GiveWeapon(WEAPON_GRENADE, -1);
			GiveWeapon(WEAPON_SHOTGUN, -1);
			return;
		}
		
		// Рисовка артифактов
		if(m_pPlayer->m_BigBot || Server()->GetItemCount(m_pPlayer->GetCID(), SNAPAMMOREGEN))
			new CSnapFullProject(GameWorld(), m_Pos, m_pPlayer->GetCID(), 9, WEAPON_HAMMER, true);
			
		if(Server()->GetItemCount(m_pPlayer->GetCID(), SPECSNAPDRAW))
			new CSnapFullProject(GameWorld(), m_Pos, m_pPlayer->GetCID(), 6, WEAPON_SHOTGUN, true);

		if(Server()->GetItemSettings(m_pPlayer->GetCID(), RAREEVENTHAMMER))
			new CSnapFullProject(GameWorld(), m_Pos, m_pPlayer->GetCID(), 3, 4, true);
		else if(Server()->GetItemCount(m_pPlayer->GetCID(), SNAPDAMAGE))
			new CSnapFullProject(GameWorld(), m_Pos, m_pPlayer->GetCID(), 3, WEAPON_GRENADE, true);
			
		if(Server()->GetItemCount(m_pPlayer->GetCID(), SNAPHANDLE))
			new CSnapFullProject(GameWorld(), m_Pos, m_pPlayer->GetCID(), 5, 1, true);
	}
	
	Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_HAMMER, 10000);	
	if(Server()->GetItemSettings(m_pPlayer->GetCID(), LAMPHAMMER)) Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_HAMMER, 1200);
	else Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_HAMMER, getsp);

	Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_HAMMER, 0);
	
	Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_GUN, geta);
	Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_GUN, getsp);
	Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_GUN, 500);
	
	Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_SHOTGUN, geta);
	Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_SHOTGUN, getsp);
	Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_SHOTGUN, getar);
	
	Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_GRENADE, geta);

	if(Server()->GetItemSettings(m_pPlayer->GetCID(), PIZDAMET)) Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_GRENADE, 7000);
	else Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_GRENADE, getspg);

	Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_GRENADE, getar);
	
	Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_RIFLE, geta);
	Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_RIFLE, getsp);
	Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_RIFLE, getar);
	
	Server()->SetMaxAmmo(m_pPlayer->GetCID(), INFWEAPON_NONE, -1);
	Server()->SetFireDelay(m_pPlayer->GetCID(), INFWEAPON_NONE, 0);
	Server()->SetAmmoRegenTime(m_pPlayer->GetCID(), INFWEAPON_NONE, 0);
	
	// 发放武器
	if(Server()->GetItemCount(m_pPlayer->GetCID(), IGUN) || Server()->GetItemCount(m_pPlayer->GetCID(), WEAPONPRESSED))
		GiveWeapon(WEAPON_GUN, geta);
	if(Server()->GetItemCount(m_pPlayer->GetCID(), ISHOTGUN) || Server()->GetItemCount(m_pPlayer->GetCID(), WEAPONPRESSED))
		GiveWeapon(WEAPON_SHOTGUN, geta);
	if(Server()->GetItemCount(m_pPlayer->GetCID(), IGRENADE) || Server()->GetItemCount(m_pPlayer->GetCID(), WEAPONPRESSED))
		GiveWeapon(WEAPON_GRENADE, geta);
	if(Server()->GetItemCount(m_pPlayer->GetCID(), ILASER) || Server()->GetItemCount(m_pPlayer->GetCID(), WEAPONPRESSED))
		GiveWeapon(WEAPON_RIFLE, geta);

	m_aWeapons[WEAPON_HAMMER].m_Got = true;
	GiveWeapon(WEAPON_HAMMER, -1);
	m_ActiveWeapon = WEAPON_HAMMER;
}

void CCharacter::DestroyChildEntities()
{
	m_NinjaVelocityBuff = 0;
	m_NinjaStrengthBuff = 0;
	m_NinjaAmmoBuff = 0;

	m_HookMode = 0;
	m_PositionLockTick = 0;
	m_PositionLocked = false;
	m_PositionLockAvailable = false;
}

void CCharacter::SetClass(int ClassChoosed)
{
	ClassSpawnAttributes();
	DestroyChildEntities();
	
	m_QueuedWeapon = -1;
	GameServer()->CreatePlayerSpawn(m_Pos);
}

void CCharacter::SlipperyEffect()
{
	if(m_SlipperyTick <= 0)
		m_SlipperyTick = Server()->TickSpeed()/2;
}

void CCharacter::Freeze(float Time)
{
	if(m_IsFrozen)
		return;
		
	m_IsFrozen = true;
	m_FrozenTime = Server()->TickSpeed()*Time;
}

void CCharacter::Unfreeze()
{
	m_IsFrozen = false;
	m_FrozenTime = -1;
	GameServer()->CreatePlayerSpawn(m_Pos);
}

void CCharacter::Poison(int Count, int From)
{
	if(m_Poison <= 0)
	{
		m_PoisonTick = 0;
		m_Poison = Count;
		m_PoisonFrom = From;
	}
}

bool CCharacter::IsFrozen() const { return m_IsFrozen; }
int CCharacter::GetInfWeaponID(int WID)
{
	if(WID == WEAPON_HAMMER)
	{
		switch(GetClass())
		{
			default: return INFWEAPON_HAMMER;
		}
	}
	else if(WID == WEAPON_GUN)
	{
		switch(GetClass())
		{
			default: return INFWEAPON_GUN;
		}
		return INFWEAPON_GUN;
	}
	else if(WID == WEAPON_SHOTGUN)
	{
		switch(GetClass())
		{
			default: return INFWEAPON_SHOTGUN;
		}
	}
	else if(WID == WEAPON_GRENADE)
	{
		switch(GetClass())
		{
			default: return INFWEAPON_GRENADE;
		}
	}
	else if(WID == WEAPON_RIFLE)
	{
		switch(GetClass())
		{
			default: return INFWEAPON_RIFLE;
		}
	}
	else if(WID == WEAPON_NINJA) return INFWEAPON_NINJA;
	else return INFWEAPON_NONE;
}

void CCharacter::CreateDropItem(int ItemID, int Count, int HowID, int Enchant)
{
	if(!IsAlive())
		return; 
	
	vec2 Direction = normalize(vec2(m_LatestInput.m_TargetX, m_LatestInput.m_TargetY));
	vec2 ProjStartPos = (Direction * max(0.001f, 2.0f));

	new CDropItem(GameWorld(), m_Pos-normalize(m_Pos-ProjStartPos)*m_ProximityRadius*1.5f, Direction/5, ItemID, Count, HowID, Enchant);
}

void CCharacter::CreateDropRandom(int ItemID, int Count, int Random, int HowID, vec2 Force)
{
	if(!IsAlive())
		return; 
	
	if(Random == false) 
	{
		new CDropItem(GameWorld(), m_Pos, Force, ItemID, Count, HowID, 0);		
		return;
	}
	if(random_prob(1.0f/(float)Random)) 
		new CDropItem(GameWorld(), m_Pos, Force, ItemID, Count, HowID, 0);
}

void CCharacter::TakeItemChar(int ClientID)
{
	if(!IsAlive() || m_ReloadTimer || m_pPlayer->IsBot())
		return;

	for(auto *pDrop = (CDropItem*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_DROPITEM); pDrop; pDrop = (CDropItem*) pDrop->TypeNext()) {
        if (pDrop) {
            if (distance(pDrop->m_Pos, m_Pos) < 40) {
                m_ReloadTimer = Server()->TickSpeed();
                if (pDrop->TakeItem(ClientID)) {
                    pDrop->Reset();
                    return;
                }
            }
        }
    }
}

void CCharacter::ParseEmoticionButton(int ClientID, int Emtion)
{
	if(!IsAlive() || m_ReloadTimer || m_pPlayer->m_InArea)
		return;

	m_ReloadTimer = Server()->TickSpeed()/3;
	if(Server()->GetItemCount(ClientID, SKWALL) && Server()->GetItemSettings(ClientID, SKWALL) == Emtion)
	{
		if(m_pPlayer->m_Mana < 30)
			return GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("你的魔能不足."), NULL);

		vec2 Direction = normalize(vec2(m_LatestInput.m_TargetX, m_LatestInput.m_TargetY));		
		vec2 To = m_Pos + Direction*1600.0f;
		if(GameServer()->Collision()->IntersectLine(m_Pos, To, 0x0, &To))
		{
			m_pPlayer->m_Mana -= 30;

			int NumMines = 0;
			for(auto *pMine = (CBiologistMine*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_BIOLOGIST_MINE); pMine; pMine = (CBiologistMine*) pMine->TypeNext())
			{
				if(pMine->m_Owner == m_pPlayer->GetCID())
					NumMines++;
			}
			if(NumMines < 5) new CBiologistMine(GameWorld(), m_Pos, To, m_pPlayer->GetCID(), 280);
			else GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("光墙已达最大数量."), NULL);
		}
	}
	else if(Server()->GetItemCount(ClientID, SKHEAL) && Server()->GetItemSettings(ClientID, SKHEAL) == Emtion)
	{
		if(m_pPlayer->m_Mana < 50)
			return GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("你的魔能不足."), NULL);

		m_pPlayer->m_Mana -= 50;
		m_ReloadTimer = Server()->TickSpeed()/3;
		for(auto *pHeal = (CHealthHealer*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_HEALTHHEALER); pHeal; pHeal = (CHealthHealer*) pHeal->TypeNext())
		{
			if(pHeal->m_Owner == m_pPlayer->GetCID())
				pHeal->Reset();
		}
		new CHealthHealer(GameWorld(), m_Pos, m_pPlayer->GetCID(), 30, 1000);
	}
	else if(Server()->GetItemCount(ClientID, SSWORD) && Server()->GetItemSettings(ClientID, SSWORD) == Emtion)
	{
		for(auto *pSword = (CSword*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_SWORD); pSword; pSword = (CSword*) pSword->TypeNext())
		{
			if(pSword->m_Owner == m_pPlayer->GetCID())
			{
				pSword->Reset();
				GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("光剑已关闭."), NULL);
				return;
			}
		}
		new CSword(GameWorld(), m_Pos, m_pPlayer->GetCID());
	}
	else if(Server()->GetItemCount(ClientID, SHEALSUMMER) && Server()->GetItemSettings(ClientID, SHEALSUMMER) == Emtion)
	{
		if(m_pPlayer->m_Mana < 150)
			return GameServer()->SendChatTarget_Localization(m_pPlayer->GetCID(), CHATCATEGORY_DEFAULT, _("你的魔能不足,需要150."), NULL);

		m_pPlayer->m_Mana -= 150;
		for(int i = 0; i < 40; ++i)
			new CBuff(GameWorld(), vec2(m_Pos.x-250+random_int(0, 500), m_Pos.y-200+random_int(0, 400)), m_pPlayer->GetCID());

		// Find other players
		for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
		{
			if(p->GetPlayer() && p)
			{
				if(!p->GetPlayer()->IsBot() && distance(p->m_Pos, m_Pos) < 500) 
				{
					if(p->GetPlayer()->m_Health < p->GetPlayer()->m_HealthStart)
						p->m_Health += 1000;
						
					GameServer()->CreateDeath(p->m_Pos, p->GetPlayer()->GetCID());
				}
			}
		}
	}
}

void CCharacter::PressF3orF4(int ClientID, int Vote)
{
	if(!IsAlive())
		return;

	if (Vote == 1)
	{
		if(Server()->GetItemSettings(ClientID, SDROP))
			TakeItemChar(ClientID);	

		ParseEmoticionButton(ClientID, Vote);	
	}
	else ParseEmoticionButton(ClientID, 0);
}

void CCharacter::CreatePickupDraw(int Num, int Type, int SubType, bool Changing)
{
	if(!IsAlive())
		return;
	
	new CSnapFullPickup(GameWorld(), m_Pos, m_pPlayer->GetCID(), Num, Type, SubType, Changing);
}

void CCharacter::DeleteAllPickup()
{
	if(!IsAlive())
		return;

	for(CSnapFullPickup *pPick = (CSnapFullPickup*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_SNAP_FULLPICKUP); pPick; pPick = (CSnapFullPickup*) pPick->TypeNext())
	{
		if(pPick->m_Owner == m_pPlayer->GetCID())
			pPick->Reset();
	}	
}
