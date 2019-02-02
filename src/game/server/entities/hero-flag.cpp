/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include "hero-flag.h"

CHeroFlag::CHeroFlag(CGameWorld *pGameWorld, vec2 Pos)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_HERO_FLAG)
{
	m_ProximityRadius = ms_PhysSize;
	m_Pos = Pos;
	GameWorld()->InsertEntity(this);
}

void CHeroFlag::Hide()
{

}

void CHeroFlag::Show()
{

}

void CHeroFlag::FindPosition()
{

}

void CHeroFlag::Tick()
{
	
}

void CHeroFlag::Snap(int SnappingClient)
{

	if(NetworkClipped(SnappingClient))
		return;

	CNetObj_Flag *pFlag = (CNetObj_Flag *)Server()->SnapNewItem(NETOBJTYPE_FLAG, TEAM_BLUE, sizeof(CNetObj_Flag));
	if(!pFlag)
		return;

	pFlag->m_X = (int)m_Pos.x;
	pFlag->m_Y = (int)m_Pos.y+16.0f;
	pFlag->m_Team = TEAM_BLUE;
}
