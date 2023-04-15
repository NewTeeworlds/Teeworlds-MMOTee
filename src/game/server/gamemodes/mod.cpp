/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"

#include <game/server/player.h>
#include <engine/shared/config.h>
#include <engine/server/mapconverter.h>
#include <engine/shared/network.h>
#include <game/mapitems.h>
#include <game/server/entities/items/dropitem.h>
#include <game/server/entities/items/pickup.h>
#include <game/server/entities/items/moneybag.h>
#include <game/server/entities/mobs/feya.h>
#include <game/server/entities/drawing/info.h>
#include <time.h>
#include <iostream>

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_pGameType = "MMOTee-Azataz-F idm";
	m_pHeroFlag = 0;
}

CGameControllerMOD::~CGameControllerMOD()
{

}

bool CGameControllerMOD::OnEntity(const char* pName, vec2 Pivot, vec2 P0, vec2 P1, vec2 P2, vec2 P3, int PosEnv)
{
	bool res = IGameController::OnEntity(pName, Pivot, P0, P1, P2, P3, PosEnv);

	if(str_comp(pName, "armor") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CPickup(&GameServer()->m_World, 1, Pos, 1);
	}
	else if(str_comp(pName, "health") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CPickup(&GameServer()->m_World, 0, Pos, 0);
	}
	else if(str_comp(pName, "Farm") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CPickup(&GameServer()->m_World, 1, Pos, 0);
	}
	else if(str_comp(pName, "Miner") == 0)
	{
		// miner
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CPickup(&GameServer()->m_World, 1, Pos, 2);
	}
	else if(str_comp(pName, "blueFlag") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CHeroFlag(&GameServer()->m_World, Pos);
	}
	else if(str_comp(pName, "Wood") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CPickup(&GameServer()->m_World, 0, Pos, 3);
	}
	else if(str_comp(pName, "ExitZone") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CFeya(&GameServer()->m_World, Pos, -1, -1, false);
	}	
	else if(str_comp(pName, "InfoCl1") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 0, 0, Pos);
	}	
	else if(str_comp(pName, "InfoCl2") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 0, 1, Pos);
	}	
	else if(str_comp(pName, "InfoCl3") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 0 ,2, Pos);
	}
	else if(str_comp(pName, "InfoCl4") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 0, 3, Pos);
	}
	else if(str_comp(pName, "InfoCl5") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 0, 4, Pos);
	}	
	else if(str_comp(pName, "IMater1") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 1, 0, Pos);
	}
	else if(str_comp(pName, "IMater2") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 1, 1, Pos);
	}
	else if(str_comp(pName, "IMater3") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CInfo(&GameServer()->m_World, 1, 2, Pos);
	}
	else if(str_comp(pName, "MoneyBag") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CMoneyBag(&GameServer()->m_World, 0, Pos, 5);
	}	
	else if(str_comp(pName, "wMater") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CPickup(&GameServer()->m_World, 0, Pos, 4);
	}	
	else if(str_comp(pName, "MaterFarm") == 0)
	{
		vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;
		new CPickup(&GameServer()->m_World, 0, Pos, 5);
	}	

	return res;
}

void CGameControllerMOD::GetPlayerCounter(int ClientException, int& NumHumans, int& NumInfected, int& NumFirstInfected)
{
	NumHumans = 0;
	NumInfected = 0;
}

void CGameControllerMOD::Tick()
{
	IGameController::Tick();
}

