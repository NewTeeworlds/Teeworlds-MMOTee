#ifndef GAME_SERVER_ENTITIES_SWORDS_H
#define GAME_SERVER_ENTITIES_SWORDS_H

#include <game/server/entity.h>

class CSword : public CEntity
{
public:
	CSword(CGameWorld *pGameWorld, vec2 Pos, int Owner);

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);
	int m_Owner;
	vec2 PosLine;

private:
	int m_Lifetime;
	vec2 m_Pos2;
};


#endif 
