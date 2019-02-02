/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include "info.h"

CInfo::CInfo(CGameWorld *pGameWorld, int Type, vec2 Pos)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_DRAW)
{
	m_Type = Type;
	m_Pos = Pos;

	GameWorld()->InsertEntity(this);
}

void CInfo::Tick()
{
	if(Server()->Tick() % (1 * Server()->TickSpeed() * 10) == 0 && m_Type < 3)
	{
		if(m_Type == 0)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "House %s Top1", Server()->GetClanName(Server()->GetTopHouse(0)));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 500, aBuf);
		}
		else if(m_Type == 1)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "House %s Top2", Server()->GetClanName(Server()->GetTopHouse(1)));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 500, aBuf);
		}
		else if(m_Type == 2)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "House %s Top3", Server()->GetClanName(Server()->GetTopHouse(2)));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 500, aBuf);
		}
	}
	else if(Server()->Tick() % (1 * Server()->TickSpeed() * 5) == 0 && m_Type >= 3)
	{
		if(m_Type == 3)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "Mat %d", Server()->GetMaterials(0));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 250, aBuf);			
		}
		else if(m_Type == 4)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "Mat %d", Server()->GetMaterials(1));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 250, aBuf);
		}
		else if(m_Type == 5)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "Mat %d", Server()->GetMaterials(2));
			GameServer()->CreateLolText(this, true, vec2(0,0), vec2 (0, 0), 250, aBuf);
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
