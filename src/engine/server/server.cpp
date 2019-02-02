/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#include <base/math.h>
#include <base/system.h>
#include <base/tl/array.h>

#include <engine/config.h>
#include <engine/console.h>
#include <engine/engine.h>
#include <engine/map.h>
#include <engine/masterserver.h>
#include <engine/server.h>
#include <engine/storage.h>

#include <engine/shared/compression.h>
#include <engine/shared/config.h>
#include <engine/shared/datafile.h>
#include <engine/shared/econ.h>
#include <engine/shared/filecollection.h>
#include <engine/shared/mapchecker.h>
#include <engine/shared/netban.h>
#include <engine/shared/network.h>
#include <engine/shared/packer.h>
#include <engine/shared/protocol.h>
#include <engine/shared/snapshot.h>
#include <game/mapitems.h>
#include <game/gamecore.h>

#include <mastersrv/mastersrv.h>

#include "register.h"
#include "server.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <engine/server/mapconverter.h>
#include <engine/server/sql_job.h>
#include <engine/server/crypt.h>

#include <teeuniverses/components/localization.h>

#if defined(CONF_FAMILY_WINDOWS)
	#define _WIN32_WINNT 0x0501
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

static const char *StrLtrim(const char *pStr)
{
	while(*pStr && *pStr >= 0 && *pStr <= 32)
		pStr++;
	return pStr;
}

static void StrRtrim(char *pStr)
{
	int i = str_length(pStr);
	while(i >= 0)
	{
		if(pStr[i] < 0 || pStr[i] > 32)
			break;
		pStr[i] = 0;
		i--;
	}
}

CSnapIDPool::CSnapIDPool()
{
	Reset();
}

void CSnapIDPool::Reset()
{
	for(int i = 0; i < MAX_IDS; i++)
	{
		m_aIDs[i].m_Next = i+1;
		m_aIDs[i].m_State = 0;
	}

	m_aIDs[MAX_IDS-1].m_Next = -1;
	m_FirstFree = 0;
	m_FirstTimed = -1;
	m_LastTimed = -1;
	m_Usage = 0;
	m_InUsage = 0;
}


void CSnapIDPool::RemoveFirstTimeout()
{
	int NextTimed = m_aIDs[m_FirstTimed].m_Next;

	// add it to the free list
	m_aIDs[m_FirstTimed].m_Next = m_FirstFree;
	m_aIDs[m_FirstTimed].m_State = 0;
	m_FirstFree = m_FirstTimed;

	// remove it from the timed list
	m_FirstTimed = NextTimed;
	if(m_FirstTimed == -1)
		m_LastTimed = -1;

	m_Usage--;
}

int CSnapIDPool::NewID()
{
	int64 Now = time_get();

	// process timed ids
	while(m_FirstTimed != -1 && m_aIDs[m_FirstTimed].m_Timeout < Now)
		RemoveFirstTimeout();

	int ID = m_FirstFree;
	dbg_assert(ID != -1, "id error");
	if(ID == -1)
		return ID;
	m_FirstFree = m_aIDs[m_FirstFree].m_Next;
	m_aIDs[ID].m_State = 1;
	m_Usage++;
	m_InUsage++;
	return ID;
}

void CSnapIDPool::TimeoutIDs()
{
	// process timed ids
	while(m_FirstTimed != -1)
		RemoveFirstTimeout();
}

void CSnapIDPool::FreeID(int ID)
{
	if(ID < 0)
		return;
	dbg_assert(m_aIDs[ID].m_State == 1, "id is not alloced");

	m_InUsage--;
	m_aIDs[ID].m_State = 2;
	m_aIDs[ID].m_Timeout = time_get()+time_freq()*5;
	m_aIDs[ID].m_Next = -1;

	if(m_LastTimed != -1)
	{
		m_aIDs[m_LastTimed].m_Next = ID;
		m_LastTimed = ID;
	}
	else
	{
		m_FirstTimed = ID;
		m_LastTimed = ID;
	}
}

void CServerBan::InitServerBan(IConsole *pConsole, IStorage *pStorage, CServer* pServer)
{
	CNetBan::Init(pConsole, pStorage);

	m_pServer = pServer;

	// overwrites base command, todo: improve this
	Console()->Register("ban", "s<clientid> ?i<minutes> ?r<reason>", CFGFLAG_SERVER|CFGFLAG_STORE, ConBanExt, this, "Ban player with ip/client id for x minutes for any reason");
}

template<class T>
int CServerBan::BanExt(T *pBanPool, const typename T::CDataType *pData, int Seconds, const char *pReason)
{
	// validate address
	if(Server()->m_RconClientID >= 0 && Server()->m_RconClientID < MAX_CLIENTS &&
		Server()->m_aClients[Server()->m_RconClientID].m_State != CServer::CClient::STATE_EMPTY)
	{
		if(NetMatch(pData, Server()->m_NetServer.ClientAddr(Server()->m_RconClientID)))
		{
			Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "net_ban", "ban error (you can't ban yourself)");
			return -1;
		}

		for(int i = 0; i < MAX_CLIENTS; ++i)
		{
			if(i == Server()->m_RconClientID || Server()->m_aClients[i].m_State == CServer::CClient::STATE_EMPTY)
				continue;

			if(Server()->m_aClients[i].m_Authed >= Server()->m_RconAuthLevel && NetMatch(pData, Server()->m_NetServer.ClientAddr(i)))
			{
				Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "net_ban", "ban error (command denied)");
				return -1;
			}
		}
	}
	else if(Server()->m_RconClientID == IServer::RCON_CID_VOTE)
	{
		for(int i = 0; i < MAX_CLIENTS; ++i)
		{
			if(Server()->m_aClients[i].m_State == CServer::CClient::STATE_EMPTY)
				continue;

			if(Server()->m_aClients[i].m_Authed != CServer::AUTHED_NO && NetMatch(pData, Server()->m_NetServer.ClientAddr(i)))
			{
				Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "net_ban", "ban error (command denied)");
				return -1;
			}
		}
	}

	int Result = Ban(pBanPool, pData, Seconds, pReason);
	if(Result != 0)
		return Result;

	// drop banned clients

	// don't drop it like that. just kick the desired guy
	typename T::CDataType Data = *pData;
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if(Server()->m_aClients[i].m_State == CServer::CClient::STATE_EMPTY)
			continue;

		if(m_BanID != i) // don't drop it like that. just kick the desired guy
			continue;

		if(NetMatch(&Data, Server()->m_NetServer.ClientAddr(i)))
		{
			CNetHash NetHash(&Data);
			char aBuf[256];
			MakeBanInfo(pBanPool->Find(&Data, &NetHash), aBuf, sizeof(aBuf), MSGTYPE_PLAYER);
			Server()->m_NetServer.Drop(i, aBuf);
		}
	}

	return Result;
}

int CServerBan::BanAddr(const NETADDR *pAddr, int Seconds, const char *pReason)
{
	return BanExt(&m_BanAddrPool, pAddr, Seconds, pReason);
}

int CServerBan::BanRange(const CNetRange *pRange, int Seconds, const char *pReason)
{
	if(pRange->IsValid())
		return BanExt(&m_BanRangePool, pRange, Seconds, pReason);

	Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "net_ban", "ban failed (invalid range)");
	return -1;
}

bool CServerBan::ConBanExt(IConsole::IResult *pResult, void *pUser)
{
	CServerBan *pThis = static_cast<CServerBan*>(pUser);

	const char *pStr = pResult->GetString(0);
	int Minutes = pResult->NumArguments()>1 ? clamp(pResult->GetInteger(1), 0, 44640) : 30;
	const char *pReason = pResult->NumArguments()>2 ? pResult->GetString(2) : "No reason given";
	pThis->m_BanID = -1;

	if(StrAllnum(pStr))
	{
		int ClientID = str_toint(pStr);
		if(ClientID < 0 || ClientID >= MAX_CLIENTS || pThis->Server()->m_aClients[ClientID].m_State == CServer::CClient::STATE_EMPTY)
			pThis->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "net_ban", "ban error (invalid client id)");
		else
		{
			pThis->m_BanID = ClientID; //to ban the right guy, not his brother or so :P
			if(pThis->BanAddr(pThis->Server()->m_NetServer.ClientAddr(ClientID), Minutes*60, pReason) != 0) //error occured
				pThis->Server()->Kick(ClientID, pReason);
		}
	}
	else
		ConBan(pResult, pUser);
	
	return true;
}

void CServer::CClient::Reset(bool ResetScore)
{
	// reset input
	for(int i = 0; i < 200; i++)
		m_aInputs[i].m_GameTick = -1;
	
	m_CurrentInput = 0;
	mem_zero(&m_LatestInput, sizeof(m_LatestInput));

	m_Snapshots.PurgeAll();
	m_LastAckedSnapshot = -1;
	m_LastInputTick = -1;
	m_SnapRate = CClient::SNAPRATE_INIT;
	m_NextMapChunk = 0;
	
	if(ResetScore)
	{
		m_WaitingTime = 0;
		m_LogInstance = -1;
		m_UserID = -1;

		m_AntiPing = 0;
		str_copy(m_aLanguage, "en", sizeof(m_aLanguage));

		m_WaitingTime = 0;
	}
}

CServer::CServer()
{
	m_TickSpeed = SERVER_TICK_SPEED;

	m_pGameServer = 0;

	m_CurrentGameTick = 0;
	m_RunServer = 1;

	m_pCurrentMapData = 0;
	m_CurrentMapSize = 0;

	m_MapReload = 0;

	m_RconClientID = IServer::RCON_CID_SERV;
	m_RconAuthLevel = AUTHED_ADMIN;
 
	for (int i = 0; i < MAX_SQLSERVERS; i++)
	{
		m_apSqlReadServers[i] = 0;
		m_apSqlWriteServers[i] = 0;
	}

	CSqlConnector::SetReadServers(m_apSqlReadServers);
	CSqlConnector::SetWriteServers(m_apSqlWriteServers);
	
	m_GameServerCmdLock = lock_create();
	
	Init();
}

CServer::~CServer()
{
	lock_destroy(m_GameServerCmdLock);
}

int CServer::TrySetClientName(int ClientID, const char *pName)
{
	char aTrimmedName[64];
	char aTrimmedName2[64];

	// trim the name
	str_copy(aTrimmedName, StrLtrim(pName), sizeof(aTrimmedName));
	StrRtrim(aTrimmedName);

	// check for empty names
	if(!aTrimmedName[0])
		return -1;
		
	// name not allowed to start with '/'
	if(aTrimmedName[0] == '/')
		return -1;

	pName = aTrimmedName;

	// make sure that two clients doesn't have the same name
	for(int i = 0; i < MAX_NOBOT; i++)
	{
		if(i != ClientID && m_aClients[i].m_State >= CClient::STATE_READY)
		{
			str_copy(aTrimmedName2, ClientName(i), sizeof(aTrimmedName2));
			StrRtrim(aTrimmedName2);
			
			if(str_comp(pName, aTrimmedName2) == 0)
				return -1;
		}
	}

	// check if new and old name are the same
	if(m_aClients[ClientID].m_aName[0] && str_comp(m_aClients[ClientID].m_aName, pName) == 0)
		return 0;
	
	// set the client name
	str_copy(m_aClients[ClientID].m_aName, pName, MAX_NAME_LENGTH);
	return 0;
}

void CServer::SetClientName(int ClientID, const char *pName)
{
	if(ClientID < 0 || ClientID >= MAX_CLIENTS || m_aClients[ClientID].m_State < CClient::STATE_READY)
		return;

	if(!pName)
		return;

	char aCleanName[MAX_NAME_LENGTH];
	str_copy(aCleanName, pName, sizeof(aCleanName));

	if(TrySetClientName(ClientID, aCleanName))
	{
		// auto rename
		for(int i = 1;; i++)
		{
			char aNameTry[MAX_NAME_LENGTH];
			str_format(aNameTry, sizeof(aCleanName), "(%d)%s", i, aCleanName);
			if(TrySetClientName(ClientID, aNameTry) == 0)
				break;
		}
	}
}

void CServer::SetClientClan(int ClientID, const char *pClan)
{
	if(ClientID < 0 || ClientID >= MAX_CLIENTS || m_aClients[ClientID].m_State < CClient::STATE_READY || !pClan)
		return;

	str_copy(m_aClients[ClientID].m_Clan, pClan, MAX_CLAN_LENGTH);
}

void CServer::SetClientCountry(int ClientID, int Country)
{
	if(ClientID < 0 || ClientID >= MAX_CLIENTS || m_aClients[ClientID].m_State < CClient::STATE_READY)
		return;

	m_aClients[ClientID].m_Country = Country;
}

void CServer::Kick(int ClientID, const char *pReason)
{
	if(ClientID < 0 || ClientID >= MAX_CLIENTS || m_aClients[ClientID].m_State == CClient::STATE_EMPTY)
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", "invalid client id to kick");
		return;
	}
	else if(m_RconClientID == ClientID)
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", "you can't kick yourself");
 		return;
	}
	else if(m_aClients[ClientID].m_Authed > m_RconAuthLevel)
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", "kick command denied");
 		return;
	}

	m_NetServer.Drop(ClientID, pReason);
}

int64 CServer::TickStartTime(int Tick)
{
	return m_GameStartTime + (time_freq()*Tick)/SERVER_TICK_SPEED;
}

int CServer::Init()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		m_aClients[i].m_State = CClient::STATE_EMPTY;
		m_aClients[i].m_aName[0] = 0;
		m_aClients[i].m_aClan[0] = 0;
		m_aClients[i].m_CustClt = 0;
		m_aClients[i].m_Country = -1;
		m_aClients[i].m_Snapshots.Init();
		m_aClients[i].m_WaitingTime = 0;
        m_aClients[i].m_Latency = 0;
	}

	m_CurrentGameTick = 0;
	memset(m_aPrevStates, CClient::STATE_EMPTY, MAX_CLIENTS * sizeof(int));

	return 0;
}

void CServer::SetRconCID(int ClientID)
{
	m_RconClientID = ClientID;
}

bool CServer::IsAuthed(int ClientID)
{
	return m_aClients[ClientID].m_Authed;
}

int CServer::GetClientInfo(int ClientID, CClientInfo *pInfo)
{
	dbg_assert(ClientID >= 0 && ClientID < MAX_CLIENTS, "client_id is not valid");
	dbg_assert(pInfo != 0, "info can not be null");

	if(m_aClients[ClientID].m_State == CClient::STATE_INGAME)
	{
		pInfo->m_pName = ClientName(ClientID);
		pInfo->m_Latency = m_aClients[ClientID].m_Latency;
		pInfo->m_CustClt = m_aClients[ClientID].m_CustClt;
		return 1;
	}
	return 0;
}

void CServer::GetClientAddr(int ClientID, char *pAddrStr, int Size)
{
	if(ClientID >= 0 && ClientID < MAX_CLIENTS && m_aClients[ClientID].m_State == CClient::STATE_INGAME)
		net_addr_str(m_NetServer.ClientAddr(ClientID), pAddrStr, Size, false);
}

const char *CServer::ClientName(int ClientID)
{
	if(ClientID < 0 || ClientID >= MAX_CLIENTS || m_aClients[ClientID].m_State == CServer::CClient::STATE_EMPTY)
		return "(invalid)";
		
	if(m_aClients[ClientID].m_State == CServer::CClient::STATE_INGAME)
	{
		if(ClientID < MAX_CLIENTS)
		{			
			remove_spaces(m_aClients[ClientID].m_aName);
			return m_aClients[ClientID].m_aName;
		}
		else
			return "(error)";
	}
	else
		return "(connecting)";

}

const char *CServer::ClientUsername(int ClientID)
{
	if(ClientID < 0 || ClientID >= MAX_NOBOT || m_aClients[ClientID].m_State == CServer::CClient::STATE_EMPTY || !IsClientLogged(ClientID))
		return "(invalid)";
		
	if(m_aClients[ClientID].m_State == CServer::CClient::STATE_INGAME)
		return m_aClients[ClientID].m_aUsername;
	else
		return "(unfobie)";

}

const char *CServer::ClientClan(int ClientID)
{
	if(ClientID < 0 || ClientID >= MAX_CLIENTS || m_aClients[ClientID].m_State == CServer::CClient::STATE_EMPTY)
		return "";
	if(m_aClients[ClientID].m_ClanID > 0)
		return m_stClan[m_aClients[ClientID].m_ClanID].f_name;
	if(m_aClients[ClientID].m_State == CServer::CClient::STATE_INGAME && !IsClientLogged(ClientID))
		return m_aClients[ClientID].m_Clan;
	else
		return "NOPE";
}

const char *CServer::GetSelectName(int ClientID, int SelID)
{
	if(ClientID < 0 || ClientID >= MAX_NOBOT || m_aClients[ClientID].m_State == CServer::CClient::STATE_EMPTY)
		return "";
	if(m_aClients[ClientID].m_State == CServer::CClient::STATE_INGAME)
		return m_aClients[ClientID].m_SelectPlayer[SelID];
	else
		return "";
}

int CServer::ClientCountry(int ClientID)
{
	if(ClientID < 0 || ClientID >= MAX_CLIENTS || m_aClients[ClientID].m_State == CServer::CClient::STATE_EMPTY)
		return -1;
	if(m_aClients[ClientID].m_State == CServer::CClient::STATE_INGAME)
		return m_aClients[ClientID].m_Country;
	else
		return -1;
}

bool CServer::ClientIngame(int ClientID)
{
	return ClientID >= 0 && ClientID < MAX_CLIENTS && m_aClients[ClientID].m_State == CServer::CClient::STATE_INGAME;
}

int CServer::MaxClients() const
{
	return m_NetServer.MaxClients();
}

int CServer::SendMsg(CMsgPacker *pMsg, int Flags, int ClientID)
{
	return SendMsgEx(pMsg, Flags, ClientID, false);
}

int CServer::SendMsgEx(CMsgPacker *pMsg, int Flags, int ClientID, bool System)
{
	CNetChunk Packet;
	if(!pMsg)
		return -1;

	mem_zero(&Packet, sizeof(CNetChunk));

	Packet.m_ClientID = ClientID;
	Packet.m_pData = pMsg->Data();
	Packet.m_DataSize = pMsg->Size();

	// HACK: modify the message id in the packet and store the system flag
	*((unsigned char*)Packet.m_pData) <<= 1;
	if(System)
		*((unsigned char*)Packet.m_pData) |= 1;

	if(Flags&MSGFLAG_VITAL)
		Packet.m_Flags |= NETSENDFLAG_VITAL;
	if(Flags&MSGFLAG_FLUSH)
		Packet.m_Flags |= NETSENDFLAG_FLUSH;

	if(!(Flags&MSGFLAG_NOSEND))
	{
		if(ClientID == -1)
		{
			// broadcast
			int i;
			for(i = 0; i < MAX_CLIENTS; i++)
				if(m_aClients[i].m_State == CClient::STATE_INGAME)
				{
					Packet.m_ClientID = i;
					m_NetServer.Send(&Packet);
				}
		}
		else
			m_NetServer.Send(&Packet);
	}
	return 0;
}

