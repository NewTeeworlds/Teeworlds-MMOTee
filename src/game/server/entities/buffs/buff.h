/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_BUFFS_H
#define GAME_SERVER_ENTITIES_BUFFS_H

#include <game/server/entity.h>
#include <base/tl/array.h>

class CBuff : public CEntity
{
public:
	CBuff(CGameWorld *pGameWorld, vec2 Pos, int Owner);

	virtual void Snap(int SnappingClient);
	virtual void Tick();
	virtual void Reset();
	
public:
	int m_LoadingTick;
	int m_Owner;
	int m_Damage;
};

#endif