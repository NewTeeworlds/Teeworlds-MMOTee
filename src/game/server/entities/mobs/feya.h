/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_FEYA_H
#define GAME_SERVER_ENTITIES_FEYA_H

#include <game/server/entity.h>

class CFeya : public CEntity
{
public:
	enum
	{
		NUM_SIDE = 3,
		NUM_IDS,
	};
	
public:
	CFeya(CGameWorld *pGameWorld, vec2 Pos, int Type, int SubType, bool Proj);
	virtual ~CFeya();

	virtual void Snap(int SnappingClient);
	virtual void Reset();
	virtual void Tick();

private:
	int m_IDs[NUM_IDS];
	int m_Type;
	int m_SubType;
	bool m_Proj;
};

#endif
