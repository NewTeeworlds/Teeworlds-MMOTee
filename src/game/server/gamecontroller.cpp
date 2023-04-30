/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>
#include <game/mapitems.h>

#include <game/generated/protocol.h>

#include "gamecontroller.h"
#include "gamecontext.h"

IGameController::IGameController(class CGameContext *pGameServer)
{
	m_pGameServer = pGameServer;
	m_pServer = m_pGameServer->Server();
	m_pGameType = "unknown";

	//
	m_GameOverTick = -1;
	m_SuddenDeath = 0;
	m_RoundStartTick = Server()->Tick();
	m_GameFlags = 22;
	m_aMapWish[0] = 0;

	m_UnbalancedTick = -1;
}

IGameController::~IGameController()
{
}

bool IGameController::PreSpawn(CPlayer *pPlayer, vec2 *pOutPos)
{
	return false;
}

bool IGameController::OnEntity(const char *pName, vec2 Pivot, vec2 P0, vec2 P1, vec2 P2, vec2 P3, int PosEnv)
{
	vec2 Pos = (P0 + P1 + P2 + P3) / 4.0f;

	if (str_comp(pName, "NoRegister") == 0)
		m_SpawnPoints[0].add(Pos);
	else if (str_comp(pName, "Register") == 0)
		m_SpawnPoints[1].add(Pos);
	else if (str_comp(pName, "Guard") == 0)
		m_SpawnPoints[2].add(Pos);
	else if (str_comp(pName, "Bot1Type") == 0)
		m_SpawnPoints[3].add(Pos);
	else if (str_comp(pName, "Jail") == 0)
		m_SpawnPoints[4].add(Pos);
	else if (str_comp(pName, "Bot2Type") == 0)
		m_SpawnPoints[5].add(Pos);
	else if (str_comp(pName, "Bot3Type") == 0)
		m_SpawnPoints[6].add(Pos);
	else if (str_comp(pName, "WaitBoss") == 0)
		m_SpawnPoints[7].add(Pos);
	else if (str_comp(pName, "BossSpawn") == 0)
		m_SpawnPoints[8].add(Pos);
	else if (str_comp(pName, "ArenaIns") == 0)
		m_SpawnPoints[9].add(Pos);
	else if (str_comp(pName, "NPCSHOP") == 0)
		m_SpawnPoints[10].add(Pos);
	else if (str_comp(pName, "NPCWQUEST") == 0)
		m_SpawnPoints[11].add(Pos);
	else if (str_comp(pName, "NPCWCRAFT") == 0)
		m_SpawnPoints[12].add(Pos);
	else if (str_comp(pName, "ArenaFng") == 0)
		m_SpawnPoints[13].add(Pos);
	else if (str_comp(pName, "NPCFARMER") == 0)
		m_SpawnPoints[14].add(Pos);
	else if (str_comp(pName, "FRSpawn") == 0)
		m_SpawnPoints[15].add(Pos);
	else if (str_comp(pName, "ClanSp1") == 0)
		m_SpawnPoints[16].add(Pos);
	else if (str_comp(pName, "ClanSp2") == 0)
		m_SpawnPoints[17].add(Pos);
	else if (str_comp(pName, "ClanSp3") == 0)
		m_SpawnPoints[18].add(Pos);
	else if (str_comp(pName, "ClanSp4") == 0)
		m_SpawnPoints[19].add(Pos);
	else if (str_comp(pName, "ClanSp5") == 0)
		m_SpawnPoints[20].add(Pos);

	return false;
}

double IGameController::GetTime()
{
	return static_cast<double>(Server()->Tick() - m_RoundStartTick) / Server()->TickSpeed();
}

void IGameController::ResetGame()
{
	GameServer()->m_World.m_ResetRequested = true;
}

const char *IGameController::GetTeamName(int Team)
{
	if (IsTeamplay())
	{
		if (Team == TEAM_RED)
			return "red team";
		else if (Team == TEAM_BLUE)
			return "blue team";
	}
	else
	{
		if (Team == 0)
			return "game";
	}

	return "spectators";
}

