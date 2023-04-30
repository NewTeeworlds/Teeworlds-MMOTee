/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                
#ifndef GAME_SERVER_ENTITIES_PHOTOID_H
#define GAME_SERVER_ENTITIES_PHOTOID_H

#include <game/server/entity.h>

class CPhotoid : public CEntity
{
private:
	vec2 m_InitialVel;
	float m_InitialAmount;
	int m_OwnerID;
	
public:
	CPhotoid(CGameWorld *pGameWorld, vec2 Pos, int OwnerID, int GetPlayer, vec2 InitialVel);
	
	virtual void Tick();
	virtual void Snap(int SnappingClient);
};

#endif
*/