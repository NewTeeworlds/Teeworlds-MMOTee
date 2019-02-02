/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>
#include "sword.h"

#define M_PI 3.14159265358979323846


CSword::CSword(CGameWorld *pGameWorld, vec2 Pos, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_SWORD)
{
	m_Pos = Pos;
	m_Owner = Owner;
	GameWorld()->InsertEntity(this);	
}

void CSword::Reset() 
{	
	if(GameServer()->m_apPlayers[m_Owner] && GameServer()->m_apPlayers[m_Owner]->GetCharacter())
	{
		GameServer()->CreateDeath(m_Pos2, m_Owner);
		GameServer()->CreateDeath(m_Pos, m_Owner);
	}
	GameServer()->m_World.DestroyEntity(this);
}

void CSword::Tick()
{
	CPlayer *pOwner = GameServer()->m_apPlayers[m_Owner];
	if (!pOwner || !pOwner->GetCharacter() || !pOwner->m_Mana)
	{
		Reset();
		return;
	}
	
	if(Server()->Tick() % Server()->TickSpeed()/10 == 0)
		pOwner->m_Mana--;

	m_Pos = pOwner->GetCharacter()->m_Pos;
	m_Pos2 = normalize(vec2(pOwner->GetCharacter()->m_Core.m_Input.m_TargetX, pOwner->GetCharacter()->m_Core.m_Input.m_TargetY));
	float a = GetAngle(m_Pos2);	
	m_Pos2 = vec2(cos(a + M_PI / 1 * 4)*(180.0) + m_Pos.x, sin(a + M_PI / 1 * 4)*(180.0) + m_Pos.y);
	GameServer()->Collision()->IntersectLine(m_Pos, m_Pos2, 0x0, &m_Pos2);

	// Find other sword
	for(CSword *p = (CSword*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_SWORD); p; p = (CSword *)p->TypeNext())
	{
		if(p && m_Owner != p->m_Owner)
		{
			PosLine = closest_point_on_line(m_Pos2, m_Pos, p->PosLine);
			if(distance(p->PosLine, PosLine) < 20) 
				GameServer()->CreateExplosion(PosLine, m_Owner, WEAPON_GRENADE, false, TAKEDAMAGEMODE_INFECTION);
		}
	}

	// Find other players
	for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
	{
		if(p->GetPlayer() && p)
		{
			vec2 InsertPos = closest_point_on_line(m_Pos2, m_Pos, p->m_Pos);
			if(p->GetPlayer()->GetCID() != m_Owner && distance(p->m_Pos, InsertPos) < 10) 
			{
				GameServer()->CreateDeath(InsertPos, p->GetPlayer()->GetCID());
				GameServer()->CreateExplosion(InsertPos, m_Owner, WEAPON_GRENADE, false, TAKEDAMAGEMODE_INFECTION);
			}
		}
	}


}

void CSword::Snap(int SnappingClient)
{
	if (NetworkClipped(SnappingClient))
		return;
	
	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if(!pObj)
		return;


	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_FromX = (int)m_Pos2.x;	
	pObj->m_FromY = (int)m_Pos2.y;
	pObj->m_StartTick = Server()->Tick()-4;
}
