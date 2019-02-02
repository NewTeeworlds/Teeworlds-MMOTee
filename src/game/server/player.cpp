/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <new>
#include <iostream>
#include <engine/shared/config.h>
#include "player.h"
#include <engine/shared/network.h>
#include <game/server/entities/bots/monster.h> 
#include <game/server/entities/bots/npcs.h> 
#include <game/server/entities/bots/npcsw.h> 
#include <game/server/entities/bots/kwah.h> 
#include <game/server/entities/bots/boomer.h> 
#include <game/server/entities/bots/bossslime.h> 
#include <game/server/entities/bots/farmer.h> 

MACRO_ALLOC_POOL_ID_IMPL(CPlayer, MAX_CLIENTS)

IServer *CPlayer::Server() const { return m_pGameServer->Server(); }

CPlayer::CPlayer(CGameContext *pGameServer, int ClientID, int Team)
{
	m_pGameServer = pGameServer;
	m_RespawnTick = Server()->Tick();
	m_DieTick = Server()->Tick();
	m_ScoreStartTick = Server()->Tick();
	
	m_pCharacter = 0;
	m_ClientID = ClientID;
	m_Team = GameServer()->m_pController->ClampTeam(Team);
	m_SpectatorID = SPEC_FREEVIEW;
	m_LastActionTick = Server()->Tick();
	
	m_Bot = (ClientID >= g_Config.m_SvMaxClients-MAX_BOTS);
	m_BotType = m_BotSubType = m_SelectItem = m_SelectArmor = -1;
	
	m_Authed = IServer::AUTHED_NO;
	int *pIdMap = Server()->GetIdMap(m_ClientID);
	for (int i = 1;i < VANILLA_MAX_CLIENTS;i++)
	{
		pIdMap[i] = -1;
	}
	pIdMap[0] = m_ClientID;

	m_MapMenu = 0;
	m_MapMenuItem = -1;
	m_MapMenuTick = -1;

	m_PrevTuningParams = *pGameServer->Tuning();
	m_NextTuningParams = m_PrevTuningParams;

	m_MoneyAdd = m_ExperienceAdd = m_InviteClanID = m_Mana = 0;
	m_Search = m_BigBot = m_InArea = m_IsInGame = m_InBossed = false;

	AccData.Level = AccUpgrade.SkillPoint = AccUpgrade.Upgrade = AccUpgrade.Speed = AccUpgrade.Health = AccUpgrade.Damage = -1;
	AccData.Exp = AccData.Money = AccData.Rel = AccData.Gold = -1;
	AccData.Class = PLAYERCLASS_NONE;

	m_pChatCmd = new CCmd(this, m_pGameServer);	
	SetLanguage(Server()->GetClientLanguage(ClientID));
}

CPlayer::~CPlayer()
{
	delete m_pChatCmd;
	m_pChatCmd = 0;

	delete m_pCharacter;
	m_pCharacter = 0;
}

bool CPlayer::GetShop()
{
	if(m_pCharacter && m_pCharacter->InShop)
		return true;
	
	return false;
}

bool CPlayer::GetWork()
{
	if(m_pCharacter && m_pCharacter->InWork)
		return true;
	
	return false;
}

void CPlayer::RandomBoxTick()
{
	if(m_OpenBox && m_OpenBoxType == RANDOMCRAFTITEM)
	{
		int getitem = 0;
		if(m_OpenBox % 30 == 0)
		{
			int RandGet = rand()%100;
			if(RandGet >= 0 && RandGet <= 95)
			{
				int RandItem = rand()%2;
				switch(RandItem)
				{
					default: getitem = FOOTKWAH; break;
					case 1: getitem = HEADBOOMER; break;
				}
			}
			else
			{
				int RandItem = rand()%4;
				switch(RandItem)
				{
					default: getitem = FORMULAEARRINGS; break;
					case 1: getitem = FORMULAFORRING; break;
					case 2: getitem = FORMULAWEAPON; break;
					case 3: getitem = RARESLIMEDIRT; break;
				}
			}
			if(m_pCharacter)
				GameServer()->CreateLolText(m_pCharacter, false, vec2(0,-75), vec2 (0,-1), 10, Server()->GetItemName(m_ClientID, getitem, false));

			if(m_OpenBox == 30)
			{
				m_OpenBox = 0;
				m_OpenBoxType = 0;

				if(m_pCharacter)
					GameServer()->CreateDeath(m_pCharacter->m_Pos, m_ClientID);

				int Get = 1;
				GameServer()->GiveItem(m_ClientID, getitem, Get);
				GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("{str:name} used {str:used} x{int:num} and get {str:get} x{int:num2}"), 
					"name", Server()->ClientName(m_ClientID), "used", Server()->GetItemName(m_ClientID, RANDOMCRAFTITEM, false), "num", &Get, "get", Server()->GetItemName(m_ClientID, getitem, false), "num2", &Get, NULL);	
			
			}
		}	
	}
	if(m_OpenBox && m_OpenBoxType == EVENTBOX)
	{
		int getitem = 0;
		if(m_OpenBox % 30 == 0)
		{
			int RandGet = rand()%160;
			if(RandGet >= 0 && RandGet <= 158) getitem = MONEYBAG;
			else getitem = RAREEVENTHAMMER;

			if(m_pCharacter)
				GameServer()->CreateLolText(m_pCharacter, false, vec2(0,-75), vec2 (0,-1), 10, Server()->GetItemName(m_ClientID, getitem, false));

			if(m_OpenBox == 30)
			{
				m_OpenBox = 0;
				m_OpenBoxType = 0;

				if(m_pCharacter)
					GameServer()->CreateDeath(m_pCharacter->m_Pos, m_ClientID);

				int Get = 1;
				GameServer()->GiveItem(m_ClientID, getitem, Get);
				GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("{str:name} used {str:used} x{int:num} and get {str:get} x{int:num2}"), 
					"name", Server()->ClientName(m_ClientID), "used", Server()->GetItemName(m_ClientID, EVENTBOX, false), "num", &Get, "get", Server()->GetItemName(m_ClientID, getitem, false), "num2", &Get, NULL);	
			
			}
		}	
	}
	if(m_OpenBox && m_OpenBoxType == FARMBOX)
	{
		int getitem = 0;
		if(m_OpenBox % 30 == 0)
		{
			int Get = 1;
			int RandGet = rand()%100;
			if(RandGet >= 0 && RandGet <= 92)
			{
				int RandItem = rand()%3;
				switch(RandItem)
				{
					default: getitem = FARMLEVEL, Get = 5; break;
					case 1: getitem = MONEYBAG, Get = 2; break;
					case 2: getitem = EVENTBOX, Get = 5; break;
				}
			}
			else
			{
				int RandItem = rand()%4;
				switch(RandItem)
				{
					default: getitem = JUMPIMPULS; break;
					case 1: getitem = FREEAZER; break;
					case 3: getitem = RARESLIMEDIRT; break;
				}
			}
			if(m_pCharacter)
				GameServer()->CreateLolText(m_pCharacter, false, vec2(0,-75), vec2 (0,-1), 10, Server()->GetItemName(m_ClientID, getitem, false));

			if(m_OpenBox == 30)
			{
				m_OpenBox = 0;
				m_OpenBoxType = 0;

				if(m_pCharacter)
					GameServer()->CreateDeath(m_pCharacter->m_Pos, m_ClientID);

				GameServer()->GiveItem(m_ClientID, getitem, Get);
				GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("{str:name} used {str:used} x1 and get {str:get} x{int:num2}"), 
					"name", Server()->ClientName(m_ClientID), "used", Server()->GetItemName(m_ClientID, FARMBOX, false), "get", Server()->GetItemName(m_ClientID, getitem, false), "num2", &Get, NULL);	
			
			}
		}	
	}
	if(m_OpenBox)
		m_OpenBox--;
}

