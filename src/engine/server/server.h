/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef ENGINE_SERVER_SERVER_H
#define ENGINE_SERVER_SERVER_H

#include <engine/server.h>
#include <game/server/classes.h>

/* DDNET MODIFICATION START *******************************************/
#include "sql_connector.h"
#include "sql_server.h"
/* DDNET MODIFICATION END *********************************************/

class CSnapIDPool
{
	enum
	{
		MAX_IDS = 16*1024,
	};

	class CID
	{
	public:
		short m_Next;
		short m_State; // 0 = free, 1 = alloced, 2 = timed
		int m_Timeout;
	};

	CID m_aIDs[MAX_IDS];

	int m_FirstFree;
	int m_FirstTimed;
	int m_LastTimed;
	int m_Usage;
	int m_InUsage;

public:

	CSnapIDPool();

	void Reset();
	void RemoveFirstTimeout();
	int NewID();
	void TimeoutIDs();
	void FreeID(int ID);
};


class CServerBan : public CNetBan
{
	class CServer *m_pServer;
	template<class T> int BanExt(T *pBanPool, const typename T::CDataType *pData, int Seconds, const char *pReason);

public:
	class CServer *Server() const { return m_pServer; }

	void InitServerBan(class IConsole *pConsole, class IStorage *pStorage, class CServer* pServer);
	virtual int BanAddr(const NETADDR *pAddr, int Seconds, const char *pReason);
	virtual int BanRange(const CNetRange *pRange, int Seconds, const char *pReason);
	static bool ConBanExt(class IConsole::IResult *pResult, void *pUser);
	int m_BanID;
};


class CServer : public IServer
{
	class IGameServer *m_pGameServer;
	class IConsole *m_pConsole;
	class IStorage *m_pStorage;
	CSqlServer* m_apSqlReadServers[MAX_SQLSERVERS];
	CSqlServer* m_apSqlWriteServers[MAX_SQLSERVERS];
	
public:
	class IGameServer *GameServer() { return m_pGameServer; }
	class IConsole *Console() { return m_pConsole; }
	class IStorage *Storage() { return m_pStorage; }

	enum
	{
		MAX_RCONCMD_SEND=16,
	};

	class CClient
	{
	public:

		enum
		{
			STATE_EMPTY = 0,
			STATE_AUTH,
			STATE_CONNECTING,
			STATE_READY,
			STATE_INGAME,

			SNAPRATE_INIT=0,
			SNAPRATE_FULL,
			SNAPRATE_RECOVER
		};

		class CInput
		{
		public:
			int m_aData[MAX_INPUT_SIZE];
			int m_GameTick; // the tick that was chosen for the input
		};

		// connection state info
		int m_State;
		int m_Latency;
		int m_SnapRate;

		int m_LastAckedSnapshot;
		int m_LastInputTick;
		CSnapshotStorage m_Snapshots;

		CInput m_LatestInput;
		CInput m_aInputs[200]; // TODO: handle input better
		int m_CurrentInput;

		char m_aName[MAX_NAME_LENGTH];
		char m_aClan[MAX_CLAN_LENGTH];
		int m_Country;
		int m_Authed;
		int m_AuthTries;
		int m_NextMapChunk;

		const IConsole::CCommandInfo *m_pRconCmdToSend;
		
		void Reset(bool ResetScore=true);
		
		int m_AntiPing;
		int m_DefaultScoreMode;
		char m_aLanguage[16];
		int m_WaitingTime;

		int m_ItemReward[21];
		int m_ItemNumReward[21];
		int m_MailID[21];

		char m_SelectPlayer[64][25];

		//Login
		int m_LogInstance;
		int m_UserID;
		int m_ClanID;
		int m_Level;
		int m_Exp;
		long int m_Money;
		long int m_Gold;
		int m_Donate;
		int m_Rel;
		bool m_Jail;
		int m_Class;
		int m_Quest;
		int m_Kill;
		int m_WinArea;
		bool m_Seccurity;
		char m_Clan[64];
		long int m_ClanAdded;

