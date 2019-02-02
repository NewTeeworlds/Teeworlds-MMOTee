/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_BIOLOGIST_MINE_H
#define GAME_SERVER_ENTITIES_BIOLOGIST_MINE_H

#include <game/server/entity.h>

class CBiologistMine : public CEntity
{
public:
	enum
	{
		NUM_SIDE = 5,
		NUM_PARTICLES = 1,
		NUM_IDS = NUM_SIDE + NUM_PARTICLES,
	};
	
public:
	CBiologistMine(CGameWorld *pGameWorld, vec2 Pos, vec2 EndPos, int Owner, int Health);
	virtual ~CBiologistMine();

	virtual void Snap(int SnappingClient);
	virtual void Reset();
	virtual void Tick();

private:
	int m_IDs[NUM_IDS];
	
public:
	int m_Owner;
	vec2 m_EndPos;
	int m_Health;
};

#endif