void CPlayer::BasicAuthedTick()
{
	if(tickstr)
	{
		tickstr--;
		if(tickstr == 1)
		{
			tickstr = 15;
			int sz = strlen(pTitle);
			if(sz-sz+10 <= sz-1)
			{
				memmove(pTitle, pTitle + 1, sz - 1);
				pTitle[sz - 1] = '\0';

				if(sz-sz+10 >= sz-1)
					tickstr = 90;
			}
			else 
				SetMoveChar();		
		}
	}

	if(Server()->GetItemCount(m_ClientID, PIGPORNO) > 1000 && !Server()->GetItemCount(m_ClientID, PIGPIG))
		GameServer()->SendMail(m_ClientID, "You unlock new title!", PIGPIG, 1);

	if(AccData.Money >= 10000)
	{
		AccData.Gold += AccData.Money/10000;
		int Got = (int)(AccData.Money/10000);
		
		AccData.Money -= Got*10000;
	}

	if(AccData.Exp >= AccData.Level*GetNeedForUp())
	{
		GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("[Level UP] Added Skill and Upgrade points."), NULL);
		AccData.Exp -= AccData.Level*GetNeedForUp();
		AccData.Level++;
		AccUpgrade.SkillPoint += 1;
		AccUpgrade.Upgrade += 2;

		int GetBag = Server()->GetItemCount(m_ClientID, AMULETCLEEVER) ? 2 : 1;
		GameServer()->GiveItem(m_ClientID, MONEYBAG, GetBag);
		if(AccData.Level % 10 == 0)
			GameServer()->SendMail(m_ClientID, "You got bonus every 10 level!", RANDOMCRAFTITEM, 3);
		if(AccData.Level == 2)
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("You have opened quest in vote quest."), NULL);		
		if(m_pCharacter)
		{
			GameServer()->CreateLolText(m_pCharacter, false, vec2(0,-75), vec2 (0,-1), 50, "Level ++");
			GameServer()->CreateDeath(m_pCharacter->m_Pos, m_ClientID);
		}
		GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE, m_ClientID);

		GameServer()->UpdateUpgrades(m_ClientID);
		GameServer()->UpdateStats(m_ClientID);
	}
}

