#ifndef ENGINE_SHARED_NETBAN_H
#define ENGINE_SHARED_NETBAN_H

#include <base/system.h>
#include "netdatabase.h"

class CNetBan : public CNetDatabase
{
protected:
	struct CBanInfo
	{
		enum
		{
			EXPIRES_NEVER=-1,
			REASON_LENGTH=64,
		};
		int m_Expires;
		char m_aReason[REASON_LENGTH];
	};

	typedef CPool<NETADDR, CBanInfo, 1> CBanAddrPool;
	typedef CPool<CNetRange, CBanInfo, 16> CBanRangePool;
	typedef CNode<NETADDR, CBanInfo> CBanAddr;
	typedef CNode<CNetRange, CBanInfo> CBanRange;
	
	template<class DATATYPE> void MakeBanInfo(const CNode<DATATYPE, CBanInfo> *pBan, char *pBuf, unsigned BuffSize, int Type) const;
	template<class POOL> int Ban(POOL *pBanPool, const typename POOL::CDataType *pData, int Seconds, const char *pReason);
	template<class POOL> int Unban(POOL *pBanPool, const typename POOL::CDataType *pData);

	class IConsole *m_pConsole;
	class IStorage *m_pStorage;
	CBanAddrPool m_BanAddrPool;
	CBanRangePool m_BanRangePool;
	NETADDR m_LocalhostIPV4, m_LocalhostIPV6;

public:
	enum
	{
		MSGTYPE_PLAYER=0,
		MSGTYPE_LIST,
		MSGTYPE_BANADD,
		MSGTYPE_BANREM,
	};

	class IConsole *Console() const { return m_pConsole; }
	class IStorage *Storage() const { return m_pStorage; }

	virtual ~CNetBan() {}
	void Init(class IConsole *pConsole, class IStorage *pStorage);
	void Update();

	virtual int BanAddr(const NETADDR *pAddr, int Seconds, const char *pReason);
	virtual int BanRange(const CNetRange *pRange, int Seconds, const char *pReason);
	int UnbanByAddr(const NETADDR *pAddr);
	int UnbanByRange(const CNetRange *pRange);
	int UnbanByIndex(int Index);
	void UnbanAll() { m_BanAddrPool.Reset(); m_BanRangePool.Reset(); }
	bool IsBanned(const NETADDR *pAddr, char *pBuf, unsigned BufferSize) const;

	static bool ConBan(class IConsole::IResult *pResult, void *pUser);
	static bool ConBanRange(class IConsole::IResult *pResult, void *pUser);
	static bool ConUnban(class IConsole::IResult *pResult, void *pUser);
	static bool ConUnbanRange(class IConsole::IResult *pResult, void *pUser);
	static bool ConUnbanAll(class IConsole::IResult *pResult, void *pUser);
	static bool ConBans(class IConsole::IResult *pResult, void *pUser);
	static bool ConBansSave(class IConsole::IResult *pResult, void *pUser);
};

template<class DATATYPE>
void CNetBan::MakeBanInfo(const CNode<DATATYPE, CBanInfo> *pBan, char *pBuf, unsigned BuffSize, int Type) const
{
	if(pBan == 0 || pBuf == 0)
	{
		if(BuffSize > 0)
			pBuf[0] = 0;
		return;
	}

	// build type based part
	char aBuf[256];
	if(Type == MSGTYPE_PLAYER)
		str_copy(aBuf, "You have been banned", sizeof(aBuf));
	else
	{
		char aTemp[256];
		switch(Type)
		{
		case MSGTYPE_LIST:
			str_format(aBuf, sizeof(aBuf), "%s banned", NetToString(&pBan->m_Data, aTemp, sizeof(aTemp))); break;
		case MSGTYPE_BANADD:
			str_format(aBuf, sizeof(aBuf), "banned %s", NetToString(&pBan->m_Data, aTemp, sizeof(aTemp))); break;
		case MSGTYPE_BANREM:
			str_format(aBuf, sizeof(aBuf), "unbanned %s", NetToString(&pBan->m_Data, aTemp, sizeof(aTemp))); break;
		default:
			aBuf[0] = 0;
		}
	}

	// add info part
	if(pBan->m_Info.m_Expires != CBanInfo::EXPIRES_NEVER)
	{
		int Mins = ((pBan->m_Info.m_Expires-time_timestamp()) + 59) / 60;
		if(Mins <= 1)
			str_format(pBuf, BuffSize, "%s for 1 minute (%s)", aBuf, pBan->m_Info.m_aReason);
		else
			str_format(pBuf, BuffSize, "%s for %d minutes (%s)", aBuf, Mins, pBan->m_Info.m_aReason);
	}
	else
		str_format(pBuf, BuffSize, "%s for life (%s)", aBuf, pBan->m_Info.m_aReason);
}

#endif
