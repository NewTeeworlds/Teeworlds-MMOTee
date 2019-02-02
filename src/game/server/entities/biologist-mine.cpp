/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>

#include "biologist-mine.h"
#include "biologist-laser.h"

CBiologistMine::CBiologistMine(CGameWorld *pGameWorld, vec2 Pos, vec2 EndPos, int Owner, int Health)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_BIOLOGIST_MINE)
{
	m_Pos = Pos;
	m_EndPos = EndPos;
	m_Owner = Owner;
	m_Health = Health;

	GameWorld()->InsertEntity(this);
	for(int i=0; i<NUM_IDS; i++)
	{
		m_IDs[i] = Server()->SnapNewID();
	}
}

CBiologistMine::~CBiologistMine()
{
	for(int i=0; i<NUM_IDS; i++)
	{
		Server()->SnapFreeID(m_IDs[i]);
	}
}

void CBiologistMine::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CBiologistMine::Snap(int SnappingClient)
{
	float AngleStep = 2.0f * pi / CBiologistMine::NUM_SIDE;
	float Radius = 16.0f;
	for(int i=0; i<CBiologistMine::NUM_SIDE; i++)
	{
		vec2 VertexPos = m_Pos + vec2(Radius * cos(AngleStep*i), Radius * sin(AngleStep*i));
		
		CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_IDs[i], sizeof(CNetObj_Projectile)));
		if(!pObj)
			return;

		pObj->m_X = (int)VertexPos.x;
		pObj->m_Y = (int)VertexPos.y;
		pObj->m_VelX = 0;
		pObj->m_VelY = 0;
		pObj->m_StartTick = Server()->Tick()-3;
		pObj->m_Type = WEAPON_GRENADE;
	}
	
	{
		CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_IDs[CBiologistMine::NUM_SIDE], sizeof(CNetObj_Laser)));
		if(!pObj)
			return;

		pObj->m_X = (int)m_EndPos.x;
		pObj->m_Y = (int)m_EndPos.y;
		pObj->m_FromX = (int)m_Pos.x;
		pObj->m_FromY = (int)m_Pos.y;
		pObj->m_StartTick = Server()->Tick()-4;
	}
}

void CBiologistMine::Tick()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_Owner];
	if(!pOwner || !pOwner->GetCharacter())
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}
	
	if(m_Health <= 0)
	{
		GameServer()->CreateExplosionDisk(m_Pos, 100.0f, 157.5f, 300, 22.0f, m_Owner, WEAPON_GRENADE, TAKEDAMAGEMODE_INFECTION);
		GameServer()->CreateSound(m_Pos, SOUND_GRENADE_EXPLODE);
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	// Find other players
	for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
	{
		if(p->GetPlayer()->GetCID() != m_Owner)
		{
			vec2 IntersectPos = closest_point_on_line(m_Pos, m_EndPos, p->m_Pos);
			float Len = distance(p->m_Pos, IntersectPos);
			if(Len < 20)
			{
				GameServer()->CreateExplosionDisk(IntersectPos, 200.0f, 267.5f, 500, 42.0f, m_Owner, WEAPON_GRENADE, 0);
				GameServer()->CreateSound(m_Pos, SOUND_GRENADE_EXPLODE);
				GameServer()->m_World.DestroyEntity(this);
				return;
			}
		}
	}
}