void CPlayer::Tick()
{
#ifdef CONF_DEBUG
	if(!g_Config.m_DbgDummies || m_ClientID < MAX_CLIENTS-g_Config.m_DbgDummies)
#endif
	if(!Server()->ClientIngame(m_ClientID))
		return;

	if(Server()->IsClientLogged(m_ClientID) && AccData.Level == -1)
	{
		AccData.Level = 1;
		if(!Server()->GetSeccurity(m_ClientID))
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Your account is at risk, set security"), NULL);				
	}
	
	if(!IsBot())
	{
		// Мана сучка ебал вас геи ебанные в рт вы шлюхи
		if(m_Mana < GetNeedMana())
		{
			if(!m_ManaTick)
			{
				m_Mana++;
				m_ManaTick = 10;
				GameServer()->SendBroadcast_LStat(m_ClientID, 2, 50, -1);
			} else m_ManaTick--;
		}

		// Снимаем ангру
		if(m_AngryWroth && Server()->Tick() % (1 * Server()->TickSpeed() * 20) == 0)
		{
			if(m_AngryWroth < 20) 
				m_AngryWroth = 0;
			else 
				m_AngryWroth -= 20;
		}
		
		// ОПЫТ КНИГИ ДОБАВКА НУЖНО ОПТИМИЗИРОВАТЬ
		if(m_MoneyAdd)
		{
			m_MoneyAdd--;
			if(Server()->Tick() % (1 * Server()->TickSpeed() * 120) == 0 && m_MoneyAdd > 1500)		
			{
				int Time = m_MoneyAdd/Server()->TickSpeed()/60;
				GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Item's ending {str:name} after {int:ends} min."), "name", Server()->GetItemName(m_ClientID, BOOKMONEYMIN), "ends", &Time, NULL);				
			}
			if(m_MoneyAdd == 1)
				GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Item's ending {str:name}"), "name", Server()->GetItemName(m_ClientID, BOOKMONEYMIN), NULL);				
		}
		if(m_ExperienceAdd)
		{
			m_ExperienceAdd--;
			if(Server()->Tick() % (1 * Server()->TickSpeed() * 120) == 0 && m_ExperienceAdd > 1500)		
			{
				int Time = m_ExperienceAdd/Server()->TickSpeed()/60;
				GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("This item ending {str:name} after {int:ends} min."), "name", Server()->GetItemName(m_ClientID, BOOKEXPMIN), "ends", &Time, NULL);				
			}
			if(m_ExperienceAdd == 1)
				GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("This item ending {str:name}"), "name", Server()->GetItemName(m_ClientID, BOOKEXPMIN), NULL);				
		}
		
		// Уровни и все такое повышение
		if(Server()->IsClientLogged(m_ClientID) && GetTeam() != TEAM_SPECTATORS)
		{		
			if(g_Config.m_SvEventSchool)
			{
				if(Server()->Tick() % (1 * Server()->TickSpeed() * 600) == 0)
				{
					int Type;
					switch(rand()%9)
					{
						case 1: Type = COOPERPIX; break;
						case 2: Type = WOOD; break;
						case 3: Type = DRAGONORE; break;
						case 4: Type = COOPERORE; break;
						case 5: Type = IRONORE; break;
						case 6: Type = GOLDORE; break;
						case 7: Type = DIAMONDORE; break;
						default: Type = EVENTCUSTOMSOUL;
					}

					GameServer()->SendMail(m_ClientID, "Event Back to School you got item!", Type, 1);
					GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("[Back to School] {str:name} got {str:item}."), "name", Server()->ClientName(m_ClientID), "item", Server()->GetItemName(m_ClientID, Type), NULL);					

					if(Server()->GetItemCount(m_ClientID, EVENTCUSTOMSOUL) >= 25)
					{
						Server()->RemItem(m_ClientID, EVENTCUSTOMSOUL, 25, -1);
						GameServer()->SendMail(m_ClientID, "Soul automatic changes Custom Skin!", CUSTOMSKIN, 1);
						GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("[Back to School] {str:name} Collect 25 Soul & get Custom Skin."), "name", Server()->ClientName(m_ClientID), NULL);					
					}
				}
			}
			BasicAuthedTick();
			RandomBoxTick();
		}
		
		// Агресия и тюрьма
		if(!m_Search && AccData.Rel >= 1000)
		{
			m_Search = true;
			GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("Player {str:name}, in the wanted list"), "name", Server()->ClientName(m_ClientID), NULL);
		}
		if(m_JailTick && AccData.Jail)
		{
			int Time = m_JailTick/Server()->TickSpeed();
			GameServer()->SendBroadcast_Localization(m_ClientID, 100, 100, _("You in jail, wait {sec:siska}."), "siska", &Time, NULL);		
			
			m_JailTick--;
			if(!m_JailTick)
			{
				m_JailTick = 0;
				AccData.Jail = false;
				
				if(m_pCharacter)
					m_pCharacter->Die(m_ClientID, WEAPON_WORLD);
				
				GameServer()->UpdateStats(m_ClientID);
				GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("The player {str:name}, leaves the prison"), "name", Server()->ClientName(m_ClientID), NULL);
			}
		}
		if(GetTeam() != TEAM_SPECTATORS && AccData.Rel > 0 && Server()->Tick() % (1 * Server()->TickSpeed() * 60) == 0)
		{
			AccData.Rel -= 100;
			if(AccData.Rel < 0)
				AccData.Rel = 0;
			
			if(AccData.Rel == 0 && m_Search)
			{
				m_Search = false;
				GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("Player {str:name}, crosed out in wanted list"), "name", Server()->ClientName(m_ClientID), NULL);
			}
			GameServer()->SendBroadcast_Localization(m_ClientID, BROADCAST_PRIORITY_GAMEANNOUNCE, BROADCAST_DURATION_GAMEANNOUNCE, _("Relations angry -100. Your {int:rel}"), "rel", &AccData.Rel, NULL);		
			GameServer()->UpdateStats(m_ClientID);
		}
		
		// вывод текста АРЕНА
		if(m_InArea)
		{
			if(GameServer()->m_AreaStartTick)
			{
				int Time = GameServer()->m_AreaStartTick/Server()->TickSpeed();
				GameServer()->SendBroadcast_Localization(m_ClientID, 101, 100, _("Wait start area {int:siska} sec."), "siska", &Time, NULL);						
					
				if(GameServer()->m_AreaStartTick == 100)
					GameServer()->SendBroadcast_Localization(m_ClientID, 105, 100, _("Fight!!."), NULL);						
			}
			else if(GameServer()->m_AreaEndGame)
			{
				int Time = GameServer()->m_AreaEndGame/Server()->TickSpeed();
				int couns = GameServer()->GetAreaCount();
				GameServer()->SendBroadcast_Localization(m_ClientID, 102, 100, _("End area {int:siska} sec. Life {int:num} player's"), "siska", &Time, "num", &couns, NULL);								
			}		
		}
		
		// вывод текста по поводу ожидания времени босса
		if(m_InBossed)
		{	
			if(GameServer()->m_WinWaitBoss)
			{
				int Time = GameServer()->m_WinWaitBoss/Server()->TickSpeed();
				GameServer()->SendBroadcast_Localization(m_ClientID, 101, 100, _("Wait players for pick item's {int:siska} sec."), "siska", &Time, NULL);						
			}
			else if(GameServer()->m_BossStartTick > 10*Server()->TickSpeed())
			{
				int Time = GameServer()->m_BossStartTick/Server()->TickSpeed();
				GameServer()->SendBroadcast_Localization(m_ClientID, 101, 100, _("Wait players for raid {sec:siska}. Boss: {str:name}"), "siska", &Time, "name", GameServer()->GetBossName(GameServer()->m_BossType), NULL);		
			}
			else if(Server()->Tick() % (1 * Server()->TickSpeed()) == 0 && GameServer()->m_BossStartTick > 100)
				GameServer()->SendGuide(m_ClientID, GameServer()->m_BossType);
			else if(GameServer()->m_BossStart)
				GameServer()->SendBroadcast_LBossed(m_ClientID, 250, 100);
		}
		
		// таймер синхронизации
		if(m_LoginSync)
		{
			m_LoginSync--;
			if(!m_LoginSync)
			{
				if(Server()->IsClientLogged(m_ClientID))
				{
					if(Server()->GetClanID(m_ClientID) > 0)
						Server()->UpdClanCount(Server()->GetClanID(m_ClientID));
						
					GameServer()->ResetVotes(m_ClientID, AUTH);
				}
			}
		}
		if(!Server()->IsClientLogged(m_ClientID))
			m_Team = -1;
			
		if(m_MapMenu > 0)
			m_MapMenuTick++;
	}

	// do latency stuff
	{
		IServer::CClientInfo Info;
		if(Server()->GetClientInfo(m_ClientID, &Info))
		{
			m_Latency.m_Accum += Info.m_Latency;
			m_Latency.m_AccumMax = max(m_Latency.m_AccumMax, Info.m_Latency);
			m_Latency.m_AccumMin = min(m_Latency.m_AccumMin, Info.m_Latency);
		}
		// each second
		if(Server()->Tick()%Server()->TickSpeed() == 0)
		{
			m_Latency.m_Avg = m_Latency.m_Accum/Server()->TickSpeed();
			m_Latency.m_Max = m_Latency.m_AccumMax;
			m_Latency.m_Min = m_Latency.m_AccumMin;
			m_Latency.m_Accum = 0;
			m_Latency.m_AccumMin = 1000;
			m_Latency.m_AccumMax = 0;
		}
	}

	if(!GameServer()->m_World.m_Paused)
	{
		if(!m_pCharacter && m_Team == TEAM_SPECTATORS && m_SpectatorID == SPEC_FREEVIEW)
			m_ViewPos -= vec2(clamp(m_ViewPos.x-m_LatestActivity.m_TargetX, -500.0f, 500.0f), clamp(m_ViewPos.y-m_LatestActivity.m_TargetY, -400.0f, 400.0f));

		if(!m_pCharacter && m_DieTick+Server()->TickSpeed()*3 <= Server()->Tick())
			m_Spawning = true;

		if(m_pCharacter)
		{
			if(m_pCharacter->IsAlive())
			{
				m_ViewPos = m_pCharacter->m_Pos;
			}
			else
			{
				m_pCharacter->Destroy();
				delete m_pCharacter;
				m_pCharacter = 0;
			}
		}
		else if(m_Spawning && m_RespawnTick <= Server()->Tick())
			TryRespawn();
	}
	else
	{
		++m_RespawnTick;
		++m_DieTick;
		++m_ScoreStartTick;
		++m_LastActionTick;
 	}
 	HandleTuningParams();
}

