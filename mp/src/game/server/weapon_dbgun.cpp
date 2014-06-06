// 15.02.06
// Кто кодил? Кодил Lolmen
// Как кодил? Сам! :D
// Весь код принадлежит протасову виталию и отправлен на изучение
// людьми сообщества Nashalife :)
// Вам разрешается его использовать по полной программе :)
#include "cbase.h" // засунем базу без неё никуда
#include "basehlcombatweapon.h" // определим какие свойства юзать бум юзать
#include "NPCevent.h" // для NPC
#include "basecombatcharacter.h" // для игрока
#include "AI_BaseNPC.h" // для AI
#include "player.h" // сам игрок
#include "game.h" // игра
#include "in_buttons.h" // для кнопок мыши
#include "AI_Memory.h" // для AI память
#include "soundent.h" // хз вродь звуки или чёто такое
// Обозначили все в своём классе
// Класс оружиеАк47 юзаем для него класс Автоматов :)
class CWeaponDBGUN : public CHLSelectFireMachineGun
{
DECLARE_DATADESC();
public:
DECLARE_CLASS( CWeaponDBGUN, CHLSelectFireMachineGun );

CWeaponDBGUN();
DECLARE_SERVERCLASS();

void Precache( void ); // подгрузка
void AddViewKick( void ); // как нас будет трясти при стрельбе
void SecondaryAttack( void ); // вторичная атака
void ItemPostFrame( void ); // оружие на каждый кадр
int GetMinBurst() { return 2; } // взять минимальный разброс
int GetMaxBurst() { return 6; } // взять максимальный разброс

virtual void Equip( CBaseCombatCharacter *pOwner ); // когда NPC снабжен пушкой
bool Reload( void ); // обозначим тип BOOL переключатель для перезарядки

float GetFireRate( void ) { return 0.9f; } // скорость стрельбы
int CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

virtual const Vector& GetBulletSpread( void ) // берем конус разброса
{
static const Vector cone = VECTOR_CONE_15DEGREES; // конус разброса от дула
return cone;
}

const WeaponProficiencyInfo_t *GetProficiencyValues(); // приоритеты

void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
switch( pEvent->event )
{
case EVENT_WEAPON_SHOTGUN_FIRE: // Дает понять что мы юзаем опции SMG1 для NPC
{
Vector vecShootOrigin, vecShootDir; // дадим названия вкторам
QAngle angDiscard;
if ((pEvent->options == NULL) || (pEvent->options[0] == '\0') || (!pOperator->GetAttachment(pEvent->options, vecShootOrigin, angDiscard)))
{
vecShootOrigin = pOperator->Weapon_ShootPosition(); // позиция откуда вылетают пули
}

CAI_BaseNPC *npc = pOperator->MyNPCPointer(); // найдём NPC
ASSERT( npc != NULL ); // если NPC нету то ждём его

vecShootDir = npc->GetActualShootTrajectory( vecShootOrigin ); // куда NPC стрелять

WeaponSoundRealtime( SINGLE_NPC ); // звук для NPC

CSoundEnt::InsertSound( SOUND_COMBAT, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator ); // параметры звука
pOperator->FireBullets( 1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED,
MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2, entindex(), 0 ); //чем будет NPC стрелять
pOperator->DoMuzzleFlash(); // Будет мерцать Вспышка от выстрелов
m_iClip1 = m_iClip1 - 1; // сколько патронов займем за один выстрел
}
break;

default:
BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
break;
}
}

