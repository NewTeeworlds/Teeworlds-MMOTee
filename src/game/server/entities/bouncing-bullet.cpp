/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/math.h>
#include <base/vmath.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>

#include "bouncing-bullet.h"
#include "biologist-mine.h"

CBouncingBullet::CBouncingBullet(CGameWorld *pGameWorld, int Owner, vec2 Pos, vec2 Dir, bool Explosive, int Weapon, int LifeSpan)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_BOUNCING_BULLET)
{
	m_Pos = Pos;
	m_ActualPos = Pos;
	m_ActualDir = Dir;
	m_Direction = Dir;
	m_Owner = Owner;
	m_StartTick = Server()->Tick();
	m_LifeSpan = LifeSpan;
	m_Explosive = Explosive;
	m_BounceLeft = Weapon == WEAPON_GRENADE ? 10 : 3; // the number of time that a bullet can bounce. It's usefull to remove bullets laying on the ground
	m_DistanceLeft = Weapon == WEAPON_GRENADE ? 2400 : 1200; // the max distance a bullet can travel
	m_Weapon = Weapon;

	GameWorld()->InsertEntity(this);
}

void CBouncingBullet::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

vec2 CBouncingBullet::GetPos(float Time)
{
	float Curvature = GameServer()->Tuning()->m_ShotgunCurvature;
	float Speed = GameServer()->Tuning()->m_ShotgunSpeed;
	switch(m_Weapon)
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

void CBouncingBullet::TickPaused()
{
	m_StartTick++;
}

void CBouncingBullet::Tick()
{
	float Pt = (Server()->Tick()-m_StartTick-1)/(float)Server()->TickSpeed();
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();
	vec2 PrevPos = GetPos(Pt);
	vec2 CurPos = GetPos(Ct);
	
	m_ActualPos = CurPos;
	m_ActualDir = normalize(CurPos - PrevPos);

	m_DistanceLeft -= distance(CurPos, PrevPos);
	m_LifeSpan--;

	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	if(!GameServer()->m_apPlayers[m_Owner] || !OwnerChar || GameLayerClipped(CurPos) || m_LifeSpan < 0 || m_BounceLeft < 0 || m_DistanceLeft < 0.0f)
	{
		if(GameServer()->m_apPlayers[m_Owner] && OwnerChar && Server()->GetItemSettings(m_Owner, ENDEXPLOSION))
			GameServer()->CreateExplosion(CurPos, m_Owner, WEAPON_SHOTGUN, false, 0);

		GameServer()->m_World.DestroyEntity(this);
		return;
	}
	
	for(CBiologistMine* pMine = (CBiologistMine*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_BIOLOGIST_MINE); pMine; pMine = (CBiologistMine*) pMine->TypeNext())
	{
		vec2 IntersectPos = closest_point_on_line(pMine->m_Pos, pMine->m_EndPos, CurPos);
		if(distance(CurPos, IntersectPos) < 30)
		{
			if(m_Explosive)
				GameServer()->CreateExplosion(CurPos, m_Owner, m_Weapon, false, 0);

			pMine->m_Health -= 10;			
			GameServer()->m_World.DestroyEntity(this);
			return;
		}
	}

	CCharacter *TargetChr = GameServer()->m_World.IntersectCharacter(PrevPos, CurPos, 6.0f, CurPos, OwnerChar);
	if(TargetChr)
	{
		if(OwnerChar)
		{
			if(m_Explosive)
				GameServer()->CreateExplosion(CurPos, m_Owner, WEAPON_SHOTGUN, false, 0);
			else
				TargetChr->TakeDamage(m_Direction * max(0.001f, 2.0f), (random_prob(0.33f) ? 2 : 1), m_Owner, WEAPON_SHOTGUN, TAKEDAMAGEMODE_NOINFECTION);
		}

		GameServer()->m_World.DestroyEntity(this);
	}
	else
	{
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
			vel.y = m_Direction.y + 2*GameServer()->Tuning()->m_ShotgunCurvature/10000*Ct*GameServer()->Tuning()->m_ShotgunSpeed;
			
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
			m_BounceLeft--;
		}
	}
}

void CBouncingBullet::FillInfo(CNetObj_Projectile *pProj)
{
	pProj->m_X = (int)m_Pos.x;
	pProj->m_Y = (int)m_Pos.y;
	pProj->m_VelX = (int)(m_Direction.x*100.0f);
	pProj->m_VelY = (int)(m_Direction.y*100.0f);
	pProj->m_StartTick = m_StartTick;
	pProj->m_Type = m_Weapon;
}

void CBouncingBullet::Snap(int SnappingClient)
{
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();

	if(NetworkClipped(SnappingClient, GetPos(Ct)))
		return;

	CNetObj_Projectile *pProj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
	if(pProj)
		FillInfo(pProj);
}