int CPlayer::GetNeedForUp()
{

	if(AccData.Level >= 100) return 10000;
	else if(AccData.Level >= 200) return 50000;
	else if(AccData.Level >= 300) return 80000;
	else if(AccData.Level >= 400) return 110000;
	else if(AccData.Level >= 500) return 130000;
	else if(AccData.Level >= 600) return 160000;
	else if(AccData.Level >= 700) return 190000;
	else if(AccData.Level >= 1000) return 240000;
	else if(AccData.Level >= 1100) return 300000;
	else if(AccData.Level >= 1200) return 400000;
	else return 400;

	/*
	if(AccData.Level >= 100) return 1000;
	else if(AccData.Level >= 200) return 5000;
	else if(AccData.Level >= 300) return 10000;
	else if(AccData.Level >= 400) return 50000;
	else if(AccData.Level >= 500) return 100000;
	else if(AccData.Level >= 600) return 150000;
	else if(AccData.Level >= 700) return 200000;
	else if(AccData.Level >= 1000) return 250000;
	else if(AccData.Level >= 1100) return 300000;
	else if(AccData.Level >= 1200) return 350000;
	else return 100;
	*/
}
int CPlayer::GetNeedForUpClan()
{
	int Get = Server()->GetClan(DLEVEL, Server()->GetClanID(m_ClientID));
	return 1000000+Get*1000000;
}
int CPlayer::GetNeedForUpgClan(int Type)
{
	int Get = Server()->GetClan(Type, Server()->GetClanID(m_ClientID));
	return 100+Get*500;
}