void IGameController::StartRound()
{
	ResetGame();

	m_RoundStartTick = Server()->Tick();
	m_SuddenDeath = 0;
	m_GameOverTick = -1;
	GameServer()->m_World.m_Paused = false;
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "start round type='%s' teamplay='%d'", m_pGameType, m_GameFlags & GAMEFLAG_TEAMS);
	GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);
}

void IGameController::PostReset()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (GameServer()->m_apPlayers[i])
		{
			GameServer()->m_apPlayers[i]->Respawn();
			GameServer()->m_apPlayers[i]->m_RespawnTick = Server()->Tick() + Server()->TickSpeed() / 2;
		}
	}
}

void IGameController::OnPlayerInfoChange(class CPlayer *pP)
{
	const int aTeamColors[2] = {65387, 10223467};

	if (pP->GetBotType() == BOT_L1MONSTER && pP->GetBotSubType() == 1)
	{
		pP->m_TeeInfos.m_UseCustomColor = 1;
		pP->m_TeeInfos.m_ColorBody = 65387;
		pP->m_TeeInfos.m_ColorFeet = 65387;
	}
	if (pP->GetBotType() == BOT_L2MONSTER && pP->GetBotSubType() == 1)
	{
		pP->m_TeeInfos.m_UseCustomColor = 1;
		pP->m_TeeInfos.m_ColorBody = 100;
		pP->m_TeeInfos.m_ColorFeet = 100;
	}
	if (pP->GetBotType() == BOT_L3MONSTER && pP->GetBotSubType() == 1)
	{
		pP->m_TeeInfos.m_UseCustomColor = 1;
		pP->m_TeeInfos.m_ColorBody = 15387000;
		pP->m_TeeInfos.m_ColorFeet = 15387000;
	}

	// Тело броня

	if (!Server()->GetItemSettings(pP->GetCID(), CUSTOMCOLOR))
	{
		if (Server()->GetItemSettings(pP->GetCID(), LEATHERBODY))
		{
			if (pP->GetCharacter())
			{
				pP->GetCharacter()->DeleteAllPickup();
				pP->GetCharacter()->CreatePickupDraw(1, 0, 3, true);
			}
			pP->m_TeeInfos.m_ColorBody = 22315;
		}
		else if (Server()->GetItemSettings(pP->GetCID(), COOPERBODY))
		{
			if (pP->GetCharacter())
			{
				pP->GetCharacter()->DeleteAllPickup();
				pP->GetCharacter()->CreatePickupDraw(2, 0, 3, true);
			}
			pP->m_TeeInfos.m_ColorBody = 1245056;
		}
		else if (Server()->GetItemSettings(pP->GetCID(), IRONBODY))
		{
			if (pP->GetCharacter())
			{
				pP->GetCharacter()->DeleteAllPickup();
				pP->GetCharacter()->CreatePickupDraw(3, 0, 3, true);
			}
			pP->m_TeeInfos.m_ColorBody = 9895835;
		}
		else if (Server()->GetItemSettings(pP->GetCID(), GOLDBODY))
		{
			if (pP->GetCharacter())
			{
				pP->GetCharacter()->DeleteAllPickup();
				pP->GetCharacter()->CreatePickupDraw(2, 0, 3, true);
				pP->GetCharacter()->CreatePickupDraw(2, 1, 3, true);
			}
			pP->m_TeeInfos.m_ColorBody = 2883328;
		}
		else if (Server()->GetItemSettings(pP->GetCID(), DIAMONDBODY))
		{
			if (pP->GetCharacter())
			{
				pP->GetCharacter()->DeleteAllPickup();
				pP->GetCharacter()->CreatePickupDraw(3, 0, 3, true);
				pP->GetCharacter()->CreatePickupDraw(3, 1, 3, true);
			}
			pP->m_TeeInfos.m_ColorBody = 9436928;
		}
		else if (Server()->GetItemSettings(pP->GetCID(), DRAGONBODY))
		{
			if (pP->GetCharacter())
			{
				pP->GetCharacter()->DeleteAllPickup();
				pP->GetCharacter()->CreatePickupDraw(6, 0, 3, true);
				pP->GetCharacter()->CreatePickupDraw(6, 1, 3, true);
			}
			pP->m_TeeInfos.m_ColorBody = 15204096;
		}
		else
		{
			if (pP->GetCharacter())
				pP->GetCharacter()->DeleteAllPickup();

			pP->m_TeeInfos.m_ColorBody = 1000;
		}

		// Ноги броня
		if (Server()->GetItemSettings(pP->GetCID(), LEATHERFEET))
			pP->m_TeeInfos.m_ColorFeet = 22315;
		else if (Server()->GetItemSettings(pP->GetCID(), COOPERFEET))
			pP->m_TeeInfos.m_ColorFeet = 1245056;
		else if (Server()->GetItemSettings(pP->GetCID(), IRONFEET))
			pP->m_TeeInfos.m_ColorFeet = 9895835;
		else if (Server()->GetItemSettings(pP->GetCID(), GOLDFEET))
			pP->m_TeeInfos.m_ColorFeet = 2883328;
		else if (Server()->GetItemSettings(pP->GetCID(), DIAMONDFEET))
			pP->m_TeeInfos.m_ColorFeet = 9436928;
		else if (Server()->GetItemSettings(pP->GetCID(), DRAGONFEET))
			pP->m_TeeInfos.m_ColorFeet = 15204096;
		else
			pP->m_TeeInfos.m_ColorFeet = 1000;
	}
	else
	{
		pP->m_TeeInfos.m_UseCustomColor = false;
	}

	if (IsTeamplay())
	{
		pP->m_TeeInfos.m_UseCustomColor = 1;
		if (pP->GetTeam() >= TEAM_RED && pP->GetTeam() <= TEAM_BLUE)
		{
			pP->m_TeeInfos.m_ColorBody = aTeamColors[pP->GetTeam()];
			pP->m_TeeInfos.m_ColorFeet = aTeamColors[pP->GetTeam()];
		}
		else
		{
			pP->m_TeeInfos.m_ColorBody = 12895054;
			pP->m_TeeInfos.m_ColorFeet = 12895054;
		}
	}
}