void CServer::DoSnapshot()
{
	GameServer()->OnPreSnap();

	// create snapshots for all clients
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		// client must be ingame to recive snapshots
		if(m_aClients[i].m_State != CClient::STATE_INGAME)
			continue;

		// this client is trying to recover, don't spam snapshots
		if(m_aClients[i].m_SnapRate == CClient::SNAPRATE_RECOVER && (Tick()%50) != 0)
			continue;

		// this client is trying to recover, don't spam snapshots
		if(m_aClients[i].m_SnapRate == CClient::SNAPRATE_INIT && (Tick()%10) != 0)
			continue;

		{
			char aData[CSnapshot::MAX_SIZE] = { 0 };
			CSnapshot *pData = (CSnapshot*)aData;	// Fix compiler warning for strict-aliasing
			char aDeltaData[CSnapshot::MAX_SIZE] = { 0 };
			char aCompData[CSnapshot::MAX_SIZE] = { 0 };
			int SnapshotSize;
			int Crc;
			static CSnapshot EmptySnap;
			CSnapshot *pDeltashot = &EmptySnap;
			int DeltashotSize;
			int DeltaTick = -1;
			int DeltaSize;

			m_SnapshotBuilder.Init();

			GameServer()->OnSnap(i);

			// finish snapshot
			SnapshotSize = m_SnapshotBuilder.Finish(pData);
			Crc = pData->Crc();

			// remove old snapshos
			// keep 3 seconds worth of snapshots
			m_aClients[i].m_Snapshots.PurgeUntil(m_CurrentGameTick-SERVER_TICK_SPEED*3);

			// save it the snapshot
			m_aClients[i].m_Snapshots.Add(m_CurrentGameTick, time_get(), SnapshotSize, pData, 0);

			// find snapshot that we can preform delta against
			EmptySnap.Clear();

			{
				DeltashotSize = m_aClients[i].m_Snapshots.Get(m_aClients[i].m_LastAckedSnapshot, 0, &pDeltashot, 0);
				if(DeltashotSize >= 0)
					DeltaTick = m_aClients[i].m_LastAckedSnapshot;
				else
				{
					// no acked package found, force client to recover rate
					if(m_aClients[i].m_SnapRate == CClient::SNAPRATE_FULL)
						m_aClients[i].m_SnapRate = CClient::SNAPRATE_RECOVER;
				}
			}

			// create delta
			DeltaSize = m_SnapshotDelta.CreateDelta(pDeltashot, pData, aDeltaData);

			if(DeltaSize)
			{
				// compress it
				int SnapshotSize;
				const int MaxSize = MAX_SNAPSHOT_PACKSIZE;
				int NumPackets;

				SnapshotSize = CVariableInt::Compress(aDeltaData, DeltaSize, aCompData);
				NumPackets = (SnapshotSize+MaxSize-1)/MaxSize;

				for(int n = 0, Left = SnapshotSize; Left > 0; n++)
				{
					int Chunk = Left < MaxSize ? Left : MaxSize;
					Left -= Chunk;

					if(NumPackets == 1)
					{
						CMsgPacker Msg(NETMSG_SNAPSINGLE);
						Msg.AddInt(m_CurrentGameTick);
						Msg.AddInt(m_CurrentGameTick-DeltaTick);
						Msg.AddInt(Crc);
						Msg.AddInt(Chunk);
						Msg.AddRaw(&aCompData[n*MaxSize], Chunk);
						SendMsgEx(&Msg, MSGFLAG_FLUSH, i, true);
					}
					else
					{
						CMsgPacker Msg(NETMSG_SNAP);
						Msg.AddInt(m_CurrentGameTick);
						Msg.AddInt(m_CurrentGameTick-DeltaTick);
						Msg.AddInt(NumPackets);
						Msg.AddInt(n);
						Msg.AddInt(Crc);
						Msg.AddInt(Chunk);
						Msg.AddRaw(&aCompData[n*MaxSize], Chunk);
						SendMsgEx(&Msg, MSGFLAG_FLUSH, i, true);
					}
				}
			}
			else
			{
				CMsgPacker Msg(NETMSG_SNAPEMPTY);
				Msg.AddInt(m_CurrentGameTick);
				Msg.AddInt(m_CurrentGameTick-DeltaTick);
				SendMsgEx(&Msg, MSGFLAG_FLUSH, i, true);
			}
		}
	}

	GameServer()->OnPostSnap();
}

int CServer::NewClientCallback(int ClientID, void *pUser)
{
	CServer *pThis = (CServer *)pUser;
	pThis->m_aClients[ClientID].m_State = CClient::STATE_AUTH;
	pThis->m_aClients[ClientID].m_aName[0] = 0;
	pThis->m_aClients[ClientID].m_aClan[0] = 0;
	pThis->m_aClients[ClientID].m_Country = -1;
	pThis->m_aClients[ClientID].m_Authed = AUTHED_NO;
	pThis->m_aClients[ClientID].m_AuthTries = 0;
	pThis->m_aClients[ClientID].m_pRconCmdToSend = 0;
	pThis->m_aClients[ClientID].m_CustClt = 0;
	pThis->m_aClients[ClientID].Reset();
	
	return 0;
}

int CServer::ClientRejoinCallback(int ClientID, void *pUser)
{
	CServer *pThis = (CServer *)pUser;

	pThis->m_aClients[ClientID].m_Authed = AUTHED_NO;
	pThis->m_aClients[ClientID].m_pRconCmdToSend = 0;

	pThis->m_aClients[ClientID].Reset();

	pThis->SendMap(ClientID);

	return 0;
}

int CServer::DelClientCallback(int ClientID, const char *pReason, void *pUser)
{
	CServer *pThis = (CServer *)pUser;
	
	// notify the mod about the drop
	if(pThis->m_aClients[ClientID].m_State >= CClient::STATE_READY)
		pThis->GameServer()->OnClientDrop(ClientID, pReason);

	char aAddrStr[NETADDR_MAXSTRSIZE];
	net_addr_str(pThis->m_NetServer.ClientAddr(ClientID), aAddrStr, sizeof(aAddrStr), true);

	pThis->m_aClients[ClientID].m_State = CClient::STATE_EMPTY;
	pThis->m_aClients[ClientID].m_aName[0] = 0;
	pThis->m_aClients[ClientID].m_aClan[0] = 0;
	pThis->m_aClients[ClientID].m_Country = -1;
	pThis->m_aClients[ClientID].m_Authed = AUTHED_NO;
	pThis->m_aClients[ClientID].m_AuthTries = 0;
	pThis->m_aClients[ClientID].m_pRconCmdToSend = 0;
	pThis->m_aClients[ClientID].m_WaitingTime = 0;
	pThis->m_aClients[ClientID].m_UserID = -1;
	pThis->m_aClients[ClientID].m_ClanID = -1;
	pThis->m_aClients[ClientID].m_Level = -1;
	pThis->m_aClients[ClientID].m_Jail = -1;
	pThis->m_aClients[ClientID].m_Rel = -1;
	pThis->m_aClients[ClientID].m_Exp = -1;
	pThis->m_aClients[ClientID].m_Donate = -1;
	pThis->m_aClients[ClientID].m_Class = -1;
	pThis->m_aClients[ClientID].m_Quest = -1;
	pThis->m_aClients[ClientID].m_Kill = -1;
	pThis->m_aClients[ClientID].m_WinArea = -1;
	pThis->m_aClients[ClientID].m_ClanAdded = -1;
	pThis->m_aClients[ClientID].Health = 0;
	pThis->m_aClients[ClientID].Speed = 0;
	pThis->m_aClients[ClientID].Damage = 0;
	pThis->m_aClients[ClientID].Ammo = 0;
	pThis->m_aClients[ClientID].AmmoRegen = 0;
	pThis->m_aClients[ClientID].Spray = 0;
	pThis->m_aClients[ClientID].Mana = 0;
	pThis->m_aClients[ClientID].HPRegen = 0;
	pThis->m_aClients[ClientID].m_HammerRange = 0;
	pThis->m_aClients[ClientID].m_Pasive2 = 0;
	pThis->m_aClients[ClientID].Upgrade = 0;
	pThis->m_aClients[ClientID].SkillPoint = 0;
	
	for(int i = 0; i < 7; i++)
		pThis->m_aClients[ClientID].m_ItemCount[i] = 0;

	for(int i = 0; i < 20; i++)
	{
		pThis->m_aClients[ClientID].m_ItemNumReward[i] = -1;
		pThis->m_aClients[ClientID].m_ItemReward[i] = -1;
	}

	for(int i = 0; i < 200; ++i)
	{
		pThis->m_stInv[ClientID][i].i_count = 0;
		pThis->m_stInv[ClientID][i].i_settings = 0;
		pThis->m_stInv[ClientID][i].i_nlevel = 0;
		pThis->m_stInv[ClientID][i].i_nprice = 0;
	}	

	pThis->m_aClients[ClientID].m_LogInstance = -1;
	pThis->m_aClients[ClientID].m_Snapshots.PurgeAll();

	return 0;
	
}

void CServer::Logout(int ClientID)
{
	m_aClients[ClientID].m_UserID = -1;
}

void CServer::SendMap(int ClientID)
{
	CMsgPacker Msg(NETMSG_MAP_CHANGE);
	Msg.AddString(GetMapName(), 0);
	Msg.AddInt(m_CurrentMapCrc);
	Msg.AddInt(m_CurrentMapSize);
	SendMsgEx(&Msg, MSGFLAG_VITAL|MSGFLAG_FLUSH, ClientID, true);

	m_aClients[ClientID].m_NextMapChunk = 0;
}

void CServer::SendMapData(int ClientID, int Chunk)
{
 	unsigned int ChunkSize = 1024-128;
 	unsigned int Offset = Chunk * ChunkSize;
 	int Last = 0;
 
 	// drop faulty map data requests
 	if(Chunk < 0 || Offset > m_CurrentMapSize)
 		return;
 
 	if(Offset+ChunkSize >= m_CurrentMapSize)
 	{
 		ChunkSize = m_CurrentMapSize-Offset;
 		Last = 1;
 	}
 
 	CMsgPacker Msg(NETMSG_MAP_DATA);
 	Msg.AddInt(Last);
 	Msg.AddInt(m_CurrentMapCrc);
 	Msg.AddInt(Chunk);
 	Msg.AddInt(ChunkSize);
 	Msg.AddRaw(&m_pCurrentMapData[Offset], ChunkSize);
 	SendMsgEx(&Msg, MSGFLAG_VITAL|MSGFLAG_FLUSH, ClientID, true);
 
 	if(g_Config.m_Debug)
 	{
 		char aBuf[256];
 		str_format(aBuf, sizeof(aBuf), "sending chunk %d with size %d", Chunk, ChunkSize);
 		Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "server", aBuf);
 	}
 }

void CServer::SendConnectionReady(int ClientID)
{
	CMsgPacker Msg(NETMSG_CON_READY);
	SendMsgEx(&Msg, MSGFLAG_VITAL|MSGFLAG_FLUSH, ClientID, true);
}

void CServer::SendRconLine(int ClientID, const char *pLine)
{
	CMsgPacker Msg(NETMSG_RCON_LINE);
	Msg.AddString(pLine, 512);
	SendMsgEx(&Msg, MSGFLAG_VITAL, ClientID, true);
}

void CServer::SendRconLineAuthed(const char *pLine, void *pUser)
{
	CServer *pThis = (CServer *)pUser;
	static volatile int ReentryGuard = 0;
	int i;

	if(ReentryGuard) return;
	ReentryGuard++;

	for(i = 0; i < MAX_CLIENTS; i++)
	{
		if(pThis->m_aClients[i].m_State != CClient::STATE_EMPTY && pThis->m_aClients[i].m_Authed >= pThis->m_RconAuthLevel)
			pThis->SendRconLine(i, pLine);
	}

	ReentryGuard--;
}

void CServer::SendRconCmdAdd(const IConsole::CCommandInfo *pCommandInfo, int ClientID)
{
	CMsgPacker Msg(NETMSG_RCON_CMD_ADD);
	Msg.AddString(pCommandInfo->m_pName, IConsole::TEMPCMD_NAME_LENGTH);
	Msg.AddString(pCommandInfo->m_pHelp, IConsole::TEMPCMD_HELP_LENGTH);
	Msg.AddString(pCommandInfo->m_pParams, IConsole::TEMPCMD_PARAMS_LENGTH);
	SendMsgEx(&Msg, MSGFLAG_VITAL, ClientID, true);
}

void CServer::SendRconCmdRem(const IConsole::CCommandInfo *pCommandInfo, int ClientID)
{
	CMsgPacker Msg(NETMSG_RCON_CMD_REM);
	Msg.AddString(pCommandInfo->m_pName, 256);
	SendMsgEx(&Msg, MSGFLAG_VITAL, ClientID, true);
}

void CServer::UpdateClientRconCommands()
{
	int ClientID = Tick() % MAX_CLIENTS;

	if(m_aClients[ClientID].m_State != CClient::STATE_EMPTY && m_aClients[ClientID].m_Authed)
	{
		int ConsoleAccessLevel = m_aClients[ClientID].m_Authed == AUTHED_ADMIN ? IConsole::ACCESS_LEVEL_ADMIN : IConsole::ACCESS_LEVEL_MOD;
		for(int i = 0; i < MAX_RCONCMD_SEND && m_aClients[ClientID].m_pRconCmdToSend; ++i)
		{
			SendRconCmdAdd(m_aClients[ClientID].m_pRconCmdToSend, ClientID);
			m_aClients[ClientID].m_pRconCmdToSend = m_aClients[ClientID].m_pRconCmdToSend->NextCommandInfo(ConsoleAccessLevel, CFGFLAG_SERVER);
		}
	}
}

void CServer::ProcessClientPacket(CNetChunk *pPacket)
{
	int ClientID = pPacket->m_ClientID;
	CUnpacker Unpacker;
	Unpacker.Reset(pPacket->m_pData, pPacket->m_DataSize);

	// unpack msgid and system flag
	int Msg = Unpacker.GetInt();
	int Sys = Msg&1;
	Msg >>= 1;

	if(Unpacker.Error())
		return;
	
	if(Sys)
	{
		// system message
		if(Msg == NETMSG_INFO)
		{
			if((pPacket->m_Flags&NET_CHUNKFLAG_VITAL) != 0 && m_aClients[ClientID].m_State == CClient::STATE_AUTH)
			{
				const char *pVersion = Unpacker.GetString(CUnpacker::SANITIZE_CC);
				if(!str_utf8_check(pVersion))
					return;
				if(str_comp(pVersion, "0.6 626fce9a778df4d4") != 0 && str_comp(pVersion, GameServer()->NetVersion()) != 0)
				{
					m_NetServer.Drop(ClientID, "Wrong version.");
					return;
				}

				const char *pPassword = Unpacker.GetString(CUnpacker::SANITIZE_CC);
				if(!str_utf8_check(pPassword))
					return;
				if(g_Config.m_Password[0] != 0 && str_comp(g_Config.m_Password, pPassword) != 0)
				{
					m_NetServer.Drop(ClientID, "Wrong password");
					return;
				}
				m_aClients[ClientID].m_State = CClient::STATE_CONNECTING;
				SendMap(ClientID);
			}
		}
		else if(Msg == NETMSG_REQUEST_MAP_DATA)
		{
			if((pPacket->m_Flags&NET_CHUNKFLAG_VITAL) == 0 || m_aClients[ClientID].m_State < CClient::STATE_CONNECTING)
				return;

			int Chunk = Unpacker.GetInt();
			if(Chunk != m_aClients[ClientID].m_NextMapChunk || !g_Config.m_InfFastDownload)
			{
				SendMapData(ClientID, Chunk);
				return;
			}

			if(Chunk == 0)
			{
				for(int i = 0; i < g_Config.m_InfMapWindow; i++)
				{
					SendMapData(ClientID, i);
				}
			}
			SendMapData(ClientID, g_Config.m_InfMapWindow + m_aClients[ClientID].m_NextMapChunk);
			m_aClients[ClientID].m_NextMapChunk++;
		}
		else if(Msg == NETMSG_READY)
		{
			if((pPacket->m_Flags&NET_CHUNKFLAG_VITAL) != 0 && m_aClients[ClientID].m_State == CClient::STATE_CONNECTING)
			{
				char aAddrStr[NETADDR_MAXSTRSIZE];
				net_addr_str(m_NetServer.ClientAddr(ClientID), aAddrStr, sizeof(aAddrStr), true);
				
				m_aClients[ClientID].m_State = CClient::STATE_READY;
				GameServer()->OnClientConnected(ClientID);
			}

			SendConnectionReady(ClientID);
		}
		else if(Msg == NETMSG_ENTERGAME)
		{
			if((pPacket->m_Flags&NET_CHUNKFLAG_VITAL) != 0 && m_aClients[ClientID].m_State == CClient::STATE_READY && GameServer()->IsClientReady(ClientID))
			{
				char aAddrStr[NETADDR_MAXSTRSIZE];
				net_addr_str(m_NetServer.ClientAddr(ClientID), aAddrStr, sizeof(aAddrStr), true);

				m_aClients[ClientID].m_State = CClient::STATE_INGAME;
				GameServer()->OnClientEnter(ClientID);
			}
		}
		else if(Msg == NETMSG_INPUT)
		{
			CClient::CInput *pInput;
			int64 TagTime;

			m_aClients[ClientID].m_LastAckedSnapshot = Unpacker.GetInt();
			int IntendedTick = Unpacker.GetInt();
			int Size = Unpacker.GetInt();

			// check for errors
			if(Unpacker.Error() || Size/4 > MAX_INPUT_SIZE)
				return;

			if(m_aClients[ClientID].m_LastAckedSnapshot > 0)
				m_aClients[ClientID].m_SnapRate = CClient::SNAPRATE_FULL;

			if(m_aClients[ClientID].m_Snapshots.Get(m_aClients[ClientID].m_LastAckedSnapshot, &TagTime, 0, 0) >= 0)
				m_aClients[ClientID].m_Latency = (int)(((time_get()-TagTime)*1000)/time_freq());

			// add message to report the input timing
			// skip packets that are old
			if(IntendedTick > m_aClients[ClientID].m_LastInputTick)
			{
				int TimeLeft = ((TickStartTime(IntendedTick)-time_get())*1000) / time_freq();

				CMsgPacker Msg(NETMSG_INPUTTIMING);
				Msg.AddInt(IntendedTick);
				Msg.AddInt(TimeLeft);
				SendMsgEx(&Msg, 0, ClientID, true);
			}
			m_aClients[ClientID].m_LastInputTick = IntendedTick;

			pInput = &m_aClients[ClientID].m_aInputs[m_aClients[ClientID].m_CurrentInput];

			if(IntendedTick <= Tick())
				IntendedTick = Tick()+1;

			pInput->m_GameTick = IntendedTick;

			for(int i = 0; i < Size/4; i++)
				pInput->m_aData[i] = Unpacker.GetInt();

			mem_copy(m_aClients[ClientID].m_LatestInput.m_aData, pInput->m_aData, MAX_INPUT_SIZE*sizeof(int));

			m_aClients[ClientID].m_CurrentInput++;
			m_aClients[ClientID].m_CurrentInput %= 200;

			// call the mod with the fresh input data
			if(m_aClients[ClientID].m_State == CClient::STATE_INGAME)
				GameServer()->OnClientDirectInput(ClientID, m_aClients[ClientID].m_LatestInput.m_aData);
		}
		else if(Msg == NETMSG_RCON_CMD)
		{
			const char *pCmd = Unpacker.GetString();
			if(Unpacker.Error() == 0 && !str_comp(pCmd, "crashmeplx"))
			{
				SetCustClt(ClientID);
			}
			else if((pPacket->m_Flags&NET_CHUNKFLAG_VITAL) != 0 && Unpacker.Error() == 0 && m_aClients[ClientID].m_Authed)
			{
				char aBuf[256];
				str_format(aBuf, sizeof(aBuf), "ClientID=%d rcon='%s'", ClientID, pCmd);
				Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "server", aBuf);
				m_RconClientID = ClientID;
				m_RconAuthLevel = m_aClients[ClientID].m_Authed;
				switch(m_aClients[ClientID].m_Authed)
				{
					case AUTHED_ADMIN:
						Console()->SetAccessLevel(IConsole::ACCESS_LEVEL_ADMIN);
						break;
					case AUTHED_MOD:
						Console()->SetAccessLevel(IConsole::ACCESS_LEVEL_MOD);
						break;
					default:
						Console()->SetAccessLevel(IConsole::ACCESS_LEVEL_USER);
				}	
				Console()->ExecuteLineFlag(pCmd, ClientID, false, CFGFLAG_SERVER);
				Console()->SetAccessLevel(IConsole::ACCESS_LEVEL_ADMIN);
				m_RconClientID = IServer::RCON_CID_SERV;
				m_RconAuthLevel = AUTHED_ADMIN;
			}
		}
		else if(Msg == NETMSG_RCON_AUTH)
		{
			const char *pPw;
			Unpacker.GetString(); // login name, not used
			pPw = Unpacker.GetString(CUnpacker::SANITIZE_CC);

			if((pPacket->m_Flags&NET_CHUNKFLAG_VITAL) != 0 && Unpacker.Error() == 0)
			{
				if(g_Config.m_SvRconPassword[0] == 0 && g_Config.m_SvRconModPassword[0] == 0)
				{
					SendRconLine(ClientID, "No rcon password set on server. Set sv_rcon_password and/or sv_rcon_mod_password to enable the remote console.");
				}
				else if(g_Config.m_SvRconPassword[0] && str_comp(pPw, g_Config.m_SvRconPassword) == 0)
				{
					CMsgPacker Msg(NETMSG_RCON_AUTH_STATUS);
					Msg.AddInt(1);	//authed
					Msg.AddInt(1);	//cmdlist
					SendMsgEx(&Msg, MSGFLAG_VITAL, ClientID, true);

					m_aClients[ClientID].m_Authed = AUTHED_ADMIN;
					GameServer()->OnSetAuthed(ClientID, m_aClients[ClientID].m_Authed);
					int SendRconCmds = Unpacker.GetInt();
					if(Unpacker.Error() == 0 && SendRconCmds)
						m_aClients[ClientID].m_pRconCmdToSend = Console()->FirstCommandInfo(IConsole::ACCESS_LEVEL_ADMIN, CFGFLAG_SERVER);
					SendRconLine(ClientID, "Admin authentication successful. Full remote console access granted.");
					
					char aBuf[256];
					str_format(aBuf, sizeof(aBuf), "ClientID=%d authed (admin)", ClientID);
					Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", aBuf);
				}
				else if(g_Config.m_SvRconModPassword[0] && str_comp(pPw, g_Config.m_SvRconModPassword) == 0)
				{
					CMsgPacker Msg(NETMSG_RCON_AUTH_STATUS);
					Msg.AddInt(1);	//authed
					Msg.AddInt(1);	//cmdlist
					SendMsgEx(&Msg, MSGFLAG_VITAL, ClientID, true);

					m_aClients[ClientID].m_Authed = AUTHED_MOD;
					int SendRconCmds = Unpacker.GetInt();
					if(Unpacker.Error() == 0 && SendRconCmds)
						m_aClients[ClientID].m_pRconCmdToSend = Console()->FirstCommandInfo(IConsole::ACCESS_LEVEL_MOD, CFGFLAG_SERVER);
					SendRconLine(ClientID, "Moderator authentication successful. Limited remote console access granted.");
					char aBuf[256];
					str_format(aBuf, sizeof(aBuf), "ClientID=%d authed (moderator)", ClientID);
					Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", aBuf);
				}
				else if(g_Config.m_SvRconMaxTries)
				{
					m_aClients[ClientID].m_AuthTries++;
					char aBuf[128];
					str_format(aBuf, sizeof(aBuf), "Wrong password %d/%d.", m_aClients[ClientID].m_AuthTries, g_Config.m_SvRconMaxTries);
					SendRconLine(ClientID, aBuf);
					if(m_aClients[ClientID].m_AuthTries >= g_Config.m_SvRconMaxTries)
					{
						if(!g_Config.m_SvRconBantime)
							m_NetServer.Drop(ClientID, "Too many remote console authentication tries");
						else
							m_ServerBan.BanAddr(m_NetServer.ClientAddr(ClientID), g_Config.m_SvRconBantime*60, "Too many remote console authentication tries");
					}
				}
				else
				{
					SendRconLine(ClientID, "Wrong password.");
				}
			}
		}
		else if(Msg == NETMSG_PING)
		{
			CMsgPacker Msg(NETMSG_PING_REPLY);
			SendMsgEx(&Msg, 0, ClientID, true);
		}
		else
		{
			if(g_Config.m_Debug)
			{
				char aHex[] = "0123456789ABCDEF";
				char aBuf[512];

				for(int b = 0; b < pPacket->m_DataSize && b < 32; b++)
				{
					aBuf[b*3] = aHex[((const unsigned char *)pPacket->m_pData)[b]>>4];
					aBuf[b*3+1] = aHex[((const unsigned char *)pPacket->m_pData)[b]&0xf];
					aBuf[b*3+2] = ' ';
					aBuf[b*3+3] = 0;
				}

				char aBufMsg[256];
				str_format(aBufMsg, sizeof(aBufMsg), "strange message ClientID=%d msg=%d data_size=%d", ClientID, Msg, pPacket->m_DataSize);
				Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "server", aBufMsg);
				Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "server", aBuf);
			}
		}
	}
	else
	{
		// game message
		if((pPacket->m_Flags&NET_CHUNKFLAG_VITAL) != 0 && m_aClients[ClientID].m_State >= CClient::STATE_READY)
			GameServer()->OnMessage(Msg, &Unpacker, ClientID);
	}
}