void CPlayer::PostTick()
{
	// update latency value
	if(m_PlayerFlags&PLAYERFLAG_SCOREBOARD)
	{
		for(int i = 0; i < MAX_CLIENTS; ++i)
		{
			if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() != TEAM_SPECTATORS)
				m_aActLatency[i] = GameServer()->m_apPlayers[i]->m_Latency.m_Min;
		}
	}

	// update view pos for spectators
	if(m_Team == TEAM_SPECTATORS && m_SpectatorID != SPEC_FREEVIEW && GameServer()->m_apPlayers[m_SpectatorID])
		m_ViewPos = GameServer()->m_apPlayers[m_SpectatorID]->m_ViewPos;
}

void CPlayer::HandleTuningParams()
{
	if(!(m_PrevTuningParams == m_NextTuningParams))
	{
		if(m_IsReady)
		{
			CMsgPacker Msg(NETMSGTYPE_SV_TUNEPARAMS);
			int *pParams = (int *)&m_NextTuningParams;
			for(unsigned i = 0; i < sizeof(m_NextTuningParams)/sizeof(int); i++)
				Msg.AddInt(pParams[i]);
			Server()->SendMsg(&Msg, MSGFLAG_VITAL, GetCID());
		}
		m_PrevTuningParams = m_NextTuningParams;
	}
	m_NextTuningParams = *GameServer()->Tuning();
}

void CPlayer::MoneyAdd(int Size, bool ClanBonus, bool MoneyDouble)
{
	if(IsBot())
		return;

	int GetMoney = Size;
	if(ClanBonus && Server()->GetClanID(m_ClientID))
		GetMoney = (Size+Server()->GetClan(DADDMONEY, Server()->GetClanID(m_ClientID))*100);
	
	if(MoneyDouble)
	{
		if(Server()->GetItemSettings(m_ClientID, X2MONEYEXPVIP))
			GetMoney = (int)GetMoney*(Server()->GetItemCount(m_ClientID, X2MONEYEXPVIP)*2);
		else if(MoneyDouble && (m_MoneyAdd))
			GetMoney = (int)(GetMoney*2);
	}

	if(Size >= 10000)
	{
		AccData.Gold += Size/10000;
		int Got = (int)(Size/10000);
		
		AccData.Money -= Got*10000;
	}

	GameServer()->SendBroadcast_LStat(m_ClientID, BROADCAST_PRIORITY_GAMEANNOUNCE, 100, INADDMONEY, GetMoney);
	AccData.Money += GetMoney;
	if(rand()%8 == 1) 
		GameServer()->UpdateStats(m_ClientID);

	GameServer()->ResetVotes(m_ClientID, AUTH);
	return;
}

void CPlayer::ExpAdd(int Size, bool Bonus)
{
	if(IsBot())
		return;
	
	int GetExp = Size, Get = 0;
	if(Bonus && Server()->GetClanID(m_ClientID))
	{
		Get = Size*50;
		Server()->InitClanID(Server()->GetClanID(m_ClientID), PLUS, "Exp", Get, true);
		GetExp = Size+Server()->GetClan(DADDEXP, Server()->GetClanID(m_ClientID));
	}	

	if(Bonus && m_ExperienceAdd)
		GetExp = GetExp*2;
	if(Server()->GetItemSettings(m_ClientID, X2MONEYEXPVIP))
		GetExp = GetExp*((Server()->GetItemCount(m_ClientID, X2MONEYEXPVIP))*2);

	if(Server()->GetClanID(m_ClientID) && 
		Server()->GetClan(DEXP, Server()->GetClanID(m_ClientID)) >= Server()->GetClan(DLEVEL, Server()->GetClanID(m_ClientID))*GetNeedForUpClan())
	{
		GameServer()->SendChatClan(Server()->GetClanID(m_ClientID), "[Clan Level UP] Great!");

		int warpminus = Server()->GetClan(DLEVEL, Server()->GetClanID(m_ClientID))*GetNeedForUpClan();
		Server()->InitClanID(Server()->GetClanID(m_ClientID), MINUS, "Exp", warpminus, true);
		Server()->InitClanID(Server()->GetClanID(m_ClientID), PLUS, "Level", 1, true);
	}

	GameServer()->SendBroadcast_LStat(m_ClientID, BROADCAST_PRIORITY_GAMEANNOUNCE, 100, Server()->GetClanID(m_ClientID) > 0 ? INADDCEXP : INADDEXP, GetExp, Get);
	AccData.Exp += GetExp;
	if(rand()%8 == 1) 
		GameServer()->UpdateStats(m_ClientID);

	return;
}