int IGameController::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	// do scoreing
	if (!pKiller || Weapon == WEAPON_GAME)
		return 0;
	if (Weapon == WEAPON_SELF)
		pVictim->GetPlayer()->m_RespawnTick = Server()->Tick() + Server()->TickSpeed() * 3.0f;

	return 0;
}

void IGameController::OnCharacterSpawn(class CCharacter *pChr)
{
	// default health
	pChr->IncreaseHealth(10);

	// give default weapons
	pChr->GiveWeapon(WEAPON_HAMMER, -1);
	pChr->GiveWeapon(WEAPON_GUN, -1);
}

bool IGameController::IsFriendlyFire(int ClientID1, int ClientID2)
{
	if (ClientID1 == ClientID2)
		return false;

	if (IsTeamplay())
	{
		if (!GameServer()->m_apPlayers[ClientID1] || !GameServer()->m_apPlayers[ClientID2])
			return false;

		if (GameServer()->m_apPlayers[ClientID1]->GetTeam() == GameServer()->m_apPlayers[ClientID2]->GetTeam())
			return true;
	}

	return false;
}

bool IGameController::CanBeMovedOnBalance(int ClientID)
{
	return true;
}

void IGameController::Tick()
{
	// game is Paused
	if (GameServer()->m_World.m_Paused)
		++m_RoundStartTick;

	// check for inactive players
	if (g_Config.m_SvInactiveKickTime > 0)
	{
		for (int i = 0; i < MAX_NOBOT; ++i)
		{
#ifdef CONF_DEBUG
			if (g_Config.m_DbgDummies)
			{
				if (i >= MAX_CLIENTS - g_Config.m_DbgDummies)
					break;
			}
#endif
			if (GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() != TEAM_SPECTATORS && !Server()->IsAuthed(i))
			{
				if (Server()->Tick() > GameServer()->m_apPlayers[i]->m_LastActionTick + g_Config.m_SvInactiveKickTime * Server()->TickSpeed() * 60 && !GameServer()->m_apPlayers[i]->IsBot())
				{
					switch (g_Config.m_SvInactiveKick)
					{
					case 0:
					{
						// move player to spectator
						GameServer()->m_apPlayers[i]->SetTeam(TEAM_SPECTATORS);
					}
					break;
					case 1:
					{
						// move player to spectator if the reserved slots aren't filled yet, kick him otherwise
						int Spectators = 0;
						for (int j = 0; j < MAX_NOBOT; ++j)
							if (GameServer()->m_apPlayers[j] && GameServer()->m_apPlayers[j]->GetTeam() == TEAM_SPECTATORS)
								++Spectators;
						if (Spectators >= g_Config.m_SvSpectatorSlots)
							Server()->Kick(i, "Kicked for inactivity");
						else
							GameServer()->m_apPlayers[i]->SetTeam(TEAM_SPECTATORS);
					}
					break;
					case 2:
					{
						// kick the player
						Server()->Kick(i, "Kicked for inactivity");
					}
					}
				}
			}
		}
	}
	if (g_Config.m_SvSpecKickTime > 0)
	{
		for (int i = 0; i < MAX_NOBOT; ++i)
		{
			if (GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() == TEAM_SPECTATORS && !Server()->IsAuthed(i))
			{
				int seconds;
				// dbg_msg("test","%d",Server()->Tick() - GameServer()->m_apPlayers[i]->m_LastActionTick-g_Config.m_SvSpecKickTime*Server()->TickSpeed()*60);
				if (Server()->Tick() >= GameServer()->m_apPlayers[i]->m_SpecTick + (g_Config.m_SvSpecKickTime - 1) * Server()->TickSpeed() * 60 && !GameServer()->m_apPlayers[i]->IsBot() && Server()->Tick() % Server()->TickSpeed() * 60 * 10 == 0)
				{
					seconds = GameServer()->m_apPlayers[i]->m_SpecTick / Server()->TickSpeed() + g_Config.m_SvSpecKickTime * 60 - Server()->Tick() / Server()->TickSpeed();
					GameServer()->SendChatTarget_Localization(i, CHATCATEGORY_DEFAULT, _("请在{int:seconds}秒内登录,否则将被系统自动踢出."), "seconds", &seconds, NULL);
				}
				if (Server()->Tick() > GameServer()->m_apPlayers[i]->m_SpecTick + g_Config.m_SvSpecKickTime * Server()->TickSpeed() * 60 && !GameServer()->m_apPlayers[i]->IsBot())
				{
					Server()->Kick(i, "Kicked for inactivity (Spec)");
				}
			}
		}
	}
}

