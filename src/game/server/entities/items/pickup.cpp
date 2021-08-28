/* 2019年2月10日13:55:38 */
/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include "pickup.h"

CPickup::CPickup(CGameWorld *pGameWorld, int Type, vec2 Pos, int SubType)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP)
{
	m_Type = Type;
	m_Pos = Pos;
	m_SubType = SubType;
	m_Drop = 0;

	Reset();

	GameWorld()->InsertEntity(this);
}

void CPickup::Reset()
{
	if (g_pData->m_aPickups[m_Type].m_Spawndelay > 0)
		m_SpawnTick = Server()->Tick() + Server()->TickSpeed() * g_pData->m_aPickups[m_Type].m_Spawndelay;
	else
		m_SpawnTick = -1;
}

void CPickup::Tick()
{
	// wait for respawn
	if(m_SpawnTick > 0)
	{
		if(Server()->Tick() > m_SpawnTick)
		{
			// respawn
			m_SpawnTick = -1;

			if(m_Type == WEAPON_GRENADE || m_Type == WEAPON_SHOTGUN || m_Type == WEAPON_RIFLE)
				GameServer()->CreateSound(m_Pos, SOUND_WEAPON_SPAWN);
		}
		else
			return;
	}

	// Find other players
	for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
	{
		if(!p->GetPlayer()->IsBot() && distance(p->m_Pos, m_Pos) < 20) 
		{
			switch (m_Type)
			{
				case 0:
					if(m_SubType != 3 && m_SubType != 4 && m_SubType != 5)
					{
						GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH);
						Picking(0);
					}
					break;

				case 1:
					//Picked = true;
					break;

				case 3:
					Picking(0);
					break;

				case 2:
					Picking(0);
					break;

				case 4:
					Picking(0);
					break;

				case 5:
					Picking(0);
					break;

				default:
					break;
			};

		}
	}
}

void CPickup::Picking(int Time)
{
	int RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime+Time;
	if(RespawnTime >= 0)
		m_SpawnTick = Server()->Tick() + Server()->TickSpeed() * RespawnTime;
}

