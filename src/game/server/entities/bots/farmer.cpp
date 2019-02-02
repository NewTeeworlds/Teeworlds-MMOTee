/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <new>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/mapitems.h>
#include "farmer.h"
#include "../character.h"
#include "../projectile.h"

MACRO_ALLOC_POOL_ID_IMPL(CNpcFarmer, MAX_CLIENTS)

CNpcFarmer::CNpcFarmer(CGameWorld *pWorld)
: CCharacter(pWorld)
{
	m_BotDir = 0;
	m_BotLastPos = m_Pos;
	m_BotLastStuckTime = 0.0f;
	m_BotStuckCount = 0;
	m_BotTimePlayerFound = Server()->Tick();
	m_BotTimeGrounded = Server()->Tick();
	m_BotTimeLastOption = Server()->Tick();
	m_BotTimeLastDamage = 0.0f;
	m_BotClientIDFix = -1;
	m_BotTimeLastSound = Server()->Tick();
	m_BotTimeLastChat = Server()->Tick();
	m_BotJumpTry = false;
}

void CNpcFarmer::Tick()
{
	TickBotAI();
	CCharacter::Tick();
}

bool CNpcFarmer::TakeDamage(vec2 Force, int Dmg, int From, int Weapon)
{
	if (CCharacter::TakeDamage(Force, Dmg, From, Weapon, 0))
	{
		m_BotTimeLastDamage = Server()->Tick();
		return true;
	}

	return false;
}

void CNpcFarmer::PlaySound()
{
	GameServer()->SendEmoticon(m_pPlayer->GetCID(), 4);			
	GameServer()->CreateSound(m_Pos, SOUND_TEE_CRY);
}

void CNpcFarmer::RunAction()
{
	if (m_BotClientIDFix != -1 && GameServer()->m_apPlayers[m_BotClientIDFix])
	{
		CCharacter *pChar = GameServer()->m_apPlayers[m_BotClientIDFix]->GetCharacter();
		if (!pChar)
		{
			m_BotClientIDFix = -1;
			return;
		}

		if (Server()->Tick() - m_BotTimePlayerFound > Server()->TickSpeed()*0.01f)
		{
			m_LatestInput.m_Fire = m_Input.m_Fire = 1;
			m_BotTimePlayerFound = Server()->Tick();
		}

		m_BotClientIDFix = -1;
	}
}

void CNpcFarmer::TickBotAI()
{
    // ЗВУКИ
    if (Server()->Tick() - m_BotTimeLastSound > Server()->TickSpeed()*5.0f && !(rand()%50))
    {
        PlaySound();
        m_BotTimeLastSound = Server()->Tick();
    }
    
    EmoteNormal = EMOTE_HAPPY;

    // ОЧИСТКА ДЕЙСТВИЙ
	m_Input.m_Hook = 0;
	m_Input.m_Fire = 0;
	m_Input.m_Jump = 0;

    // ВЫПОЛНЕНИЕ СЦЕНАРИЯ
    RunAction();

    // ИНТЕРАКТ С ИГРОКАМИ
    bool PlayerClose = false;
    bool PlayerFound = false;
    bool PlayerNFound = false;
    float LessDist = 500.0f;
    m_BotClientIDFix = -1;
    
	if (Server()->Tick() % (1 * Server()->TickSpeed() * 4) == 0)
	{
		GameServer()->SendEmoticon(m_pPlayer->GetCID(), 2);
    }
    
	for (int i=0; i<g_Config.m_SvMaxClients-MAX_BOTS; i++)
	{
		CPlayer *pPlayer = GameServer()->m_apPlayers[i];
		if (!pPlayer || !pPlayer->GetCharacter() || (pPlayer->IsBot() && pPlayer->GetBotType() == BOT_NPC))
			continue;

		int Dist = distance(pPlayer->GetCharacter()->m_Pos, m_Pos);
		if (Dist < LessDist)
			LessDist = Dist;
		else
			continue;
		
		if (Dist < 400.0f)
		{
			int Collide = GameServer()->Collision()->IntersectLine(pPlayer->GetCharacter()->m_Pos, m_Pos, 0, 0);
			if(g_Config.m_SvCityStart != 1 && Collide)
				continue;
			
			if(Dist > 300.0f && pPlayer->GetCharacter()->InWork)
			{
				pPlayer->GetCharacter()->InWork = false;
				GameServer()->ResetVotes(i, AUTH);
			}
			
			if(Dist <= 300.0f && !pPlayer->GetCharacter()->InWork)
			{
				pPlayer->GetCharacter()->InWork = true;
				GameServer()->ResetVotes(i, AUTH);
			}

			// ЧАТ
			if (Server()->Tick()-m_BotTimeLastChat > 20*Server()->TickSpeed())
			{
				GameServer()->SendEmoticon(m_pPlayer->GetCID(), 4);
				if(Server()->GetItemSettings(i, SCHAT) != 2)
				{
					static const char *pPhrases[3] = {
						"[WORKER] Hello {str:name}, open vote menu!!"
					};
					GameServer()->SendChatTarget_Localization(i, CHATCATEGORY_ACCUSATION, _(pPhrases[0]), "name", Server()->ClientName(i), NULL);
				}
				m_BotTimeLastChat = Server()->Tick();
			}

			m_Input.m_TargetX = static_cast<int>(pPlayer->GetCharacter()->m_Pos.x - m_Pos.x);
			m_Input.m_TargetY = static_cast<int>(pPlayer->GetCharacter()->m_Pos.y - m_Pos.y);

			PlayerNFound = true;
		}
	}

    //Fix target
    if (!PlayerFound && !PlayerNFound)
    {
        m_Input.m_TargetX = m_BotDir;
        m_Input.m_TargetY = 0;
    }

    //Fix Stuck
    if (IsGrounded())
        m_BotTimeGrounded = Server()->Tick();

    //Delay of actions
    if (!PlayerClose)
        m_BotTimePlayerFound = Server()->Tick();

    //Set data
    m_Input.m_Direction = m_BotDir;
	m_Input.m_PlayerFlags = PLAYERFLAG_PLAYING;
	//Check for legacy input
	if (m_LatestPrevInput.m_Fire && m_Input.m_Fire) m_Input.m_Fire = 0;
	if (m_LatestInput.m_Jump && m_Input.m_Jump) m_Input.m_Jump = 0;
	//Ceck Double Jump
	if (m_Input.m_Jump && (m_Jumped&1) && !(m_Jumped&2) && m_Core.m_Vel.y < GameServer()->Tuning()->m_Gravity)
		m_Input.m_Jump = 0;

	m_LatestPrevInput = m_LatestInput;
	m_LatestInput = m_Input;
	m_BotLastPos = m_Pos;
	CCharacter::FireWeapon();
}
