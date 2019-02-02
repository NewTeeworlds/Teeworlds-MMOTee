//const char *itob(char *input);
//void chat_command(CNetMsg_Cl_Say *msg, CPlayer *p, CGameContext *game);
//void BuyItem(int *AccItem, char ItemName[128], int Price, int Money);
#ifndef GAME_SERVER_CMD_H
#define GAME_SERVER_CMD_H
#include "game/server/gamecontext.h"

class CCmd
{
public:
	CCmd(CPlayer *pPlayer, CGameContext *pGameServer);
	void ChatCmd(CNetMsg_Cl_Say *Msg);

private:
	CPlayer *m_pPlayer;
	CGameContext *m_pGameServer;
	CGameContext *GameServer() const { return m_pGameServer; }

	void LastChat();
};

#endif