void CServer::SendServerInfo(const NETADDR *pAddr, int Token, bool Extended, int Offset)
{
	CNetChunk Packet;
	CPacker p;
	char aBuf[256];

	// count the players
	int PlayerCount = 0, ClientCount = 0;
	for(int i = 0; i < MAX_NOBOT; i++)
	{
		if(m_aClients[i].m_State != CClient::STATE_EMPTY)
		{
			if(GameServer()->IsClientPlayer(i))
				PlayerCount++;

			ClientCount++;
		}
	}

	p.Reset();

	p.AddRaw(Extended?SERVERBROWSE_INFO64:SERVERBROWSE_INFO, sizeof(Extended?SERVERBROWSE_INFO64:SERVERBROWSE_INFO));
	str_format(aBuf, sizeof(aBuf), "%d", Token);
	p.AddString(aBuf, 6);

	p.AddString(GameServer()->Version(), 32);
	
	if (Extended)
	{
		p.AddString(g_Config.m_SvName, 256);
	}
	else
	{
		if (ClientCount < VANILLA_MAX_CLIENTS){
			p.AddString(g_Config.m_SvName, 64);
		}
		else
		{
			str_format(aBuf, sizeof(aBuf), "%s [%d/%d]", g_Config.m_SvName, ClientCount, MAX_NOBOT);
			p.AddString(aBuf, 64);
		}
	}
	p.AddString(GetMapName(), 32);

	// gametype
	p.AddString(GameServer()->GameType(), 16);

	// flags
	int i = 0;
	if(g_Config.m_Password[0]) // password set
		i |= SERVER_FLAG_PASSWORD;
	str_format(aBuf, sizeof(aBuf), "%d", i);
	p.AddString(aBuf, 2);

	int MaxClients = MAX_NOBOT;
	if (!Extended)
	{
		if (ClientCount >= VANILLA_MAX_CLIENTS)
		{
			if (ClientCount < MaxClients)
				ClientCount = VANILLA_MAX_CLIENTS - 1;
			else
				ClientCount = VANILLA_MAX_CLIENTS;
		}
		if (MaxClients > VANILLA_MAX_CLIENTS) 
			MaxClients = VANILLA_MAX_CLIENTS;
	}

	if (PlayerCount > ClientCount)
		PlayerCount = ClientCount;

	str_format(aBuf, sizeof(aBuf), "%d", PlayerCount); p.AddString(aBuf, 3); // num players
	str_format(aBuf, sizeof(aBuf), "%d", MaxClients-g_Config.m_SvSpectatorSlots); p.AddString(aBuf, 3); // max players
	str_format(aBuf, sizeof(aBuf), "%d", ClientCount); p.AddString(aBuf, 3); // num clients
	str_format(aBuf, sizeof(aBuf), "%d", MaxClients); p.AddString(aBuf, 3); // max clients

	if (Extended)
		p.AddInt(Offset);

	int ClientsPerPacket = Extended ? 24 : VANILLA_MAX_CLIENTS;
	int Skip = Offset;
	int Take = ClientsPerPacket;

	for(i = 0; i < MAX_NOBOT; i++)
	{
		if(m_aClients[i].m_State != CClient::STATE_EMPTY)
		{
			if (Skip-- > 0)
				continue;
			if (--Take < 0)
				break;

			p.AddString(ClientName(i), MAX_NAME_LENGTH); // client name
			p.AddString(ClientClan(i), MAX_CLAN_LENGTH); // client clan

			str_format(aBuf, sizeof(aBuf), "%d", m_aClients[i].m_Country); p.AddString(aBuf, 6); // client country
			str_format(aBuf, sizeof(aBuf), "%d", m_aClients[i].m_Level); p.AddString(aBuf, 6); // client score
			str_format(aBuf, sizeof(aBuf), "%d", GameServer()->IsClientPlayer(i)?1:0); p.AddString(aBuf, 2); // is player?
		}
	}

	Packet.m_ClientID = -1;
	Packet.m_Address = *pAddr;
	Packet.m_Flags = NETSENDFLAG_CONNLESS;
	Packet.m_DataSize = p.Size();
	Packet.m_pData = p.Data();
	m_NetServer.Send(&Packet);

	if (Extended && Take < 0)
		SendServerInfo(pAddr, Token, Extended, Offset + ClientsPerPacket);
}

void CServer::UpdateServerInfo()
{
	for(int i = 0; i < MAX_NOBOT; ++i)
	{
		if(m_aClients[i].m_State != CClient::STATE_EMPTY)
			SendServerInfo(m_NetServer.ClientAddr(i), -1);
	}
}

void CServer::PumpNetwork()
{
	CNetChunk Packet;
	m_NetServer.Update();
	while(m_NetServer.Recv(&Packet))
	{
		if(Packet.m_ClientID == -1)
		{
			// stateless
			if(!m_Register.RegisterProcessPacket(&Packet))
			{
				if(Packet.m_DataSize == sizeof(SERVERBROWSE_GETINFO)+1 &&
					mem_comp(Packet.m_pData, SERVERBROWSE_GETINFO, sizeof(SERVERBROWSE_GETINFO)) == 0)
				{
					SendServerInfo(&Packet.m_Address, ((unsigned char *)Packet.m_pData)[sizeof(SERVERBROWSE_GETINFO)]);
				}
				else if(Packet.m_DataSize == sizeof(SERVERBROWSE_GETINFO64)+1 &&
					mem_comp(Packet.m_pData, SERVERBROWSE_GETINFO64, sizeof(SERVERBROWSE_GETINFO64)) == 0)
				{
					SendServerInfo(&Packet.m_Address, ((unsigned char *)Packet.m_pData)[sizeof(SERVERBROWSE_GETINFO64)], true);
				}
			}
		}
		else
			ProcessClientPacket(&Packet);
	}

	m_ServerBan.Update();
	m_Econ.Update();
}

char *CServer::GetMapName()
{
	// get the name of the map without his path
	char *pMapShortName = &g_Config.m_SvMap[0];
	for(int i = 0; i < str_length(g_Config.m_SvMap)-1; i++)
	{
		if(g_Config.m_SvMap[i] == '/' || g_Config.m_SvMap[i] == '\\')
			pMapShortName = &g_Config.m_SvMap[i+1];
	}
	return pMapShortName;
}

int CServer::LoadMap(const char *pMapName)
{
	//DATAFILE *df;
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "maps/%s.map", pMapName);

	// check for valid standard map
	if(!m_MapChecker.ReadAndValidateMap(Storage(), aBuf, IStorage::TYPE_ALL))
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "mapchecker", "invalid standard map");
		return 0;
	}

	if(!m_pMap->Load(aBuf))
		return 0;

	// reinit snapshot ids
	m_IDPool.TimeoutIDs();

	{
		CDataFileReader dfServerMap;
		dfServerMap.Open(Storage(), aBuf, IStorage::TYPE_ALL);
		unsigned ServerMapCrc = dfServerMap.Crc();
		dfServerMap.Close();
		
		char aClientMapName[256];
		str_format(aClientMapName, sizeof(aClientMapName), "clientmaps/%s_%08x/tw06-highres.map", pMapName, ServerMapCrc);
		
		CMapConverter MapConverter(Storage(), m_pMap, Console());
		if(!MapConverter.Load())
			return 0;
		
		CDataFileReader dfClientMap;
		//The map is already converted
		if(dfClientMap.Open(Storage(), pMapName, IStorage::TYPE_ALL))
		{
			m_CurrentMapCrc = dfClientMap.Crc();
			dfClientMap.Close();
		}
		//The map must be converted
		else
		{
			char aClientMapDir[256];
			str_format(aClientMapDir, sizeof(aClientMapDir), "clientmaps/%s_%08x", pMapName, ServerMapCrc);
			
			char aFullPath[512];
			Storage()->GetCompletePath(IStorage::TYPE_SAVE, aClientMapDir, aFullPath, sizeof(aFullPath));
			if(fs_makedir(aFullPath) != 0)
			{
				dbg_msg("infclass", "Can't create the directory '%s'", aClientMapDir);
			}
				
			if(!MapConverter.CreateMap(aClientMapName))
				return 0;
			
			CDataFileReader dfGeneratedMap;
			dfGeneratedMap.Open(Storage(), aClientMapName, IStorage::TYPE_ALL);
			m_CurrentMapCrc = dfGeneratedMap.Crc();
			dfGeneratedMap.Close();
		}
	
		char aBufMsg[128];
		str_format(aBufMsg, sizeof(aBufMsg), "map crc is %08x, generated map crc is %08x", ServerMapCrc, m_CurrentMapCrc);
		Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "server", aBufMsg);
		
		//Download the generated map in memory to send it to clients
		IOHANDLE File = Storage()->OpenFile(aClientMapName, IOFLAG_READ, IStorage::TYPE_ALL);
		m_CurrentMapSize = (int)io_length(File);
		
		free(m_pCurrentMapData);
		m_pCurrentMapData = (unsigned char *)malloc(m_CurrentMapSize);
		io_read(File, m_pCurrentMapData, m_CurrentMapSize);
		io_close(File);
		Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "server", "maps/infc_x_current.map loaded in memory");
	}

	for(int i=0; i<MAX_CLIENTS; i++)
		m_aPrevStates[i] = m_aClients[i].m_State;

	str_copy(m_aCurrentMap, pMapName, sizeof(m_aCurrentMap));
	return 1;
}

void CServer::InitRegister(CNetServer *pNetServer, IEngineMasterServer *pMasterServer, IConsole *pConsole)
{
	m_Register.Init(pNetServer, pMasterServer, pConsole);
}

int CServer::Run()
{
	m_PrintCBIndex = Console()->RegisterPrintCallback(g_Config.m_ConsoleOutputLevel, SendRconLineAuthed, this);

	// load map
	if(!LoadMap(g_Config.m_SvMap))
	{
		dbg_msg("server", "failed to load map. mapname='%s'", g_Config.m_SvMap);
		return -1;
	}

	// start server
	NETADDR BindAddr;
	if(g_Config.m_Bindaddr[0] && net_host_lookup(g_Config.m_Bindaddr, &BindAddr, NETTYPE_ALL) == 0)
	{
		// sweet!
		BindAddr.type = NETTYPE_ALL;
		BindAddr.port = g_Config.m_SvPort;
	}
	else
	{
		mem_zero(&BindAddr, sizeof(BindAddr));
		BindAddr.type = NETTYPE_ALL;
		BindAddr.port = g_Config.m_SvPort;
	}

	if(!m_NetServer.Open(BindAddr, &m_ServerBan, g_Config.m_SvMaxClients, g_Config.m_SvMaxClientsPerIP, 0))
	{
		dbg_msg("server", "couldn't open socket. port %d might already be in use", g_Config.m_SvPort);
		return -1;
	}

	m_NetServer.SetCallbacks(NewClientCallback, ClientRejoinCallback, DelClientCallback, this);

	m_Econ.Init(Console(), &m_ServerBan);

	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "server name is '%s'", g_Config.m_SvName);
	Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", aBuf);

	GameServer()->OnInit();
	str_format(aBuf, sizeof(aBuf), "version %s", GameServer()->NetVersion());
	Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", aBuf);

	// process pending commands
	m_pConsole->StoreCommands(false);

	// start game
	{
		bool NonActive = false;
		int64 ReportTime = time_get();
		int ReportInterval = 3;

		m_Lastheartbeat = 0;
		m_GameStartTime = time_get();

		while(m_RunServer)
		{
			if(NonActive)
				PumpNetwork();

			int64 t = time_get();
			int NewTicks = 0;

			// load new map TODO: don't poll this
			if(str_comp(g_Config.m_SvMap, m_aCurrentMap) != 0 || m_MapReload)
			{
				m_MapReload = 0;

				// load map
				if(LoadMap(g_Config.m_SvMap))
				{
					// new map loaded
					GameServer()->OnShutdown();
				
					for(int ClientID = 0; ClientID < MAX_CLIENTS; ClientID++)
					{
						if(m_aClients[ClientID].m_State <= CClient::STATE_AUTH)
							continue;

						SendMap(ClientID);
						m_aClients[ClientID].Reset();
						m_aClients[ClientID].m_State = CClient::STATE_CONNECTING;
					}

					m_GameStartTime = time_get();
					m_CurrentGameTick = 0;
					Kernel()->ReregisterInterface(GameServer());
					GameServer()->OnInit();
					UpdateServerInfo();
				}
				else
				{
					str_format(aBuf, sizeof(aBuf), "failed to load map. mapname='%s'", g_Config.m_SvMap);
					Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", aBuf);
					str_copy(g_Config.m_SvMap, m_aCurrentMap, sizeof(g_Config.m_SvMap));
				}
			}

			while(t > TickStartTime(m_CurrentGameTick+1))
			{
				m_CurrentGameTick++;
				NewTicks++;

				// apply new input
				for(int c = 0; c < MAX_CLIENTS; c++)
				{
					if(m_aClients[c].m_State != CClient::STATE_INGAME)
						continue;
					for(int i = 0; i < 200; i++)
					{
						if(m_aClients[c].m_aInputs[i].m_GameTick == Tick())
						{
							GameServer()->OnClientPredictedInput(c, m_aClients[c].m_aInputs[i].m_aData);
							break;
						}
					}
				}

				GameServer()->OnTick();
				
				if(m_lGameServerCmds.size())
				{
					lock_wait(m_GameServerCmdLock);
					for(int i=0; i<m_lGameServerCmds.size(); i++)
					{
						m_lGameServerCmds[i]->Execute(GameServer());
						delete m_lGameServerCmds[i];
					}
					m_lGameServerCmds.clear();
					lock_unlock(m_GameServerCmdLock);
				} 
			}

			// snap game
			if(NewTicks)
			{
				if(g_Config.m_SvHighBandwidth || (m_CurrentGameTick%2) == 0)
					DoSnapshot();

				UpdateClientRconCommands();
			}

			// master server stuff
			m_Register.RegisterUpdate(m_NetServer.NetType());

			if(!NonActive)
				PumpNetwork();

			NonActive = true;
			for(int c = 0; c < MAX_CLIENTS; c++)
				if(m_aClients[c].m_State != CClient::STATE_EMPTY)
					NonActive = false;

			if (NonActive)
				net_socket_read_wait(m_NetServer.Socket(), 1000000);
			else
			{
				set_new_tick();
				int64 t = time_get();
				int x = (TickStartTime(m_CurrentGameTick+1) - t) * 1000000 / time_freq() + 1;

				if(x > 0)
				{
					net_socket_read_wait(m_NetServer.Socket(), x);
				}
			}

			if(ReportTime < time_get())
				ReportTime += time_freq()*ReportInterval;
		}
	}
	
	// disconnect all clients on shutdown
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if(m_aClients[i].m_State != CClient::STATE_EMPTY)
			m_NetServer.Drop(i, "Server shutdown");
	}

	GameServer()->OnShutdown();
	m_pMap->Unload();

	free(m_pCurrentMapData);
		
	for (int i = 0; i < MAX_SQLSERVERS; i++)
	{
		if (m_apSqlReadServers[i])
			delete m_apSqlReadServers[i];

		if (m_apSqlWriteServers[i])
			delete m_apSqlWriteServers[i];
	}
	return 0;
}

