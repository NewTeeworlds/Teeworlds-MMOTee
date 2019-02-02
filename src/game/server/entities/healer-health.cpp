/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>

#include "healer-health.h"
#include "hearth.h"

CHealthHealer::CHealthHealer(CGameWorld *pGameWorld, vec2 Pos, int Owner, int ReloadTick, int LifeSpan)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_HEALTHHEALER)
{
	m_Pos = Pos;
	m_Owner = Owner;
	m_ReloadTick = m_SaveReloadTick = ReloadTick;
	m_LifeSpan = LifeSpan;

	GameWorld()->InsertEntity(this);	
	for(int i=0; i<NUM_IDS; i++)
	{
		m_IDs[i] = Server()->SnapNewID();
	}
}

CHealthHealer::~CHealthHealer()
{
	for(int i=0; i<NUM_IDS; i++)
	{
		Server()->SnapFreeID(m_IDs[i]);
	}
}

void CHealthHealer::Reset()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_Owner];
	if(pOwner && pOwner->GetCharacter())
	{
		GameServer()->CreateExplosionDisk(m_Pos, 60.0f, 87.5f, 200, 10.0f, m_Owner, WEAPON_GRENADE, TAKEDAMAGEMODE_INFECTION);
		GameServer()->CreateSound(m_Pos, SOUND_GRENADE_EXPLODE);
	}
	GameServer()->m_World.DestroyEntity(this);
	return;
}

void CHealthHealer::Tick()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_Owner];
	if(!pOwner || !pOwner->GetCharacter() || !m_LifeSpan)
	{
		Reset();
		return;
	}
	
	m_LifeSpan--;
	m_ReloadTick--;
	if(!m_ReloadTick)
	{
		// Find other players
		for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
		{
			if(p->GetPlayer() && p)
			{
				if(!p->GetPlayer()->IsBot() && distance(p->m_Pos, m_Pos) < 300 && !GameServer()->Collision()->IntersectLine(m_Pos, p->m_Pos, 0x0, &p->m_Pos)) 
					new CHearth(&GameServer()->m_World, m_Pos, p->GetPlayer()->GetCID(), p->m_Core.m_Vel);
			}
		}
		m_ReloadTick = m_SaveReloadTick;
	}
}

void CHealthHealer::Snap(int SnappingClient)
{
	float AngleStep = 2.0f * pi / CHealthHealer::NUM_SIDE;
	float Radius = 50.0f-(int)m_ReloadTick;
	for(int i=0; i<CHealthHealer::NUM_SIDE; i++)
	{
		vec2 VertexPos = m_Pos + vec2(Radius * cos(AngleStep*i), Radius * sin(AngleStep*i));
		CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_IDs[i], sizeof(CNetObj_Pickup)));
		if(!pObj)
			return;

		pObj->m_X = (int)VertexPos.x;
		pObj->m_Y = (int)VertexPos.y;
		pObj->m_Type = 1;
		pObj->m_Subtype = 0;
	}
	
	{	
		CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_IDs[CHealthHealer::NUM_SIDE], sizeof(CNetObj_Pickup)));
		if(!pObj)
			return;

		pObj->m_X = (int)m_Pos.x;
		pObj->m_Y = (int)m_Pos.y;
		pObj->m_Type = 0;
		pObj->m_Subtype = 0;
	}
}
