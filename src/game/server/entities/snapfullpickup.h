/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_SNAP_FULLPICKUP_H
#define GAME_SERVER_ENTITIES_SNAP_FULLPICKUP_H

#include <game/server/entity.h>
#include <base/tl/array.h>

class CSnapFullPickup : public CEntity
{
public:
	enum
	{
		NUM_SIDE = 64,
		NUM_IDS = NUM_SIDE,
	};
	
public:
	CSnapFullPickup(CGameWorld *pGameWorld, vec2 Pos, int Owner, int Num, int Type, int SubType, bool Changing);
	~CSnapFullPickup();

	virtual void Snap(int SnappingClient);
	virtual void Tick();
	virtual void Reset();

private:
	int m_IDs[NUM_IDS];
	
public:
	int m_LoadingTick;
	int m_Owner; 
	int m_Num;
	int m_Type; 
	int m_SubType;
	bool m_boolreback;
	bool m_Changing;
};

#endif