void CPickup::StartFarm(int ClientID)
{
	CPlayer *pFarm = GameServer()->m_apPlayers[ClientID];
	if(!pFarm || !pFarm->GetCharacter() || m_SpawnTick != -1 || pFarm->IsBot())
		return;

	if(!m_SubType) // ########################### FARMING
	{
		int Dropable = 0;
		int Broke = 0;
		int Count = 0;
		const char* ItemName = "啥都没有";
		m_Drop += 20;	
		if(Server()->GetItemCount(ClientID, DRAGONHOE))
		{
			Count = Server()->GetItemCount(ClientID, DRAGONHOE);
			Broke = 8000*Server()->GetItemCount(ClientID, DRAGONHOE);
			Dropable = Server()->GetItemSettings(ClientID, DRAGONHOE);
			if(Dropable <= 0)
			{
				Server()->RemItem(ClientID, DRAGONHOE, Server()->GetItemCount(ClientID, DRAGONHOE), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ 农具: {str:name} 已损毁, 不能用了"), "name", Server()->GetItemName(ClientID, DRAGONHOE), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, DRAGONHOE, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, DRAGONHOE);
			m_Drop += 5;	
		}
		
		GameServer()->CreateSound(m_Pos, 20); 

		int LevelItem = 1+Server()->GetItemCount(ClientID, FARMLEVEL)/g_Config.m_SvFarmExp;
		int NeedExp = LevelItem*g_Config.m_SvFarmExp;
		int Exp = Server()->GetItemCount(ClientID, FARMLEVEL);

		float getlv = (m_Drop*100.0)/100;
		const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("专长 - 种地: {int:lvl}级: {int:exp}/{int:expneed}经验\n工具: {str:name}x{int:count} ({int:brok}/{int:brok2})\n物品 : 采集进度: {str:got} / {int:gotp}%"), 
			"lvl", &LevelItem, "exp", &Exp, "expneed", &NeedExp, "name", ItemName, "count", &Count, "brok", &Dropable, "brok2", &Broke, "got", Pick, "gotp", &m_Drop, NULL);
		delete Pick;

		if(m_Drop == 100)
		{
			if(rand()%2 == 0) Server()->SetMaterials(2, Server()->GetMaterials(2)+1);

			switch(rand()%5)
			{
				case 0:	GameServer()->GiveItem(ClientID, TOMATE, LevelItem); break; 
				case 1: GameServer()->GiveItem(ClientID, POTATO, LevelItem); break;
				case 2: GameServer()->GiveItem(ClientID, CABBAGE, LevelItem); break;
				default: GameServer()->GiveItem(ClientID, CARROT, LevelItem); break;
			}
			if(Server()->GetItemCount(ClientID, FARMLEVEL) % g_Config.m_SvFarmExp == 0)
			{
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ 种地等级提升~ 获得了农耕盲盒"), NULL);
				GameServer()->GiveItem(ClientID, FARMBOX, 1);
			}
			if(rand()%120 == 0)
				GameServer()->GiveItem(ClientID, FARMBOX, 1);

			GameServer()->GiveItem(ClientID, FARMLEVEL, 1);
		}
	}
	else if(m_SubType == 2) // ########################### MINER
	{
		int Dropable = 0;
		int Broke = 0;
		int Count = 0;
		const char* ItemName = "Nope";
		if(Server()->GetItemCount(ClientID, DIAMONDPIX))
		{
			Count = Server()->GetItemCount(ClientID, DIAMONDPIX);
			Broke = 699*Server()->GetItemCount(ClientID, DIAMONDPIX);
			Dropable = Server()->GetItemSettings(ClientID, DIAMONDPIX);
			if(Dropable <= 0)
			{
				Server()->RemItem(ClientID, DIAMONDPIX, Server()->GetItemCount(ClientID, DIAMONDPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ 矿具: {str:name} 已损毁, 不能用了"), "name", Server()->GetItemName(ClientID, DIAMONDPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, DIAMONDPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, DIAMONDPIX);
			m_Drop += 35;	
		}
		else if(Server()->GetItemCount(ClientID, GOLDPIX))
		{
			Count = Server()->GetItemCount(ClientID, GOLDPIX);
			Broke = 491*Server()->GetItemCount(ClientID, GOLDPIX);
			Dropable = Server()->GetItemSettings(ClientID, GOLDPIX);
			if(Dropable <= 0)
			{
				Server()->RemItem(ClientID, GOLDPIX, Server()->GetItemCount(ClientID, GOLDPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ 矿具: {str:name} 已损毁, 不能用了"), "name", Server()->GetItemName(ClientID, GOLDPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, GOLDPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, GOLDPIX);
			m_Drop += 20;	
		}
		else if(Server()->GetItemCount(ClientID, IRONPIX))
		{
			Count = Server()->GetItemCount(ClientID, IRONPIX);
			Broke = 211*Server()->GetItemCount(ClientID, IRONPIX);
			Dropable = Server()->GetItemSettings(ClientID, IRONPIX);
			if(Dropable <= 0)
			{
				Server()->RemItem(ClientID, IRONPIX, Server()->GetItemCount(ClientID, IRONPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ 矿具: {str:name} 已损毁, 不能用了"), "name", Server()->GetItemName(ClientID, IRONPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, IRONPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, IRONPIX);
			m_Drop += 15;	
		}
		else if(Server()->GetItemCount(ClientID, COOPERPIX))
		{
			Count = Server()->GetItemCount(ClientID, COOPERPIX);
			Broke = 180*Server()->GetItemCount(ClientID, COOPERPIX);
			Dropable = Server()->GetItemSettings(ClientID, COOPERPIX);
			if(Dropable <= 0)
			{
				Server()->RemItem(ClientID, COOPERPIX, Server()->GetItemCount(ClientID, COOPERPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ 矿具: {str:name} 已损毁, 不能用了"), "name", Server()->GetItemName(ClientID, COOPERPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, COOPERPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, COOPERPIX);
			m_Drop += 10;	
		}
		else
		{
			m_Drop += 5;
		}
		GameServer()->CreateSound(m_Pos, 20); 

		int LevelItem = 1+Server()->GetItemCount(ClientID, MINEREXP)/g_Config.m_SvMinerExp;
		int ExpNeed = LevelItem*g_Config.m_SvMinerExp;
		int Exp = Server()->GetItemCount(ClientID, MINEREXP);
		
		float getlv = (m_Drop*100.0)/100;
		const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("专长 - 采掘: {int:lvl}级 : {int:exp}/{int:expneed}经验\n工具: {str:name}x{int:count} ({int:brok}/{int:brok2})\n挖掘进度: {str:got} / {int:gotp}%"), 
			"lvl", &LevelItem, "exp", &Exp, "expneed", &ExpNeed, "brok", &Dropable, "brok2", &Broke, "name", ItemName, "count", &Count, "got", Pick, "gotp", &m_Drop, NULL);
		delete Pick;

		if(m_Drop >= 100)
		{
			int ItemDrop = 3+LevelItem/g_Config.m_SvMinerUpgrade;
			if(ItemDrop > 11)
				ItemDrop = 11;
			int DragonOre = 1 + LevelItem / 250;
			if(DragonOre >= 6)
			{
				DragonOre = 6;
			}
			switch(rand()%ItemDrop)
			{
				case 3: GameServer()->GiveItem(ClientID, IRONORE, 1+LevelItem/15); break; 
				case 4: GameServer()->GiveItem(ClientID, GOLDORE, 1+LevelItem/15); break; 
				case 5: GameServer()->GiveItem(ClientID, DIAMONDORE, 1+LevelItem/15); break; 
				case 7: GameServer()->GiveItem(ClientID, DRAGONORE, DragonOre); break; 
				default: GameServer()->GiveItem(ClientID, COOPERORE, 1+LevelItem/15); break;
			}
			GameServer()->GiveItem(ClientID, MINEREXP, 1);
			
			if(rand()%500==1)
				GameServer()->GiveItem(ClientID, STANNUM, 1);

			

			// 加经验
			GameServer()->m_apPlayers[ClientID]->AccData.Exp += 10+LevelItem;
			GameServer()->SendChatTarget_Localization(ClientID, -1, _("[Player] 经验+10 +{int:bonus}点专长经验"), "bonus", &LevelItem, NULL);
		}
	}
	else if(m_SubType == 3) // ########################### WOOOD
	{
		int Dropable = 0;
		int Broke = 0;
		int Count = 0;
		m_Drop += 10+rand()%25;
		const char* ItemName = "啥都没有";
		if(Server()->GetItemCount(ClientID, DRAGONAXE))
		{
			Count = Server()->GetItemCount(ClientID, DRAGONAXE);
			Broke = 10000*Server()->GetItemCount(ClientID, DRAGONAXE);
			Dropable = Server()->GetItemSettings(ClientID, DRAGONAXE);
			if(Dropable <= 0)
			{
				Server()->RemItem(ClientID, DRAGONAXE, Server()->GetItemCount(ClientID, DRAGONAXE), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ 工具: {str:name} 已损毁, 不能用了"), "name", Server()->GetItemName(ClientID, DRAGONAXE), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, DRAGONAXE, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, DRAGONAXE);
			m_Drop *=2 ;	
		}

		GameServer()->CreateSound(m_Pos, 20); 

		float getlv = (m_Drop*100.0)/100;
		const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("专长 - 伐木工: (光头强不会升级)\n工具: {str:name}x{int:count} ({int:brok}/{int:brok2})\n砍伐进度: {str:got} / {int:gotp}%"), 
			"name", ItemName, "count", &Count, "brok", &Dropable, "brok2", &Broke, "got", Pick, "gotp", &m_Drop, NULL);
		delete Pick;

		if(m_Drop >= 100)
		{
			GameServer()->GiveItem(ClientID, WOOD, 1);
		
			// 加经验
			GameServer()->m_apPlayers[ClientID]->AccData.Exp += 10;
			GameServer()->SendChatTarget_Localization(ClientID, -1, _("[Player] 经验+10 +并不存在的专长经验"), NULL);
		}
	}

	else if(m_SubType == 4) // ########################### LOADER
	{
		MaterFarm(ClientID, 1);
	}

	else if(m_SubType == 5) // ########################### LOADER FARM
	{
		MaterFarm(ClientID, 2);
	}

	if(m_Drop >= 100)
	{
		m_Drop = 0;
		GameServer()->CreateSoundGlobal(7, ClientID);
		if(m_SubType != 4)
			Picking(30);
	}
}
//TODO
void CPickup::MaterFarm(int ClientID, int MaterialID)
{
	if(Server()->GetMaterials(MaterialID) < 25)
		return	GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("你没有足够的附魔材料(material). 至少需要25个"), NULL); 
	
	if(Server()->GetItemCount(ClientID, MATERIAL) > 50000)
		return	GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("物品栏内最多塞50000个附魔材料(material). 在物品栏对装备附魔吧!"), NULL); 

	m_Drop += 25;
	GameServer()->CreateSound(m_Pos, 20); 

	int LevelItem = 1+Server()->GetItemCount(ClientID, LOADEREXP)/g_Config.m_SvMaterExp;
	int NeedExp = LevelItem*g_Config.m_SvMaterExp;
	int Exp = Server()->GetItemCount(ClientID, LOADEREXP);
	int Bonus = LevelItem*3 ;

	float getlv = (m_Drop*100.0)/100;
	const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
	GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("专长 - 萃取: {int:lvl}级 : {int:exp}/{int:expneed}经验\n奖励: 25+{int:bonus} : 萃取进度: {str:got} / {int:gotp}%"), 
		"lvl", &LevelItem, "exp", &Exp, "expneed", &NeedExp, "bonus", &Bonus, "got", Pick, "gotp", &m_Drop, NULL);
	delete Pick;
	
	if(m_Drop >= 100)
	{
		Server()->SetMaterials(MaterialID, Server()->GetMaterials(MaterialID)-25);
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("在装备栏对装备附魔或者卖给商店吧."), NULL);
		GameServer()->GiveItem(ClientID, MATERIAL, 25+LevelItem*3);
		GameServer()->GiveItem(ClientID, LOADEREXP, 10);

		// 加经验
		GameServer()->m_apPlayers[ClientID]->AccData.Exp += 20+LevelItem;
		GameServer()->SendChatTarget_Localization(ClientID, -1, _("[Player] 经验+20 +{int:bonus}点专长经验"), "bonus", &LevelItem, NULL);

		Picking(5);
	}
}

void CPickup::TickPaused()
{
	if(m_SpawnTick != -1)
		++m_SpawnTick;
}

void CPickup::Snap(int SnappingClient)
{
	if(m_SpawnTick != -1 || NetworkClipped(SnappingClient))
		return;

	if(m_SubType != 1)
	{
		CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
		if(!pP)
			return;

		pP->m_X = (int)m_Pos.x;
		pP->m_Y = (int)m_Pos.y;
		pP->m_Type = m_Type;
	}
	else
	{
		CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
		if(pObj)
		{
			pObj->m_X = (int)m_Pos.x;
			pObj->m_Y = (int)m_Pos.y;
			pObj->m_VelX = 0;
			pObj->m_VelY = 0;
			pObj->m_StartTick = Server()->Tick();
			pObj->m_Type = WEAPON_HAMMER;
		}		
	}
}
