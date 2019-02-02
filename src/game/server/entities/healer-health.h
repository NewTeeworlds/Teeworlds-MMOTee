/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_HEALTH_HEALER_H
#define GAME_SERVER_ENTITIES_HEALTH_HEALER_H

#include <game/server/entity.h>

class CHealthHealer : public CEntity
{
public:
	enum
	{
		NUM_SIDE = 6,
		NUM_PARTICLES = 1,
		NUM_IDS = NUM_SIDE + NUM_PARTICLES,
	};
	
public:
	CHealthHealer(CGameWorld *pGameWorld, vec2 Pos, int Owner, int ReloadTick, int LifeSpan);
	virtual ~CHealthHealer();

	virtual void Snap(int SnappingClient);
	virtual void Reset();
	virtual void Tick();

private:
	int m_IDs[NUM_IDS];
	int m_SaveReloadTick;
	int m_LifeSpan;	

public:
	int m_Owner;
	int m_ReloadTick;
};

#endif
