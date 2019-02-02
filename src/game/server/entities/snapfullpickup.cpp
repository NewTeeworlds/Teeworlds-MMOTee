/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>
#include "snapfullpickup.h"
#include "biologist-laser.h"

CSnapFullPickup::CSnapFullPickup(CGameWorld *pGameWorld, vec2 Pos, int Owner, int Num, int Type, int SubType, bool Changing)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_SNAP_FULLPICKUP)
{
	m_Pos = Pos;
	m_LoadingTick = Server()->TickSpeed();
	m_boolreback = false;
	m_Owner = Owner;
	
	m_Num = Num;
	m_Type = Type;
	m_SubType = SubType;
	m_Changing = Changing;

	GameWorld()->InsertEntity(this);	
	for(int i=0; i<m_Num; ++i)
		m_IDs[i] = Server()->SnapNewID();
}

CSnapFullPickup::~CSnapFullPickup()
{
	for(int i=0; i<m_Num; ++i)
		Server()->SnapFreeID(m_IDs[i]);
}

void CSnapFullPickup::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CSnapFullPickup::Tick()
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
		{
			/*if(m_LoadingTick == 1)
			{
				for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
				{
					if(p->GetPlayer() && p)
					{
						if(p->GetPlayer()->IsBot() && distance(p->m_Pos, m_Pos) < 100 && !GameServer()->Collision()->IntersectLine(m_Pos, p->m_Pos, 0x0, &p->m_Pos)) 
						{
							GameServer()->SendEmoticon(p->GetPlayer()->GetCID(), EMOTICON_GHOST);
							GameServer()->CreateExplosion(vec2(p->m_Pos.x,p->m_Pos.y+0.1f), m_Owner, WEAPON_GRENADE, false);
						}
					}
				}
			}*/
			m_boolreback = true;
		}
	}
	if(m_Changing && m_boolreback)
	{
		m_LoadingTick++;
		if(m_LoadingTick >= 20)
			m_boolreback = false;
	}
	m_Pos = pOwner->GetCharacter() ? pOwner->GetCharacter()->m_Pos : vec2(0, 0);
}

void CSnapFullPickup::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return; 

	float AngleStart = (2.0f * pi * Server()->Tick()/static_cast<float>(Server()->TickSpeed()))/10.0f;
	float AngleStep = (2.0f * pi / CSnapFullPickup::m_Num);
	float R = 30.0f+m_LoadingTick;
 
	AngleStart = AngleStart*2.0f;
	for(int i=0; i<CSnapFullPickup::m_Num; ++i)
	{
		vec2 PosStart = m_Pos + vec2(R * cos((AngleStart + AngleStep*i)+m_Type*60), R * sin((AngleStart + AngleStep*i)+m_Type*60));
		CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_IDs[i], sizeof(CNetObj_Pickup)));
		if(!pObj)
			return;

		pObj->m_X = (int)PosStart.x;
		pObj->m_Y = (int)PosStart.y;
		pObj->m_Type = m_Type;
		pObj->m_Subtype = m_SubType;
	}
}
