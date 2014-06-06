// 15.02.06
// ��� �����? ����� Lolmen
// ��� �����? ���! :D
// ���� ��� ����������� ��������� ������� � ��������� �� ��������
// ������ ���������� Nashalife :)
// ��� ����������� ��� ������������ �� ������ ��������� :)
#include "cbase.h" // ������� ���� ��� �� ������
#include "basehlcombatweapon.h" // ��������� ����� �������� ����� ��� �����
#include "NPCevent.h" // ��� NPC
#include "basecombatcharacter.h" // ��� ������
#include "AI_BaseNPC.h" // ��� AI
#include "player.h" // ��� �����
#include "game.h" // ����
#include "in_buttons.h" // ��� ������ ����
#include "AI_Memory.h" // ��� AI ������
#include "soundent.h" // �� ����� ����� ��� ���� �����
// ���������� ��� � ���� ������
// ����� ��������47 ����� ��� ���� ����� ��������� :)
class CWeaponDBGUN : public CHLSelectFireMachineGun
{
DECLARE_DATADESC();
public:
DECLARE_CLASS( CWeaponDBGUN, CHLSelectFireMachineGun );

CWeaponDBGUN();
DECLARE_SERVERCLASS();

void Precache( void ); // ���������
void AddViewKick( void ); // ��� ��� ����� ������ ��� ��������
void SecondaryAttack( void ); // ��������� �����
void ItemPostFrame( void ); // ������ �� ������ ����
int GetMinBurst() { return 2; } // ����� ����������� �������
int GetMaxBurst() { return 6; } // ����� ������������ �������

virtual void Equip( CBaseCombatCharacter *pOwner ); // ����� NPC ������� ������
bool Reload( void ); // ��������� ��� BOOL ������������� ��� �����������

float GetFireRate( void ) { return 0.9f; } // �������� ��������
int CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

virtual const Vector& GetBulletSpread( void ) // ����� ����� ��������
{
static const Vector cone = VECTOR_CONE_15DEGREES; // ����� �������� �� ����
return cone;
}

const WeaponProficiencyInfo_t *GetProficiencyValues(); // ����������

void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
switch( pEvent->event )
{
case EVENT_WEAPON_SHOTGUN_FIRE: // ���� ������ ��� �� ����� ����� SMG1 ��� NPC
{
Vector vecShootOrigin, vecShootDir; // ����� �������� �������
QAngle angDiscard;
if ((pEvent->options == NULL) || (pEvent->options[0] == '\0') || (!pOperator->GetAttachment(pEvent->options, vecShootOrigin, angDiscard)))
{
vecShootOrigin = pOperator->Weapon_ShootPosition(); // ������� ������ �������� ����
}

CAI_BaseNPC *npc = pOperator->MyNPCPointer(); // ����� NPC
ASSERT( npc != NULL ); // ���� NPC ���� �� ��� ���

vecShootDir = npc->GetActualShootTrajectory( vecShootOrigin ); // ���� NPC ��������

WeaponSoundRealtime( SINGLE_NPC ); // ���� ��� NPC

CSoundEnt::InsertSound( SOUND_COMBAT, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator ); // ��������� �����
pOperator->FireBullets( 1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED,
MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2, entindex(), 0 ); //��� ����� NPC ��������
pOperator->DoMuzzleFlash(); // ����� ������� ������� �� ���������
m_iClip1 = m_iClip1 - 1; // ������� �������� ������ �� ���� �������
}
break;

default:
BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
break;
}
}

DECLARE_ACTTABLE();
};
// ������ ������������ ������� �� ������
IMPLEMENT_SERVERCLASS_ST( CWeaponDBGUN, DT_WeaponDBGUN )
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_dbgun, CWeaponDBGUN );
PRECACHE_WEAPON_REGISTER(weapon_dbgun);

BEGIN_DATADESC( CWeaponDBGUN )

END_DATADESC()
#ifndef CLIENT_DLL
acttable_t	CWeaponDBGUN::m_acttable[] = 
{
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_SHOTGUN,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_SHOTGUN,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_SHOTGUN,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_SHOTGUN,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_SHOTGUN,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_SHOTGUN,		false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_SHOTGUN,					false },
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_SHOTGUN,				false },
};

IMPLEMENT_ACTTABLE(CWeaponDBGUN);

#endif

