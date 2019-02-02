/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_MOBEYBAG_H
#define GAME_SERVER_ENTITIES_MOBEYBAG_H

#include <game/server/entity.h>

class CMoneyBag : public CEntity
{
public:
	enum
	{
		NUM_SIDE = 2,
		NUM_IDS,
	};
	
public:
	CMoneyBag(CGameWorld *pGameWorld, int Type, vec2 Pos, int SubType);
	virtual ~CMoneyBag();

	virtual void Reset();
	virtual void Tick();
	virtual void TickPaused();
	virtual void Snap(int SnappingClient);
	virtual void Picking(int Time);
	int m_SpawnTick;

private:
	int m_IDs[NUM_IDS];
	int m_Type;
	int m_SubType;
	int m_TimeActive;
};

#endif