bool IGameController::IsTeamplay() const
{
	return m_GameFlags & GAMEFLAG_TEAMS;
}

void IGameController::Snap(int SnappingClient)
{
	CNetObj_GameInfo *pGameInfoObj = (CNetObj_GameInfo *)Server()->SnapNewItem(NETOBJTYPE_GAMEINFO, 0, sizeof(CNetObj_GameInfo));
	if (!pGameInfoObj)
		return;

	pGameInfoObj->m_GameFlags = m_GameFlags;
	pGameInfoObj->m_GameStateFlags = 0;
	if (m_GameOverTick != -1)
		pGameInfoObj->m_GameStateFlags |= GAMESTATEFLAG_GAMEOVER;

	if (m_SuddenDeath)
		pGameInfoObj->m_GameStateFlags |= GAMESTATEFLAG_SUDDENDEATH;

	if (GameServer()->m_World.m_Paused)
		pGameInfoObj->m_GameStateFlags |= GAMESTATEFLAG_PAUSED;

	pGameInfoObj->m_RoundStartTick = m_RoundStartTick;
	pGameInfoObj->m_WarmupTimer = 0;

	pGameInfoObj->m_ScoreLimit = g_Config.m_SvScorelimit;
	pGameInfoObj->m_TimeLimit = 0;

	pGameInfoObj->m_RoundNum = (str_length(g_Config.m_SvMaprotation) && g_Config.m_SvRoundsPerMap) ? g_Config.m_SvRoundsPerMap : 0;
	pGameInfoObj->m_RoundCurrent = m_RoundCount + 1;
}