//=========================================================
CWeaponDBGUN::CWeaponDBGUN( )
{
m_bReloadsSingly = true;

	/*m_bNeedPump		= false;
	m_bDelayedFire1 = false;
	m_bDelayedFire2 = false;*/

	m_fMinRange1		= 0.0;
	m_fMaxRange1		= 500;
	m_fMinRange2		= 0.0;
	m_fMaxRange2		= 200;
}
//-----------------------------------------------------------------------------
// ��� ������? : ����������
//-----------------------------------------------------------------------------
void CWeaponDBGUN::Precache( void )
{
PrecacheModel( "custom/weapons/dbgun/v_shot_xm1014.mdl" );
PrecacheModel( "custom/weapons/dbgun/w_shot_xm1014.mdl" );
// ��� ����������? ������ ������ �������������� ���������
// �� ���� �� ������ :)
BaseClass::Precache();
}
//-----------------------------------------------------------------------------
// ��� ������? : ���� NPC ����� ������� ������ ������ ����.
//-----------------------------------------------------------------------------
void CWeaponDBGUN::Equip( CBaseCombatCharacter *pOwner )
{
if( pOwner->Classify() == CLASS_PLAYER_ALLY ) // ���� ����� � ������ ��
{
m_fMaxRange1 = 3000; // ������������ ���� ����
}
else // ������ ��� ���� ��� �� � ������ ���� ���� �����
{
m_fMaxRange1 = 1400; // ����� 1400
}

BaseClass::Equip( pOwner );
}
//-----------------------------------------------------------------------------
// ��� ������? ������ ��� ��� �����������
//-----------------------------------------------------------------------------
bool CWeaponDBGUN::Reload( void )
{
bool fRet; // �������������
float fCacheTime = m_flNextSecondaryAttack; // ����� ��� �����������

fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD ); // ��� ������������? ������ 1 ������ 2 � ����� ������ ����������� ACT_VM_RELOAD
if ( fRet ) // ���� ������������� fRet ������� , ��
{
// ����������� ������ � ��������� ����� ����� ������ �����
// ����������� �������� ����� �� ��������������
m_flNextSecondaryAttack = GetOwner()->m_flNextAttack = fCacheTime;

WeaponSound( RELOAD ); // ���� �� ������� ��������� RELOAD
}

return fRet; // �������� fRet
}
//-----------------------------------------------------------------------------
// ��� ������? ��������� ����������� ������ ��� ��������
//-----------------------------------------------------------------------------
void CWeaponDBGUN::AddViewKick( void )
{
#define EASY_DAMPEN 10.0f // ������ ������������
#define MAX_VERTICAL_KICK 50.0f //���� ������������ ������ � ��������
#define SLIDE_LIMIT 3.0f // ��� ��� ����� ����� ������ �� ������� �� ������

//���� ������ �����?
CBasePlayer *pPlayer = ToBasePlayer( GetOwner() ); // ������ :D

if ( pPlayer == NULL )
return; // ���� ������ � ��� ���� �� � ������ �� �����!

DoMachineGunKick( pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT ); // � ��� ��� ��� �������������� ��� ��� �� �������
}
//-----------------------------------------------------------------
// �� ������? ��������� �� ������ �� ������ �� ������ ���� :)
// ��������� ����� � ����� ���������� �� ������� ��� �� �������
// � ��� ��� ������, ���� �� ������ �� ������ ����,
// ������� � ��� ����� � ���������� ��� ��������� �����?
//-----------------------------------------------------------------
void CWeaponDBGUN::ItemPostFrame( void )
{
// ���� ������ ���� �� ��� ��� :)
CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
// ���� ������� ���� �� ����� ���� �� ������ :)
if ( pOwner == NULL )
return;
// ���� ������ ����� ������ ATTACK2(��������� ������ ������ ����)
// �� ����� ��������� ����� ������ ���� ����� �������� �������
if ( pOwner->m_nButtons & IN_ATTACK2 )
{
if (m_flNextSecondaryAttack <= gpGlobals->curtime)
{
SecondaryAttack(); // � ��� �� ������� ������� ��������� �������� ������
pOwner->m_nButtons &= ~IN_ATTACK2; // � ��� ���� ������� ������ ATTACK2
return;// �� ����� ����� ���� ������ :)
}
}

BaseClass::ItemPostFrame(); // ���������� � ������� ����� ����
}
//-----------------------------------------------------------------------------
// ��� ������? ���������� ��� � ��� ����� ��������� ������ :)
//-----------------------------------------------------------------------------
void CWeaponDBGUN::SecondaryAttack( void )
{
// � ��� ��� ���� ����� ���� �� ����� ������� :
// �����������,������� �� ����������,���� ������ � ��
}
//------------------------------------------------------------------
// ��� ������? ��� ����� ���������� ����� "���������� ������"
// ������ ��� ���������� ��������� ���� ��� �����
// ���� �������� � ��������� ����������� ������� �� ��� �� ������� ������
// � ������ �������? �� ����� ������ ����� :)
// ��� ����� ����� ������� ��� SMG :)
//-----------------------------------------------------------------
const WeaponProficiencyInfo_t *CWeaponDBGUN::GetProficiencyValues()
{
static WeaponProficiencyInfo_t proficiencyTable[] =
{
{ 8.0, 0.75 },
{ 6.00, 0.75 },
{ 10.0/2.0, 0.75 },
{ 5.0/3.0, 0.75 },
{ 2.00, 1.0 },
};

COMPILE_TIME_ASSERT( ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1); // � ��� ��� �������� ����� ������� ����� :)

return proficiencyTable;
}