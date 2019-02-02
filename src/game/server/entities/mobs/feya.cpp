/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>

#include "feya.h"

CFeya::CFeya(CGameWorld *pGameWorld, vec2 Pos, int Type, int SubType, bool Proj)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_FEYA)
{
	m_Pos = Pos;

	GameWorld()->InsertEntity(this);
	for(int i=0; i<NUM_IDS; i++)
	{
		m_IDs[i] = Server()->SnapNewID();
	}

	m_Type = Type;
	m_SubType = SubType;
	m_Proj = Proj;
}

CFeya::~CFeya()
{
	for(int i=0; i<NUM_IDS; i++)
	{
		Server()->SnapFreeID(m_IDs[i]);
	}
}

void CFeya::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CFeya::Snap(int SnappingClient)
{	
	float AngleStart = (2.0f * pi * Server()->Tick()/static_cast<float>(Server()->TickSpeed()))/10.0f;
	float AngleStep = (2.0f * pi / CFeya::NUM_SIDE);
	float R = 10.0f;

	AngleStart = AngleStart*2.0f;
	for(int i=0; i<CFeya::NUM_SIDE; ++i)
	{
		vec2 PosStart = m_Pos + vec2(R * cos((AngleStart + AngleStep*i)+m_Type*60), R * sin((AngleStart + AngleStep*i)+m_Type*60));
		CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_IDs[i], sizeof(CNetObj_Projectile)));
		if(pObj)
		{
			pObj->m_X = (int)PosStart.x;
			pObj->m_Y = (int)PosStart.y;
			pObj->m_VelX = 0;
			pObj->m_VelY = 0;
			pObj->m_StartTick = Server()->Tick();
			pObj->m_Type = WEAPON_HAMMER;
		}
	}

	if(m_Type > -1)
	{
		CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
		if(!pP)
			return;

		pP->m_X = (int)m_Pos.x;
		pP->m_Y = (int)m_Pos.y;
		pP->m_Type = m_Type;
		pP->m_Subtype = m_SubType;
	}

}

void CFeya::Tick()
{	

}
