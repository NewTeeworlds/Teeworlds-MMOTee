/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include "pizdamet.h"

CPizdamet::CPizdamet(CGameWorld *pGameWorld, vec2 Pos, int OwnerID)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_FLYINGPOINT)
{
	m_Pos = Pos;
	m_OwnerID = OwnerID;
	
	CPlayer *pOwner = GameServer()->m_apPlayers[m_OwnerID];
	if(pOwner && pOwner->GetCharacter())
		m_Cursour = vec2(pOwner->GetCharacter()->m_Core.m_Input.m_TargetX+rand()%50-rand()%100, pOwner->GetCharacter()->m_Core.m_Input.m_TargetY+rand()%50-rand()%100);

	GameWorld()->InsertEntity(this);
}

void CPizdamet::Tick()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_OwnerID];
	if((!pOwner || !pOwner->GetCharacter()))
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	vec2 To = m_Pos + normalize(m_Cursour) * 15.0f;
	if(GameServer()->Collision()->IntersectLine(m_Pos, To, 0x0, &To))
	{
		GameServer()->CreateExplosion(m_Pos, m_OwnerID, WEAPON_GRENADE, false, TAKEDAMAGEMODE_INFECTION);
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	if(Server()->Tick() % (1 * Server()->TickSpeed()) == 0)
	{
		GameServer()->CreatePlayerSpawn(m_Pos);
	}

	// Find other players
	for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
	{
		if(p->GetPlayer() && p && p->GetPlayer()->GetCID() != m_OwnerID && distance(p->m_Pos, m_Pos) < 30) 
		{
			GameServer()->CreateDeath(m_Pos, p->GetPlayer()->GetCID());
			GameServer()->CreateExplosion(m_Pos, m_OwnerID, WEAPON_GRENADE, false, TAKEDAMAGEMODE_INFECTION);
			GameServer()->m_World.DestroyEntity(this);
			return;
		}
	}
	m_Pos += normalize(m_Cursour) * 25.0f;
}

void CPizdamet::Snap(int SnappingClient)
{
	CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_Type = 0;
	pObj->m_Subtype = 0;
}
