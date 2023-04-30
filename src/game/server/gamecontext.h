/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMECONTEXT_H
#define GAME_SERVER_GAMECONTEXT_H

#include <engine/server.h>
#include <engine/storage.h>
#include <engine/console.h>
#include <engine/shared/memheap.h>
#include "../src/base/tl/array.h"

#include <game/layers.h>
#include <game/voting.h>
#include <game/server/classes.h>

#include <teeuniverses/components/localization.h>

#include "eventhandler.h"
#include "gamecontroller.h"
#include "gameworld.h"
#include "player.h"

#ifdef _MSC_VER
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#define BROADCAST_DURATION_REALTIME (0)
#define BROADCAST_DURATION_GAMEANNOUNCE (Server()->TickSpeed() * 2)

const int BOSSID = MAX_CLIENTS - 1;
const int MAX_COUNT = 20000000;

enum
{
	BROADCAST_PRIORITY_LOWEST = 0,
	BROADCAST_PRIORITY_WEAPONSTATE,
	BROADCAST_PRIORITY_EFFECTSTATE,
	BROADCAST_PRIORITY_GAMEANNOUNCE,
	BROADCAST_PRIORITY_SERVERANNOUNCE,
	BROADCAST_PRIORITY_INTERFACE,
};

enum
{
	CLANUPGRADECOUNT = 2000000,
};

enum
{
	NOAUTHED = 0,
};

enum
{
	INSHOP = 1,
	EXITSHOP,
	INADDEXP,
	INADDMONEY,
	NOTWEAPON,
	INADDCEXP,
	INANTIPVP,
	INCRAFT,
	INQUEST,
	EXITANTIPVP
};

enum
{
	AMAXHEALTH = 400,
	BMAXHEALTH = 300,
	HMAXHEALTH = 500,

	AMAXDAMAGE = 200,
	BMAXDAMAGE = 260,
	HMAXDAMAGE = 140,

	AMAXAREGEN = 270,
	BMAXAREGEN = 300,
	HMAXAREGEN = 270,

	AMAXAMMO = 80,
	BMAXAMMO = 70,
	HMAXAMMO = 100,

	AMAXHPREGEN = 200,
	BMAXHPREGEN = 150,
	HMAXHPREGEN = 250,

	AMAXHANDLE = 400,
	BMAXHANDLE = 350,
	HMAXHANDLE = 300,

	AMAXSPREAD = 30,
	BMAXSPREAD = 30,
	HMAXSPREAD = 30,
};

enum
{
	QUEST1 = 20,
	QUEST2 = 40,
	QUEST3 = 60,
	QUEST4 = 80,
	QUEST5 = 100,
	QUEST6 = 120,
};
enum
{
	HAMMERRANGE = 15
};

class CGameContext : public IGameServer
{
	IServer *m_pServer;
	IStorage *m_pStorage;
	class IConsole *m_pConsole;
	CLayers m_Layers;
	CCollision m_Collision;
	CNetObjHandler m_NetObjHandler;
	CTuningParams m_Tuning;
	// int m_TargetToKill;
	// int m_TargetToKillCoolDown;

	static bool ConTuneParam(IConsole::IResult *pResult, void *pUserData);
	static bool ConTuneReset(IConsole::IResult *pResult, void *pUserData);
	static bool ConTuneDump(IConsole::IResult *pResult, void *pUserData);
	static bool ConPause(IConsole::IResult *pResult, void *pUserData);
	static bool ConRestart(IConsole::IResult *pResult, void *pUserData);
	static bool ConBroadcast(IConsole::IResult *pResult, void *pUserData);
	static bool ConSay(IConsole::IResult *pResult, void *pUserData);
	static bool ConchainSpecialMotdupdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData);

	CGameContext(int Resetting);
	void Construct(int Resetting);

	bool m_Resetting;

