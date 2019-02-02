/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include "hearth.h"

CHearth::CHearth(CGameWorld *pGameWorld, vec2 Pos, int OwnerID, vec2 InitialVel)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_FLYINGPOINT)
{
	m_Pos = Pos;
	m_InitialVel = InitialVel;
	m_OwnerID = OwnerID;
	m_InitialAmount = 1.0f;
	GameWorld()->InsertEntity(this);
}

void CHearth::Tick()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_OwnerID];
	if((!pOwner || !pOwner->GetCharacter()))
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	float Dist = distance(m_Pos, pOwner->GetCharacter()->m_Pos);
	if(Dist < 24.0f)
	{
		GameServer()->CreateSound(m_Pos, 15);
		if(pOwner->m_Health <= pOwner->m_HealthStart)
			pOwner->GetCharacter()->m_Health += 100;		
		else GameServer()->CreateDeath(pOwner->GetCharacter()->m_Pos, m_OwnerID);

		GameServer()->m_World.DestroyEntity(this);
		return;
	}
	vec2 Dir = normalize(pOwner->GetCharacter()->m_Pos - m_Pos);
	m_Pos += Dir*clamp(Dist, 0.0f, 16.0f) * (1.0f - m_InitialAmount) + m_InitialVel * m_InitialAmount;
	
	m_InitialAmount *= 0.98f;
}

void CHearth::Snap(int SnappingClient)
{
	CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_Type = 0;
	pObj->m_Subtype = 0;
}