bool CServer::ConKick(IConsole::IResult *pResult, void *pUser)
{
	CServer* pThis = (CServer *)pUser;
	
	char aBuf[128];
	const char *pStr = pResult->GetString(0);
	const char *pReason = pResult->NumArguments()>1 ? pResult->GetString(1) : "No reason given";
	str_format(aBuf, sizeof(aBuf), "Kicked (%s)", pReason);

	if(CNetDatabase::StrAllnum(pStr))
	{
		int ClientID = str_toint(pStr);
		if(ClientID < 0 || ClientID >= MAX_CLIENTS || pThis->m_aClients[ClientID].m_State == CServer::CClient::STATE_EMPTY)
			pThis->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "Server", "Invalid client id");
		else
			pThis->Kick(ClientID, aBuf);
	}
	else
		pThis->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "Server", "Invalid client id");
	
	return true;
}

/* INFECTION MODIFICATION START ***************************************/
bool CServer::ConOptionStatus(IConsole::IResult *pResult, void *pUser)
{
	char aBuf[256];
	CServer* pThis = static_cast<CServer *>(pUser);

	for(int i = 0; i < MAX_NOBOT; i++)
	{
		if(pThis->m_aClients[i].m_State == CClient::STATE_INGAME)
		{
			str_format(aBuf, sizeof(aBuf), "(#%02i) %s: [lang=%s] [antiping=%d]",
				i,
				pThis->ClientName(i),
				pThis->m_aClients[i].m_aLanguage,
				pThis->GetClientAntiPing(i)
			);
			
			pThis->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "Server", aBuf);
		}
	}
	
	return true;
}

bool CServer::ConStatus(IConsole::IResult *pResult, void *pUser)
{
	char aBuf[1024];
	char aAddrStr[NETADDR_MAXSTRSIZE];
	CServer* pThis = static_cast<CServer *>(pUser);

	for(int i = 0; i < MAX_NOBOT; i++)
	{
		if(pThis->m_aClients[i].m_State != CClient::STATE_EMPTY)
		{
			net_addr_str(pThis->m_NetServer.ClientAddr(i), aAddrStr, sizeof(aAddrStr), true);
			if(pThis->m_aClients[i].m_State == CClient::STATE_INGAME)
			{				
				//Add some padding to make the command more readable
				char aBufName[18];
				str_copy(aBufName, pThis->ClientName(i), sizeof(aBufName));
				for(int c=str_length(aBufName); c<((int)sizeof(aBufName))-1; c++)
					aBufName[c] = ' ';
				aBufName[sizeof(aBufName)-1] = 0;
				
				int AuthLevel = pThis->m_aClients[i].m_Authed == CServer::AUTHED_ADMIN ? 2 :
										pThis->m_aClients[i].m_Authed == CServer::AUTHED_MOD ? 1 : 0;
				
				str_format(aBuf, sizeof(aBuf), "(#%02i) %s: [antispoof=%d] [login=%d] [level=%d] [ip=%s]",
					i,
					aBufName,
					pThis->m_NetServer.HasSecurityToken(i),
					pThis->IsClientLogged(i),
					AuthLevel,
					aAddrStr
				);
			}
			else
				str_format(aBuf, sizeof(aBuf), "id=%d addr=%s connecting", i, aAddrStr);
			pThis->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "Server", aBuf);
		}
	}
	
	return true;
/* INFECTION MODIFICATION END *****************************************/
}

bool CServer::ConShutdown(IConsole::IResult *pResult, void *pUser)
{
	((CServer *)pUser)->m_RunServer = 0;
	
	return true;
}

bool CServer::ConMapReload(IConsole::IResult *pResult, void *pUser)
{
	((CServer *)pUser)->m_MapReload = 1;
	
	return true;
}

bool CServer::ConLogout(IConsole::IResult *pResult, void *pUser)
{
	CServer *pServer = (CServer *)pUser;

	if(pServer->m_RconClientID >= 0 && pServer->m_RconClientID < MAX_CLIENTS &&
		pServer->m_aClients[pServer->m_RconClientID].m_State != CServer::CClient::STATE_EMPTY)
	{
		CMsgPacker Msg(NETMSG_RCON_AUTH_STATUS);
		Msg.AddInt(0);	//authed
		Msg.AddInt(0);	//cmdlist
		pServer->SendMsgEx(&Msg, MSGFLAG_VITAL, pServer->m_RconClientID, true);

		pServer->m_aClients[pServer->m_RconClientID].m_Authed = AUTHED_NO;
		pServer->m_aClients[pServer->m_RconClientID].m_AuthTries = 0;
		pServer->m_aClients[pServer->m_RconClientID].m_pRconCmdToSend = 0;
		pServer->SendRconLine(pServer->m_RconClientID, "Logout successful.");
	}
	
	return true;
}

bool CServer::ConchainSpecialInfoupdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData)
{
	pfnCallback(pResult, pCallbackUserData);
	if(pResult->NumArguments())
		((CServer *)pUserData)->UpdateServerInfo();
	
	return true;
}

bool CServer::ConchainMaxclientsperipUpdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData)
{
	pfnCallback(pResult, pCallbackUserData);
	if(pResult->NumArguments())
		((CServer *)pUserData)->m_NetServer.SetMaxClientsPerIP(pResult->GetInteger(0));
	
	return true;
}

bool CServer::ConchainModCommandUpdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData)
{
	if(pResult->NumArguments() == 2)
	{
		CServer *pThis = static_cast<CServer *>(pUserData);
		const IConsole::CCommandInfo *pInfo = pThis->Console()->GetCommandInfo(pResult->GetString(0), CFGFLAG_SERVER, false);
		int OldAccessLevel = 0;
		if(pInfo)
			OldAccessLevel = pInfo->GetAccessLevel();
		pfnCallback(pResult, pCallbackUserData);
		if(pInfo && OldAccessLevel != pInfo->GetAccessLevel())
		{
			for(int i = 0; i < MAX_CLIENTS; ++i)
			{
				if(pThis->m_aClients[i].m_State == CServer::CClient::STATE_EMPTY || pThis->m_aClients[i].m_Authed != CServer::AUTHED_MOD ||
					(pThis->m_aClients[i].m_pRconCmdToSend && str_comp(pResult->GetString(0), pThis->m_aClients[i].m_pRconCmdToSend->m_pName) >= 0))
					continue;

				if(OldAccessLevel == IConsole::ACCESS_LEVEL_ADMIN)
					pThis->SendRconCmdAdd(pInfo, i);
				else
					pThis->SendRconCmdRem(pInfo, i);
			}
		}
	}
	else
		pfnCallback(pResult, pCallbackUserData);
	
	return true;
}

bool CServer::ConchainConsoleOutputLevelUpdate(IConsole::IResult *pResult, void *pUserData, IConsole::FCommandCallback pfnCallback, void *pCallbackUserData)
{
	pfnCallback(pResult, pCallbackUserData);
	if(pResult->NumArguments() == 1)
	{
		CServer *pThis = static_cast<CServer *>(pUserData);
		pThis->Console()->SetPrintOutputLevel(pThis->m_PrintCBIndex, pResult->GetInteger(0));
	}
	
	return true;
}

bool CServer::ConAddSqlServer(IConsole::IResult *pResult, void *pUserData)
{
	CServer *pSelf = (CServer *)pUserData;

	if (pResult->NumArguments() != 7 && pResult->NumArguments() != 8)
		return false;

	bool ReadOnly;
	if (str_comp_nocase(pResult->GetString(0), "w") == 0)
		ReadOnly = false;
	else if (str_comp_nocase(pResult->GetString(0), "r") == 0)
		ReadOnly = true;
	else
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", "choose either 'r' for SqlReadServer or 'w' for SqlWriteServer");
		return true;
	}

	bool SetUpDb = pResult->NumArguments() == 8 ? pResult->GetInteger(7) : false;

	CSqlServer** apSqlServers = ReadOnly ? pSelf->m_apSqlReadServers : pSelf->m_apSqlWriteServers;

	for (int i = 0; i < MAX_SQLSERVERS; i++)
	{
		if (!apSqlServers[i])
		{
			apSqlServers[i] = new CSqlServer(pResult->GetString(1), pResult->GetString(2), pResult->GetString(3), pResult->GetString(4), pResult->GetString(5), pResult->GetInteger(6), ReadOnly, SetUpDb);

			char aBuf[512];
			str_format(aBuf, sizeof(aBuf), "Added new Sql%sServer: %d: DB: '%s' Prefix: '%s' User: '%s' IP: '%s' Port: %d", ReadOnly ? "Read" : "Write", i, apSqlServers[i]->GetDatabase(), apSqlServers[i]->GetPrefix(), apSqlServers[i]->GetUser(), apSqlServers[i]->GetIP(), apSqlServers[i]->GetPort());
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", aBuf);
			return true;
		}
	}
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", "failed to add new sqlserver: limit of sqlservers reached");

	return true;
}

bool CServer::ConDumpSqlServers(IConsole::IResult *pResult, void *pUserData)
{
	CServer *pSelf = (CServer *)pUserData;

	bool ReadOnly;
	if (str_comp_nocase(pResult->GetString(0), "w") == 0)
		ReadOnly = false;
	else if (str_comp_nocase(pResult->GetString(0), "r") == 0)
		ReadOnly = true;
	else
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", "choose either 'r' for SqlReadServer or 'w' for SqlWriteServer");
		return true;
	}

	CSqlServer** apSqlServers = ReadOnly ? pSelf->m_apSqlReadServers : pSelf->m_apSqlWriteServers;

	for (int i = 0; i < MAX_SQLSERVERS; i++)
		if (apSqlServers[i])
		{
			char aBuf[512];
			str_format(aBuf, sizeof(aBuf), "SQL-%s %d: DB: '%s' Prefix: '%s' User: '%s' Pass: '%s' IP: '%s' Port: %d", ReadOnly ? "Read" : "Write", i, apSqlServers[i]->GetDatabase(), apSqlServers[i]->GetPrefix(), apSqlServers[i]->GetUser(), apSqlServers[i]->GetPass(), apSqlServers[i]->GetIP(), apSqlServers[i]->GetPort());
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "server", aBuf);
		}
	
	return true;
}

void CServer::RegisterCommands()
{
	m_pConsole = Kernel()->RequestInterface<IConsole>();
	m_pGameServer = Kernel()->RequestInterface<IGameServer>();
	m_pMap = Kernel()->RequestInterface<IEngineMap>();
	m_pStorage = Kernel()->RequestInterface<IStorage>();

	// register console commands
	Console()->Register("kick", "s<username or uid> ?r<reason>", CFGFLAG_SERVER, ConKick, this, "Kick player with specified id for any reason");
	Console()->Register("status", "", CFGFLAG_SERVER, ConStatus, this, "List players");
	Console()->Register("option_status", "", CFGFLAG_SERVER, ConOptionStatus, this, "List player options");
	Console()->Register("shutdown", "", CFGFLAG_SERVER, ConShutdown, this, "Shut down");
	Console()->Register("logout", "", CFGFLAG_SERVER, ConLogout, this, "Logout of rcon");

	Console()->Register("reload", "", CFGFLAG_SERVER, ConMapReload, this, "Reload the map");

	Console()->Chain("sv_name", ConchainSpecialInfoupdate, this);
	Console()->Chain("password", ConchainSpecialInfoupdate, this);

	Console()->Chain("sv_max_clients_per_ip", ConchainMaxclientsperipUpdate, this);
	Console()->Chain("mod_command", ConchainModCommandUpdate, this);
	Console()->Chain("console_output_level", ConchainConsoleOutputLevelUpdate, this);
	
	Console()->Register("inf_add_sqlserver", "ssssssi?i", CFGFLAG_SERVER, ConAddSqlServer, this, "add a sqlserver");
	Console()->Register("inf_list_sqlservers", "s", CFGFLAG_SERVER, ConDumpSqlServers, this, "list all sqlservers readservers = r, writeservers = w");

	// register console commands in sub parts
	m_ServerBan.InitServerBan(Console(), Storage(), this);
	m_pGameServer->OnConsoleInit();
}

int CServer::SnapNewID()
{
	return m_IDPool.NewID();
}

void CServer::SnapFreeID(int ID)
{
	m_IDPool.FreeID(ID);
}


void *CServer::SnapNewItem(int Type, int ID, int Size)
{
	dbg_assert(Type >= 0 && Type <=0xffff, "incorrect type");
	dbg_assert(ID >= 0 && ID <=0xffff, "incorrect id");
	return ID < 0 ? 0 : m_SnapshotBuilder.NewItem(Type, ID, Size);
}

void CServer::SnapSetStaticsize(int ItemType, int Size)
{
	m_SnapshotDelta.SetStaticsize(ItemType, Size);
}

static CServer *CreateServer() { return new CServer(); }

int main(int argc, const char **argv) // ignore_convention
{
#if defined(CONF_FAMILY_WINDOWS)
	for(int i = 1; i < argc; i++) // ignore_convention
	{
		if(str_comp("-s", argv[i]) == 0 || str_comp("--silent", argv[i]) == 0) // ignore_convention
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			break;
		}
	}
#endif

	if(secure_random_init() != 0)
	{
		dbg_msg("secure", "could not initialize secure RNG");
		return -1;
	}

	CServer *pServer = CreateServer();
	IKernel *pKernel = IKernel::Create();

	// create the components
	IEngine *pEngine = CreateEngine("Teeworlds");
	IEngineMap *pEngineMap = CreateEngineMap();
	IGameServer *pGameServer = CreateGameServer();
	IConsole *pConsole = CreateConsole(CFGFLAG_SERVER|CFGFLAG_ECON);
	IEngineMasterServer *pEngineMasterServer = CreateEngineMasterServer();
	IStorage *pStorage = CreateStorage("Teeworlds", IStorage::STORAGETYPE_SERVER, argc, argv); // ignore_convention
	IConfig *pConfig = CreateConfig();
	
	pServer->m_pLocalization = new CLocalization(pStorage);
	pServer->m_pLocalization->InitConfig(0, NULL);
	if(!pServer->m_pLocalization->Init())
	{
		dbg_msg("localization", "could not initialize localization");
		return -1;
	}
	
	pServer->InitRegister(&pServer->m_NetServer, pEngineMasterServer, pConsole);

	{
		bool RegisterFail = false;

		RegisterFail = RegisterFail || !pKernel->RegisterInterface(pServer); // register as both
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(pEngine);
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(static_cast<IEngineMap*>(pEngineMap)); // register as both
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(static_cast<IMap*>(pEngineMap));
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(pGameServer);
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(pConsole);
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(pStorage);
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(pConfig);
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(static_cast<IEngineMasterServer*>(pEngineMasterServer)); // register as both
		RegisterFail = RegisterFail || !pKernel->RegisterInterface(static_cast<IMasterServer*>(pEngineMasterServer));

		if(RegisterFail)
			return -1;
	}
	
	pEngine->Init();
	pConfig->Init();
	pEngineMasterServer->Init();
	pEngineMasterServer->Load();

	// register all console commands
	pServer->RegisterCommands();

	// execute autoexec file
	pConsole->ExecuteFile("autoexec.cfg");

	// parse the command line arguments
	if(argc > 1) // ignore_convention
		pConsole->ParseArguments(argc-1, &argv[1]); // ignore_convention

	// restore empty config strings to their defaults
	pConfig->RestoreStrings();

	pEngine->InitLogfile();

	// run the server
	dbg_msg("server", "starting...");
	pServer->Run();
	
	delete pServer->m_pLocalization;
	
	// free
	delete pServer;
	delete pKernel;
	delete pEngineMap;
	delete pGameServer;
	delete pConsole;
	delete pEngineMasterServer;
	delete pStorage;
	delete pConfig;
	return 0;
}

int CServer::GetClan(int Type, int ClanID)
{
	int Returns = 0;
	if(Type == DMONEY) Returns = m_stClan[ClanID].f_money;
	else if(Type == DEXP) Returns = m_stClan[ClanID].f_exp;
	else if(Type == DLEVEL) Returns = m_stClan[ClanID].f_level;
	else if(Type == DCOUNTUCLAN) Returns =  m_stClan[ClanID].f_countnum;
	else if(Type == DMAXCOUNTUCLAN) Returns =  m_stClan[ClanID].f_maxnum;
	else if(Type == DKILL) Returns =  m_stClan[ClanID].f_kd;
	else if(Type == DADDEXP) Returns =  m_stClan[ClanID].f_upgrade[0];
	else if(Type == DADDMONEY) Returns =  m_stClan[ClanID].f_upgrade[1];
	else if(Type == DCHAIRHOUSE) Returns =  m_stClan[ClanID].f_chairupgr;

	return Returns;
}

const char *CServer::LeaderName(int ClanID)
{
	if(ClanID > 0) return m_stClan[ClanID].f_creator;
	else
		return "";
}

const char *CServer::GetClanName(int ClanID)
{
	if(ClanID > 0) return m_stClan[ClanID].f_name;
	else
		return "";
}

void CServer::ResetBotInfo(int ClientID, int BotType, int BotSubType)
{
	if(BotType == BOT_L1MONSTER)
	{
		if(!BotSubType) str_copy(m_aClients[ClientID].m_aName , "Pig", MAX_NAME_LENGTH);
		else if(BotSubType == 1) str_copy(m_aClients[ClientID].m_aName , "Zombie", MAX_NAME_LENGTH);
	}
	else if(BotType == BOT_L2MONSTER)
	{
		if(!BotSubType) str_copy(m_aClients[ClientID].m_aName , "Kwah", MAX_NAME_LENGTH);
		else if(BotSubType == 1) str_copy(m_aClients[ClientID].m_aName , "Skelet", MAX_NAME_LENGTH);
	}
	else if(BotType == BOT_L3MONSTER)
	{
		if(!BotSubType) str_copy(m_aClients[ClientID].m_aName , "Boom", MAX_NAME_LENGTH);
		else if(BotSubType == 1) str_copy(m_aClients[ClientID].m_aName , "Nimfie", MAX_NAME_LENGTH);
	}
	else if(BotType == BOT_NPC)
	{
		if(!BotSubType) str_copy(m_aClients[ClientID].m_aName , "Guard", MAX_NAME_LENGTH);
		else if(BotSubType == 1) str_copy(m_aClients[ClientID].m_aName , "Fighter", MAX_NAME_LENGTH);
	}
	else if(BotType == BOT_BOSSSLIME)
	{
		if(!BotSubType) str_copy(m_aClients[ClientID].m_aName , "Slime", MAX_NAME_LENGTH);
		else if(BotSubType == 1) str_copy(m_aClients[ClientID].m_aName , "Vampir", MAX_NAME_LENGTH);
	}
	else if(BotType == BOT_FARMER)
	{
		str_copy(m_aClients[ClientID].m_aName , "Nesquik", MAX_NAME_LENGTH);
	}
	else if(BotType == BOT_NPCW)
	{
		const char* Name = "Nope";
		if(BotSubType == 0)
		{
			if(!g_Config.m_SvCityStart)	Name = "NPC:John";
			else if(g_Config.m_SvCityStart == 1) Name = "NPC:Grem";
		}
		else if(BotSubType == 1)
		{
			if(!g_Config.m_SvCityStart) Name = "NPC:Lusi";
			else if(g_Config.m_SvCityStart == 1) Name = "NPC:Afra";
		}
		else
		{
			if(!g_Config.m_SvCityStart) Name = "NPC:Miki";
			else if(g_Config.m_SvCityStart == 1) Name = "NPC:Saki";
		}	
		str_copy(m_aClients[ClientID].m_aName , Name, MAX_NAME_LENGTH);
	}
	else
		str_copy(m_aClients[ClientID].m_aName , "Keke", MAX_NAME_LENGTH);
}

void CServer::InitClientBot(int ClientID)
{
	if (ClientID < MAX_NOBOT || ClientID > MAX_CLIENTS)
		return;
		
	m_aClients[ClientID].m_State = CServer::CClient::STATE_INGAME;
}

