/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_PIZDAMET_H
#define GAME_SERVER_ENTITIES_PIZDAMET_H

#include <game/server/entity.h>

class CPizdamet : public CEntity
{
private:
	int m_OwnerID;
	vec2 m_Cursour;
	
public:
	CPizdamet(CGameWorld *pGameWorld, vec2 Pos, int OwnerID);
	
	virtual void Tick();
	virtual void Snap(int SnappingClient);
};

#endif
