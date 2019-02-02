/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_VOTING_H
#define GAME_VOTING_H

enum
{
	VOTE_DESC_LENGTH=64,
	VOTE_CMD_LENGTH=512,
	VOTE_REASON_LENGTH=16,
};

struct CVoteOptionClient
{
	CVoteOptionClient *m_pNext = { 0 };
	CVoteOptionClient *m_pPrev = { 0 };
	char m_aDescription[VOTE_DESC_LENGTH];
};

struct CVoteOptionServer
{
	CVoteOptionServer *m_pNext = { 0 };
	CVoteOptionServer *m_pPrev = { 0 };
	char m_aDescription[MAX_NOBOT][VOTE_DESC_LENGTH];
	char m_aCommand[MAX_NOBOT][1];
};

#endif