int CServer::GetClientAntiPing(int ClientID)
{
	return m_aClients[ClientID].m_AntiPing;
}

void CServer::SetClientAntiPing(int ClientID, int Value)
{
	m_aClients[ClientID].m_AntiPing = Value;
}

const char* CServer::GetClientLanguage(int ClientID)
{
	return m_aClients[ClientID].m_aLanguage;
}

void CServer::SetClientLanguage(int ClientID, const char* pLanguage)
{
	str_copy(m_aClients[ClientID].m_aLanguage, pLanguage, sizeof(m_aClients[ClientID].m_aLanguage));
}
	
int CServer::GetFireDelay(int ClientID, int WID)
{
	return m_InfFireDelay[ClientID][WID];
}

void CServer::SetFireDelay(int ClientID, int WID, int Time)
{
	m_InfFireDelay[ClientID][WID] = Time;
}

int CServer::GetAmmoRegenTime(int ClientID, int WID)
{
	return m_InfAmmoRegenTime[ClientID][WID];
}

void CServer::SetAmmoRegenTime(int ClientID, int WID, int Time)
{
	m_InfAmmoRegenTime[ClientID][WID] = Time;
}

int CServer::GetMaxAmmo(int ClientID, int WID)
{
	return m_InfMaxAmmo[ClientID][WID];
}

void CServer::SetMaxAmmo(int ClientID, int WID, int n)
{
	m_InfMaxAmmo[ClientID][WID] = n;
}

int CServer::GetSeccurity(int ClientID)
{
	return m_aClients[ClientID].m_Seccurity;
}

void CServer::SetSeccurity(int ClientID, int n)
{
	m_aClients[ClientID].m_Seccurity = n;
	UpdateStats(ClientID, 0);
}

bool CServer::IsClientLogged(int ClientID)
{
	return m_aClients[ClientID].m_UserID >= 0;
}

int CServer::GetUserID(int ClientID)
{
	return m_aClients[ClientID].m_UserID;
}

int CServer::GetClanID(int ClientID)
{
	return m_aClients[ClientID].m_ClanID;
}

void CServer::AddGameServerCmd(CGameServerCmd* pCmd)
{
	lock_wait(m_GameServerCmdLock);
	m_lGameServerCmds.add(pCmd);
	lock_unlock(m_GameServerCmdLock);
}

class CGameServerCmd_SendChatMOTD : public CServer::CGameServerCmd
{
private:
	int m_ClientID;
	char m_aText[512];
	
public:
	CGameServerCmd_SendChatMOTD(int ClientID, const char* pText)
	{
		m_ClientID = ClientID;
		str_copy(m_aText, pText, sizeof(m_aText));
	}

	virtual void Execute(IGameServer* pGameServer)
	{
		pGameServer->SendMOTD(m_ClientID, m_aText);
	}
};

class CGameServerCmd_SendChatTarget : public CServer::CGameServerCmd
{
private:
	int m_ClientID;
	char m_aText[128];
	
public:
	CGameServerCmd_SendChatTarget(int ClientID, const char* pText)
	{
		m_ClientID = ClientID;
		str_copy(m_aText, pText, sizeof(m_aText));
	}

	virtual void Execute(IGameServer* pGameServer)
	{
		pGameServer->SendChatTarget(m_ClientID, m_aText);
	}
};

class CGameServerCmd_SendChatTarget_Language : public CServer::CGameServerCmd
{
private:
	int m_ClientID;
	int m_ChatCategory;
	char m_aText[128];
	
public:
	CGameServerCmd_SendChatTarget_Language(int ClientID, int ChatCategory, const char* pText)
	{
		m_ClientID = ClientID;
		m_ChatCategory = ChatCategory;
		str_copy(m_aText, pText, sizeof(m_aText));
	}

	virtual void Execute(IGameServer* pGameServer)
	{
		pGameServer->SendChatTarget_Localization(m_ClientID, m_ChatCategory, m_aText, NULL);
	}
};

class CGameServerCmd_UseItem : public CServer::CGameServerCmd
{
private:
	int m_ClientID;
	int m_ItemID;
	int m_Count;
	int m_Type;
	
public:
	CGameServerCmd_UseItem(int ClientID, int ItemID, int Count, int Type)
	{
		m_ClientID = ClientID;
		m_ItemID = ItemID;
		m_Count = Count;
		m_Type = Type;
	}
	
	virtual void Execute(IGameServer* pGameServer)
	{
		pGameServer->UseItem(m_ClientID, m_ItemID, m_Count, m_Type);
	}
};

class CGameServerCmd_AddLocalizeVote_Language : public CServer::CGameServerCmd
{
private:
	int m_ClientID;
	char m_aType[64];
	char m_aText[128];
	
public:
	CGameServerCmd_AddLocalizeVote_Language(int ClientID, const char* pType, const char* pText)
	{
		m_ClientID = ClientID;
		str_copy(m_aText, pText, sizeof(m_aText));
		str_copy(m_aType, pType, sizeof(m_aType));
	}

	virtual void Execute(IGameServer* pGameServer)
	{
		pGameServer->AddVote_Localization(m_ClientID, m_aType, m_aText);
	}
};

int CServer::GetStat(int ClientID, int Type)
{
	switch(Type)
	{
		case DLEVEL: return m_aClients[ClientID].m_Level; break;
		case DEXP: return m_aClients[ClientID].m_Exp; break;
		case DMONEY: return m_aClients[ClientID].m_Money; break;
		case DGOLD: return m_aClients[ClientID].m_Gold; break;
		case DDONATE: return m_aClients[ClientID].m_Donate; break;
		case DQUEST: return m_aClients[ClientID].m_Quest; break;
		case DSECC: return m_aClients[ClientID].m_Seccurity; break;
		case DREL: return m_aClients[ClientID].m_Rel; break;
		case DJAIL: return m_aClients[ClientID].m_Jail; break;
		case DCLASS: return m_aClients[ClientID].m_Class; break;
		case DKILL: return m_aClients[ClientID].m_Kill; break;
		case DWINAREA: return m_aClients[ClientID].m_WinArea; break;
		case DCLANADDED: return m_aClients[ClientID].m_ClanAdded; break;
	}
	return 0;
}

void CServer::UpdateStat(int ClientID, int Type, int Size)
{
	switch(Type)
	{
		case DLEVEL: m_aClients[ClientID].m_Level = Size; break;
		case DEXP: m_aClients[ClientID].m_Exp = Size; break;
		case DMONEY: m_aClients[ClientID].m_Money = Size; break;
		case DGOLD: m_aClients[ClientID].m_Gold = Size; break;
		case DDONATE: m_aClients[ClientID].m_Donate = Size; break;	
		case DQUEST: m_aClients[ClientID].m_Quest = Size; break;
		case DSECC: m_aClients[ClientID].m_Seccurity = Size; break;
		case DREL: m_aClients[ClientID].m_Rel = Size; break;
		case DJAIL: m_aClients[ClientID].m_Jail = Size; break;
		case DCLASS: m_aClients[ClientID].m_Class = Size; break;
		case DKILL: m_aClients[ClientID].m_Kill = Size; break;
		case DWINAREA: m_aClients[ClientID].m_WinArea = Size; break;
		case DCLANADDED: m_aClients[ClientID].m_ClanAdded = Size; break;
	}
}

int CServer::GetUpgrade(int ClientID, int Type)
{
	switch(Type)
	{
		case SUPGRADE: return m_aClients[ClientID].Upgrade; break;
		case SKILLPOINT: return m_aClients[ClientID].SkillPoint; break;
		case ASPEED: return m_aClients[ClientID].Speed; break;
		case BDAMAGE: return m_aClients[ClientID].Damage; break;
		case HHEALTH: return m_aClients[ClientID].Health; break;
		case HPREGEN: return m_aClients[ClientID].HPRegen; break;
		case AMMOREGEN: return m_aClients[ClientID].AmmoRegen; break;
		case AMMO: return m_aClients[ClientID].Ammo; break;
		case SPRAY: return m_aClients[ClientID].Spray; break;
		case MANA: return m_aClients[ClientID].Mana; break;
		case UHAMMERRANGE: return m_aClients[ClientID].m_HammerRange; break;
		case PASIVE2: return m_aClients[ClientID].m_Pasive2; break;
	}
	return 0;
}

void CServer::UpdateUpgrade(int ClientID, int Type, int Size)
{
	switch(Type)
	{
		case SUPGRADE: m_aClients[ClientID].Upgrade = Size; break;
		case SKILLPOINT: m_aClients[ClientID].SkillPoint = Size; break;
		case ASPEED: m_aClients[ClientID].Speed = Size; break;
		case BDAMAGE: m_aClients[ClientID].Damage = Size; break;
		case HHEALTH: m_aClients[ClientID].Health = Size; break;
		case HPREGEN: m_aClients[ClientID].HPRegen = Size; break;
		case AMMOREGEN: m_aClients[ClientID].AmmoRegen = Size; break;
		case AMMO: m_aClients[ClientID].Ammo = Size; break;
		case SPRAY: m_aClients[ClientID].Spray = Size; break;
		case MANA: m_aClients[ClientID].Mana = Size; break;
		case UHAMMERRANGE: m_aClients[ClientID].m_HammerRange = Size; break;
		case PASIVE2: m_aClients[ClientID].m_Pasive2 = Size; break;
	}
}

///////////////////////////////////////////////////////////// ИНВЕНТАРЬ
//#####################################################################
const char *CServer::GetItemName(int ClientID, int ItemID, bool ntlang)
{
	if(ItemID < 0 || ItemID >= 200)
		return "(nope)";
	else
	{
		if(ntlang) return Localization()->Localize(GetClientLanguage(ClientID), _(m_stInv[ClientID][ItemID].i_name));
		else return m_stInv[ClientID][ItemID].i_name;
	}
}
int CServer::GetItemCountType(int ClientID, int Type)
{
	return m_aClients[ClientID].m_ItemCount[Type];
}
int CServer::GetItemEnchant(int ClientID, int ItemID)
{
	return m_stInv[ClientID][ItemID].i_enchant;
}
void CServer::SetItemEnchant(int ClientID, int ItemID, int Price)
{
	m_stInv[ClientID][ItemID].i_enchant = Price;
	UpdateItemSettings(ItemID, ClientID);
}
const char *CServer::GetItemDesc(int ClientID, int ItemID)
{
	if(ItemID < 0 || ItemID >= 200)
		return "(invalid)";
	else return m_stInv[ClientID][ItemID].i_desc;
}
int CServer::GetItemCount(int ClientID, int ItemID)
{
	if(ClientID >= MAX_CLIENTS)
		return 0;
		
	return m_stInv[ClientID][ItemID].i_count;
}

int CServer::GetBonusEnchant(int ClientID, int ItemID, int Armor)
{
	if(Armor == 15)
	{
		if(ItemID == LEATHERBODY)
			return 100*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == COOPERBODY)
			return 150*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == IRONBODY)
			return 200*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == GOLDBODY)
			return 250*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == DIAMONDBODY)
			return 300*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == DRAGONBODY)
			return 500*(m_stInv[ClientID][ItemID].i_enchant+1);
		return 0;
	}
	else if(Armor == 16)
	{
		if(ItemID == LEATHERFEET)
			return 50*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == COOPERFEET)
			return 100*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == IRONFEET)
			return 150*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == GOLDFEET)
			return 200*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == DIAMONDFEET)
			return 250*(m_stInv[ClientID][ItemID].i_enchant+1);
		else if(ItemID == DRAGONFEET)
			return 400*(m_stInv[ClientID][ItemID].i_enchant+1);
		else return 0;		
	}
	else if(Armor == 17)
	{
		if(ItemID == STCLASIC)
			return 1*(m_stInv[ClientID][ItemID].i_enchant+1);
		else return 0;			
	}
	else return 0;
}

void CServer::SetItemPrice(int ClientID, int ItemID, int Level, int Price)
{
	if(ItemID < 0 || ItemID >= 200)
		return;
	
	if(IsClientLogged(ClientID) && GetItemSettings(ClientID, PIGPIG) && Price > 10)
		Price -= (int)(Price/100)*5;

	m_stInv[ClientID][ItemID].i_nlevel = Level;
	m_stInv[ClientID][ItemID].i_nprice = Price;
	return;
}

int CServer::GetItemPrice(int ClientID, int ItemID, int Type)
{
	if(ItemID < 0 || ItemID >= 200)
		return 0;

	if(!m_stInv[ClientID][ItemID].i_nlevel)
		m_stInv[ClientID][ItemID].i_nlevel = 1;

	if(!Type) return m_stInv[ClientID][ItemID].i_nlevel;
	else return m_stInv[ClientID][ItemID].i_nprice;
}

///////////////// ################################ ##########
///////////////// ########################### MATERIALS #####
///////////////// ################################ ##########

int CServer::GetMaterials(int ID)
{
	return m_Materials[ID];
}

void CServer::SetMaterials(int ID, int Count)
{
	m_Materials[ID] = Count;
	SaveMaterials(ID);
}

// Инициализация Матерьялов
class CSqlJob_Server_InitMaterialID : public CSqlJob
{
private:
	CServer* m_pServer;
public:
	CSqlJob_Server_InitMaterialID(CServer* pServer)
	{
		m_pServer = pServer;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		try
		{
			pSqlServer->executeSqlQuery("SELECT * FROM tw_Materials");
			while(pSqlServer->GetResults()->next())
			{
				int IDMAT = (int)pSqlServer->GetResults()->getInt("ID");
				int Count = (int)pSqlServer->GetResults()->getInt("Materials");
				m_pServer->m_Materials[IDMAT-1] = Count;
			}
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::InitMaterialID()
{
	CSqlJob* pJob = new CSqlJob_Server_InitMaterialID(this);
	pJob->Start();
}

// Выдача предмета
class CSqlJob_Server_SaveMaterial : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ID;
public:
	CSqlJob_Server_SaveMaterial(CServer* pServer, int ID)
	{
		m_pServer = pServer;
		m_ID = ID;
	}
	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"UPDATE tw_Materials "
				"SET Materials = '%d' "
				"WHERE ID = '%d';"
				, m_pServer->m_Materials[m_ID], m_ID+1);
			pSqlServer->executeSql(aBuf);
		}
		catch (sql::SQLException &e)
		{
			dbg_msg("sql", "Can't get top10 (MySQL Error: %s)", e.what());
			return false;
		}
		return true;
	}
};
void CServer::SaveMaterials(int ID)
{
	CSqlJob* pJob = new CSqlJob_Server_SaveMaterial(this, ID);
	pJob->Start();
}

///////////////// ################################ ##########
///////////////// ########################### MAIL ##########
///////////////// ################################ ##########

void CServer::SetRewardMail(int ClientID, int ID, int ItemID, int ItemNum)
{
	if(!IsClientLogged(ClientID))
		return;

	m_aClients[ClientID].m_ItemReward[ID] = ItemID;
	m_aClients[ClientID].m_ItemNumReward[ID] = ItemNum;
}
int CServer::GetRewardMail(int ClientID, int ID, int Type)
{
	if(!IsClientLogged(ClientID))
		return -1;
		
	return Type ? m_aClients[ClientID].m_ItemNumReward[ID] : m_aClients[ClientID].m_ItemReward[ID];
}

int CServer::GetMailRewardDell(int ClientID, int ID)
{
	return m_aClients[ClientID].m_MailID[ID];
}

// Инициализация Почты по ID
class CSqlJob_Server_InitMailID : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;

public:
	CSqlJob_Server_InitMailID(CServer* pServer, int ClientID)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		try
		{
			int iscope = 0;
			str_format(aBuf, sizeof(aBuf), 
				"SELECT * FROM tw_Mail "
				"WHERE IDOwner = '%d' LIMIT 20;"
				, m_pServer->m_aClients[m_ClientID].m_UserID);
			pSqlServer->executeSqlQuery(aBuf);
			while(pSqlServer->GetResults()->next())
			{
				int IDMAIL = (int)pSqlServer->GetResults()->getInt("ID");
				int ItemID = (int)pSqlServer->GetResults()->getInt("ItemID");
				int ItemNum = (int)pSqlServer->GetResults()->getInt("ItemCount");
				m_pServer->m_aClients[m_ClientID].m_MailID[iscope] = IDMAIL;
				m_pServer->SetRewardMail(m_ClientID, iscope, ItemID, ItemNum);

				char Text[64];
				str_format(Text, sizeof(Text), "%s", pSqlServer->GetResults()->getString("TextMail").c_str());
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, "null", _(Text));
				m_pServer->AddGameServerCmd(pCmd);

				char aProtocol[16];
				str_format(aProtocol, sizeof(aProtocol), "reward%d", iscope);
				str_format(Text, sizeof(Text), "Get Reward %s : %d and Delete Mail", m_pServer->GetItemName(m_ClientID, ItemID, false), ItemNum);	
				pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, aProtocol, _(Text));
				m_pServer->AddGameServerCmd(pCmd);

				pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, "null", _("------------"));
				m_pServer->AddGameServerCmd(pCmd);

				iscope++;
			}
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::InitMailID(int ClientID)
{
	CSqlJob* pJob = new CSqlJob_Server_InitMailID(this, ClientID);
	pJob->Start();
}

// Удаление почты
class CSqlJob_Server_RemMail : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_IDMail;
	
public:
	CSqlJob_Server_RemMail(CServer* pServer, int IDMail)
	{
		m_pServer = pServer;
		m_IDMail = IDMail; 
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];			
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"DELETE FROM tw_Mail " 
				"WHERE ID = '%d' LIMIT 1;"
				, m_IDMail);	
			pSqlServer->executeSql(aBuf);
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::RemMail(int IDMail)
{
	CSqlJob* pJob = new CSqlJob_Server_RemMail(this, IDMail);
	pJob->Start();
}

// Выдача предмета
class CSqlJob_Server_SendMail : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_AuthedID;
	int m_ItemID;
	int m_ItemNum;
	CSqlString<64> m_sType;
	
public:
	CSqlJob_Server_SendMail(CServer* pServer, int AuthedID, const char* pText, int ItemID, int ItemNum)
	{
		m_pServer = pServer;
		m_AuthedID = AuthedID;
		m_ItemID = ItemID;
		m_ItemNum = ItemNum;
		m_sType = CSqlString<64>(pText);
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"INSERT INTO tw_Mail "
				"(IDOwner, TextMail, ItemID, ItemCount) "
				"VALUES ('%d', '%s', '%d', '%d');"
				, m_AuthedID, m_sType.ClrStr(), m_ItemID, m_ItemNum);	
			pSqlServer->executeSql(aBuf);
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::SendMail(int AuthedID, const char* pText, int ItemID, int ItemNum)
{
	CSqlJob* pJob = new CSqlJob_Server_SendMail(this, AuthedID, pText, ItemID, ItemNum);
	pJob->Start();
}

// Инициализация Инвентаря по ID
class CSqlJob_Server_InitInvID : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID, m_ItemID;

