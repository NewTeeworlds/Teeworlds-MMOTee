/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/math.h>
#include <base/vmath.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>

#include "bonus.h"

CBonus::CBonus(CGameWorld *pGameWorld, vec2 Pos, vec2 Dir, int Type, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_BONUS)
{
	m_Pos = Pos;
	m_ActualPos = Pos;
	m_ActualDir = Dir;
	m_Direction = Dir;
	m_Direction.y += 0.01f;

	m_Type = Type;
	m_GetLevel = 1;
	
	if(Owner > -1 && Owner < MAX_CLIENTS)
	{
		if(GameServer()->m_apPlayers[Owner])
			m_GetLevel = GameServer()->m_apPlayers[Owner]->AccData.Level*100;
	}
	
	m_Flashing = false;
	m_StartTick = Server()->Tick();
	m_LifeSpan = Server()->TickSpeed()*20;
	m_SpawnTick = -1;
	GameWorld()->InsertEntity(this);
}

void CBonus::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

vec2 CBonus::GetPos(float Time)
{
	float Curvature = GameServer()->Tuning()->m_ShotgunCurvature;
	float Speed = GameServer()->Tuning()->m_ShotgunSpeed*2+250;

	return CalcPos(m_Pos, m_Direction, Curvature, Speed, Time);
}

void CBonus::Tick()
{
	float Pt = (Server()->Tick()-m_StartTick-1)/(float)Server()->TickSpeed();
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();
	vec2 PrevPos = GetPos(Pt);
	vec2 CurPos = GetPos(Ct);
	
	m_ActualPos = CurPos;
	m_ActualDir = normalize(CurPos - PrevPos);
	
	if(m_LifeSpan < 100)
	{
		m_Flashing = true;
		if(m_LifeSpan < 0)
		{
			GameServer()->m_World.DestroyEntity(this);
			return;
		}
	}
	m_LifeSpan--;
	
	// a small visual effect before pickup disappears
	if (m_Flashing)
	{
		m_FlashTimer--;
		
		if (m_FlashTimer <= 0)
			m_FlashTimer = 20;
			
		if (m_FlashTimer > 10)
			m_SpawnTick = 999;
		else
			m_SpawnTick = -1;
	}
	
	CCharacter *TargetChr = GameServer()->m_World.IntersectCharacter(PrevPos, CurPos, 5.0f, CurPos);
	if(TargetChr && TargetChr->GetPlayer() && !TargetChr->GetPlayer()->IsBot())
	{
		bool Delete = true;
		int ClientID = TargetChr->GetPlayer()->GetCID();
		switch(m_Type)
		{
			case 0:
			{
				TargetChr->GetPlayer()->MoneyAdd((int)m_GetLevel, true, true);
				GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH);
			} break; 
			case 1:
			{
				int Get = (int)((m_GetLevel/100)/2);
				TargetChr->GetPlayer()->ExpAdd(Get);
				GameServer()->CreateSound(m_Pos, SOUND_PICKUP_ARMOR);
			} break; 
			case 2:
			{
				if(!Server()->GetItemCount(TargetChr->GetPlayer()->GetCID(), ISHOTGUN))
				{
					Delete = false;
					GameServer()->SendBroadcast_LStat(ClientID, 100, 20, NOTWEAPON);
				}
				else
				{
					TargetChr->GiveWeapon(WEAPON_SHOTGUN, rand()%8+1, true);
					GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN);
				}
			} break; 
			case 3:
			{	
				if(!Server()->GetItemCount(TargetChr->GetPlayer()->GetCID(), IGRENADE))
				{
					Delete = false;
					GameServer()->SendBroadcast_LStat(ClientID, 100, 20, NOTWEAPON);
				}
				else
				{
					TargetChr->GiveWeapon(WEAPON_GRENADE, rand()%8+1, true);
					GameServer()->CreateSound(m_Pos, SOUND_PICKUP_GRENADE);
				}
			} break; 
			case 4:
			{
				if(!Server()->GetItemCount(TargetChr->GetPlayer()->GetCID(), ILASER))
				{
					Delete = false;
					GameServer()->SendBroadcast_LStat(ClientID, 100, 20, NOTWEAPON);
				}
				else
				{				
					TargetChr->GiveWeapon(WEAPON_RIFLE, rand()%8+1, true);
					GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN);
				}
			} break; 
			case 5:
			{
				TargetChr->GetPlayer()->ExpAdd(m_GetLevel/2);
				GameServer()->CreateSound(m_Pos, SOUND_PICKUP_ARMOR);
			} break;
		}
		
		if(Delete)
		{
			GameServer()->m_World.DestroyEntity(this);
			return;
		}
	}

	vec2 LastPos;
	int Collide = GameServer()->Collision()->IntersectLine(PrevPos, CurPos, NULL, &LastPos);
	if(Collide)
	{			
		//Thanks to TeeBall 0.6
		vec2 CollisionPos;
		CollisionPos.x = LastPos.x;
		CollisionPos.y = CurPos.y;
		int CollideY = GameServer()->Collision()->IntersectLine(PrevPos, CollisionPos, NULL, NULL);
		CollisionPos.x = CurPos.x;
		CollisionPos.y = LastPos.y;
		int CollideX = GameServer()->Collision()->IntersectLine(PrevPos, CollisionPos, NULL, NULL);

		m_Pos = LastPos;
		m_ActualPos = m_Pos;
		vec2 vel;
		vel.x = m_Direction.x;
		vel.y = m_Direction.y + 2*GameServer()->Tuning()->m_ShotgunCurvature/10000*Ct*(GameServer()->Tuning()->m_ShotgunSpeed*2+250);
		
		if (CollideX && !CollideY)
		{
			m_Direction.x = -vel.x;
			m_Direction.y = vel.y;
		}
		else if (!CollideX && CollideY)
		{
			m_Direction.x = vel.x;
			m_Direction.y = -vel.y;
		}
		else
		{
			m_Direction.x = -vel.x;
			m_Direction.y = -vel.y;
		}
		
		m_Direction.x *= (100 - 50) / 100.0;
		m_Direction.y *= (100 - 50) / 100.0;
		m_StartTick = Server()->Tick();
		
		m_ActualDir = normalize(m_Direction);
	}
}

void CBonus::TickPaused()
{
	m_StartTick++;
}

void CBonus::Snap(int SnappingClient)
{
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();
	
	if(m_SpawnTick != -1 || NetworkClipped(SnappingClient, GetPos(Ct)))
		return;

	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pP)
		return;

	pP->m_X = (int)m_ActualPos.x+m_ActualDir.x;
	pP->m_Y = (int)m_ActualPos.y+m_ActualDir.y;
	if(m_Type <= 1)
	{
		pP->m_Type = m_Type;
		pP->m_Subtype = 0;
	}
	else if(m_Type > 1 && m_Type <= 4)
	{
		pP->m_Type = POWERUP_WEAPON;
		pP->m_Subtype = m_Type;	
	}
	else
	{
		pP->m_Type = POWERUP_WEAPON;
		pP->m_Subtype = WEAPON_HAMMER;	
	}
}