		int m_ItemCount[7];

		// All
		int Upgrade;
		int SkillPoint;
		
		int m_HammerRange;
		int m_Pasive2;
		
		//upgrade
		int Health;
		int Damage;
		int Speed;
		int HPRegen;
		int AmmoRegen;
		int Ammo;
		int Spray;
		int Mana;

		char m_aUsername[MAX_NAME_LENGTH];
		int m_SelectItem;
		bool m_CustClt;
	};

	struct _m_stClan
	{
		int f_id;
		char f_name [32];
		char f_creator [MAX_NAME_LENGTH];
		int f_level;
		int f_exp;
		int f_money;
		int f_maxnum;
		int f_countnum;
		int f_kd;
		int f_upgrade[2]; // 1 EXP : 2 Money
		bool f_openhouse;
		bool f_spawninhouse;
		int f_chairupgr;
	};
	_m_stClan m_stClan[500];

	virtual int GetClan(int Type, int ClanID);
	virtual bool GetLeader(int ClientID, int ClanID);
	const char *LeaderName(int ClientID);
	const char *GetClanName(int ClanID);

	virtual int GetUserID(int ClientID);

	virtual int GetOwnHouse(int ClientID);
	virtual bool GetSpawnInClanHouse(int ClientID, int HouseID);
	virtual int GetTopHouse(int HouseID);
	virtual bool GetHouse(int ClientID);
	virtual bool GetOpenHouse(int HouseID);
	virtual bool SetOpenHouse(int HouseID);
	int m_HouseClanID[3];
	int m_HouseOldClanID[3];

	int m_Materials[3];
	virtual void InitMaterialID();
	virtual void SaveMaterials(int ID);
	virtual int GetMaterials(int ID);
	virtual void SetMaterials(int ID, int Count);

	struct _m_stInv
	{
		int i_id;
		char i_name [32];
		char i_desc [50];
		int i_count;
		int i_settings;
		int i_type;
		int i_nlevel;
		int i_nprice;
		int i_enchant;
	};
	_m_stInv m_stInv[MAX_CLIENTS][200];
	
	virtual const char *GetItemName(int ClientID, int ItemID, bool tlang = true);
	virtual const char *GetItemDesc(int ClientID, int ItemID);
	virtual int GetItemCount(int ClientID, int ItemID);
	virtual int GetItemSettings(int ClientID, int ItemID);
	virtual int GetItemType(int ClientID, int ItemID);
	virtual int GetBonusEnchant(int ClientID, int ItemID, int Armor);
	virtual void SetItemSettings(int ClientID, int ItemID, int ItemType = 0);
	virtual int GetItemEnquip(int ClientID, int ItemType);
	virtual void SetItemSettingsCount(int ClientID, int ItemID, int Count);
	virtual void UpdateItemSettings(int ClientID, int ItemID);
	virtual void GiveItem(int ClientID, int ItemID, int Count, int Settings = 0, int Enchant = 0);
	virtual void RemItem(int ClientID, int ItemID, int Count, int Type);
	virtual void SetItemPrice(int ClientID, int ItemID, int Level, int Price);
	virtual int GetItemPrice(int ClientID, int ItemID, int Type);
	virtual int GetItemEnchant(int ClientID, int ItemID);
	virtual void SetItemEnchant(int ClientID, int ItemID, int Price);
	virtual void Logout(int ClientID);

	virtual void SetRewardMail(int ClientID, int ID, int ItemID, int ItemNum);
	virtual int GetRewardMail(int ClientID, int ID, int Type);
	virtual int GetMailRewardDell(int ClientID, int ID);

	virtual void ResetBotInfo(int ClientID, int BotType, int BotSubType);
	virtual void InitClientBot(int ClientID);

	CClient m_aClients[MAX_CLIENTS];
	int IdMap[MAX_CLIENTS * VANILLA_MAX_CLIENTS];

	CSnapshotDelta m_SnapshotDelta;
	CSnapshotBuilder m_SnapshotBuilder;
	CSnapIDPool m_IDPool;
	CNetServer m_NetServer;
	CEcon m_Econ;
	CServerBan m_ServerBan;

