/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>
#include "snapfullproj.h"

CSnapFullProject::CSnapFullProject(CGameWorld *pGameWorld, vec2 Pos, int Owner, int Num, int Type, bool Changing)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_SNAP_FULLPROJECT)
{
	m_Pos = Pos;
	m_LoadingTick = Server()->TickSpeed();
	m_Owner = Owner;
	
	m_Num = Num;
	m_Type = Type;
	m_Changing = Changing;

	GameWorld()->InsertEntity(this);
	for(int i=0; i<m_Num; i++)
		m_IDs[i] = Server()->SnapNewID();
}

CSnapFullProject::~CSnapFullProject()
{
	for(int i=0; i<m_Num; i++)
		Server()->SnapFreeID(m_IDs[i]);
}

void CSnapFullProject::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CSnapFullProject::Tick()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_Owner];
	if(!pOwner || !pOwner->GetCharacter())
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	if(m_Changing && !m_boolreback)
	{
		m_LoadingTick--;
		if(m_LoadingTick <= 1)
			m_boolreback = true;
	}
	if(m_Changing && m_boolreback)
	{
		m_LoadingTick++;
		if(m_LoadingTick >= 20)
			m_boolreback = false;
	}
	m_Pos = pOwner->GetCharacter()->m_Pos;
}


void CSnapFullProject::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	float AngleStart = (2.0f * pi * Server()->Tick()/static_cast<float>(Server()->TickSpeed()))/10.0f;
	float AngleStep = 2.0f * pi / CSnapFullProject::m_Num;
	float R = 60.0f+m_LoadingTick;
	AngleStart = AngleStart*2.0f;
	for(int i=0; i<CSnapFullProject::m_Num; i++)
	{
		vec2 PosStart = m_Pos + vec2(R * cos(AngleStart + AngleStep*i), R * sin(AngleStart + AngleStep*i));
		
		CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_IDs[i], sizeof(CNetObj_Projectile)));
		if(!pObj)
			return;

		pObj->m_X = (int)PosStart.x;
		pObj->m_Y = (int)PosStart.y;
		pObj->m_VelX = 0;
		pObj->m_VelY = 0;
		pObj->m_StartTick = Server()->Tick()-1;
		pObj->m_Type = m_Type;
	}
}
