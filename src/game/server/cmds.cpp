#include <stdio.h>
#include <string.h>

#include <engine/shared/config.h>
#include <engine/server.h>
#include <game/version.h>
#include "cmds.h"
/*
聊天指令一览： ()为可选项，<>为必填项
1.注册与登录
/login (用户名) <密码> 登录
/register <用户名> <密码> 注册
/password <密码> <重复密码> 修改密码
2.公会指令
/newclan <公会名称> 创建公会
/invite <玩家名称> 邀请玩家进入公会 (需要公会所有者权限)
3.其他常用指令
/cmdlist 显示命令列表 (不全)
/createboss 创建boss
/lang (语言ID) 设置语言 (留空显示可用语言列表)
4.管理员指令
/sd <声音ID> 设置声音(?)
/trah <玩家ID> 与某玩家做爱(雾)
/giveitem <玩家ID> <物品ID> <物品数量> (物品等级) 给某人物品 
/remitem <玩家ID> <物品ID> <物品数量> 拿走某人物品 
/sendmail <玩家ID> <物品ID> <物品数量> 通过邮件向某人发送物品
/givedonate <玩家ID> <黄金数量> 给某人点券,购买捐赠物品
/jail <玩家ID> <时长(秒)> 将某人关进监狱
/unjail <玩家ID> 将某人放出监狱
/chpw <用户名> <密码> 修改某人密码
*/
CCmd::CCmd(CPlayer *pPlayer, CGameContext *pGameServer)
{
	m_pPlayer = pPlayer;
	m_pGameServer = pGameServer;
}

