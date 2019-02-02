/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>
#include "buff.h"

CBuff::CBuff(CGameWorld *pGameWorld, vec2 Pos, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_BUFFS)
{
	m_Pos = Pos;
	GameWorld()->InsertEntity(this);
	m_LoadingTick = Server()->TickSpeed();
	m_Owner = Owner;
	m_Damage = 0;
}

void CBuff::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CBuff::Tick()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_Owner];
	if(!pOwner || !pOwner->GetCharacter() || !m_LoadingTick)
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}	

	m_LoadingTick--;
	if(!m_LoadingTick)
		GameServer()->CreateDeath(m_Pos, m_Owner);

	m_Pos.y -= 3;
}

void CBuff::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pP)
		return;

	float AngleStart = (2.0f * pi * Server()->Tick()/static_cast<float>(Server()->TickSpeed())/5.0f);
	float AngleStep = 2.0f * pi;
	float R = 50.0f;

	pP->m_X = (int)m_Pos.x + R * cos(AngleStart + AngleStep);
	pP->m_Y = (int)m_Pos.y;
	pP->m_Type = POWERUP_HEALTH;
	pP->m_Subtype = 0;
}