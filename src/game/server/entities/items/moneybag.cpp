/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include "moneybag.h"

CMoneyBag::CMoneyBag(CGameWorld *pGameWorld, int Type, vec2 Pos, int SubType)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_MONEYBAG)
{
	m_Type = Type;
	m_Pos = Pos;
	m_SubType = SubType;

	Reset();
	GameWorld()->InsertEntity(this);
	for(int i=0; i<NUM_IDS; i++)
	{
		m_IDs[i] = Server()->SnapNewID();
	}
}

CMoneyBag::~CMoneyBag()
{
	for(int i=0; i<NUM_IDS; i++)
	{
		Server()->SnapFreeID(m_IDs[i]);
	}
}

void CMoneyBag::Reset()
{
	Picking(300+rand()%2000);
}

void CMoneyBag::Tick() 
{
	// wait for respawn
	if(m_SpawnTick > 0)
	{
		if(Server()->Tick() > m_SpawnTick)
		{
			// respawn
			m_SpawnTick = -1;
		}
		else
			return;
	}
	if(Server()->Tick() % (1 * Server()->TickSpeed() * 60) == 0)
		m_TimeActive++;

	// Find other players
	for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
	{
		if(!p->GetPlayer()->IsBot() && distance(p->m_Pos, m_Pos) < 20) 
		{
			switch (m_Type)
			{
				case 0:
					if(m_SubType != 3)
					{
						int Count = 4+rand()%4;
						GameServer()->SendChatTarget_Localization(-1, -1, _("{str:name} found Secret Bag! Got {int:got} + Time {int:bonus} Money Bag!"), "name", Server()->ClientName(p->GetPlayer()->GetCID()), "got", &Count, "bonus", &m_TimeActive, NULL);
						GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH);
						GameServer()->GiveItem(p->GetPlayer()->GetCID(), MONEYBAG, Count+m_TimeActive);
						Picking(300+rand()%2000);
					}
					break;
			};

		}
	}
}

void CMoneyBag::Picking(int Time)
{
	m_TimeActive = 0;

	int RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime+Time;
	if(RespawnTime >= 0)
		m_SpawnTick = Server()->Tick() + Server()->TickSpeed() * RespawnTime;
}

void CMoneyBag::TickPaused()
{
	if(m_SpawnTick != -1)
		++m_SpawnTick;
}

void CMoneyBag::Snap(int SnappingClient)
{
	if(m_SpawnTick != -1 || NetworkClipped(SnappingClient))
		return;

	float AngleStart = (2.0f * pi * Server()->Tick()/static_cast<float>(Server()->TickSpeed()))/10.0f;
	float AngleStep = (2.0f * pi / CMoneyBag::NUM_SIDE);
	float R = 20.0f;

	AngleStart = AngleStart*2.0f;
	for(int i=0; i<CMoneyBag::NUM_SIDE; ++i)
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

	CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
	if(pObj)
	{
		pObj->m_X = (int)m_Pos.x;
		pObj->m_Y = (int)m_Pos.y;
		pObj->m_VelX = 0;
		pObj->m_VelY = 0;
		pObj->m_StartTick = Server()->Tick()-4;
		pObj->m_Type = WEAPON_RIFLE;
	}		
}
