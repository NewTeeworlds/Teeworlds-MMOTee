/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_INFO_H
#define GAME_SERVER_ENTITIES_INFO_H

#include <game/server/entity.h>

class CInfo : public CEntity
{
public:
	CInfo(CGameWorld *pGameWorld, int Type, vec2 Pos);

	virtual void Tick();
	virtual void TickPaused();
	virtual void Snap(int SnappingClient);

private:
	int m_Type;
};

#endif