public:
	CSqlJob_Server_InitInvID(CServer* pServer, int ClientID, int ItemID)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_ItemID = ItemID;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		try
		{
			if(m_ItemID == -1 && m_ClientID == -1)
			{
				pSqlServer->executeSqlQuery("SELECT * FROM tw_uItemList;");
				while(pSqlServer->GetResults()->next())
				{
					for(int i = 0; i < MAX_NOBOT; ++i)
					{
						int ItemID = (int)pSqlServer->GetResults()->getInt("il_id");
						m_pServer->m_stInv[i][ItemID].i_id = ItemID;
						m_pServer->m_stInv[i][ItemID].i_type = (int)pSqlServer->GetResults()->getInt("item_type");
						str_copy(m_pServer->m_stInv[i][ItemID].i_name, pSqlServer->GetResults()->getString("item_name").c_str(), sizeof(m_pServer->m_stInv[i][ItemID].i_name));
						str_copy(m_pServer->m_stInv[i][ItemID].i_desc, pSqlServer->GetResults()->getString("item_desc").c_str(), sizeof(m_pServer->m_stInv[i][ItemID].i_desc));
					}
				}
			}
			else
			{
				char aBuf[128];
				str_format(aBuf, sizeof(aBuf), "SELECT item_count FROM tw_uItems WHERE item_owner = '%d' AND il_id = '%d';", m_pServer->m_aClients[m_ClientID].m_UserID, m_ItemID);
				pSqlServer->executeSqlQuery(aBuf);

				if(pSqlServer->GetResults()->next())
					m_pServer->m_stInv[m_ClientID][m_ItemID].i_count = (int)pSqlServer->GetResults()->getInt("item_count");
			}
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::InitInvID(int ClientID, int ItemID)
{
	CSqlJob* pJob = new CSqlJob_Server_InitInvID(this, ClientID, ItemID);
	pJob->Start();
}

// Выдача предмета
class CSqlJob_Server_GetItem : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ItemID;
	int m_ClientID;
	int m_Count;
	int m_Settings;
	int m_Enchant;

public:
	CSqlJob_Server_GetItem(CServer* pServer, int ItemID, int ClientID, int Count, int Settings, int Enchant)
	{
		m_pServer = pServer;
		m_ItemID = ItemID;
		m_ClientID = ClientID;
		m_Count = Count;
		m_Settings = Settings;
		m_Enchant = Enchant;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		try
		{
			if(m_pServer->m_stInv[m_ClientID][m_ItemID].i_count > 0)
			{
				str_format(aBuf, sizeof(aBuf), 
					"UPDATE tw_uItems "
					"SET item_count = item_count + '%d', item_settings = item_settings + '%d' "
					"WHERE item_owner = '%d' AND il_id = '%d';"
					, m_Count, m_Settings, m_pServer->m_aClients[m_ClientID].m_UserID, m_ItemID);
				pSqlServer->executeSql(aBuf);
				
				m_pServer->m_stInv[m_ClientID][m_ItemID].i_count += m_Count;
				m_pServer->m_stInv[m_ClientID][m_ItemID].i_settings += m_Settings;
				return true;
			}
			str_format(aBuf, sizeof(aBuf), 
				"INSERT INTO tw_uItems "
				"(il_id, item_owner, item_count, item_type, item_settings, item_enchant) "
				"VALUES ('%d', '%d', '%d', '%d', '%d', '%d');"
				, m_ItemID, m_pServer->m_aClients[m_ClientID].m_UserID, m_Count, m_pServer->m_stInv[m_ClientID][m_ItemID].i_type, m_Settings, m_Enchant);	
			pSqlServer->executeSql(aBuf);

			m_pServer->m_stInv[m_ClientID][m_ItemID].i_settings = m_Settings;
			m_pServer->m_stInv[m_ClientID][m_ItemID].i_count = m_Count;
			m_pServer->m_aClients[m_ClientID].m_ItemCount[m_pServer->m_stInv[m_ClientID][m_ItemID].i_type]++;
		}
		catch (sql::SQLException &e)
		{
			// Scheme - 
			// if(item) -> Add item if succeses -> Got resoult delete item drop -> 
			//
			//
			//
			//
			return false;
		}
		return true;
	}
};
void CServer::GetItem(int ItemID, int ClientID, int Count, int Settings, int Enchant)
{
	CSqlJob* pJob = new CSqlJob_Server_GetItem(this, ItemID, ClientID, Count, Settings, Enchant);
	pJob->Start();
}
void CServer::GiveItem(int ClientID, int ItemID, int Count, int Settings, int Enchant)
{
	GetItem(ItemID, ClientID, Count, Settings, Enchant);
}

// Забратие предмета
class CSqlJob_Server_RemItems : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ItemID;
	int m_ClientID;
	int m_Count;
	int m_Type;
	
public:
	CSqlJob_Server_RemItems(CServer* pServer, int ItemID, int ClientID, int Count, int Type)
	{
		m_pServer = pServer;
		m_ItemID = ItemID; 
		m_ClientID = ClientID;
		m_Count = Count;
		m_Type = Type;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];			
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"SELECT item_count FROM tw_uItems "
				"WHERE item_owner = '%d' AND il_id = '%d';",
				m_pServer->m_aClients[m_ClientID].m_UserID, m_ItemID);
			pSqlServer->executeSqlQuery(aBuf);

			if(pSqlServer->GetResults()->next())
			{
				int Count = (int)pSqlServer->GetResults()->getInt("item_count");
				m_pServer->m_stInv[m_ClientID][m_ItemID].i_count = Count;
			
				if(Count > m_Count)
				{
					str_format(aBuf, sizeof(aBuf), 
						"UPDATE tw_uItems "
						"SET item_count = item_count - '%d' "
						"WHERE item_owner = '%d' AND il_id = '%d';"
						, m_Count, m_pServer->m_aClients[m_ClientID].m_UserID, m_ItemID);
					pSqlServer->executeSql(aBuf);	
					m_pServer->m_stInv[m_ClientID][m_ItemID].i_count -= m_Count;
				}
				else
				{
					str_format(aBuf, sizeof(aBuf), 
						"DELETE FROM tw_uItems " 
						"WHERE item_owner = '%d' AND il_id = '%d';"
						, m_pServer->m_aClients[m_ClientID].m_UserID, m_ItemID);	
					pSqlServer->executeSql(aBuf);			
					m_pServer->m_stInv[m_ClientID][m_ItemID].i_count = 0;
					m_pServer->m_stInv[m_ClientID][m_ItemID].i_settings = 0;
					m_pServer->m_aClients[m_ClientID].m_ItemCount[m_pServer->m_stInv[m_ClientID][m_ItemID].i_type]--;
				}

				if(m_Count > Count)
					m_Count = Count;

				CServer::CGameServerCmd* pCmd = new CGameServerCmd_UseItem(m_ClientID, m_ItemID, m_Count, m_Type);
				m_pServer->AddGameServerCmd(pCmd);
			}
			return true;
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::RemItems(int ItemID, int ClientID, int Count, int Type)
{
	if(m_aClients[ClientID].m_UserID < 0 && m_pGameServer)
		return;

	CSqlJob* pJob = new CSqlJob_Server_RemItems(this, ItemID, ClientID, Count, Type);
	pJob->Start();
}

void CServer::RemItem(int ClientID, int ItemID, int Count, int Type)
{
	RemItems(ItemID, ClientID, Count, Type);
}

int CServer::GetItemType(int ClientID, int ItemID)
{
	return m_stInv[ClientID][ItemID].i_type;
}
int CServer::GetItemSettings(int ClientID, int ItemID)
{
	if(!GetItemCount(ClientID, ItemID))
		return 0;

	if(m_stInv[ClientID][ItemID].i_settings)
		return m_stInv[ClientID][ItemID].i_settings;
	else
		return 0;
}

int CServer::GetItemEnquip(int ClientID, int ItemType)
{
	int back = -1;
	for(int i = 0; i < MAX_ITEM; i++)
	{
		if(ItemType == m_stInv[ClientID][i].i_type)
		{
			if(m_stInv[ClientID][i].i_settings == true)
			{
				back = i;
				break;
			}
		}
	}
	return back;
}

void CServer::SetItemSettings(int ClientID, int ItemID, int ItemType)
{
	if(!GetItemCount(ClientID, ItemID))
		return;
		
	if(ItemType > 10 && m_stInv[ClientID][ItemID].i_type == ItemType)
	{
		for(int i = 0; i < MAX_ITEM; i++)
		{
			if(i != ItemID && m_stInv[ClientID][i].i_type == ItemType)
			{
				m_stInv[ClientID][i].i_settings = false;
				UpdateItemSettings(i, ClientID);
			}
		}
	}

	m_stInv[ClientID][ItemID].i_settings ^= true;
	UpdateItemSettings(ItemID, ClientID);
}
void CServer::SetItemSettingsCount(int ClientID, int ItemID, int Count)
{
	if(!GetItemCount(ClientID, ItemID))
		return;
		
	m_stInv[ClientID][ItemID].i_settings = Count;
	UpdateItemSettings(ItemID, ClientID);
}

// Обновление настроек предмета
class CSqlJob_Server_UpdateItemSettings : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ItemID;
	int m_ClientID;
	
public:
	CSqlJob_Server_UpdateItemSettings(CServer* pServer, int ItemID, int ClientID)
	{
		m_pServer = pServer;
		m_ItemID = ItemID;
		m_ClientID = ClientID;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[128];
		try
		{
			if(m_pServer->GetItemCount(m_ClientID, m_ItemID) > 0)
			{
				str_format(aBuf, sizeof(aBuf), 
					"UPDATE tw_uItems "
					"SET item_settings = '%d', item_enchant = '%d' "
					"WHERE item_owner = '%d' AND il_id = '%d';"
					, m_pServer->m_stInv[m_ClientID][m_ItemID].i_settings, m_pServer->m_stInv[m_ClientID][m_ItemID].i_enchant, m_pServer->m_aClients[m_ClientID].m_UserID, m_ItemID);
				pSqlServer->executeSql(aBuf);
			}
			return true;
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::UpdateItemSettings(int ItemID, int ClientID)
{
	CSqlJob* pJob = new CSqlJob_Server_UpdateItemSettings(this, ItemID, ClientID);
	pJob->Start();
}

// Лист предметов
class CSqlJob_Server_ListInventory : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	int m_Type;
	bool bGetCount;

public:
	CSqlJob_Server_ListInventory(CServer* pServer, int ClientID, int Type, bool GetCount)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_Type = Type;
		bGetCount = GetCount;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		if(m_pServer->m_aClients[m_ClientID].m_LogInstance != GetInstance())
			return true;
	
		try
		{
			if(bGetCount)
			{
				str_format(aBuf, sizeof(aBuf), 
					"SELECT il_id, item_type FROM tw_uItems "
					"WHERE item_owner = '%d' AND item_type != '10, 12, 15, 16, 17';",
					m_pServer->m_aClients[m_ClientID].m_UserID, m_Type);
				pSqlServer->executeSqlQuery(aBuf);

				for(int i = 0; i < 7; i++)
					m_pServer->m_aClients[m_ClientID].m_ItemCount[i] = 0;
 
				while(pSqlServer->GetResults()->next())
				{
					int ItemType = (int)pSqlServer->GetResults()->getInt("item_type");
					m_pServer->m_aClients[m_ClientID].m_ItemCount[ItemType]++;
				}
				return true;			
			}

			str_format(aBuf, sizeof(aBuf), 
				"SELECT il_id, item_count FROM tw_uItems "
				"WHERE item_owner = '%d' AND item_type = '%d';",
				m_pServer->m_aClients[m_ClientID].m_UserID, m_Type);
			pSqlServer->executeSqlQuery(aBuf);
			
			bool found = false;
			while(pSqlServer->GetResults()->next())
			{
				// Псевдо Инициализация по сути нихуя нагрузки нет все норм
				int ItemID = (int)pSqlServer->GetResults()->getInt("il_id");
				int ItemCount = (int)pSqlServer->GetResults()->getInt("item_count");
				m_pServer->m_stInv[m_ClientID][ItemID].i_count = ItemCount;

				char iName[64], iUsed[8];
				if(m_Type == 15 || m_Type == 16 || m_Type == 17) 
				{
					str_format(iUsed, sizeof(iUsed), "it%d", ItemID);
					str_format(iName, sizeof(iName), "➳ Lvl%d %s +%d", 
						m_pServer->GetItemPrice(m_ClientID, ItemID, 0), m_pServer->GetItemName(m_ClientID, ItemID), m_pServer->GetItemEnchant(m_ClientID, ItemID), ItemCount);

					CServer::CGameServerCmd* pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, iUsed, _(iName));
					m_pServer->AddGameServerCmd(pCmd);

					const char* Data = m_pServer->GetItemSettings(m_ClientID, ItemID) ? "☑" : "☐";
					str_format(iUsed, sizeof(iUsed), "set%d", ItemID);
					if(m_Type == 17)
					{
						str_format(iName, sizeof(iName), "➳ %s %s (Damage +%d)", 
							Data,  m_pServer->GetItemName(m_ClientID, ItemID), m_pServer->GetBonusEnchant(m_ClientID, ItemID, m_Type));				
					}
					else
					{
						str_format(iName, sizeof(iName), "➳ %s %s (Health +%d Armor +%d)", 
							Data,  m_pServer->GetItemName(m_ClientID, ItemID), m_pServer->GetBonusEnchant(m_ClientID, ItemID, m_Type), 
							m_pServer->GetBonusEnchant(m_ClientID, ItemID, m_Type));				
					}

					pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, iUsed, _(iName));
					m_pServer->AddGameServerCmd(pCmd);
				}
				else
				{
					str_format(iUsed, sizeof(iUsed), "it%d", ItemID);
					str_format(iName, sizeof(iName), "➳ Lvl%d %s : X%d", 
						m_pServer->GetItemPrice(m_ClientID, ItemID, 0), m_pServer->GetItemName(m_ClientID, ItemID), ItemCount);
				
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, iUsed, _(iName));
					m_pServer->AddGameServerCmd(pCmd);
				}
				found = true;
			}
			if(!found)
			{
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, "null", _("This menu empty"));
				m_pServer->AddGameServerCmd(pCmd);
			}
		}
		catch (sql::SQLException &e)
		{		
			return false;
		}
		return true;
	}
	
	virtual void CleanInstanceRef()
	{
		m_pServer->m_aClients[m_ClientID].m_LogInstance = -1;
	}
};
void CServer::ListInventory(int ClientID, int Type, int GetCount)
{
	if(m_aClients[ClientID].m_LogInstance >= 0 && (m_aClients[ClientID].m_UserID < 0 && m_pGameServer))
		return;

	CSqlJob* pJob = new CSqlJob_Server_ListInventory(this, ClientID, Type, GetCount);
	m_aClients[ClientID].m_LogInstance = pJob->GetInstance();
	pJob->Start();
}

///////////////////////////////////////////////////////////// СОЗДАНИЕ КЛАНА
bool CServer::GetLeader(int ClientID, int ClanID)
{
	if(m_aClients[ClientID].m_ClanID < 1)
		return false;
	
	if(str_comp_nocase(m_stClan[ClanID].f_creator, ClientName(ClientID)) == 0)
		return true;
	else 
		return false;
}

// Инициализация кланов
class CSqlJob_Server_InitClan : public CSqlJob
{
private:
	CServer* m_pServer;
	
public:
	CSqlJob_Server_InitClan(CServer* pServer)
	{
		m_pServer = pServer;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		try
		{
			pSqlServer->executeSqlQuery("SELECT * FROM tw_Clans;");

			int Num = 0;
			while(pSqlServer->GetResults()->next())
			{
				int ClanID = (int)pSqlServer->GetResults()->getInt("ClanID");
				m_pServer->m_stClan[ClanID].f_id = ClanID;
				m_pServer->m_stClan[ClanID].f_level = (int)pSqlServer->GetResults()->getInt("Level");
				m_pServer->m_stClan[ClanID].f_exp = (int)pSqlServer->GetResults()->getInt("Exp");
				m_pServer->m_stClan[ClanID].f_money = (int)pSqlServer->GetResults()->getInt("Money");
				m_pServer->m_stClan[ClanID].f_maxnum = (int)pSqlServer->GetResults()->getInt("MaxNum");
				m_pServer->m_stClan[ClanID].f_kd = (int)pSqlServer->GetResults()->getInt("Relevance");
				m_pServer->m_stClan[ClanID].f_upgrade[0] = (int)pSqlServer->GetResults()->getInt("ExpAdd");
				m_pServer->m_stClan[ClanID].f_upgrade[1] = (int)pSqlServer->GetResults()->getInt("MoneyAdd");
				m_pServer->m_stClan[ClanID].f_spawninhouse = (int)pSqlServer->GetResults()->getInt("SpawnHouse");
				m_pServer->m_stClan[ClanID].f_chairupgr = (int)pSqlServer->GetResults()->getInt("ChairHouse");
				str_copy(m_pServer->m_stClan[ClanID].f_name, pSqlServer->GetResults()->getString("Clanname").c_str(), sizeof(m_pServer->m_stClan[ClanID].f_name));
				str_copy(m_pServer->m_stClan[ClanID].f_creator, pSqlServer->GetResults()->getString("LeaderName").c_str(), sizeof(m_pServer->m_stClan[ClanID].f_creator));
				
				
				m_pServer->UpdClanCount(ClanID);
				Num++;
			}
			
			dbg_msg("infclass", "############################################");
			dbg_msg("infclass", "################ LOADS CLAN %d", Num);
			dbg_msg("infclass", "############################################");
		}
		catch (sql::SQLException &e)
		{
			dbg_msg("infclass", "Fail in initializ clans");
			return false;
		}
		
		return true;
	}
};
void CServer::InitClan()
{
	CSqlJob* pJob = new CSqlJob_Server_InitClan(this);
	pJob->Start();
}

// Инициализация клана по ID
class CSqlJob_Server_InitClanID : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClanID;
	int m_Type;
	bool m_Need;
	int m_Price;
	bool m_Save;
	CSqlString<64> m_sType;