DECLARE_ACTTABLE();
};
// начали использовать функции из класса
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
// Что делаем? : Подгружаем
//-----------------------------------------------------------------------------
void CWeaponDBGUN::Precache( void )
{
PrecacheModel( "custom/weapons/dbgun/v_shot_xm1014.mdl" );
PrecacheModel( "custom/weapons/dbgun/w_shot_xm1014.mdl" );
// что подгружаем? ничего просто инициализируем подгрузку
// но ниче не грузим :)
BaseClass::Precache();
}
//-----------------------------------------------------------------------------
// Что делаем? : даем NPC более длинный маштаб полета пули.
//-----------------------------------------------------------------------------
void CWeaponDBGUN::Equip( CBaseCombatCharacter *pOwner )
{
if( pOwner->Classify() == CLASS_PLAYER_ALLY ) // если пушка у игрока то
{
m_fMaxRange1 = 3000; // максммальный полёт пули
}
else // потому что пока она не у игрока полёт пули равен
{
m_fMaxRange1 = 1400; // равен 1400
}

BaseClass::Equip( pOwner );
}
//-----------------------------------------------------------------------------
// Что делаем? Делаем все для перезарядки
//-----------------------------------------------------------------------------
bool CWeaponDBGUN::Reload( void )
{
bool fRet; // переключатель
float fCacheTime = m_flNextSecondaryAttack; // время для перезарядки

fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD ); // что перезаряжаем? обойму 1 обойму 2 и юзаем анимку перезарядки ACT_VM_RELOAD
if ( fRet ) // если переключатель fRet включён , то
{
// Перекрываем доступ к вторичной атаке чтобы небыло багов
// Неразрешено стрелять когда мы перезаряжаемся
m_flNextSecondaryAttack = GetOwner()->m_flNextAttack = fCacheTime;

WeaponSound( RELOAD ); // звук из скрипта проиграем RELOAD
}

return fRet; // возратим fRet
}
//-----------------------------------------------------------------------------
// Что делаем? Добовляем потясывание экрана при стрельбе
//-----------------------------------------------------------------------------
void CWeaponDBGUN::AddViewKick( void )
{
#define EASY_DAMPEN 10.0f // легкое подергивание
#define MAX_VERTICAL_KICK 50.0f //макс вертикальный наклон в градусах
#define SLIDE_LIMIT 3.0f // всю эту батву будем делать за столько то секунд

//Кого трясти будем?
CBasePlayer *pPlayer = ToBasePlayer( GetOwner() ); // игрока :D

if ( pPlayer == NULL )
return; // если игрока у нас нету то и трясти не будем!

DoMachineGunKick( pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT ); // а вот это уже инициализирует все что мы набрали
}
//-----------------------------------------------------------------
// Че делаем? Проверяем не нажали ли геймер по кнопке мыши :)
// Первичный огонь у пушки вызывается по дефолту его не трогаем
// А вот что делать, если мы нажали по правой мыше,
// которая у нас стоит в настройках как вторичная атака?
//-----------------------------------------------------------------
void CWeaponDBGUN::ItemPostFrame( void )
{
// Если геймер есть то все ока :)
CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
// Если геймера нету то пипец ниче не делаем :)
if ( pOwner == NULL )
return;
// Если геймер нажал кнопку ATTACK2(подефолту правая кнопка мыши)
// то время вторичной атаки меньше либо равно текущему времени
if ( pOwner->m_nButtons & IN_ATTACK2 )
{
if (m_flNextSecondaryAttack <= gpGlobals->curtime)
{
SecondaryAttack(); // и тут мы вызовем функцию вторичной стрельбы оружия
pOwner->m_nButtons &= ~IN_ATTACK2; // а вот если ненажал кнопку ATTACK2
return;// то будем ждать пока нажмет :)
}
}

BaseClass::ItemPostFrame(); // используем и базовый класс тоже
}
//-----------------------------------------------------------------------------
// Что делаем? определяем что у нас будет вторичной Атакой :)
//-----------------------------------------------------------------------------
void CWeaponDBGUN::SecondaryAttack( void )
{
// А вот тут пока пусто сюда мы можем сделать :
// Приближение,Выстрел из подстволки,Удар штыком и тд
}
//------------------------------------------------------------------
// Что делаем? Опа самая непонятная часть "приоритеты оружия"
// Короче это определяет насколько крут наш ствол
// Если например в пистолете закончились патроны на что же сменить оружие
// В первую очередь? На самый крутой ствол :)
// Наш ствол будет покруче чем SMG :)
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

COMPILE_TIME_ASSERT( ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1); // а это нам сократит время компила пушки :)

return proficiencyTable;
}