	IEngineMap *m_pMap;

	int64 m_GameStartTime;
	int m_RunServer;
	int m_MapReload;
	int m_RconClientID;
	int m_RconAuthLevel;
	int m_PrintCBIndex;

	int64 m_Lastheartbeat;
	char m_aCurrentMap[64];
	
	unsigned m_CurrentMapCrc;
	unsigned char *m_pCurrentMapData;
	unsigned int m_CurrentMapSize;

	CRegister m_Register;
	CMapChecker m_MapChecker;

	CServer();
	virtual ~CServer();

	int TrySetClientName(int ClientID, const char *pName);

	virtual void SetClientName(int ClientID, const char *pName);
	virtual void SetClientClan(int ClientID, char const *pClan);
	virtual void SetClientCountry(int ClientID, int Country);

	void Kick(int ClientID, const char *pReason);

	int64 TickStartTime(int Tick);
	int Init();

	void SetRconCID(int ClientID);
	bool IsAuthed(int ClientID);
	int GetClientInfo(int ClientID, CClientInfo *pInfo);
	void GetClientAddr(int ClientID, char *pAddrStr, int Size);
	const char *ClientName(int ClientID);
	const char *ClientUsername(int ClientID);
	const char *ClientClan(int ClientID);
	const char *GetSelectName(int ClientID, int SelID);
	int ClientCountry(int ClientID);
	bool ClientIngame(int ClientID);
	int MaxClients() const;

	virtual int SendMsg(CMsgPacker *pMsg, int Flags, int ClientID);
	int SendMsgEx(CMsgPacker *pMsg, int Flags, int ClientID, bool System);

	void DoSnapshot();

	static int NewClientCallback(int ClientID, void *pUser);
	static int DelClientCallback(int ClientID, const char *pReason, void *pUser);

	static int ClientRejoinCallback(int ClientID, void *pUser);

	void SendMap(int ClientID);
	void SendMapData(int ClientID, int Chunk);
	
	void SendConnectionReady(int ClientID);
	void SendRconLine(int ClientID, const char *pLine);
	static void SendRconLineAuthed(const char *pLine, void *pUser);

	void SendRconCmdAdd(const IConsole::CCommandInfo *pCommandInfo, int ClientID);
	void SendRconCmdRem(const IConsole::CCommandInfo *pCommandInfo, int ClientID);
	void UpdateClientRconCommands();

	void ProcessClientPacket(CNetChunk *pPacket);

	void SendServerInfo(const NETADDR *pAddr, int Token, bool Extended=false, int Offset=0);
	void UpdateServerInfo();

	void PumpNetwork();

	char *GetMapName();
	int LoadMap(const char *pMapName);

	void InitRegister(CNetServer *pNetServer, IEngineMasterServer *pMasterServer, IConsole *pConsole);
	int Run();

	static bool ConKick(IConsole::IResult *pResult, void *pUser);
	static bool ConStatus(IConsole::IResult *pResult, void *pUser);
	static bool ConOptionStatus(IConsole::IResult *pResult, void *pUser);
	static bool ConShutdown(IConsole::IResult *pResult, void *pUser);
	static bool ConMapReload(IConsole::IResult *pResult, void *pUser);
	static bool ConLogout(IConsole::IResult *pResult, void *pUser);
	static bool ConchainSpecialInfoupdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData);
	static bool ConchainMaxclientsperipUpdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData);
	static bool ConchainModCommandUpdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData);
	static bool ConchainConsoleOutputLevelUpdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData);
	
	static bool ConAddSqlServer(IConsole::IResult *pResult, void *pUserData);
	static bool ConDumpSqlServers(IConsole::IResult *pResult, void *pUserData);
	void RegisterCommands();

	virtual int SnapNewID();
	virtual void SnapFreeID(int ID);
	virtual void *SnapNewItem(int Type, int ID, int Size);
	void SnapSetStaticsize(int ItemType, int Size);

	