void CGameControllerMOD::Snap(int SnappingClient)
{
	CNetObj_GameInfo *pGameInfoObj = (CNetObj_GameInfo *)Server()->SnapNewItem(NETOBJTYPE_GAMEINFO, 0, sizeof(CNetObj_GameInfo));
	if(!pGameInfoObj)
		return;

	pGameInfoObj->m_GameFlags = 0;
	pGameInfoObj->m_GameStateFlags = 0;
	if(GameServer()->m_World.m_Paused)
		pGameInfoObj->m_GameStateFlags |= GAMESTATEFLAG_PAUSED;
	pGameInfoObj->m_RoundStartTick = Server()->Tick();
	pGameInfoObj->m_WarmupTimer = 0;

	pGameInfoObj->m_ScoreLimit = 0;
	pGameInfoObj->m_TimeLimit = 0;

	pGameInfoObj->m_RoundNum = 0;
	pGameInfoObj->m_RoundCurrent = 0;

	int ClassMask = 0;
	ClassMask |= CMapConverter::MASK_DEFENDER;
	ClassMask |= CMapConverter::MASK_MEDIC;
	ClassMask |= CMapConverter::MASK_HERO;
	ClassMask |= CMapConverter::MASK_SUPPORT;

	// FFS注: 这段代码或将导致三体文明毁灭（WTF???）
	if(GameServer()->m_apPlayers[SnappingClient])
	{
		if(GameServer()->m_apPlayers[SnappingClient]->AccData.Class == PLAYERCLASS_NONE && !GameServer()->m_apPlayers[SnappingClient]->IsBot())
		{
			pGameInfoObj->m_RoundStartTick = 0;
			int Page = -1;
			
			if(GameServer()->m_apPlayers[SnappingClient]->MapMenu() == 1)
			{
				int Item = GameServer()->m_apPlayers[SnappingClient]->m_MapMenuItem;
				Page = CMapConverter::TIMESHIFT_MENUCLASS + 3*((Item+1) + ClassMask*CMapConverter::TIMESHIFT_MENUCLASS_MASK) + 1;
			}
			
			if(Page >= 0)
			{
				double PageShift = static_cast<double>(Page * Server()->GetTimeShiftUnit())/1000.0f;
				double CycleShift = fmod(static_cast<double>(Server()->Tick() - pGameInfoObj->m_RoundStartTick)/Server()->TickSpeed(), Server()->GetTimeShiftUnit()/1000.0);
				int TimeShift = (PageShift + CycleShift)*Server()->TickSpeed();
				
				pGameInfoObj->m_RoundStartTick = 0;
				pGameInfoObj->m_TimeLimit += (TimeShift/(Server()->TickSpeed()/20))/60;
			}
		}
	}
	

}

int CGameControllerMOD::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	// do scoreing
	if(!pKiller || Weapon == WEAPON_GAME)
		return 0;
		
	CPlayer* pVictimPlayer = pVictim->GetPlayer();
	if(pVictimPlayer && pVictimPlayer != pKiller)
	{
		pKiller->ExpAdd(pVictimPlayer->AccData.Level);
	}
	
	if(Weapon == WEAPON_SELF)
		pVictim->GetPlayer()->m_RespawnTick = Server()->Tick()+Server()->TickSpeed()*3.0f;
		
	return 0;
}

void CGameControllerMOD::OnCharacterSpawn(class CCharacter *pChr)
{
	// default health
	pChr->IncreaseHealth(10);
	pChr->GiveWeapon(WEAPON_HAMMER, -1);
}