void CPlayer::Snap(int SnappingClient)
{
#ifdef CONF_DEBUG
	if(!g_Config.m_DbgDummies || m_ClientID < MAX_CLIENTS-g_Config.m_DbgDummies)
#endif
	if(!Server()->ClientIngame(m_ClientID))
		return;

	int id = m_ClientID;
	if (SnappingClient > -1 && !Server()->Translate(id, SnappingClient)) return;

	CNetObj_ClientInfo *pClientInfo = static_cast<CNetObj_ClientInfo *>(Server()->SnapNewItem(NETOBJTYPE_CLIENTINFO, id, sizeof(CNetObj_ClientInfo)));
	if(!pClientInfo)
		return;

	if(Server()->IsClientLogged(m_ClientID) && GetTeam() != TEAM_SPECTATORS)
	{
		char pSendName[32];
		str_format(pSendName, sizeof(pSendName), "%d:%s", AccData.Level, Server()->ClientName(m_ClientID));
		StrToInts(&pClientInfo->m_Name0, 4, pSendName);
		
		if(IsBot() && m_pCharacter)
		{
			
			float getlv = ((m_Health*100.0)/m_HealthStart)-1;
			switch(GetBotType())
			{
				default: str_format(pSendName, sizeof(pSendName), "%d:%s[%d\%]", AccData.Level, Server()->ClientName(m_ClientID), (int)getlv);	break;
				case BOT_NPC:
				case BOT_BOSSSLIME: str_format(pSendName, sizeof(pSendName), "%s[%d\%]", Server()->ClientName(m_ClientID), (int)getlv);	break;
				case BOT_NPCW: str_format(pSendName, sizeof(pSendName), "%s", Server()->ClientName(m_ClientID));	break;
			}
			StrToInts(&pClientInfo->m_Name0, 4, pSendName);
		}
	}
	else
		StrToInts(&pClientInfo->m_Name0, 4, Server()->ClientName(m_ClientID));

	if(Server()->IsClientLogged(m_ClientID) && tickstr) StrToInts(&pClientInfo->m_Clan0, 3, pTitle);
	else StrToInts(&pClientInfo->m_Clan0, 3, Server()->ClientClan(m_ClientID));
		
	pClientInfo->m_Country = Server()->ClientCountry(m_ClientID);

	StrToInts(&pClientInfo->m_Skin0, 6, m_TeeInfos.m_SkinName);
	pClientInfo->m_UseCustomColor = m_TeeInfos.m_UseCustomColor;
	pClientInfo->m_ColorBody = m_TeeInfos.m_ColorBody;
	pClientInfo->m_ColorFeet = m_TeeInfos.m_ColorFeet;

	CNetObj_PlayerInfo *pPlayerInfo = static_cast<CNetObj_PlayerInfo *>(Server()->SnapNewItem(NETOBJTYPE_PLAYERINFO, id, sizeof(CNetObj_PlayerInfo)));
	if(!pPlayerInfo)
		return;

	pPlayerInfo->m_Latency = SnappingClient == -1 ? m_Latency.m_Min : GameServer()->m_apPlayers[SnappingClient]->m_aActLatency[m_ClientID];
	pPlayerInfo->m_Local = 0;
	pPlayerInfo->m_ClientID = id;
	
	if(GetBotType() == BOT_BOSSSLIME || !IsBot()) pPlayerInfo->m_Score = AccData.Level;
	else pPlayerInfo->m_Score = 0;
		
	if(!IsBot()) pPlayerInfo->m_Team = m_Team;
	else pPlayerInfo->m_Team = 10;

	if(m_ClientID == SnappingClient)
		pPlayerInfo->m_Local = 1;

	if(m_ClientID == SnappingClient && m_Team == TEAM_SPECTATORS)
	{
		CNetObj_SpectatorInfo *pSpectatorInfo = static_cast<CNetObj_SpectatorInfo *>(Server()->SnapNewItem(NETOBJTYPE_SPECTATORINFO, m_ClientID, sizeof(CNetObj_SpectatorInfo)));
		if(!pSpectatorInfo)
			return;

		pSpectatorInfo->m_SpectatorID = m_SpectatorID;
		pSpectatorInfo->m_X = m_ViewPos.x;
		pSpectatorInfo->m_Y = m_ViewPos.y;
	}
}

void CPlayer::FakeSnap(int SnappingClient)
{
	IServer::CClientInfo info;
	Server()->GetClientInfo(SnappingClient, &info);
	if (info.m_CustClt)
		return;

	int id = VANILLA_MAX_CLIENTS - 1;

	CNetObj_ClientInfo *pClientInfo = static_cast<CNetObj_ClientInfo *>(Server()->SnapNewItem(NETOBJTYPE_CLIENTINFO, id, sizeof(CNetObj_ClientInfo)));

	if(!pClientInfo)
		return;

	StrToInts(&pClientInfo->m_Name0, 4, " ");
	StrToInts(&pClientInfo->m_Clan0, 3, "");
	StrToInts(&pClientInfo->m_Skin0, 6, m_TeeInfos.m_SkinName);
}

void CPlayer::OnDisconnect(const char *pReason)
{
	GameServer()->ClearVotes(m_ClientID);
	KillCharacter();

	if(Server()->ClientIngame(m_ClientID))	
		GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("{str:PlayerName} has left the game {str:Reason}"), "PlayerName", Server()->ClientName(m_ClientID), NULL);
}

void CPlayer::OnPredictedInput(CNetObj_PlayerInput *NewInput)
{
	// skip the input if chat is active
	if((m_PlayerFlags&PLAYERFLAG_CHATTING) && (NewInput->m_PlayerFlags&PLAYERFLAG_CHATTING))
		return;

	if(m_pCharacter)
		m_pCharacter->OnPredictedInput(NewInput);
}

void CPlayer::OnDirectInput(CNetObj_PlayerInput *NewInput)
{
	if(NewInput->m_PlayerFlags&PLAYERFLAG_CHATTING)
	{
		// skip the input if chat is active
		if(m_PlayerFlags&PLAYERFLAG_CHATTING)
			return;

		// reset input
		if(m_pCharacter)
			m_pCharacter->ResetInput();

		m_PlayerFlags = NewInput->m_PlayerFlags;
 		return;
	}

	m_PlayerFlags = NewInput->m_PlayerFlags;

	if(m_pCharacter)
		m_pCharacter->OnDirectInput(NewInput);

	if(!m_pCharacter && m_Team != TEAM_SPECTATORS && (NewInput->m_Fire&1))
		m_Spawning = true;

	// check for activity
	if(NewInput->m_Direction || m_LatestActivity.m_TargetX != NewInput->m_TargetX ||
		m_LatestActivity.m_TargetY != NewInput->m_TargetY || NewInput->m_Jump ||
		NewInput->m_Fire&1 || NewInput->m_Hook)
	{
		m_LatestActivity.m_TargetX = NewInput->m_TargetX;
		m_LatestActivity.m_TargetY = NewInput->m_TargetY;
		m_LastActionTick = Server()->Tick();
	}
}