public:
	int m_InfAmmoRegenTime[MAX_CLIENTS][NB_INFWEAPON];
	int m_InfFireDelay[MAX_CLIENTS][NB_INFWEAPON];
	int m_InfMaxAmmo[MAX_CLIENTS][NB_INFWEAPON];

public:
	virtual int GetClientAntiPing(int ClientID);
	virtual void SetClientAntiPing(int ClientID, int Value);
	
	virtual const char* GetClientLanguage(int ClientID);
	virtual void SetClientLanguage(int ClientID, const char* pLanguage);
	
	virtual int GetFireDelay(int ClientID, int WID);
	virtual void SetFireDelay(int ClientID, int WID, int Time);
	
	virtual int GetAmmoRegenTime(int ClientID, int WID);
	virtual void SetAmmoRegenTime(int ClientID, int WID, int Time);
	
	virtual int GetMaxAmmo(int ClientID, int WID);
	virtual void SetMaxAmmo(int ClientID, int WID, int n);
	
	virtual int GetSeccurity(int WID);
	virtual void SetSeccurity(int WID, int n);
	virtual bool IsClientLogged(int ClientID);
	virtual int GetClanID(int ClientID);

	virtual void ShowTop10(int ClientID, const char* Type, int TypeGet);
	virtual void ShowTop10Clans(int ClientID, const char* Type, int TypeGet);
	virtual void GetTopClanHouse();
	// ----- Аккаунт
	
	// Функции вход выход
	virtual void Login(int ClientID, const char* pUsername, const char* pPassword);
	virtual void FirstInit(int ClientID);

	virtual void Register(int ClientID, const char* pUsername, const char* pPassword, const char* pEmail);

	// Инициализация сохранение загрузка
	virtual void UpdateStats(int ClientID, int Type = 0);
	virtual void InitClientDB(int ClientID);
	
	// ----- Инвентарь
	virtual void InitInvID(int ClientID = -1, int ItemID = -1);
	virtual void GetItem(int ItemID, int ClientID, int Count, int Settings = 0, int Enchant = 0);
	virtual void RemItems(int ItemID, int ClientID, int Count, int Type);
	virtual void ListInventory(int ClientID, int Type, int GetCount = false);
	virtual int GetItemCountType(int ClientID, int Type);
	virtual void InitMailID(int ClientID);
	virtual void SendMail(int AuthedID, const char* pText, int ItemID, int ItemNum);
	virtual void RemMail(int IDMail);

	// ----- Кланы
	
	// Функции вход выход
	virtual void ListClan(int ClientID, int ClanID);
	virtual void ExitClanOff(int ClientID, const char* pName);
	virtual void NewClan(int ClientID, const char* pName);
	virtual void EnterClan(int ClientID, int ClanID);
	virtual void ChangeLeader(int ClanID, const char* pName);
	
	// Инициализация сохранение загрузка
	virtual void InitClan();
	virtual void InitClanID(int ClanID, bool Need, const char* SubType, int Price, bool Save);
	virtual void UpdClanCount(int ClanID);

	virtual void Ban(int ClientID, int Seconds, const char* pReason);
	
public:
	class CGameServerCmd
	{
	public:
		virtual ~CGameServerCmd() {};
		virtual void Execute(IGameServer* pGameServer) = 0;
	};
	array<CGameServerCmd*> m_lGameServerCmds;
	LOCK m_GameServerCmdLock;


public: 
	void AddGameServerCmd(CGameServerCmd* pCmd);

	void GetClientAddr(int ClientID, NETADDR *pAddr);
	int m_aPrevStates[MAX_CLIENTS];

	virtual int* GetIdMap(int ClientID);
	virtual void SetCustClt(int ClientID);
	
	virtual int GetStat(int ClientID, int Type);
	virtual int GetUpgrade(int ClientID, int Type);
	virtual void UpdateStat(int ClientID, int Type, int Size);
	virtual void UpdateUpgrade(int ClientID, int Type, int Size);

};

#endif
