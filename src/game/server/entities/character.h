/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_CHARACTER_H
#define GAME_SERVER_ENTITIES_CHARACTER_H

#include <game/server/entity.h>
#include <game/generated/server_data.h>
#include <game/generated/protocol.h>

#include <game/gamecore.h>

enum
{
	WEAPON_GAME = -3, // team switching etc
	WEAPON_SELF = -2, // console kill command
	WEAPON_WORLD = -1, // death tiles etc
};

/* INFECTION MODIFICATION START ***************************************/
enum
{
	FREEZEREASON_FLASH = 0,
	FREEZEREASON_UNDEAD = 1
};

#define GHOST_RADIUS 11
#define GHOST_SEARCHMAP_SIZE (2*GHOST_RADIUS+1)

enum
{
	TAKEDAMAGEMODE_NOINFECTION=0,
	TAKEDAMAGEMODE_INFECTION,
};

enum
{
	GIFT_HEROFLAG=0,
};
/* INFECTION MODIFICATION END *****************************************/

class CCharacter : public CEntity
{
	MACRO_ALLOC_POOL_ID()

public:
	//character's size
	static const int ms_PhysSize = 28;

	CCharacter(CGameWorld *pWorld);

	virtual void Reset();
	virtual void Destroy();
	virtual void Tick();
	virtual void TickDefered();
	virtual void TickPaused();
	virtual void Snap(int SnappingClient);

	bool IsGrounded();

	void SetWeapon(int W);
	void HandleWeaponSwitch();
	void DoWeaponSwitch();

	void HandleWeapons();
	void HandleNinja();
	
	void OnPredictedInput(CNetObj_PlayerInput *pNewInput);
	void OnDirectInput(CNetObj_PlayerInput *pNewInput);
	void ResetInput();
	void FireWeapon();

	void Die(int Killer, int Weapon);
	bool TakeDamage(vec2 Force, int Dmg, int From, int Weapon, int Mode);

	bool Spawn(class CPlayer *pPlayer, vec2 Pos);
	bool Remove();
	bool InCrafted();
	bool InQuest();
	bool IncreaseHealth(int Amount);
	bool IncreaseArmor(int Amount);
	bool IncreaseOverallHp(int Amount);

	bool GiveWeapon(int Weapon, int Ammo, bool GetAmmo = false);

	void SetEmote(int Emote, int Tick);

	bool IsAlive() const { return m_Alive; }
	class CPlayer *GetPlayer() { return m_pPlayer; }
	
	void GiveNinjaBuf();
	bool ItemSucc();

	int EmoteNormal;
	int m_ActiveWeapon;

private:

	bool m_Alive;

	// weapon info
	CEntity *m_apHitObjects[10];
	int m_NumObjectsHit;

	struct WeaponStat
	{
		int m_AmmoRegenStart;
		int m_Ammo;
		int m_Ammocost;
		bool m_Got;

	} m_aWeapons[NUM_WEAPONS];

	int m_LastWeapon;
	int m_QueuedWeapon;
	int HPRegenTick;

	bool m_AntiPVP;

	int m_ReloadOther;
	int m_ReloadTimer;
	int m_AttackTick;
	int m_DamageTaken;

	int m_EmoteType;
	int m_EmoteStop;

	// last tick that the player took any action ie some input
	int m_LastAction;
	int m_LastNoAmmoSound;

	int m_DamageTakenTick;

/* INFECTION MODIFICATION START ***************************************/
	//Dart
	int m_DartLifeSpan;
	vec2 m_DartDir;
	int m_DartLeft;
	int m_DartOldVelAmount;
	
	int m_WaterJumpLifeSpan;
	bool HookPass;
	vec2 HookPosGet;
	
	// the player core for the physics
public:
	// player controlling this character
	class CPlayer *m_pPlayer;


	CCharacterCore m_Core;
	
	// these are non-heldback inputs
	CNetObj_PlayerInput m_LatestPrevInput;
	CNetObj_PlayerInput m_LatestInput;

	// input
	CNetObj_PlayerInput m_PrevInput;
	CNetObj_PlayerInput m_Input;
	
	int m_NumInputs;
	int m_Jumped;
	int m_Health;
	int m_Armor;

private:
	// info for dead reckoning
	int m_ReckoningTick; // tick that we are performing dead reckoning From
	CCharacterCore m_SendCore; // core that we should send
	CCharacterCore m_ReckoningCore; // the dead reckoning core

private:
	int m_AirJumpCounter;
	int m_HookDmgTick;
	int m_InvisibleTick;
	bool m_IsInvisible;
	int m_HealTick;
	int m_BonusTick;
	
	int m_HeartID;
	int m_AntiFireTick;
	
	bool m_InQuest;
	bool m_InCrafted;
	bool m_IsFrozen;
	int m_FrozenTime;
	int m_InAirTick;
	
	char m_GhostSearchMap[GHOST_SEARCHMAP_SIZE*GHOST_SEARCHMAP_SIZE];
	
	vec2 m_SpawnPosition;

public:
	int m_PositionLockTick;
	bool m_PositionLocked;
	bool m_PositionLockAvailable;
	int m_SlipperyTick;
	int m_PoisonTick;
	int m_Poison;
	int m_PoisonFrom;
	int m_HookMode;
	int m_InWater;
	int m_NinjaVelocityBuff;
	int m_NinjaStrengthBuff;
	int m_NinjaAmmoBuff;
	int m_RefreshTime;
	bool InShop;
	bool InWork;

public:
	void DestroyChildEntities();
	void ClassSpawnAttributes();
	void OpenClassChooser();
	int GetClass();
	void SetClass(int ClassChoosed);
	void RemoveAllGun();
	void RemoveGun(int WEAPONID);
	void Freeze(float Time);
	bool IsFrozen() const;
	void Unfreeze();
	void Poison(int Count, int From);
	void SlipperyEffect();
	int GetInfWeaponID(int WID);
	void UpdateTuningParam();
	bool FindPortalPosition(vec2 Pos, vec2& Res);
	void SaturateVelocity(vec2 Force, float MaxSpeed);
	void CreateDropItem(int ItemID, int Count, int HowID, int Enchant = 0);
	void CreateDropRandom(int ItemID, int Count, int Random, int HowID, vec2 Force);
	void TakeItemChar(int ClientID);
	void ParseEmoticionButton(int ClientID, int Emtion);
	void PressF3orF4(int ClientID, int Vote);
	void CreatePickupDraw(int Num, int Type, int SubType, bool Changing);
	void DeleteAllPickup();
	vec2 LockBotPos;
/* INFECTION MODIFICATION END *****************************************/
};

#endif