public:
	IServer *Server() const { return m_pServer; }
	IStorage *Storage() const { return m_pStorage; }
	class IConsole *Console() { return m_pConsole; }
	CCollision *Collision() { return &m_Collision; }
	CTuningParams *Tuning() { return &m_Tuning; }
	virtual class CLayers *Layers() { return &m_Layers; }

	CGameContext();
	~CGameContext();

	void Clear();

	CEventHandler m_Events;
	CPlayer *m_apPlayers[MAX_CLIENTS];

	IGameController *m_pController;
	CGameWorld m_World;

	class CCharacter *GetPlayerChar(int ClientID);

	enum
	{
		VOTE_ENFORCE_UNKNOWN = 0,
		VOTE_ENFORCE_NO,
		VOTE_ENFORCE_YES,
	};

	enum
	{
		MENUONLY,
		CRAFTONLY,
		BUYITEMONLY,
		SETTINGSONLY,
		SELLITEMWORK,
	};

	// Визуальные функции
	// 视觉功能
	void CreateDamageInd(vec2 Pos, float AngleMod, int Amount);
	void CreateExplosion(vec2 Pos, int Owner, int Weapon, bool NoDamage, int TakeDamageMode = TAKEDAMAGEMODE_NOINFECTION);
	void CreateExplosionDisk(vec2 Pos, float InnerRadius, float DamageRadius, int Damage, float Force, int Owner, int Weapon, int TakeDamageMode = TAKEDAMAGEMODE_NOINFECTION);
	void CreateHammerHit(vec2 Pos);
	void CreatePlayerSpawn(vec2 Pos);
	void CreateDeath(vec2 Pos, int Who);
	void CreateSound(vec2 Pos, int Sound, int64_t Mask = -1);
	void CreateSoundGlobal(int Sound, int Target = -1);

	// Голосования
	// 投票
	void ClearVotes(int ClientID);
	void AddVote(const char *Desc, const char *Cmd, int ClientID = -1);
	void ResetVotes(int ClientID, int Type);
	void AddBack(int ClientID);
	struct CVoteOptions
	{
		char m_aDescription[VOTE_DESC_LENGTH] = {0};
		char m_aCommand[VOTE_CMD_LENGTH] = {0};
		void *data = {0};
	};
	array<CVoteOptions> m_PlayerVotes[MAX_CLIENTS];

	// Клан функции
	// 公会
	void ExitClan(int ClientID);
	void EnterClan(int ClientID, int ClanID);

	// Предметы апгрейды
	// 物品升级
	void BuyItem(int ItemType, int ClientID, int Type = 0);
	void GiveItem(int ClientID, int ItemID, int Count, int Enchant = 0);
	void RemItem(int ClientID, int ItemID, int Count);

	void CreateItem(int ClientID, int ItemID, int Count);
	void BuyUpgradeClan(int ClientID, int Money, Clan Type, const char *SubType);
	void CreateNewShop(int ClientID, int ItemID, int Type, int Level, int Price);
	void SkillSettings(int ClientID, int ItemType, const char *Msg);
	void EyeEmoteSettings(int ClientID, int ItemType, const char *Msg);
	void BuySkill(int ClientID, int Price, int ItemID);
	void UseItem(int ClientID, int ItemID, int Count, int Type);

	void CreateSellWorkItem(int ClientID, int ItemID, int Price);

	// Арена
	// 竞技场
	void AreaTick();
	void StartArea(int WaitTime, int Type, int ClientID);
	void EnterArea(int ClientID);
	int m_AreaStartTick;
	int m_AreaEndGame;
	int m_AreaType;

	// Босс
	// Boss战
	void BossTick();
	void StartBoss(int ClientID, int WaitTime, int BossType);
	void EnterBoss(int ClientID, int BossType);
	void DeleteBotBoss();
	int GetBossCount();
	int GetBossLeveling();
	int GetAreaCount();
	const char *GetBossName(int BossType);
	int m_WinWaitBoss;
	int m_BossStartTick;
	bool m_BossStart;
	int m_BossType;

	// Помощь, прочие
	// 帮助别人(?)
	void UpdateBotInfo(int ClientID);
	void CreateBot(int ClientID, int BotType, int BotSubType = 0);
	void CreateLolText(CEntity *pParent, bool Follow, vec2 Pos, vec2 Vel, int Lifespan, const char *pText);
	const char *LevelString(int max, int value, int step, char ch1, char ch2);
	void SendMail(int ClientID, int MailType, int ItemID, int ItemNum);

	// Обновление аккаунта
	// 升级
	void GetStat(int ClientID);
	void GetUpgrade(int ClientID);
	void UpdateStat(int ClientID);
	virtual void UpdateStats(int ClientID);
	virtual void UpdateUpgrades(int ClientID);
	void UpdateUpgrade(int ClientID);

	enum
	{
		CHAT_ALL = -2,
		CHAT_SPEC = -1,
		CHAT_RED = 0,
		CHAT_BLUE = 1
	};

	// Локализация текст
	// 本地化文本
	virtual void SendGuide(int ClientID, int BossType);

	virtual void SendMOTD(int To, const char *pParam);
	virtual void SendMOTD_Localization(int To, const char *pText, ...);

	void AddBroadcast(int ClientID, const char *pText, int Priority, int LifeSpan);
	virtual void SendBroadcast(int To, const char *pText, int Priority, int LifeSpan);
	virtual void SendBroadcast_LStat(int To, int Priority, int LifeSpan, int Type, int Size = 0, int Size2 = 0);
	virtual void SendBroadcast_LChair(int To, int Size = 0, int SizeMoney = 0);
	virtual void SendBroadcast_LBossed(int To, int Priority, int LifeSpan);
	virtual void SendBroadcast_Localization(int To, int Priority, int LifeSpan, const char *pText, ...);
	virtual void SendBroadcast_Localization_P(int To, int Priority, int LifeSpan, int Number, const char *pText, ...);
	virtual void ClearBroadcast(int To, int Priority);

	virtual void AddVote_Localization(int To, const char *aCmd, const char *pText, ...);
	virtual void AddVoteMenu_Localization(int To, int MenuID, int Type, const char *pText, ...);
	virtual void AddNewCraftVote(int ClientID, const char *Need, int ItemID);

	const char *LocalizeText(int ClientID, const char *pText);

	virtual void SendChatTarget(int To, const char *pText);
	virtual void SendChatTarget_Localization(int To, int Category, const char *pText, ...);
	virtual void SendChatTarget_Localization_P(int To, int Category, int Number, const char *pText, ...);
	virtual void SendChatClan(int ClanID, const char *pText, ...);

	// Основные функции
	// 主要功能
	void SendChat(int ClientID, int Team, const char *pText);
	void SendEmoticon(int ClientID, int Emoticon);
	void SendWeaponPickup(int ClientID, int Weapon);
	virtual void OnSetAuthed(int ClientID, int Level);

	void CheckPureTuning();
	void SendTuningParams(int ClientID);

	virtual void OnInit();
	virtual void OnConsoleInit();
	virtual void OnShutdown();

	virtual void OnTick();
	virtual void OnPreSnap();
	virtual void OnSnap(int ClientID);
	virtual void OnPostSnap();

	virtual void OnMessage(int MsgID, CUnpacker *pUnpacker, int ClientID);

	virtual void OnClientConnected(int ClientID);
	virtual void OnClientEnter(int ClientID);
	virtual void OnClientDrop(int ClientID, int Type, const char *pReason);
	virtual void OnClientDirectInput(int ClientID, void *pInput);
	virtual void OnClientPredictedInput(int ClientID, void *pInput);

	virtual bool IsClientReady(int ClientID);
	virtual bool IsClientPlayer(int ClientID);

	virtual const char *GameType();
	virtual const char *Version();
	virtual const char *NetVersion();

public:
	int m_ChatResponseTargetID;
	int m_ChatPrintCBIndex;
	int m_ZoneHandle_Damage;
	int m_ZoneHandle_Teleport;
	int m_ZoneHandle_Bonus;
	int m_InviteClanID[MAX_NOBOT];
	int m_InviteTick[MAX_NOBOT];

private:
	bool PrivateMessage(const char *pStr, int ClientID, bool TeamChat);
	class CBroadcastState
	{
	public:
		int m_NoChangeTick;
		char m_PrevMessage[1024];

		int m_Priority;
		char m_NextMessage[1024];

		int m_LifeSpanTick;
		int m_TimedPriority;
		char m_TimedMessage[1024];
	};
	CBroadcastState m_BroadcastStates[MAX_NOBOT];
};

inline int64_t CmaskAll() { return -1LL; }
inline int64_t CmaskOne(int ClientID) { return 1LL << ClientID; }
inline int64_t CmaskAllExceptOne(int ClientID) { return CmaskAll() ^ CmaskOne(ClientID); }
inline bool CmaskIsSet(int64_t Mask, int ClientID) { return (Mask & CmaskOne(ClientID)) != 0; }
#endif