CCharacter *CPlayer::GetCharacter()
{
	if(m_pCharacter && m_pCharacter->IsAlive())
		return m_pCharacter;
	return 0;
}

void CPlayer::KillCharacter(int Weapon)
{
	if(m_pCharacter)
	{
		if(m_ClientID != 63)
			m_pCharacter->Die(m_ClientID, Weapon);
			
		delete m_pCharacter;
		m_pCharacter = 0;
	}
}

void CPlayer::Respawn()
{
	if(m_Team != TEAM_SPECTATORS)
		m_Spawning = true;
}

void CPlayer::SetTeam(int Team, bool DoChatMsg)
{
	// clamp the team
	Team = GameServer()->m_pController->ClampTeam(Team);
	if(DoChatMsg)
		GameServer()->SendChatTarget_Localization(-1, -1, _("{str:PlayerName} joined the RPG Azataz"), "PlayerName", Server()->ClientName(m_ClientID), NULL);
	
	KillCharacter();

	m_Team = Team;
	m_LastActionTick = Server()->Tick();
	m_SpectatorID = SPEC_FREEVIEW;
	
	// we got to wait 0.5 secs before respawning
	m_RespawnTick = Server()->Tick()+Server()->TickSpeed()/2;
	GameServer()->m_pController->OnPlayerInfoChange(GameServer()->m_apPlayers[m_ClientID]);

	if(Team == TEAM_SPECTATORS)
	{
		// update spectator modes
		for(int i = 0; i < MAX_CLIENTS; ++i)
		{
			if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->m_SpectatorID == m_ClientID)
				GameServer()->m_apPlayers[i]->m_SpectatorID = SPEC_FREEVIEW;
		}
	}	
}

void CPlayer::TryRespawn()
{
	vec2 SpawnPos;
	if(!GameServer()->m_pController->PreSpawn(this, &SpawnPos))
		return;

	m_Spawning = false;
	
	if (IsBot())
    {
		// жирный бот рандом
		if(rand()% 20 == 10) m_BigBot = true;
		else m_BigBot = false;
				
        GameServer()->UpdateBotInfo(m_ClientID);
		if(m_BotType == BOT_L1MONSTER)
		{
			m_pCharacter = new(m_ClientID) CMonster(&GameServer()->m_World);
			
			if(g_Config.m_SvCityStart == 1)
			{
				AccData.Level = m_BigBot ? 280+rand()%3 : 250;
				AccUpgrade.Health = 100+AccData.Level*2;		
				AccUpgrade.Damage = AccData.Level+50;		
			}
			else
			{
				AccData.Level = m_BigBot ? 10+rand()%3 : 5;
				AccUpgrade.Health = m_BigBot ? AccData.Level : 0;
				if(m_BigBot)
				{
					Server()->SetMaxAmmo(m_ClientID, INFWEAPON_GUN, 10);
					Server()->SetAmmoRegenTime(m_ClientID, INFWEAPON_GUN, 100);
					Server()->SetFireDelay(m_ClientID, INFWEAPON_GUN, 800);					
				}
			}
		}
		else if(m_BotType == BOT_L2MONSTER)
		{
			m_pCharacter = new(m_ClientID) CKwah(&GameServer()->m_World);
			
			if(g_Config.m_SvCityStart == 1)
			{
				AccData.Level = m_BigBot ? 370+rand()%3 : 350+rand()%3;
				AccUpgrade.Health = 100+AccData.Level*2;		
				AccUpgrade.Damage = AccData.Level+50;		
			}
			else
			{
				AccData.Level = m_BigBot ? 140 : 125+rand()%3;
				AccUpgrade.Health = 100+AccData.Level;
				AccUpgrade.Damage = AccData.Level/2;
			}
		}
		else if(m_BotType == BOT_L3MONSTER)
		{
			m_pCharacter = new(m_ClientID) CBoomer(&GameServer()->m_World);

			if(g_Config.m_SvCityStart == 1)
			{
				AccData.Level = m_BigBot ? 510+rand()%3 : 490+rand()%15;
				AccUpgrade.Health = 100+(int)(AccData.Level*2);		
				AccUpgrade.Damage = (int)(AccData.Level+50);		
			}
			else
			{
				AccData.Level = m_BigBot ? 250+rand()%3 : 200+rand()%3;
				AccUpgrade.Health = 100+AccData.Level;
				AccUpgrade.Damage = AccData.Level;
			}
		}
		else if(m_BotType == BOT_BOSSSLIME)
		{
			m_pCharacter = new(m_ClientID) CBossSlime(&GameServer()->m_World);
			AccData.Level = 1000+rand()%3;
			
			m_BigBot = true;

			AccUpgrade.Health = (int)(AccData.Level/3);
			AccUpgrade.Damage = 10;
			if(g_Config.m_SvCityStart == 1)
				AccUpgrade.Damage = 180;
		}
		else if(m_BotType == BOT_NPC)
		{
			m_pCharacter = new(m_ClientID) CNpcSold(&GameServer()->m_World);
			AccData.Level = 500+rand()%10;
			AccUpgrade.Damage = (int)(AccData.Level*5);
			AccUpgrade.Health = (int)(AccData.Level*50);
			m_BigBot = true;
		}
		else if(m_BotType == BOT_NPCW)
		{
			m_pCharacter = new(m_ClientID) CNpcWSold(&GameServer()->m_World);
			AccData.Level = 3;
			AccUpgrade.Damage = (int)(AccData.Level*5);
			AccUpgrade.Health = (int)(AccData.Level*2);
			m_BigBot = true;
		}	
		else if(m_BotType == BOT_FARMER)
		{
			m_pCharacter = new(m_ClientID) CNpcFarmer(&GameServer()->m_World);
			AccData.Level = 3;
			AccUpgrade.Damage = (int)(AccData.Level*5);
			AccUpgrade.Health = (int)(AccData.Level*2);
			m_BigBot = true;
		}
		Server()->SetMaxAmmo(m_ClientID, INFWEAPON_HAMMER, -1);
		Server()->SetAmmoRegenTime(m_ClientID, INFWEAPON_HAMMER, 0);
		Server()->SetFireDelay(m_ClientID, INFWEAPON_HAMMER, 1);
	}
	else
	{
		m_pCharacter = new(m_ClientID) CCharacter(&GameServer()->m_World);
	}
	
	m_pCharacter->Spawn(this, SpawnPos);
	if(GetClass() != PLAYERCLASS_NONE)
		GameServer()->CreatePlayerSpawn(SpawnPos);
}

