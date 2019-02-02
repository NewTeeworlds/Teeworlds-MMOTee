/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_DROPITEMS_H
#define GAME_SERVER_ENTITIES_DROPITEMS_H

class CDropItem : public CEntity
{
public:

	CDropItem(CGameWorld *pGameWorld, vec2 Pos, vec2 Dir, int ItemID, int Count, int HowID, int Enchant);

	vec2 GetPos(float Time);

	virtual void Reset();
	virtual void Tick();
	virtual void TickPaused();
	virtual void Snap(int SnappingClient);
	virtual bool TakeItem(int ClientID);

	int m_HowID;
private:
	vec2 m_ActualPos;
	vec2 m_ActualDir;
	vec2 m_Direction;
	int m_StartTick;
	int m_ItemID;
	int m_Count;
	bool m_Flashing;
	int m_LifeSpan;
	int m_FlashTimer;
	int m_SpawnTick;
	int m_Enchant;
	
};

#endif
