/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_BONUS_H
#define GAME_SERVER_ENTITIES_BONUS_H

class CBonus : public CEntity
{
public:
	int m_Owner;
	
public:
	CBonus(CGameWorld *pGameWorld, vec2 Pos, vec2 Dir, int Type, int Owner);

	vec2 GetPos(float Time);

	virtual void Reset();
	virtual void Tick();
	virtual void TickPaused();
	virtual void Snap(int SnappingClient);

private:
	vec2 m_ActualPos;
	vec2 m_ActualDir;
	vec2 m_Direction;
	int m_StartTick;
	int m_Type;
	int m_GetLevel;
	bool m_Flashing;
	int m_LifeSpan;
	int m_FlashTimer;
	int m_SpawnTick;
};

#endif
