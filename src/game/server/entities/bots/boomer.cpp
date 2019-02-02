/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <new>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/mapitems.h>
#include "boomer.h"
#include "../character.h"
#include "../projectile.h"

MACRO_ALLOC_POOL_ID_IMPL(CBoomer, MAX_CLIENTS)

CBoomer::CBoomer(CGameWorld *pWorld)
: CCharacter(pWorld)
{
	m_BotDir = 1;
	m_BotLastPos = m_Pos;
	m_BotLastStuckTime = 0.0f;
	m_BotStuckCount = 0;
	m_BotTimePlayerFound = Server()->Tick();
	m_BotTimeGrounded = Server()->Tick();
	m_BotTimeLastOption = Server()->Tick();
	m_BotTimeLastDamage = 0.0f;
	m_BotClientIDFix = -1;
	m_BotTimeLastSound = Server()->Tick();
	m_BotJumpTry = false;
}

void CBoomer::Tick()
{
	TickBotAI();
	CCharacter::Tick();
}

bool CBoomer::TakeDamage(vec2 Force, int Dmg, int From, int Weapon)
{
	if (CCharacter::TakeDamage(Force, Dmg, From, Weapon, 0))
	{
		m_BotTimeLastDamage = Server()->Tick();
		return true;
	}

	return false;
}

void CBoomer::PlaySound()
{

}

void CBoomer::RunAction()
{
	if (m_BotClientIDFix != -1 && GameServer()->m_apPlayers[m_BotClientIDFix])
	{
		CCharacter *pChar = GameServer()->m_apPlayers[m_BotClientIDFix]->GetCharacter();
		if (!pChar)
		{
			m_BotClientIDFix = -1;
			return;
		}

		if (Server()->Tick() - m_BotTimePlayerFound > Server()->TickSpeed()*0.45f)
		{
			m_LatestInput.m_Fire = m_Input.m_Fire = 1;
			m_BotDir = 0;
			m_BotTimePlayerFound = Server()->Tick();
		}

		m_BotClientIDFix = -1;
	}
}

void CBoomer::TickBotAI()
{
    //Sounds
    if (Server()->Tick() - m_BotTimeLastSound > Server()->TickSpeed()*5.0f && !(rand()%50))
    {
        PlaySound();
        m_BotTimeLastSound = Server()->Tick();
    }

    //Clean m_Input
	m_Input.m_Hook = 0;
	m_Input.m_Fire = 0;
	m_Input.m_Jump = 0;

    //Run actions
    RunAction();

    //Interact with users
    bool PlayerClose = false;
    bool PlayerFound = false;
    float LessDist = 700.0f;

    m_BotClientIDFix = -1;
	for (int i=0; i<g_Config.m_SvMaxClients; i++)
	{
	    CPlayer *pPlayer = GameServer()->m_apPlayers[i];
	    if (!pPlayer || !pPlayer->GetCharacter() || pPlayer->IsBot() || pPlayer->GetBotType() == BOT_L1MONSTER)
            continue;

        int Dist = distance(pPlayer->GetCharacter()->m_Pos, m_Pos);
        if (Dist < LessDist)
            LessDist = Dist;
        else
            continue;

        if (Dist < 700.0f)
        {
            if (Dist > 200.0f)
            {
                vec2 DirPlayer = normalize(pPlayer->GetCharacter()->m_Pos - m_Pos);
				if (DirPlayer.x < 0)
					m_BotDir = -1;
				else
					m_BotDir = 1;
					
				m_Input.m_Hook = 1;
            }
            else
            {
                PlayerClose = true;
                
				m_BotDir = 0;
				m_BotClientIDFix = pPlayer->GetCID();
            }

            m_Input.m_TargetX = static_cast<int>(pPlayer->GetCharacter()->m_Pos.x - m_Pos.x);
            m_Input.m_TargetY = static_cast<int>(pPlayer->GetCharacter()->m_Pos.y - m_Pos.y);

            PlayerFound = true;
        }
	}

    //Fix target
    if (!PlayerFound)
    {
        m_Input.m_TargetX = m_BotDir;
        m_Input.m_TargetY = 0;
    }
    else if (m_BotClientIDFix != -1)
    {
    	CPlayer *pPlayer = GameServer()->m_apPlayers[m_BotClientIDFix];
    	if (pPlayer && pPlayer->GetCharacter()) // Jump to player
    	{
			vec2 Lefpos = normalize(pPlayer->GetCharacter()->m_Core.m_Pos - m_Core.m_Pos);
			m_Core.m_Vel += Lefpos * 5;
		}
    }

    //Random Actions
	if (!PlayerFound)
	{
        if (Server()->Tick()-m_BotTimeLastOption > Server()->TickSpeed()*10.0f)
        {
            int Action = rand()%3;
            if (Action == 0)
                m_BotDir = -1;
            else if (Action == 1)
                m_BotDir = 1;
            else if (Action == 2)
                m_BotDir = 0;

            m_BotTimeLastOption = Server()->Tick();
        }
	}

    //Fix Stuck
    if (IsGrounded())
        m_BotTimeGrounded = Server()->Tick();

    //Limits
    int tx = m_Pos.x+m_BotDir*45.0f;
    if (tx < 0)
        m_BotDir = 1;
    else if (tx >= GameServer()->Collision()->GetWidth()*32.0f)
    	m_BotDir = -1;

    //Delay of actions
    if (!PlayerClose)
        m_BotTimePlayerFound = Server()->Tick();

    //Set data
    m_Input.m_Direction = m_BotDir;
	m_Input.m_PlayerFlags = PLAYERFLAG_PLAYING;
	//Check for legacy input
	if (m_LatestPrevInput.m_Fire && m_Input.m_Fire) m_Input.m_Fire = 0;

	m_LatestPrevInput = m_LatestInput;
	m_LatestInput = m_Input;
	m_BotLastPos = m_Pos;
	CCharacter::FireWeapon();
}
