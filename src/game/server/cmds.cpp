#include <stdio.h>
#include <string.h>

#include <engine/shared/config.h>
#include <engine/server.h>
#include <game/version.h>
#include "cmds.h"

CCmd::CCmd(CPlayer *pPlayer, CGameContext *pGameServer)
{
	m_pPlayer = pPlayer;
	m_pGameServer = pGameServer;
}

void CCmd::ChatCmd(CNetMsg_Cl_Say *Msg)
{
	int ClientID = m_pPlayer->GetCID() >= 0 ? m_pPlayer->GetCID() : -1;
	if(!strncmp(Msg->m_pMessage, "/login", 6))
	{
		LastChat();		
		if(GameServer()->Server()->IsClientLogged(ClientID))
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# You already authed."), NULL);
		char Username[256], Password[256];
		if(GameServer()->Server()->GetSeccurity(ClientID))
		{
			if(sscanf(Msg->m_pMessage, "/login %s %s", Username, Password) != 2) 
			{
				GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("This account seccurity active. Need login and password"), NULL);
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "Use: /login <username> <password>");
				return;
			}
			GameServer()->Server()->Login(ClientID, Username, Password);
		}
		else
		{
			if(sscanf(Msg->m_pMessage, "/login %s", Password) != 1) 
			{
				GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("This account seccurity active. Need login and password"), NULL);
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "Use: /login <password>");
				return;
			}
			GameServer()->Server()->Login(ClientID, Password, Password);
		}
		return;
	}
	
	else if(!strncmp(Msg->m_pMessage, "/giveitem", 9) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int id = 0, itemid = 0, citem = 0;
		if ((sscanf(Msg->m_pMessage, "/giveitem %d %d %d", &id, &itemid, &citem)) != 3)
			return GameServer()->SendChatTarget(ClientID, "Use: /giveitem <id> <itemid> <itemcount>");

		if(GameServer()->m_apPlayers[id] && GameServer()->Server()->IsClientLogged(id) && itemid > 0 && itemid < 500 && citem > 0)
			GameServer()->GiveItem(id, itemid, citem);	
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/sendmail", 9) && GameServer()->Server()->IsAuthed(ClientID))
	{
		LastChat();
		int id = 0, itemid = 0, citem = 0;
		if ((sscanf(Msg->m_pMessage, "/sendmail %d %d %d", &id, &itemid, &citem)) != 3)
			return GameServer()->SendChatTarget(ClientID, "Use: /sendmail <id> <itemid> <itemcount>");

		if(GameServer()->m_apPlayers[id] && GameServer()->Server()->IsClientLogged(id) && itemid > 0 && itemid < 500 && citem > 0)
			GameServer()->SendMail(id, "Sender mail Kurosio!", itemid, citem);
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
			return 	GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("You need registered area 1-100."), NULL);
		
		char Username[256], Password[256];
		if(sscanf(Msg->m_pMessage, "/register %s %s", Username, Password) != 2) 
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("Use: /register <username> <password>"), NULL);
		if(str_length(Username) > 15 || str_length(Username) < 2 || str_length(Password) > 15 || str_length(Password) < 2)
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("Username / Password must contain 2-15 characters"), NULL);

		GameServer()->Server()->Register(ClientID, Username, Password, "Lol");
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/newclan", 8))
	{
		if(GameServer()->Server()->GetClanID(ClientID) > 0 || str_comp_nocase(GameServer()->Server()->ClientClan(ClientID), "NOPE") != 0)
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# You are already in clan"), NULL);
		
		if(GameServer()->Server()->GetItemCount(ClientID, CLANTICKET))
		{	
			char Reformat[256];
			if(sscanf(Msg->m_pMessage, "/newclan %s", Reformat) != 1) 
				return GameServer()->SendChatTarget(m_pPlayer->GetCID(), "Use: /newclan <nameclan>");
	
			remove_spaces(Reformat);
			if(str_length(Reformat) > 12 || str_length(Reformat) < 1)
				return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("Characters max 12 and min 1"), NULL);

			GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("Used Ticket Create Clan"), NULL);
			GameServer()->Server()->NewClan(ClientID, Reformat);
			m_pPlayer->m_LoginSync = 150;
		}
		return;
	}

	else if(!strncmp(Msg->m_pMessage, "/invite", 7))
	{
		if(!GameServer()->Server()->GetLeader(ClientID, GameServer()->Server()->GetClanID(ClientID)))
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# You not leader this clan"), NULL);

		if(GameServer()->Server()->GetClan(DCOUNTUCLAN, GameServer()->Server()->GetClanID(ClientID)) >= GameServer()->Server()->GetClan(DMAXCOUNTUCLAN, GameServer()->Server()->GetClanID(ClientID)))
			return GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("Max search num in clan"), NULL);

		if(GameServer()->Server()->GetClanID(ClientID) > 0)
		{
			bool Found = false;
			char NameInv[256];
			if(sscanf(Msg->m_pMessage, "/invite %s", NameInv) != 1) 
				return GameServer()->SendChatTarget(m_pPlayer->GetCID(), "Use: /invite <pl name>");

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
						
						Msg.m_pDescription = GameServer()->Server()->Localization()->Localize(m_pPlayer->GetLanguage(), _("Join in clan?"));
						GameServer()->Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
						
						GameServer()->m_InviteTick[i] = 10*GameServer()->Server()->TickSpeed();
						GameServer()->SendBroadcast_Localization(i, BROADCAST_PRIORITY_INTERFACE, 600, _("The player {str:name} invite you in clan {str:cname}"), "name", GameServer()->Server()->ClientName(ClientID), "cname" , GameServer()->Server()->ClientClan(ClientID), NULL);
					}
				}
			}
			if(!Found)
				GameServer()->SendBroadcast_Localization(ClientID, BROADCAST_PRIORITY_INTERFACE, 150, _("Player not found! Or player already in clan"), NULL);
			else 
				GameServer()->SendBroadcast_Localization(ClientID, BROADCAST_PRIORITY_INTERFACE, 150, _("Player found! Request is sent"), NULL);
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
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "? ---- Command List");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "/invite ?<name>, /createboss, /cmdlist, /lang <lang>");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "/login ?<name> <pass>, /register <name> <pass>");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "/newclan <cname>");
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

	if(!strncmp(Msg->m_pMessage, "/", 1))
	{
		LastChat();
		GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("# Command {str:cmd} not found!"), "cmd", Msg->m_pMessage, NULL);
		return;
	}

}

void CCmd::LastChat()
{
	m_pPlayer->m_LastChat = GameServer()->Server()->Tick();
}