int IGameController::GetAutoTeam(int NotThisID)
{
	// this will force the auto balancer to work overtime aswell
	if (g_Config.m_DbgStress)
		return 0;

	int aNumplayers[2] = {0, 0};
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (GameServer()->m_apPlayers[i] && i != NotThisID)
		{
			if (GameServer()->m_apPlayers[i]->GetTeam() >= TEAM_RED && GameServer()->m_apPlayers[i]->GetTeam() <= TEAM_BLUE)
				aNumplayers[GameServer()->m_apPlayers[i]->GetTeam()]++;
		}
	}

	int Team = 0;
	if (IsTeamplay())
		Team = aNumplayers[TEAM_RED] > aNumplayers[TEAM_BLUE] ? TEAM_BLUE : TEAM_RED;

	if (CanJoinTeam(Team, NotThisID))
		return Team;
	return -1;
}

bool IGameController::CanJoinTeam(int Team, int NotThisID)
{
	if (Team == TEAM_SPECTATORS || (GameServer()->m_apPlayers[NotThisID] && GameServer()->m_apPlayers[NotThisID]->GetTeam() != TEAM_SPECTATORS))
		return true;

	int aNumplayers[2] = {0, 0};
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (GameServer()->m_apPlayers[i] && i != NotThisID)
		{
			if (GameServer()->m_apPlayers[i]->GetTeam() >= TEAM_RED && GameServer()->m_apPlayers[i]->GetTeam() <= TEAM_BLUE)
				aNumplayers[GameServer()->m_apPlayers[i]->GetTeam()]++;
		}
	}

	return (aNumplayers[0] + aNumplayers[1]) < Server()->MaxClients() - g_Config.m_SvSpectatorSlots;
}

bool IGameController::CanChangeTeam(CPlayer *pPlayer, int JoinTeam)
{
	int aT[2] = {0, 0};

	if (!IsTeamplay() || JoinTeam == TEAM_SPECTATORS || !g_Config.m_SvTeambalanceTime)
		return true;

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		CPlayer *pP = GameServer()->m_apPlayers[i];
		if (pP && pP->GetTeam() != TEAM_SPECTATORS)
			aT[pP->GetTeam()]++;
	}

	// simulate what would happen if changed team
	aT[JoinTeam]++;
	if (pPlayer->GetTeam() != TEAM_SPECTATORS)
		aT[JoinTeam ^ 1]--;

	// there is a player-difference of at least 2
	if (absolute(aT[0] - aT[1]) >= 2)
	{
		// player wants to join team with less players
		if ((aT[0] < aT[1] && JoinTeam == TEAM_RED) || (aT[0] > aT[1] && JoinTeam == TEAM_BLUE))
			return true;
		else
			return false;
	}
	else
		return true;
}

int IGameController::ClampTeam(int Team)
{
	if (Team < 0)
		return TEAM_SPECTATORS;
	if (IsTeamplay())
		return Team & 1;
	return 0;
}

/* INFECTION MODIFICATION START ***************************************/
/*
int IGameController::ChooseHumanClass(CPlayer* pPlayer)
{
	return PLAYERCLASS_ASSASINS;
}

bool IGameController::IsChoosableClass(int PlayerClass)
{
	return false;
}

bool IGameController::IsSpawnable(vec2 Position, int TeleZoneType)
{
	return true;
}
*/
/* INFECTION MODIFICATION END *****************************************/