int CPlayer::GetClass()
{
	return AccData.Class;
}

void CPlayer::SetClassSkin(int newClass, int State)
{
	switch(newClass)
	{
		case PLAYERCLASS_ASSASINS:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "bluekitty", sizeof(m_TeeInfos.m_SkinName));
			break;
		case PLAYERCLASS_BERSERK:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "coala", sizeof(m_TeeInfos.m_SkinName));
			break;
		case PLAYERCLASS_HEALER:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "redstripe", sizeof(m_TeeInfos.m_SkinName));
			break;
		default:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "default", sizeof(m_TeeInfos.m_SkinName));
	}
}

void CPlayer::SetClass(int newClass)
{	
	if(AccData.Class == newClass)
		return;
	
	AccData.Class = newClass;
	SetClassSkin(newClass);
	
	if(m_pCharacter)
		m_pCharacter->SetClass(newClass);
}

bool CPlayer::IsKownClass(int c)
{
	return m_knownClass[c];
}

const char* CPlayer::GetLanguage()
{
	return m_aLanguage;
}

const char* CPlayer::GetClassName()
{
	if(AccData.Class == PLAYERCLASS_ASSASINS)
		return "Assasin";
	else if(AccData.Class == PLAYERCLASS_BERSERK)
		return "Berserk";
	else if(AccData.Class == PLAYERCLASS_HEALER)
		return "Healer";
	else 
		return "You bitch";
}

void CPlayer::SetLanguage(const char* pLanguage)
{
	str_copy(m_aLanguage, pLanguage, sizeof(m_aLanguage));
}
void CPlayer::OpenMapMenu(int Menu)
{
	m_MapMenu = Menu;
	m_MapMenuTick = 0;
}

void CPlayer::CloseMapMenu()
{
	m_MapMenu = 0;
	m_MapMenuTick = -1;
}

bool CPlayer::MapMenuClickable()
{
	return (m_MapMenu > 0 && (m_MapMenuTick > Server()->TickSpeed()/2));
}

void CPlayer::ResetUpgrade(int ClientID)
{
	if(Server()->IsClientLogged(m_ClientID))
	{
		int Back = AccUpgrade.Speed + AccUpgrade.Health + AccUpgrade.Damage + AccUpgrade.HPRegen +  AccUpgrade.Mana + AccUpgrade.AmmoRegen + (AccUpgrade.Ammo*5 + AccUpgrade.Spray*10);
		AccUpgrade.Speed = AccUpgrade.Health = AccUpgrade.Damage = AccUpgrade.HPRegen = AccUpgrade.Mana = 0;
		AccUpgrade.AmmoRegen =AccUpgrade.Ammo = AccUpgrade.Spray = 0;

		AccUpgrade.Upgrade += Back;
		GameServer()->UpdateUpgrades(ClientID);
	}
}

void CPlayer::ResetSkill(int ClientID)
{
	if(Server()->IsClientLogged(m_ClientID))
	{
		int Back = AccUpgrade.HammerRange*15 + AccUpgrade.Pasive2*15;	
		AccUpgrade.Pasive2 = AccUpgrade.HammerRange = 0;
		AccUpgrade.SkillPoint += Back;	
		GameServer()->UpdateUpgrades(ClientID);
	}	
}

const char* CPlayer::TitleGot()
{
	if(Server()->GetItemSettings(m_ClientID, X2MONEYEXPVIP))
	{
		int i = Server()->GetItemCount(m_ClientID, X2MONEYEXPVIP);
		if(i == 1)
			return "_VIP Bronze";
		else if(i == 2)
			return "_VIP Silver";
		else if(i == 3)
			return "_VIP Gold";
		else if(i == 4)
			return "_VIP Gold";
		else
			return "_Platinum";
	}
	else if(Server()->GetItemSettings(m_ClientID, TITLEQUESTS))
		return "1LVQuests";
	else if(Server()->GetItemSettings(m_ClientID, BOSSDIE))
		return "#Boss?Die";
	else if(Server()->GetItemSettings(m_ClientID, PIGPIG))
		return "_Piggie_";
	else if(Server()->GetItemSettings(m_ClientID, BIGCRAFT))
		return "_Crafter_"; 
	else if(Server()->GetItemSettings(m_ClientID, TITLESUMMER))
		return "_I<3Summer";
	else if(Server()->GetItemSettings(m_ClientID, TITLEENCHANT))
		return "Enchant+10";
	else 
		return "_Newbie_";
}   
 
void CPlayer::SetMoveChar()
{  
	char aBuf[64];
	str_format(aBuf, sizeof(aBuf), "%s | %s", Server()->ClientClan(m_ClientID), TitleGot());
	str_copy(pTitle, aBuf, sizeof(pTitle));
	tickstr = 90;
}  