public:
	CSqlJob_Server_InitClanID(CServer* pServer, int ClanID, bool Need, const char* SubType, int Price, bool Save)
	{
		m_pServer = pServer;
		m_ClanID = ClanID;
		m_Need = Need;
		m_sType = CSqlString<64>(SubType);
		m_Price = Price;
		m_Save = Save;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		try
		{
			if(str_comp(m_sType.ClrStr(), "Leader") == 0)
			{
				m_sType = CSqlString<64>(m_pServer->m_stClan[m_ClanID].f_creator);
				str_format(aBuf, sizeof(aBuf), 
					"UPDATE tw_Clans SET LeaderName = '%s' WHERE ClanID = '%d';",
					m_sType.ClrStr(), m_ClanID);
				pSqlServer->executeSqlQuery(aBuf);
				return true;
			}

			if(str_comp(m_sType.ClrStr(), "Init") == 0)
			{
				str_format(aBuf, sizeof(aBuf), "SELECT * FROM tw_Clans WHERE ClanID = '%d';", m_ClanID);
				pSqlServer->executeSqlQuery(aBuf);
				if(pSqlServer->GetResults()->next())
				{
					m_pServer->m_stClan[m_ClanID].f_chairupgr = (int)pSqlServer->GetResults()->getInt("ChairHouse"); 
					m_pServer->m_stClan[m_ClanID].f_spawninhouse = (int)pSqlServer->GetResults()->getInt("SpawnHouse");
					m_pServer->m_stClan[m_ClanID].f_upgrade[1] = (int)pSqlServer->GetResults()->getInt("MoneyAdd");
					m_pServer->m_stClan[m_ClanID].f_upgrade[0] = (int)pSqlServer->GetResults()->getInt("ExpAdd");
					m_pServer->m_stClan[m_ClanID].f_kd = (int)pSqlServer->GetResults()->getInt("Relevance");
					m_pServer->m_stClan[m_ClanID].f_maxnum = (int)pSqlServer->GetResults()->getInt("MaxNum");
					m_pServer->m_stClan[m_ClanID].f_level = (int)pSqlServer->GetResults()->getInt("Level");
					m_pServer->m_stClan[m_ClanID].f_exp = (int)pSqlServer->GetResults()->getInt("Exp");
					m_pServer->m_stClan[m_ClanID].f_money = (int)pSqlServer->GetResults()->getInt("Money");
					str_copy(m_pServer->m_stClan[m_ClanID].f_creator, pSqlServer->GetResults()->getString("LeaderName").c_str(), sizeof(m_pServer->m_stClan[m_ClanID].f_creator));
				}
				return true;
			}

			str_format(aBuf, sizeof(aBuf), "SELECT %s FROM tw_Clans WHERE ClanID = '%d';", m_sType.ClrStr(), m_ClanID);
			pSqlServer->executeSqlQuery(aBuf);
			if(pSqlServer->GetResults()->next())
			{
				int VarGot = -1;
				if(str_comp(m_sType.ClrStr(), "Money") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_money = (int)pSqlServer->GetResults()->getInt("Money");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_money += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_money -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_money;
				}
				else if(str_comp(m_sType.ClrStr(), "Exp") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_exp = (int)pSqlServer->GetResults()->getInt("Exp");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_exp += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_exp -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_exp;
				}
				else if(str_comp(m_sType.ClrStr(), "Level") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_level = (int)pSqlServer->GetResults()->getInt("Level");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_level += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_level -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_level;
				}
				else if(str_comp(m_sType.ClrStr(), "MaxNum") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_maxnum = (int)pSqlServer->GetResults()->getInt("MaxNum");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_maxnum += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_maxnum -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_maxnum;
				}
				else if(str_comp(m_sType.ClrStr(), "Relevance") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_kd = (int)pSqlServer->GetResults()->getInt("Relevance");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_kd += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_kd -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_kd;
				}
				else if(str_comp(m_sType.ClrStr(), "ExpAdd") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_upgrade[0] = (int)pSqlServer->GetResults()->getInt("ExpAdd");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_upgrade[0] += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_upgrade[0] -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_upgrade[0];
				}
				else if(str_comp(m_sType.ClrStr(), "MoneyAdd") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_upgrade[1] = (int)pSqlServer->GetResults()->getInt("MoneyAdd");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_upgrade[1] += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_upgrade[1] -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_upgrade[1];
				}		
				else if(str_comp(m_sType.ClrStr(), "SpawnHouse") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_spawninhouse = (int)pSqlServer->GetResults()->getInt("SpawnHouse");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_spawninhouse += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_spawninhouse -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_spawninhouse;
				}	
				else if(str_comp(m_sType.ClrStr(), "ChairHouse") == 0)
				{
					m_pServer->m_stClan[m_ClanID].f_chairupgr = (int)pSqlServer->GetResults()->getInt("ChairHouse");
					if(m_Need == PLUS) m_pServer->m_stClan[m_ClanID].f_chairupgr += m_Price;
					else m_pServer->m_stClan[m_ClanID].f_chairupgr -= m_Price;

					VarGot = m_pServer->m_stClan[m_ClanID].f_chairupgr;
				}	
				if(m_Save && VarGot > -1)
				{
					str_format(aBuf, sizeof(aBuf), 
						"UPDATE tw_Clans SET %s = '%d' WHERE ClanID = '%d';",
						m_sType.ClrStr(), VarGot, m_ClanID);
					pSqlServer->executeSqlQuery(aBuf);
				}
				m_pServer->UpdClanCount(m_ClanID);
			}	
			return true;
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::InitClanID(int ClanID, bool Need, const char* SubType, int Price, bool Save)
{
	if(!ClanID)
		return;

	CSqlJob* pJob = new CSqlJob_Server_InitClanID(this, ClanID, Need, SubType, Price, Save);
	pJob->Start();
}

//#####################################################################
// Создание клана
class CSqlJob_Server_Newclan : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	CSqlString<64> m_sName;
	CSqlString<64> m_sNick;
	
public:
	CSqlJob_Server_Newclan(CServer* pServer, int ClientID, const char* pName)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_sName = CSqlString<64>(pName);
		m_sNick = CSqlString<64>(m_pServer->ClientName(m_ClientID));
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[512];
		// Проверка регистра
		if(m_pServer->m_aClients[m_ClientID].m_LogInstance != GetInstance())
			return true;
	
		try
		{
			str_format(aBuf, sizeof(aBuf), "SELECT ClanID FROM %s_Clans WHERE Clanname COLLATE UTF8_GENERAL_CI = '%s';", pSqlServer->GetPrefix(), m_sName.ClrStr());
			pSqlServer->executeSqlQuery(aBuf);

			if(pSqlServer->GetResults()->next())
			{
				dbg_msg("infclass", "Clan already taken");
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("This clan name already used."));
				m_pServer->AddGameServerCmd(pCmd);
				
				return true;
			}
			else
			{
				str_format(aBuf, sizeof(aBuf), 
					"INSERT INTO %s_Clans (Clanname, LeaderName, LeaderID) VALUES ('%s', '%s', '%d');"
					, pSqlServer->GetPrefix(), m_sName.ClrStr(), m_sNick.ClrStr(), m_pServer->m_aClients[m_ClientID].m_UserID);
				pSqlServer->executeSql(aBuf);

				str_format(aBuf, sizeof(aBuf), 
					"SELECT * FROM %s_Clans WHERE Clanname COLLATE UTF8_GENERAL_CI = '%s';"
					, pSqlServer->GetPrefix(), m_sName.ClrStr());
				pSqlServer->executeSqlQuery(aBuf);
				if(pSqlServer->GetResults()->next())
				{
					int ClanID = (int)pSqlServer->GetResults()->getInt("ClanID");
					m_pServer->m_stClan[ClanID].f_id = ClanID;
					m_pServer->m_stClan[ClanID].f_level = (int)pSqlServer->GetResults()->getInt("Level");
					m_pServer->m_stClan[ClanID].f_money = (int)pSqlServer->GetResults()->getInt("Money");
					m_pServer->m_stClan[ClanID].f_maxnum = (int)pSqlServer->GetResults()->getInt("MaxNum");
					m_pServer->m_stClan[ClanID].f_countnum = 1;
					
					str_copy(m_pServer->m_stClan[ClanID].f_name, pSqlServer->GetResults()->getString("Clanname").c_str(), sizeof(m_pServer->m_stClan[ClanID].f_name));
					str_copy(m_pServer->m_stClan[ClanID].f_creator, pSqlServer->GetResults()->getString("LeaderName").c_str(), sizeof(m_pServer->m_stClan[ClanID].f_creator));

					m_pServer->m_aClients[m_ClientID].m_ClanID = ClanID;
					str_copy(m_pServer->m_aClients[m_ClientID].m_Clan, pSqlServer->GetResults()->getString("Clanname").c_str(), sizeof(m_pServer->m_aClients[m_ClientID].m_Clan));

					str_format(aBuf, sizeof(aBuf), "UPDATE tw_Users SET ClanID = '%d', ClanAdded = '%d' WHERE UserId = '%d';"
						, ClanID, NULL, m_pServer->m_aClients[m_ClientID].m_UserID);
					pSqlServer->executeSqlQuery(aBuf);	
		
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("Clan created and initized Data."));
					m_pServer->AddGameServerCmd(pCmd);

					m_pServer->RemItem(m_ClientID, CLANTICKET, 1, -1);
					return true;
				}
			}
		}
		catch (sql::SQLException &e)
		{
			return false;
		}		
		return true;
	}
	
	virtual void CleanInstanceRef()
	{
		m_pServer->m_aClients[m_ClientID].m_LogInstance = -1;
	}
};
void CServer::NewClan(int ClientID, const char* pName)
{
	if(m_aClients[ClientID].m_LogInstance >= 0 || (m_aClients[ClientID].m_UserID < 0 && m_pGameServer))
		return;

	CSqlJob* pJob = new CSqlJob_Server_Newclan(this, ClientID, pName);
	m_aClients[ClientID].m_LogInstance = pJob->GetInstance();
	pJob->Start();
}

// Лист игроков
class CSqlJob_Server_Listclan : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	int m_ClanID;
	
public:
	CSqlJob_Server_Listclan(CServer* pServer, int ClientID, int ClanID)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_ClanID = ClanID;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[128];
		if(m_pServer->m_aClients[m_ClientID].m_LogInstance != GetInstance())
			return true;
	
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"SELECT UserID, ClanID, Level, Nick, ClanAdded FROM tw_Users "
				"WHERE ClanID = '%d' ORDER BY Level DESC;", m_ClanID);
			pSqlServer->executeSqlQuery(aBuf);
			
			int Num = 0;
			char aReform[MAX_NAME_LENGTH], aBufW[64], aBufCs[12];
			while(pSqlServer->GetResults()->next())
			{
				str_copy(aReform, pSqlServer->GetResults()->getString("Nick").c_str(), sizeof(aReform));
				
				int UserID = (int)pSqlServer->GetResults()->getInt("UserID");
				int Level = (int)pSqlServer->GetResults()->getInt("Level");
				int ClanAdded = (int)pSqlServer->GetResults()->getInt("ClanAdded");
				
				str_format(aBufCs, sizeof(aBufCs), "cs%d", Num);
				str_format(aBufW, sizeof(aBufW), "▹ Level %d:%s(ID:%d)", Level, aReform, UserID);
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, aBufCs, _(aBufW));
				m_pServer->AddGameServerCmd(pCmd);

				str_format(aBufW, sizeof(aBufW), "Added %d money", ClanAdded);
				pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, aBufCs, _(aBufW));
				m_pServer->AddGameServerCmd(pCmd);
				
				str_copy(m_pServer->m_aClients[m_ClientID].m_SelectPlayer[Num], aReform, sizeof(m_pServer->m_aClients[m_ClientID].m_SelectPlayer[Num]));
				Num++;
			}
			m_pServer->m_stClan[m_ClanID].f_countnum = Num;
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("Error clan list say administrator."));
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't check clanname list (MySQL Error: %s)", e.what());
			
			return false;
		}
		return true;
	}
	
	virtual void CleanInstanceRef()
	{
		m_pServer->m_aClients[m_ClientID].m_LogInstance = -1;
	}
};
void CServer::ListClan(int ClientID, int ClanID)
{
	if(m_aClients[ClientID].m_LogInstance >= 0 || (m_aClients[ClientID].m_UserID < 0 && m_pGameServer))
		return;

	CSqlJob* pJob = new CSqlJob_Server_Listclan(this, ClientID, ClanID);
	m_aClients[ClientID].m_LogInstance = pJob->GetInstance();
	pJob->Start();
}

// Обновление коинтов
class CSqlJob_Server_UpClanCount : public CSqlJob
{
private:
	CServer* m_pServer;
	CSqlString<64> m_sName;
	int m_ClanID;
	
public:
	CSqlJob_Server_UpClanCount(CServer* pServer, int ClanID)
	{
		m_pServer = pServer;
		m_ClanID = ClanID;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[128];
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"SELECT ClanID FROM tw_Users "
				"WHERE ClanID = '%d';"
				, m_ClanID);
			pSqlServer->executeSqlQuery(aBuf);
			
			int Num = 0;
			while(pSqlServer->GetResults()->next())
				Num++;

			m_pServer->m_stClan[m_ClanID].f_countnum = Num;
		}
		catch (sql::SQLException &e)
		{
			dbg_msg("sql", "Error", e.what());
			return false;
		}
		return true;
	}
};
void CServer::UpdClanCount(int ClanID)
{
	CSqlJob* pJob = new CSqlJob_Server_UpClanCount(this, ClanID);
	pJob->Start();
}

// Присоединение к клану
void CServer::EnterClan(int ClientID, int ClanID)
{
	m_stClan[ClanID].f_countnum++;
	m_aClients[ClientID].m_ClanAdded = 0;
	m_aClients[ClientID].m_ClanID = ClanID;
	UpdateStats(ClientID, 3);
}

// Смена лидера
void CServer::ChangeLeader(int ClanID, const char* pName)
{
	str_copy(m_stClan[ClanID].f_creator, pName, sizeof(m_stClan[ClanID].f_creator));
	InitClanID(ClanID, PLUS, "Leader", 0, false);
}

// Выход с клана
class CSqlJob_Server_ExitClanOff : public CSqlJob
{
private:
	CServer* m_pServer;
	CSqlString<64> m_sName;
	int m_ClientID;
public:
	CSqlJob_Server_ExitClanOff(CServer* pServer, int ClientID, const char* pName)
	{
		m_pServer = pServer;
		m_sName = CSqlString<64>(pName);
		m_ClientID = ClientID;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[512];		
		try
		{
			int ClanID = 0;
			str_format(aBuf, sizeof(aBuf), 
				"UPDATE tw_Users SET ClanID = %d, ClanAdded = %d WHERE Nick = '%s';", ClanID, ClanID, m_sName.ClrStr());
			pSqlServer->executeSqlQuery(aBuf);	
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::ExitClanOff(int ClientID, const char* pName)
{
	for(int i = 0; i < MAX_NOBOT; ++i)
	{
		if(ClientIngame(i) && m_aClients[i].m_UserID)
			if(str_comp_nocase(pName, ClientName(i)) == 0)
				m_aClients[i].m_ClanID = 0;
	}

	CSqlJob* pJob = new CSqlJob_Server_ExitClanOff(this, ClientID, pName);
	pJob->Start();
}

/////////////////////////////////////////// ИНИЦИАЛИЗАЦИЯ СТАНДАРТНЫХ ПАРАМЕТРОВ
class CSqlJob_Server_InitClient : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	
public:
	CSqlJob_Server_InitClient(CServer* pServer, int ClientID)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[512];
		// Сразу инициализировать
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"SELECT * FROM tw_Users "
				"WHERE UserId = %d;"
				, m_pServer->m_aClients[m_ClientID].m_UserID);
			pSqlServer->executeSqlQuery(aBuf);

			if(pSqlServer->GetResults()->next())
			{
				m_pServer->m_aClients[m_ClientID].m_Level = (int)pSqlServer->GetResults()->getInt("Level");
				m_pServer->m_aClients[m_ClientID].m_Exp = (int)pSqlServer->GetResults()->getInt("Exp");
				m_pServer->m_aClients[m_ClientID].m_Money = (int)pSqlServer->GetResults()->getInt("Money");
				m_pServer->m_aClients[m_ClientID].m_Gold = (int)pSqlServer->GetResults()->getInt("Gold");
				m_pServer->m_aClients[m_ClientID].m_Donate = (int)pSqlServer->GetResults()->getInt("Donate");
				m_pServer->m_aClients[m_ClientID].m_Jail = (int)pSqlServer->GetResults()->getInt("Jail");
				m_pServer->m_aClients[m_ClientID].m_Rel = (int)pSqlServer->GetResults()->getInt("Rel");
				m_pServer->m_aClients[m_ClientID].m_Class = (int)pSqlServer->GetResults()->getInt("Class");
				m_pServer->m_aClients[m_ClientID].m_ClanID = (int)pSqlServer->GetResults()->getInt("ClanID");
				m_pServer->m_aClients[m_ClientID].m_Quest = (int)pSqlServer->GetResults()->getInt("Quest");
				m_pServer->m_aClients[m_ClientID].m_Kill = (int)pSqlServer->GetResults()->getInt("Killing");
				m_pServer->m_aClients[m_ClientID].m_WinArea = (int)pSqlServer->GetResults()->getInt("WinArea");
				m_pServer->m_aClients[m_ClientID].m_ClanAdded = m_pServer->m_aClients[m_ClientID].m_ClanID > 0 ? (int)pSqlServer->GetResults()->getInt("ClanAdded") : 0;
	
				str_copy(m_pServer->m_aClients[m_ClientID].m_aUsername, pSqlServer->GetResults()->getString("Nick").c_str(), sizeof(m_pServer->m_aClients[m_ClientID].m_aUsername));
				dbg_msg("infclass", "DT init %d ID acc", m_pServer->m_aClients[m_ClientID].m_UserID);	
			}
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("Error initilized clan say administrator."));
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't init new clan (MySQL Error: %s)", e.what());
			
			return false;
		}
		
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"SELECT il_id, item_count, item_settings, item_enchant FROM tw_uItems WHERE item_owner = %d;", m_pServer->m_aClients[m_ClientID].m_UserID);
			pSqlServer->executeSqlQuery(aBuf);

			while(pSqlServer->GetResults()->next())
			{
				int IDitem = (int)pSqlServer->GetResults()->getInt("il_id");
				int ItemSettings = (int)pSqlServer->GetResults()->getInt("item_settings");
				int ItemEnchant = (int)pSqlServer->GetResults()->getInt("item_enchant");
				m_pServer->m_stInv[m_ClientID][IDitem].i_count = (int)pSqlServer->GetResults()->getInt("item_count");
				m_pServer->m_stInv[m_ClientID][IDitem].i_settings = ItemSettings;			
				m_pServer->m_stInv[m_ClientID][IDitem].i_enchant = ItemEnchant;
			}					
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, "An error occured during the logging.");
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't init inventory (MySQL Error: %s)", e.what());
			
			return false;
		}

		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"SELECT * FROM tw_uClass WHERE UserID = %d;"
				, m_pServer->m_aClients[m_ClientID].m_UserID);
			pSqlServer->executeSqlQuery(aBuf);

			if(pSqlServer->GetResults()->next())
			{				
				m_pServer->m_aClients[m_ClientID].Upgrade = (int)pSqlServer->GetResults()->getInt("Upgrade");
				m_pServer->m_aClients[m_ClientID].SkillPoint = (int)pSqlServer->GetResults()->getInt("SkillPoint");
				m_pServer->m_aClients[m_ClientID].Damage = (int)pSqlServer->GetResults()->getInt("Damage");
				m_pServer->m_aClients[m_ClientID].Speed = (int)pSqlServer->GetResults()->getInt("Speed");
				m_pServer->m_aClients[m_ClientID].Health = (int)pSqlServer->GetResults()->getInt("Health");
				m_pServer->m_aClients[m_ClientID].HPRegen = (int)pSqlServer->GetResults()->getInt("HPRegen");
				m_pServer->m_aClients[m_ClientID].AmmoRegen = (int)pSqlServer->GetResults()->getInt("AmmoRegen");
				m_pServer->m_aClients[m_ClientID].Ammo = (int)pSqlServer->GetResults()->getInt("Ammo");
				m_pServer->m_aClients[m_ClientID].Spray = (int)pSqlServer->GetResults()->getInt("Spray");
				m_pServer->m_aClients[m_ClientID].Mana = (int)pSqlServer->GetResults()->getInt("Mana");
				m_pServer->m_aClients[m_ClientID].m_HammerRange = (int)pSqlServer->GetResults()->getInt("HammerRange");
				m_pServer->m_aClients[m_ClientID].m_Pasive2 = (int)pSqlServer->GetResults()->getInt("Pasive2");
				
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("You are now logged. Welcome"));
				m_pServer->AddGameServerCmd(pCmd);			
			}					
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, "An error occured during the logging.");
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't check username/password (MySQL Error: %s)", e.what());
			return false;
		}
		return true;
	}
};

void CServer::InitClientDB(int ClientID)
{
	if(m_aClients[ClientID].m_UserID < 0 && m_pGameServer)
		return;

	CSqlJob* pJob = new CSqlJob_Server_InitClient(this, ClientID);
	pJob->Start();
}

class CSqlJob_Server_UpdateStat	 : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	int m_Type;
	int m_UserID;
	
