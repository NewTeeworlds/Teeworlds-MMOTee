/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include "flyingpoint.h"

CFlyingPoint::CFlyingPoint(CGameWorld *pGameWorld, vec2 Pos, int OwnerID, int GetPlayer, vec2 InitialVel)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_FLYINGPOINT)
{
	m_Pos = Pos;
	m_InitialVel = InitialVel;
	m_OwnerID = OwnerID;
	m_GetPlayer = GetPlayer;
	m_InitialAmount = 1.0f;
	GameWorld()->InsertEntity(this);
}

void CFlyingPoint::Tick()
{
	CPlayer *pGetPlayer = GameServer()->m_apPlayers[m_GetPlayer];
	CPlayer *pOwner = GameServer()->m_apPlayers[m_OwnerID];
	if((!pOwner || !pOwner->GetCharacter()) || (!pGetPlayer || !pGetPlayer->GetCharacter()))
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	float Dist = distance(m_Pos, pOwner->GetCharacter()->m_Pos);
	if(Dist < 24.0f)
	{
		if(pOwner->IsBot())
			GameServer()->CreateExplosion(pOwner->GetCharacter()->m_Pos, m_GetPlayer, WEAPON_HAMMER, false, 10);
		else if(pOwner->m_AngryWroth < 250)
			pOwner->m_AngryWroth++;	

		GameServer()->m_World.DestroyEntity(this);
		return;
	}
	vec2 Dir = normalize(pOwner->GetCharacter()->m_Pos - m_Pos);
	m_Pos += Dir*clamp(Dist, 0.0f, 16.0f) * (1.0f - m_InitialAmount) + m_InitialVel * m_InitialAmount;
	
	m_InitialAmount *= 0.98f;
}

void CFlyingPoint::Snap(int SnappingClient)
{
	CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
	if(pObj)
	{
		pObj->m_X = (int)m_Pos.x;
		pObj->m_Y = (int)m_Pos.y;
		pObj->m_VelX = 0;
		pObj->m_VelY = 0;
		pObj->m_StartTick = Server()->Tick();
		pObj->m_Type = WEAPON_HAMMER;
	}
}
