/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/math.h>
#include <base/vmath.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "biologist-mine.h"
#include "projectile.h"

CProjectile::CProjectile(CGameWorld *pGameWorld, int Type, int Owner, vec2 Pos, vec2 Dir, int Span,
		int Damage, bool Explosive, float Force, int SoundImpact, int Weapon, int TakeDamageMode)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Type = Type;
	m_Pos = Pos;
	m_Direction = Dir;
	m_LifeSpan = Span;
	m_Owner = Owner;
	m_Force = Force;
	m_Damage = Damage;
	m_SoundImpact = SoundImpact;
	m_Weapon = Weapon;
	m_StartTick = Server()->Tick();
	m_Explosive = Explosive;

	GameWorld()->InsertEntity(this);
	
/* INFECTION MODIFICATION START ***************************************/
	m_IsFlashGrenade = false;
	m_IsPortal = false;
	m_StartPos = Pos;
/* INFECTION MODIFICATION END *****************************************/
}

void CProjectile::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

vec2 CProjectile::GetPos(float Time)
{
	float Curvature = 0;
	float Speed = 0;

	switch(m_Type)
	{
		case WEAPON_GRENADE:
			Curvature = GameServer()->Tuning()->m_GrenadeCurvature;
			Speed = GameServer()->Tuning()->m_GrenadeSpeed;
			break;

		case WEAPON_SHOTGUN:
			Curvature = GameServer()->Tuning()->m_ShotgunCurvature;
			Speed = GameServer()->Tuning()->m_ShotgunSpeed;
			break;

		case WEAPON_GUN:
			Curvature = GameServer()->Tuning()->m_GunCurvature;
			Speed = GameServer()->Tuning()->m_GunSpeed;
			break;
	}

	return CalcPos(m_Pos, m_Direction, Curvature, Speed, Time);
}


void CProjectile::Tick()
{
	float Pt = (Server()->Tick()-m_StartTick-1)/(float)Server()->TickSpeed();
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();
	vec2 PrevPos = GetPos(Pt);
	vec2 CurPos = GetPos(Ct);
	int Collide = GameServer()->Collision()->IntersectLine(PrevPos, CurPos, &CurPos, 0);
	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	CCharacter *TargetChr = GameServer()->m_World.IntersectCharacter(PrevPos, CurPos, 6.0f, CurPos, OwnerChar);

	m_LifeSpan--;

	if(!GameServer()->m_apPlayers[m_Owner] || !OwnerChar)
	{
		GameServer()->m_World.DestroyEntity(this);
		return;	
	}

	bool Auto = false;
	if((OwnerChar && m_Weapon == WEAPON_GUN && Server()->GetItemSettings(m_Owner, GHOSTGUN) && Collide) ||
		(OwnerChar && m_Weapon == WEAPON_SHOTGUN && Server()->GetItemSettings(m_Owner, GHOSTSHOTGUN) && Collide) ||
		(OwnerChar && m_Weapon == WEAPON_GRENADE && Server()->GetItemSettings(m_Owner, GHOSTGRENADE) && Collide))
	{	
		Auto = true;
		
		if(Server()->Tick() % (1 * Server()->TickSpeed()) == 0)
			GameServer()->CreateDamageInd(CurPos, 0, 1);
	}

	for(CBiologistMine* pMine = (CBiologistMine*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_BIOLOGIST_MINE); pMine; pMine = (CBiologistMine*) pMine->TypeNext())
	{
		vec2 IntersectPos = closest_point_on_line(pMine->m_Pos, pMine->m_EndPos, CurPos);
		if(distance(CurPos, IntersectPos) < 30)
		{
			if(m_Explosive)
				GameServer()->CreateExplosion(CurPos, m_Owner, m_Weapon, false, m_TakeDamageMode);

			pMine->m_Health -= 10;			
			GameServer()->m_World.DestroyEntity(this);
			return;
		}
	}

/* INFECTION MODIFICATION START ***************************************/
	if(TargetChr || Collide || m_LifeSpan < 0 || GameLayerClipped(CurPos))
	{
		if(Auto)
		{
			if(GameServer()->Collision()->IsTileNoHook(CurPos.x, CurPos.y))
				GameServer()->m_World.DestroyEntity(this);
		}
		
		if((m_Weapon == WEAPON_GRENADE && !Auto))
			GameServer()->CreateSound(CurPos, m_SoundImpact);

		if(m_Explosive && !Auto)
		{
			GameServer()->CreateExplosion(CurPos, m_Owner, m_Weapon, false, m_TakeDamageMode);
		}
		else if(TargetChr)
		{
			if(OwnerChar)
			{
				TargetChr->TakeDamage(m_Direction * max(0.001f, m_Force), m_Damage, m_Owner, m_Weapon, m_TakeDamageMode);
			}
		}
		
		if(!Auto)
			GameServer()->m_World.DestroyEntity(this);

		return;
	}
	
/* INFECTION MODIFICATION END *****************************************/
}

void CProjectile::TickPaused()
{
	++m_StartTick;
}

void CProjectile::FillInfo(CNetObj_Projectile *pProj)
{
	pProj->m_X = (int)m_Pos.x;
	pProj->m_Y = (int)m_Pos.y;
	pProj->m_VelX = (int)(m_Direction.x*100.0f);
	pProj->m_VelY = (int)(m_Direction.y*100.0f);
	pProj->m_StartTick = m_StartTick;
	pProj->m_Type = m_Type;
}

void CProjectile::Snap(int SnappingClient)
{
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();

	if(NetworkClipped(SnappingClient, GetPos(Ct)))
		return;

	CNetObj_Projectile *pProj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
	if(pProj)
		FillInfo(pProj);
}

/* INFECTION MODIFICATION START ***************************************/
void CProjectile::FlashGrenade()
{
	m_IsFlashGrenade = true;
}

void CProjectile::Portal()
{
	m_IsPortal = true;
}
/* INFECTION MODIFICATION END *****************************************/
