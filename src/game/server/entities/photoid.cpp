/*
#include <game/server/gamecontext.h>
#include "photoid.h"

CPhotoid::CPhotoid(CGameWorld *pGameWorld, vec2 Pos, int OwnerID, int GetPlayer, vec2 InitialVel)
    : CEntity(pGameWorld, CGameWorld::ENTTYPE_PHOTOID)
{
    m_Pos = Pos;
    m_InitialVel = InitialVel;
    m_OwnerID = OwnerID;
    m_InitialAmount = 1.0f;
    GameWorld()->InsertEntity(this);
}

void CPhotoid::Tick()
{
    CPlayer *pOwner = GameServer()->m_apPlayers[m_OwnerID];
    if ((!pOwner || !pOwner->GetCharacter()))
    {
        GameServer()->m_World.DestroyEntity(this);
        return;
    }

    if ()
    {
        GameServer()->m_World.DestroyEntity(this);
        return;
    }
    
    m_Pos += m_Vel;

    m_InitialAmount *= 0.98f;
}

void CPhotoid::Snap(int SnappingClient)
{
    CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
    if (pObj)
    {
        pObj->m_X = (int)m_Pos.x;
        pObj->m_Y = (int)m_Pos.y;
        pObj->m_VelX = 0;
        pObj->m_VelY = 0;
        pObj->m_StartTick = Server()->Tick();
        pObj->m_Type = WEAPON_HAMMER;
    }
}
*/