void CCmd::ChatCmd(CNetMsg_Cl_Say *Msg)
{
	int ClientID = m_pPlayer->GetCID() >= 0 ? m_pPlayer->GetCID() : -1;
	if(!strncmp(Msg->m_pMessage, "、", 1))
	{
		LastChat();
		GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# 为防止错误输入导致的账号密码泄露，系统已禁止聊天内容以“、”开头"), "cmd", Msg->m_pMessage, NULL);
		return;
	}
	if(!strncmp(Msg->m_pMessage, "/login", 6))
	{
		LastChat();		
		if(GameServer()->Server()->IsClientLogged(ClientID))
		{
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("你已登录"), NULL);
		}
		char Username[256], Password[256];
		if(GameServer()->Server()->GetSeccurity(ClientID))
		{
			if(sscanf(Msg->m_pMessage, "/login %s %s", Username, Password) != 2) 
			{
				GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("这个账户启用了安全设置,需要用户名与密码!"), NULL);
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "登录方法: /login <用户名> <密码>");
				return;
			}
			GameServer()->Server()->Login(ClientID, Username, Password);
		}
		else
		{
			if(sscanf(Msg->m_pMessage, "/login %s", Password) != 1) 
			{
				GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("这个账户未启用安全设置,只需要密码"), NULL);
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "登录方法: /login <密码>");
				return;
			}
			GameServer()->Server()->Login(ClientID, Password, Password);
		}
		return;
	}
	
	else if(!strncmp(Msg->m_pMessage, "/giveitem", 9) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int id = 0, itemid = 0, citem = 0, enchant = 0;
		if ((sscanf(Msg->m_pMessage, "/giveitem %d %d %d %d", &id, &itemid, &citem, &enchant)) < 3)
			return GameServer()->SendChatTarget(ClientID, "Use: /giveitem <id> <itemid> <itemcount> (enchant)");

		if(GameServer()->m_apPlayers[id] && GameServer()->Server()->IsClientLogged(id) && itemid > 0 && itemid < 500 && citem > 0)
			GameServer()->GiveItem(id, itemid, citem, enchant);	
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/remitem", 8) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int id = 0, itemid = 0, citem = 0;
		if ((sscanf(Msg->m_pMessage, "/remitem %d %d %d", &id, &itemid, &citem)) != 3)
			return GameServer()->SendChatTarget(ClientID, "Use: /giveitem <id> <itemid> <itemcount>");

		if(GameServer()->m_apPlayers[id] && GameServer()->Server()->IsClientLogged(id) && itemid > 0 && itemid < 500 && citem > 0)
			GameServer()->RemItem(id,itemid,citem);
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/sendmail", 9) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int id = 0, itemid = 0, citem = 0;
		if ((sscanf(Msg->m_pMessage, "/sendmail %d %d %d", &id, &itemid, &citem)) != 3)
			return GameServer()->SendChatTarget(ClientID, "Use: /sendmail <id> <itemid> <itemcount>");

		if(GameServer()->m_apPlayers[id] && GameServer()->Server()->IsClientLogged(id) && itemid > 0 && itemid < 500 && citem > 0)
			GameServer()->SendMail(id, "来自玩家的物品", itemid, citem);
		return;
	}
	
	else if(!strncmp(Msg->m_pMessage, "/givedonate", 10) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int id = 0, citem = 0;
		if ((sscanf(Msg->m_pMessage, "/givedonate %d %d", &id, &citem)) != 2)
			return GameServer()->SendChatTarget(ClientID, "Use: /givedonate <id> <money>");

		if(GameServer()->m_apPlayers[id] && GameServer()->Server()->IsClientLogged(id))
		{
			GameServer()->SendChatTarget(ClientID, "You gived donate.");
			GameServer()->SendChatTarget(id, "Your donate balance added.");
			GameServer()->m_apPlayers[id]->AccData.Donate += citem;
			GameServer()->UpdateStats(id);
		}
		return;
	} 

	else if (!strncmp(Msg->m_pMessage, "/sd", 3) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int size = 0;
		if ((sscanf(Msg->m_pMessage, "/sd %d", &size)) != 1)
			return GameServer()->SendChatTarget(ClientID, "Please use: /sd <idsound>");

		int soundid = clamp(size, 0, 40);
		if (GameServer()->GetPlayerChar(ClientID))
			GameServer()->CreateSound(m_pPlayer->GetCharacter()->m_Pos, soundid);
		return;
	}
	else if (!strncmp(Msg->m_pMessage, "/trah", 5) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int size = 0;
		if ((sscanf(Msg->m_pMessage, "/trah %d", &size)) != 1)
			return GameServer()->SendChatTarget(ClientID, "Please use: /trah <id>");

		if(GameServer()->m_apPlayers[size] && GameServer()->m_apPlayers[size]->GetCharacter() 
			&& GameServer()->Server()->IsClientLogged(size))
		{
			GameServer()->SendChatTarget_Localization(-1, -1, _("[{str:name}] inserts fuck in [{str:name2}]"), "name", GameServer()->Server()->ClientName(ClientID), "name2", GameServer()->Server()->ClientName(size));
			GameServer()->SendChatTarget_Localization(-1, -1, _("[{str:name}] ah ah insert me please, moaned"), "name", GameServer()->Server()->ClientName(size));
			GameServer()->SendChatTarget_Localization(-1, -1, _("[{str:name}] cum in vagina [{str:name2}]"), "name", GameServer()->Server()->ClientName(ClientID), "name2", GameServer()->Server()->ClientName(size));
			GameServer()->SendChatTarget_Localization(-1, -1, _("[{str:name}] i love you! [{str:name2}]"), "name", GameServer()->Server()->ClientName(ClientID), "name2", GameServer()->Server()->ClientName(size));
			GameServer()->SendChatTarget_Localization(-1, -1, _("[{str:name}] i love you! [{str:name2}]"), "name", GameServer()->Server()->ClientName(size), "name2", GameServer()->Server()->ClientName(ClientID));
			
			for(int i = 0; i < 40; ++i)
				GameServer()->CreateSoundGlobal(20);
		}
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/register", 9))
	{
		LastChat();
		if(g_Config.m_SvCityStart > 0)
			return 	GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("请在 1-250 服务器上注册"), NULL);
		
		char Username[256], Password[256];
		if(sscanf(Msg->m_pMessage, "/register %s %s", Username, Password) != 2) 
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("使用方法: /register <用户名> <密码>"), NULL);
		if(str_length(Username) > 15 || str_length(Username) < 2 || str_length(Password) > 15 || str_length(Password) < 2)
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("用户名 / 密码必须包含 2-15 个字符"), NULL);

		GameServer()->Server()->Register(ClientID, Username, Password, "Lol");
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/newclan", 8))
	{
		if(GameServer()->Server()->GetClanID(ClientID) > 0 || str_comp_nocase(GameServer()->Server()->ClientClan(ClientID), "NOPE") != 0)
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# 你已经加入一个公会了!"), NULL);
		
		if(GameServer()->Server()->GetItemCount(ClientID, CLANTICKET))
		{	
			char Reformat[256];
			if(sscanf(Msg->m_pMessage, "/newclan %s", Reformat) != 1) 
				return GameServer()->SendChatTarget(m_pPlayer->GetCID(), "使用方法: /newclan <公会名称>");
	
			remove_spaces(Reformat);
			if(str_length(Reformat) > 12 || str_length(Reformat) < 1)
				return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("公会名称长度必须在 1~12 个字符之间"), NULL);

			GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("Ticket Create Clan 已使用"), NULL);
			GameServer()->Server()->NewClan(ClientID, Reformat);
			m_pPlayer->m_LoginSync = 150;
		}
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/invite", 7))
	{
		if(!GameServer()->Server()->GetLeader(ClientID, GameServer()->Server()->GetClanID(ClientID)))
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# 你不是公会首领!"), NULL);

		if(GameServer()->Server()->GetClan(DCOUNTUCLAN, GameServer()->Server()->GetClanID(ClientID)) >= GameServer()->Server()->GetClan(DMAXCOUNTUCLAN, GameServer()->Server()->GetClanID(ClientID)))
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("公会人数已达到上限"), NULL);

		if(GameServer()->Server()->GetClanID(ClientID) > 0)
		{
			bool Found = false;
			char NameInv[256];
			if(sscanf(Msg->m_pMessage, "/invite %s", NameInv) != 1) 
				return GameServer()->SendChatTarget(m_pPlayer->GetCID(), "用法: /invite <玩家昵称>");

			for(int i = 0; i < MAX_NOBOT; ++i)
			{
				if(GameServer()->m_apPlayers[i])
				{
					if(str_comp_nocase(NameInv, GameServer()->Server()->ClientName(i)) == 0 && GameServer()->Server()->GetClanID(i) <= 0)
					{
						Found = true;
						GameServer()->m_apPlayers[i]->m_InviteClanID = GameServer()->Server()->GetClanID(ClientID);
						
						CNetMsg_Sv_VoteSet Msg;
						Msg.m_Timeout = 10;
						Msg.m_pReason = "";
						Msg.m_pDescription = 0;
						
						Msg.m_pDescription = GameServer()->Server()->Localization()->Localize(m_pPlayer->GetLanguage(), _("是否加入公会?"));
						GameServer()->Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
						
						GameServer()->m_InviteTick[i] = 10*GameServer()->Server()->TickSpeed();
						GameServer()->SendBroadcast_Localization(i, BROADCAST_PRIORITY_INTERFACE, 600, _("玩家 {str:name} 邀请你加入 {str:cname} 公会!"), "name", GameServer()->Server()->ClientName(ClientID), "cname" , GameServer()->Server()->ClientClan(ClientID), NULL);
					}
				}
			}
			if(!Found)
				GameServer()->SendBroadcast_Localization(ClientID, BROADCAST_PRIORITY_INTERFACE, 150, _("玩家未找到,或者玩家已在公会!"), NULL);
			else 
				GameServer()->SendBroadcast_Localization(ClientID, BROADCAST_PRIORITY_INTERFACE, 150, _("玩家已找到,请求已发送."), NULL);
		}
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/createboss", 11))
	{	
		GameServer()->StartBoss(ClientID, 60, BOT_BOSSSLIME);
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/cmdlist", 8))
	{	
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "? ---- 命令列表");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "/invite ?<玩家昵称>, /createboss, /cmdlist, /lang <语言>");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "/login ?<用户名> <密码>, /register <用户名> <密码>");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "/newclan <公会名称>");
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/lang", 5))
	{	
		char pLanguageCode[128];
		char aFinalLanguageCode[8];
		aFinalLanguageCode[0] = 0;

		if(sscanf(Msg->m_pMessage, "/lang %s", pLanguageCode) == 1)
		{ 
			if(str_comp_nocase(pLanguageCode, "ua") == 0)
				str_copy(aFinalLanguageCode, "uk", sizeof(aFinalLanguageCode));
			else
			{
				for(int i=0; i< GameServer()->Server()->Localization()->m_pLanguages.size(); i++)
				{
					if(str_comp_nocase(pLanguageCode, GameServer()->Server()->Localization()->m_pLanguages[i]->GetFilename()) == 0)
						str_copy(aFinalLanguageCode, pLanguageCode, sizeof(aFinalLanguageCode));
				}
			}
		}
		
		if(aFinalLanguageCode[0])
		{
			GameServer()->Server()->SetClientLanguage(ClientID, aFinalLanguageCode);
			if(m_pPlayer)
				m_pPlayer->SetLanguage(aFinalLanguageCode);
				
			if(GameServer()->Server()->IsClientLogged(ClientID)) GameServer()->ResetVotes(ClientID, AUTH);
			else GameServer()->ResetVotes(ClientID, NOAUTH);
		}
		else
		{
			const char* pLanguage = m_pPlayer->GetLanguage();
			dynamic_string BufferList;
			int BufferIter = 0;
			for(int i=0; i< GameServer()->Server()->Localization()->m_pLanguages.size(); i++)
			{
				if(i>0)
					BufferIter = BufferList.append_at(BufferIter, ", ");
				BufferIter = BufferList.append_at(BufferIter, GameServer()->Server()->Localization()->m_pLanguages[i]->GetFilename());
			}
			
			dynamic_string Buffer;
			GameServer()->Server()->Localization()->Format_L(Buffer, pLanguage, _("Available languages: {str:ListOfLanguage}"), "ListOfLanguage", BufferList.buffer(), NULL);
			GameServer()->SendChatTarget(m_pPlayer->GetCID(), Buffer.buffer());
			Buffer.clear();
		}
		return;
	}
	// /jail & /unjail 功能还没加入
	else if (!strncmp(Msg->m_pMessage, "/jail", 5) && GameServer()->Server()->IsAuthed(ClientID)) 
	{
		int id = 0, JailLength = 0;
		if ((sscanf(Msg->m_pMessage, "/jail %d %d", &id, &JailLength)) != 2)
		{
			return GameServer()->SendChatTarget(ClientID, "Use: /jail <id> <JailLength>");
		}
		m_pPlayer->AccData.IsJailed = true;
		m_pPlayer->AccData.Jail = true;
		GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, ("Successfully jailed {str:name}"), "name", GameServer()->GetPlayerChar(id), NULL);	
	}
	else if (!strncmp(Msg->m_pMessage, "/unjail", 7) && GameServer()->Server()->IsAuthed(ClientID))
	{
		int id = 0;
		if ((sscanf(Msg->m_pMessage, "/jail %d", &id)) != 1)
		{
			return GameServer()->SendChatTarget(ClientID, "Use: /unjail <id>");
		}
		m_pPlayer->AccData.IsJailed = false;
		m_pPlayer->AccData.Jail = false;
		GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, ("Successfully unjailed {str:name}"), "name", GameServer()->GetPlayerChar(id), NULL);	
	}
	// 密码修改
	else if(!strncmp(Msg->m_pMessage, "/password", 9))
	{
		LastChat();
		if(!GameServer()->Server()->IsClientLogged(ClientID))
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# 请先登录"), NULL);
		char Password[256];
		/*
		char RepeatPassword[256]
		if(sscanf(Msg->m_pMessage, "/password %s %s", Password, RepeatPassword) != 2) 
		{
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("修改密码: /password <密码> <重复密码>"), NULL);
		}
		
		if(Password != RepeatPassword)
		{
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("两次密码不一致"), NULL);
		}
		*/
		if(sscanf(Msg->m_pMessage, "/password %s", Password) != 1) 
			{
				return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("修改密码: /password <密码>"), NULL);
			}
		if( str_length(Password) > 15 || str_length(Password) < 2)
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("密码必须包含 2~15 个字符"), NULL);

		//GameServer()->Server()->Register(ClientID, Username, Password, "Lol");
		GameServer()->Server()->ChangePassword(ClientID, Password);
		return;
	}
	// 密码修改(管理员专用)
	else if(!strncmp(Msg->m_pMessage, "/chpw", 5) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		char Username[256], Password[256];
		if(sscanf(Msg->m_pMessage, "/chpw %s %s", Username, Password) != 2) 
		{
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("修改密码(管理员): /chpw <用户名> <密码> "), NULL);
		}
		
		
		if( str_length(Password) > 15 || str_length(Password) < 2)
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("密码必须包含 2~15 个字符"), NULL);

		GameServer()->Server()->ChangePassword_Admin(ClientID, Username, Password);
		return;
	}
	if(!strncmp(Msg->m_pMessage, "/", 1))
	{
		LastChat();
		GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# 未知命令 {str:cmd} !"), "cmd", Msg->m_pMessage, NULL);
		return;
	}

}

void CCmd::LastChat()
{
	m_pPlayer->m_LastChat = GameServer()->Server()->Tick();
}