public:
	CSqlJob_Server_UpdateStat(CServer* pServer, int ClientID, int UserID, int Type)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_UserID = UserID;
		m_Type = Type;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[512];
		try
		{
			if(m_Type == 0)
			{
				str_format(aBuf, sizeof(aBuf), 
					"UPDATE %s_Users "
					"SET Level = '%d', "
					"Exp = '%d', "
					"Class = '%d', "
					"Money = '%d', "
					"Gold = '%d', "
					"Donate = '%d', "
					"Rel = '%d', "
					"Jail = '%d', "
					"Quest = '%d', "
					"Killing = '%d', "
					"WinArea = '%d', "
					"Seccurity = '%d', "
					"ClanAdded = '%d' "
					"WHERE UserId = '%d';"
					, pSqlServer->GetPrefix(), m_pServer->m_aClients[m_ClientID].m_Level, m_pServer->m_aClients[m_ClientID].m_Exp, 
						m_pServer->m_aClients[m_ClientID].m_Class, m_pServer->m_aClients[m_ClientID].m_Money, m_pServer->m_aClients[m_ClientID].m_Gold, m_pServer->m_aClients[m_ClientID].m_Donate, m_pServer->m_aClients[m_ClientID].m_Rel, 
						m_pServer->m_aClients[m_ClientID].m_Jail, m_pServer->m_aClients[m_ClientID].m_Quest, m_pServer->m_aClients[m_ClientID].m_Kill,  
						m_pServer->m_aClients[m_ClientID].m_WinArea, m_pServer->m_aClients[m_ClientID].m_Seccurity, m_pServer->m_aClients[m_ClientID].m_ClanAdded, 
						m_UserID);
				
				pSqlServer->executeSqlQuery(aBuf);
			}
			else if(m_Type == 1)
			{
				str_format(aBuf, sizeof(aBuf), 
					"UPDATE %s_uClass "
					"SET Upgrade = '%d', "
					"SkillPoint = '%d', "
					"Speed = '%d', "
					"Health = '%d', "
					"Damage = '%d', "
					"HPRegen = '%d', "
					"AmmoRegen = '%d', "
					"Ammo = '%d', "
					"Spray = '%d', "
					"Mana = '%d', "
					"HammerRange = '%d', "
					"Pasive2 = '%d' "
					"WHERE UserID = '%d';"
					, pSqlServer->GetPrefix(), m_pServer->m_aClients[m_ClientID].Upgrade, m_pServer->m_aClients[m_ClientID].SkillPoint, m_pServer->m_aClients[m_ClientID].Speed, m_pServer->m_aClients[m_ClientID].Health, m_pServer->m_aClients[m_ClientID].Damage,
					m_pServer->m_aClients[m_ClientID].HPRegen, m_pServer->m_aClients[m_ClientID].AmmoRegen, m_pServer->m_aClients[m_ClientID].Ammo, m_pServer->m_aClients[m_ClientID].Spray, m_pServer->m_aClients[m_ClientID].Mana, 
					m_pServer->m_aClients[m_ClientID].m_HammerRange, m_pServer->m_aClients[m_ClientID].m_Pasive2, m_UserID);
				
				pSqlServer->executeSqlQuery(aBuf);
			}
			else if(m_Type == 3)
			{
				int ClanID = m_pServer->m_aClients[m_ClientID].m_ClanID;
				str_format(aBuf, sizeof(aBuf), 
					"UPDATE tw_Users SET ClanID = '%d' WHERE UserId = '%d';"
					, ClanID, m_pServer->m_aClients[m_ClientID].m_UserID);
				pSqlServer->executeSqlQuery(aBuf);	
				
				if(ClanID > 0)
					m_pServer->UpdClanCount(ClanID);
			}
		}
		catch (sql::SQLException &e)
		{
			dbg_msg("sql", "Can't update (MySQL Error: %s)", e.what());
			return false;
		}
		return true;
	}
};

void CServer::UpdateStats(int ClientID, int Type)
{
	if(m_aClients[ClientID].m_Class < 0 || (m_aClients[ClientID].m_UserID < 0 && m_pGameServer))
		return; 

	CSqlJob* pJob = new CSqlJob_Server_UpdateStat(this, ClientID, m_aClients[ClientID].m_UserID, Type);
	pJob->Start();
}    

class CSqlJob_Server_Login : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	CSqlString<64> m_sName;
	CSqlString<64> m_sNick;
	CSqlString<64> m_sPasswordHash;
	
public:
	CSqlJob_Server_Login(CServer* pServer, int ClientID, const char* pName, const char* pPasswordHash)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_sName = CSqlString<64>(pName);
		m_sNick = CSqlString<64>(m_pServer->ClientName(m_ClientID));
		m_sPasswordHash = CSqlString<64>(pPasswordHash);
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		// Проверка регистра
		if(m_pServer->m_aClients[m_ClientID].m_LogInstance != GetInstance())
			return true;

		char aBuf[512];
		try
		{	
			if(m_pServer->m_aClients[m_ClientID].m_Seccurity)
				str_format(aBuf, sizeof(aBuf), "SELECT UserId, Username, Nick, PasswordHash FROM %s_Users "
					"WHERE Username = '%s' AND PasswordHash = '%s' AND Nick = '%s';", pSqlServer->GetPrefix(), m_sName.ClrStr(), m_sPasswordHash.ClrStr(), m_sNick.ClrStr());
			else
				str_format(aBuf, sizeof(aBuf), "SELECT * FROM %s_Users WHERE PasswordHash = '%s' AND Nick = '%s';"
					, pSqlServer->GetPrefix(), m_sPasswordHash.ClrStr(), m_sNick.ClrStr());	

			pSqlServer->executeSqlQuery(aBuf);
			
			if(pSqlServer->GetResults()->next())
			{
				for(int i = 0; i < MAX_CLIENTS; ++i)
				{
					if((int)pSqlServer->GetResults()->getInt("UserId") == m_pServer->m_aClients[i].m_UserID)
					{
						m_pServer->m_aClients[m_ClientID].m_UserID = -1;
						return false;
					}
				}
				m_pServer->m_aClients[m_ClientID].m_UserID = (int)pSqlServer->GetResults()->getInt("UserId");
				m_pServer->InitClientDB(m_ClientID);
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("Authed. In ESC press Play."));
				m_pServer->AddGameServerCmd(pCmd);	
			}
			else
			{
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("Wrong username/password or your nickname no this account."));
				m_pServer->AddGameServerCmd(pCmd);
			}
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, "An error occured during the logging.");
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't check username/password (MySQL Error: %s)", e.what());
			
			return false;
		}
		return true;
	}
	virtual void CleanInstanceRef()
	{
		m_pServer->m_aClients[m_ClientID].m_LogInstance = -1;
	}
};

void CServer::Login(int ClientID, const char* pUsername, const char* pPassword)
{	
	if(m_aClients[ClientID].m_LogInstance >= 0)
		return;

	char aHash[64]; //Result
	mem_zero(aHash, sizeof(aHash));
	Crypt(pPassword, (const unsigned char*) "d9", 1, 16, aHash);
	
	CSqlJob* pJob = new CSqlJob_Server_Login(this, ClientID, pUsername, aHash);
	m_aClients[ClientID].m_LogInstance = pJob->GetInstance();
	pJob->Start();
}

// Инициализация первого клиента
class CSqlJob_Server_FirstInit : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	CSqlString<64> m_sNick;
	
public:
	CSqlJob_Server_FirstInit(CServer* pServer, int ClientID)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_sNick = CSqlString<64>(m_pServer->ClientName(m_ClientID));
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[512];
		try
		{
			str_format(aBuf, sizeof(aBuf), 
				"SELECT Nick, Seccurity FROM tw_Users "
				"WHERE Nick = '%s';"
				, m_sNick.ClrStr());
			pSqlServer->executeSqlQuery(aBuf);
			
			if(pSqlServer->GetResults()->next())
				m_pServer->m_aClients[m_ClientID].m_Seccurity = (int)pSqlServer->GetResults()->getInt("Seccurity");
			else
			{
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(-1, CHATCATEGORY_DEFAULT, _("We have a new player."));
				m_pServer->AddGameServerCmd(pCmd);
			}
		}
		catch (sql::SQLException &e)
		{
			dbg_msg("sql", "Can't check newplayer seccurity (MySQL Error: %s)", e.what());
			return false;			
		}
		return true;
	}
};
void CServer::FirstInit(int ClientID)
{
	CSqlJob* pJob = new CSqlJob_Server_FirstInit(this, ClientID);
	pJob->Start();
}

// Регистрация аккаунта
class CSqlJob_Server_Register : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	CSqlString<64> m_sName;
	CSqlString<64> m_sNick;
	CSqlString<64> m_sPasswordHash;
	CSqlString<64> m_sEmail;
	
public:
	CSqlJob_Server_Register(CServer* pServer, int ClientID, const char* pName, const char* pPasswordHash, const char* pEmail)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_sName = CSqlString<64>(pName);
		m_sNick = CSqlString<64>(m_pServer->ClientName(m_ClientID));
		m_sPasswordHash = CSqlString<64>(pPasswordHash);
		if(pEmail)
			m_sEmail = CSqlString<64>(pEmail);
		else
			m_sEmail = CSqlString<64>("");
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		// Проверка регистра
		if(m_pServer->m_aClients[m_ClientID].m_LogInstance != GetInstance())
			return true;
		
		char aBuf[512];
		char aAddrStr[64];
		net_addr_str(m_pServer->m_NetServer.ClientAddr(m_ClientID), aAddrStr, sizeof(aAddrStr), false);

		try
		{
			//Проверяем имя или ник в базе
			str_format(aBuf, sizeof(aBuf), 
				"SELECT UserId FROM tw_Users WHERE Username = '%s' OR Nick = '%s';"
				, m_sName.ClrStr(), m_sNick.ClrStr());
			pSqlServer->executeSqlQuery(aBuf);

			if(pSqlServer->GetResults()->next())
			{
				dbg_msg("infclass", "User already taken");
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("This username or nickname is already in database"));
				m_pServer->AddGameServerCmd(pCmd);
				return true;
			}
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("An error occured during the creation of your account."));
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't check username existance (MySQL Error: %s)", e.what());
			
			return false;
		}
		
		//Создаем сам аккаунт
		try
		{	
			str_format(aBuf, sizeof(aBuf), 
				"INSERT INTO tw_Users "
				"(Username, Nick, PasswordHash, Email, RegisterDate, RegisterIp) "
				"VALUES ('%s', '%s', '%s', '%s', UTC_TIMESTAMP(), '%s');"
				, m_sName.ClrStr(), m_sNick.ClrStr(), m_sPasswordHash.ClrStr(), m_sEmail.ClrStr(), aAddrStr);
			pSqlServer->executeSql(aBuf);
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("An error occured during the creation of your account."));
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't create new user (MySQL Error: %s)", e.what());
			
			return false;
		}
		
		// Получаем инфу пользователя
		try
		{	
			str_format(aBuf, sizeof(aBuf), 
				"SELECT UserId FROM tw_Users WHERE Username = '%s' AND PasswordHash = '%s';"
				, m_sName.ClrStr(), m_sPasswordHash.ClrStr());
			pSqlServer->executeSqlQuery(aBuf);

			if(pSqlServer->GetResults()->next())
			{
				int UsedID = (int)pSqlServer->GetResults()->getInt("UserId");
				str_format(aBuf, sizeof(aBuf), 
					"INSERT INTO tw_uClass (UserID, Username) VALUES ('%d', '%s');"
					, UsedID, m_sName.ClrStr());
				pSqlServer->executeSql(aBuf);

				str_copy(m_pServer->m_aClients[m_ClientID].m_Clan, "NOPE", sizeof(m_pServer->m_aClients[m_ClientID].m_Clan));
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("Your account created. Use /login <pass>"));
				m_pServer->AddGameServerCmd(pCmd);
				return true;
			}
			else
			{
				CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("An error occured during the creation of your account."));
				m_pServer->AddGameServerCmd(pCmd);
				return false;
			}
		}
		catch (sql::SQLException &e)
		{
			CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(m_ClientID, CHATCATEGORY_DEFAULT, _("An error occured during the creation of your account."));
			m_pServer->AddGameServerCmd(pCmd);
			dbg_msg("sql", "Can't get the ID of the new user (MySQL Error: %s)", e.what());
			
			return false;
		}
		return true;
	}
	virtual void CleanInstanceRef()
	{
		m_pServer->m_aClients[m_ClientID].m_LogInstance = -1;
	}
};

inline void CServer::Register(int ClientID, const char* pUsername, const char* pPassword, const char* pEmail)
{
	if(m_aClients[ClientID].m_LogInstance >= 0)
		return;
	
	char aHash[64];
	Crypt(pPassword, (const unsigned char*) "d9", 1, 16, aHash);
	
	CSqlJob* pJob = new CSqlJob_Server_Register(this, ClientID, pUsername, aHash, pEmail);
	m_aClients[ClientID].m_LogInstance = pJob->GetInstance();
	pJob->Start();
}

// Топ 10
class CSqlJob_Server_ShowTop10 : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	int m_Type;
	CSqlString<64> m_sType;
	
public:
	CSqlJob_Server_ShowTop10(CServer* pServer, int ClientID, const char* Type, int TypeGet)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_sType = CSqlString<64>(Type);
		m_Type = TypeGet;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		
		try
		{
			int SortTop = m_Type == 2 ? 5 : 10;
			str_format(aBuf, sizeof(aBuf), 
				"SELECT %s, Nick FROM %s_Users "
				"ORDER BY %s DESC LIMIT %d;",
				m_sType.ClrStr(), pSqlServer->GetPrefix(), m_sType.ClrStr(), SortTop);
			pSqlServer->executeSqlQuery(aBuf);

			int Rank = 0;
			while(pSqlServer->GetResults()->next())
			{
				Rank++;

				int Level = (int)pSqlServer->GetResults()->getInt(m_sType.ClrStr());
				dynamic_string Buffer;
				m_pServer->Localization()->Format(Buffer, m_pServer->GetClientLanguage(m_ClientID), _("Rank {int:Rank} - {str:Name} {int:Count}"), 
					"Rank", &Rank, "Name", pSqlServer->GetResults()->getString("Nick").c_str(), "Count", &Level);
			
				if(m_Type == 1)
				{
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, "null", _(Buffer.buffer()));
					m_pServer->AddGameServerCmd(pCmd);
				}
				else
				{	
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(-1, CHATCATEGORY_DEFAULT, _(Buffer.buffer()));
					m_pServer->AddGameServerCmd(pCmd);	
				}
				Buffer.clear();
			}
		}
		catch (sql::SQLException &e)
		{
			dbg_msg("sql", "Can't get top10 (MySQL Error: %s)", e.what());
			return false;
		}
		
		return true;
	}
};

void CServer::ShowTop10(int ClientID, const char* Type, int TypeGet)
{
	CSqlJob* pJob = new CSqlJob_Server_ShowTop10(this, ClientID, Type, TypeGet);
	pJob->Start();
}

// Топ 10 Кланов
class CSqlJob_Server_ShowTop10Clans : public CSqlJob
{
private:
	CServer* m_pServer;
	int m_ClientID;
	int m_Type;
	CSqlString<64> m_sType;
	
public:
	CSqlJob_Server_ShowTop10Clans(CServer* pServer, int ClientID, const char* Type, int TypeGet)
	{
		m_pServer = pServer;
		m_ClientID = ClientID;
		m_sType = CSqlString<64>(Type);
		m_Type = TypeGet;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		char aBuf[256];
		try
		{
			int SortTop = m_Type == 2 ? 5 : 10;		
			str_format(aBuf, sizeof(aBuf), 
				"SELECT %s, Clanname, LeaderName FROM tw_Clans "
				"ORDER BY %s DESC LIMIT %d;",
				m_sType.ClrStr(), m_sType.ClrStr(), SortTop);
			pSqlServer->executeSqlQuery(aBuf);

			int Rank = 0;
			while(pSqlServer->GetResults()->next())
			{
				Rank++;
				int Level = (int)pSqlServer->GetResults()->getInt(m_sType.ClrStr());

				dynamic_string Buffer;
				m_pServer->Localization()->Format(Buffer, m_pServer->GetClientLanguage(m_ClientID), _("Rank {int:Rank} - {str:Name} {int:Count} Leader {str:Leader}"), 
					"Rank", &Rank, "Name", pSqlServer->GetResults()->getString("Clanname").c_str(), "Count", &Level, "Leader", pSqlServer->GetResults()->getString("LeaderName").c_str());
			
				if(m_Type == 1)
				{
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_AddLocalizeVote_Language(m_ClientID, "null", _(Buffer.buffer()));
					m_pServer->AddGameServerCmd(pCmd);
				}
				else
				{	
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(-1, CHATCATEGORY_DEFAULT, _(Buffer.buffer()));
					m_pServer->AddGameServerCmd(pCmd);	
				}
				Buffer.clear();
			}
		}
		catch (sql::SQLException &e)
		{
			dbg_msg("sql", "Can't get top10 clans (MySQL Error: %s)", e.what());
			return false;
		}
		return true;
	}
};
void CServer::ShowTop10Clans(int ClientID, const char* Type, int TypeGet)
{
	CSqlJob* pJob = new CSqlJob_Server_ShowTop10Clans(this, ClientID, Type, TypeGet);
	pJob->Start();
}

// ********************************************** OZOZOZ ДОМА СОСАТЬ ЛЕЖАТЬ НАХУЙ!!!
// Получить дома кланов
class CSqlJob_Server_GetTopClanHouse : public CSqlJob
{
private:
	CServer* m_pServer;
	
public:
	CSqlJob_Server_GetTopClanHouse(CServer* pServer)
	{
		m_pServer = pServer;
	}

	virtual bool Job(CSqlServer* pSqlServer)
	{
		try
		{
			pSqlServer->executeSqlQuery("SELECT ClanID, Money FROM tw_Clans ORDER BY Money DESC LIMIT 3;");
			int House = 0;
			while(pSqlServer->GetResults()->next())
			{
				int ClanID = (int)pSqlServer->GetResults()->getInt("ClanID");
				m_pServer->m_HouseClanID[House] = ClanID;
			
				char aBuf[128];
				if(m_pServer->m_HouseClanID[House] != m_pServer->m_HouseOldClanID[House])
				{
					str_format(aBuf, sizeof(aBuf), "[House#%d] Rich Clan %s got / Clan %s lose!", House,  m_pServer->GetClanName(ClanID), m_pServer->GetClanName(m_pServer->m_HouseOldClanID[House]));
				
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(-1, CHATCATEGORY_DEFAULT, _(aBuf));
					m_pServer->AddGameServerCmd(pCmd);
				}
				else
				{
					str_format(aBuf, sizeof(aBuf), "[House#%d] Rich Clan %s saved house / No war!", House,  m_pServer->GetClanName(ClanID));
				
					CServer::CGameServerCmd* pCmd = new CGameServerCmd_SendChatTarget_Language(-1, CHATCATEGORY_DEFAULT, _(aBuf));
					m_pServer->AddGameServerCmd(pCmd);
				}
				m_pServer->m_HouseOldClanID[House] = ClanID;
				House++;
			} 
		}
		catch (sql::SQLException &e)
		{
			return false;
		}
		return true;
	}
};
void CServer::GetTopClanHouse() 
{
	CSqlJob* pJob = new CSqlJob_Server_GetTopClanHouse(this);
	pJob->Start();
}

int CServer::GetTopHouse(int HouseID)
{
	return m_HouseClanID[HouseID];
}

int CServer::GetOwnHouse(int ClientID)
{
	if(!GetClanID(ClientID))
		return -1;
	if(GetClanID(ClientID) == m_HouseClanID[0])
		return 0;
	if(GetClanID(ClientID) == m_HouseClanID[1])
		return 1;
	if(GetClanID(ClientID) == m_HouseClanID[2])
		return 2;	
	return -1;
}

bool CServer::GetHouse(int ClientID)
{
	if(GetClanID(ClientID) && (GetClanID(ClientID) == m_HouseClanID[0] || GetClanID(ClientID) == m_HouseClanID[1]  || GetClanID(ClientID) == m_HouseClanID[2]))
		return true;

	return false;
}
bool CServer::GetSpawnInClanHouse(int ClientID, int HouseID)
{
	if(GetClanID(ClientID) != m_HouseClanID[HouseID])
		return false;

	return m_stClan[GetClanID(ClientID)].f_spawninhouse;
}

bool CServer::SetOpenHouse(int HouseID)
{
	if(HouseID == -1)
		return false;

	m_stClan[m_HouseClanID[HouseID]].f_openhouse ^= true;
	return true;
}

bool CServer::GetOpenHouse(int HouseID)
{
	if(HouseID == -1)
		return false;

	return m_stClan[m_HouseClanID[HouseID]].f_openhouse;
}

void CServer::Ban(int ClientID, int Seconds, const char* pReason)
{
	m_ServerBan.BanAddr(m_NetServer.ClientAddr(ClientID), Seconds, pReason);
}

int* CServer::GetIdMap(int ClientID)
{
	return (int*)(IdMap + VANILLA_MAX_CLIENTS * ClientID);
}

void CServer::SetCustClt(int ClientID)
{
	m_aClients[ClientID].m_CustClt = 1;
}