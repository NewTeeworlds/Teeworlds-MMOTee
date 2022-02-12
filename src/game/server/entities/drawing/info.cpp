/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include "info.h"

CInfo::CInfo(CGameWorld *pGameWorld, int Type, int ID, vec2 Pos)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_DRAW)
{
	m_Type = Type; // 0 = Clan info, 1 = Material info
	m_Pos = Pos;
	m_InfoID = ID;

	GameWorld()->InsertEntity(this);
}

void CInfo::Tick()
{

	if(Server()->Tick() % (1 * Server()->TickSpeed() * 5) == 0)
	{
		if (m_Type)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "Mat %d", Server()->GetMaterials(m_InfoID));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 250, aBuf);
		}
		else
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "House %s Top%d", Server()->GetClanName(Server()->GetTopHouse(m_InfoID)), (m_InfoID + 1));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 500, aBuf);
		}
	}
}

void CInfo::TickPaused()
{

}

void CInfo::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;
}