bool CGameControllerMOD::IsSpawnable(vec2 Pos, int TeleZoneIndex)
{
	//First check if there is a tee too close
	CCharacter *aEnts[MAX_CLIENTS];
	int Num = GameServer()->m_World.FindEntities(Pos, 64, (CEntity**)aEnts, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
	
	for(int c = 0; c < Num; ++c)
	{
		if(distance(aEnts[c]->m_Pos, Pos) <= 60)
			return false;
	}
	
	//Check the center
	int TeleIndex = GameServer()->Collision()->GetZoneValueAt(GameServer()->m_ZoneHandle_Teleport, Pos);
	if(GameServer()->Collision()->CheckPoint(Pos))
		return false;
	if(TeleZoneIndex && TeleIndex == TeleZoneIndex)
		return false;
	
	//Check the border of the tee. Kind of extrem, but more precise
	for(int i=0; i<16; i++)
	{
		float Angle = i * (2.0f * pi / 16.0f);
		vec2 CheckPos = Pos + vec2(cos(Angle), sin(Angle)) * 30.0f;
		TeleIndex = GameServer()->Collision()->GetZoneValueAt(GameServer()->m_ZoneHandle_Teleport, CheckPos);
		if(GameServer()->Collision()->CheckPoint(CheckPos))
			return false;
		if(TeleZoneIndex && TeleIndex == TeleZoneIndex)
			return false;
	}
	
	return true;
}

bool CGameControllerMOD::PreSpawn(CPlayer* pPlayer, vec2 *pOutPos)
{
	// spectators can't spawn
	if(pPlayer->GetTeam() == TEAM_SPECTATORS)
		return false;
	
	int Type = 1;
	if(!pPlayer->IsBot())
	{
		if(!pPlayer->AccData.Class)
			Type = 0;
		else if(!Server()->GetItemCount(pPlayer->GetCID(), SDROP))
		{
			Type = 15;

			GameServer()->SendMail(pPlayer->GetCID(), 10, MONEYBAG, 10);
			GameServer()->GiveItem(pPlayer->GetCID(), SCHAT, 1);
			GameServer()->GiveItem(pPlayer->GetCID(), SDROP, 1);
		}
		else if(pPlayer->m_InBossed && !GameServer()->m_BossStart)
			Type = 7;
		else if(GameServer()->m_BossStart && GameServer()->m_BossType == BOT_BOSSSLIME && pPlayer->m_InBossed)
			Type = 8;
		else if(pPlayer->m_InArea && GameServer()->m_AreaType == 1)
			Type = 9;
		else if(pPlayer->m_InArea && GameServer()->m_AreaType == 2)
			Type = 13;
		else if(pPlayer->AccData.Jail)
			Type = 4;

		else if(Server()->GetSpawnInClanHouse(pPlayer->GetCID(), 0))
			Type = 16;
		else if(Server()->GetSpawnInClanHouse(pPlayer->GetCID(), 1))
			Type = 17;
		else if(Server()->GetSpawnInClanHouse(pPlayer->GetCID(), 2))
			Type = 18;
		else if(Server()->GetSpawnInClanHouse(pPlayer->GetCID(), 3))
			Type = 19;
		else if(Server()->GetSpawnInClanHouse(pPlayer->GetCID(), 4))
			Type = 20;
	}
	else
	{
		if(pPlayer->GetBotType() == BOT_L1MONSTER)
			Type = 3;
		else if(pPlayer->GetBotType() == BOT_L2MONSTER)
			Type = 5;
		else if(pPlayer->GetBotType() == BOT_L3MONSTER)
			Type = 6;
		else if(pPlayer->GetBotType() == BOT_BOSSSLIME)
			Type = 8;
		else if(pPlayer->GetBotType() == BOT_GUARD)
			Type = 2;
		else if(pPlayer->GetBotType() == BOT_NPCW && pPlayer->GetBotSubType() == 0)
			Type = 10;
		else if(pPlayer->GetBotType() == BOT_NPCW && pPlayer->GetBotSubType() == 1)
			Type = 11;
		else if(pPlayer->GetBotType() == BOT_NPCW && pPlayer->GetBotSubType() == 2)
			Type = 12;
		else if(pPlayer->GetBotType() == BOT_FARMER)
			Type = 14;
	}

	// get spawn point
	int RandomShift = random_int(0, m_SpawnPoints[Type].size()-1);
	for(int i = 0; i < m_SpawnPoints[Type].size(); i++)
	{
		int I = (i + RandomShift)%m_SpawnPoints[Type].size();
		if(IsSpawnable(m_SpawnPoints[Type][I], 0))
		{
			*pOutPos = m_SpawnPoints[Type][I];
			return true;
		}
	}
	
	return false;
}

bool CGameControllerMOD::PickupAllowed(int Index)
{
	return Index > TILE_ENTITY_WEAPON_RIFLE || Index < TILE_ENTITY_WEAPON_SHOTGUN;
}

bool CGameControllerMOD::IsEnabledClass(int PlayerClass) {
switch(PlayerClass)
	{
		case PLAYERCLASS_HEALER:
			return true;
		case PLAYERCLASS_BERSERK:
			return true;
		case PLAYERCLASS_ASSASINS:
			return true;
		default:
			return false;
}
}

bool CGameControllerMOD::IsChoosableClass(int PlayerClass)
{
	if (!IsEnabledClass(PlayerClass))
		return false;

	return true;
}
