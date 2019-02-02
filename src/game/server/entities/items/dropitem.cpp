/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/math.h>
#include <base/vmath.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>

#include "dropitem.h"

CDropItem::CDropItem(CGameWorld *pGameWorld, vec2 Pos, vec2 Dir, int ItemID, int Count, int HowID, int Enchant)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_DROPITEM)
{
	m_Pos = Pos;
	m_ActualPos = Pos;
	m_ActualDir = Dir;
	m_Direction = Dir;
	m_ItemID = ItemID;
	m_Count = Count;
	
	m_HowID = HowID;
	m_Enchant = Enchant;

	m_Flashing = false;
	m_StartTick = Server()->Tick();
	m_LifeSpan = Server()->TickSpeed()*60;
	m_SpawnTick = -1;
	GameWorld()->InsertEntity(this);
}

void CDropItem::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

vec2 CDropItem::GetPos(float Time)
{
	float Curvature = GameServer()->Tuning()->m_ShotgunCurvature;
	float Speed = GameServer()->Tuning()->m_ShotgunSpeed*2+250;

	return CalcPos(m_Pos, m_Direction, Curvature, Speed, Time);
}

void CDropItem::Tick()
{
	float Pt = (Server()->Tick()-m_StartTick-1)/(float)Server()->TickSpeed();
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();
	vec2 PrevPos = GetPos(Pt);
	vec2 CurPos = GetPos(Ct);
	
	m_ActualPos = CurPos;
	m_ActualDir = normalize(CurPos - PrevPos);
	
	if(m_HowID != -1 && !GameServer()->m_apPlayers[m_HowID])
		m_HowID = -1;	
	
	if(m_LifeSpan < 100)
	{
		m_Flashing = true;
		if(m_LifeSpan < 0)
		{
			GameServer()->m_World.DestroyEntity(this);
			return;
		}
	}
	m_LifeSpan--;
	
	if (m_Flashing)
	{
		m_FlashTimer--;
		
		if (m_FlashTimer <= 0)
			m_FlashTimer = 20;
			
		if (m_FlashTimer > 10)
			m_SpawnTick = 999;
		else
			m_SpawnTick = -1;
	}

	CCharacter *TargetChr = GameServer()->m_World.IntersectCharacter(PrevPos, CurPos, 20.0f, CurPos);
	if(TargetChr && TargetChr->GetPlayer() && !TargetChr->GetPlayer()->IsBot())
	{
		int ClientID = TargetChr->GetPlayer()->GetCID();

		char aBuf[64];
		if(Server()->GetItemPrice(ClientID, m_ItemID, 0) > 0) str_format(aBuf, sizeof(aBuf), "LV%d:%s", Server()->GetItemPrice(ClientID, m_ItemID, 0), Server()->GetItemName(ClientID, m_ItemID));
		else str_format(aBuf, sizeof(aBuf), "%s", Server()->GetItemName(ClientID, m_ItemID));

		GameServer()->SendBroadcast_Localization(ClientID, 200, 10,
			_("Use {str:button} for drop \n'{str:name}'x{int:num}\nPersonal item for {str:nhow}"), "button", Server()->GetItemSettings(ClientID, SDROP) ? "F3" : "Hammer", "name", aBuf, "num", &m_Count, "nhow", m_HowID != -1 ? Server()->ClientName(m_HowID) : "Nope", NULL);
	}

	vec2 LastPos;
	int Collide = GameServer()->Collision()->IntersectLine(PrevPos, CurPos, NULL, &LastPos);
	if(Collide)
	{			
		//Thanks to TeeBall 0.6
		vec2 CollisionPos;
		CollisionPos.x = LastPos.x;
		CollisionPos.y = CurPos.y;
		int CollideY = GameServer()->Collision()->IntersectLine(PrevPos, CollisionPos, NULL, NULL);
		CollisionPos.x = CurPos.x;
		CollisionPos.y = LastPos.y;
		int CollideX = GameServer()->Collision()->IntersectLine(PrevPos, CollisionPos, NULL, NULL);

		m_Pos = LastPos;
		m_ActualPos = m_Pos;
		vec2 vel;
		vel.x = m_Direction.x;
		vel.y = m_Direction.y + 2*GameServer()->Tuning()->m_ShotgunCurvature/10000*Ct*(GameServer()->Tuning()->m_ShotgunSpeed*2+250);
		
		if (CollideX && !CollideY)
		{
			m_Direction.x = -vel.x;
			m_Direction.y = vel.y;
		}
		else if (!CollideX && CollideY)
		{
			m_Direction.x = vel.x;
			m_Direction.y = -vel.y;
		}
		else
		{
			m_Direction.x = -vel.x;
			m_Direction.y = -vel.y;
		}
		
		m_Direction.x *= (100 - 50) / 100.0;
		m_Direction.y *= (100 - 50) / 100.0;
		m_StartTick = Server()->Tick();
		
		m_ActualDir = normalize(m_Direction);
	}
}

bool CDropItem::TakeItem(int ClientID)
{
	CPlayer *pOwner = GameServer()->m_apPlayers[ClientID];
	if(!pOwner || !pOwner->GetCharacter())
		return false;

	if(m_HowID == ClientID || m_HowID == -1)
	{
		if(pOwner->AccData.Level < Server()->GetItemPrice(ClientID, m_ItemID, 0))
		{
			GameServer()->SendBroadcast_Localization(ClientID, 201, 100, _("Your level small for this items"), NULL);	
			return false;
		}
		switch(m_ItemID)
		{
			case IGUN: pOwner->GetCharacter()->GiveWeapon(WEAPON_GUN, 5, false); break;
			case ISHOTGUN: pOwner->GetCharacter()->GiveWeapon(WEAPON_SHOTGUN, 5, false);	break;
			case IGRENADE: pOwner->GetCharacter()->GiveWeapon(WEAPON_GRENADE, 5, false);	break;
			case ILASER: pOwner->GetCharacter()->GiveWeapon(WEAPON_RIFLE, 5, false);	break;
			case HOOKDAMAGE: pOwner->GetCharacter()->GiveWeapon(WEAPON_RIFLE, 5, false);	break;
		} 

		dbg_msg("take", "%s take item %s:%d (count: %d)", Server()->ClientName(ClientID), Server()->GetItemName(ClientID, m_ItemID, false), m_ItemID, m_Count);
		GameServer()->GiveItem(ClientID, m_ItemID, m_Count, m_Enchant);
		GameServer()->ResetVotes(ClientID, AUTH);
		return true;
	}
	return false;
}

void CDropItem::TickPaused()
{
	m_StartTick++;
}

void CDropItem::Snap(int SnappingClient)
{
	float Ct = (Server()->Tick()-m_StartTick)/(float)Server()->TickSpeed();
	
	if(m_SpawnTick != -1 || NetworkClipped(SnappingClient, GetPos(Ct)))
		return;

	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pP)
		return;

	pP->m_X = (int)m_ActualPos.x+m_ActualDir.x;
	pP->m_Y = (int)m_ActualPos.y+m_ActualDir.y;
	pP->m_Type = 2;
	pP->m_Subtype = 0;
}
