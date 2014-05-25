//==============================================================================//
//
// Purpose: 
// 
//==============================================================================//

#include "cbase.h"
#include "rz_panel_pistols.h"
#include <vgui/IVGui.h>

using namespace vgui;


CRZPistols::CRZPistols(IViewPort *pViewPort ) : Frame(NULL, PANEL_RZ_PISTOLS)
{
	m_pViewPort = pViewPort;

	SetTitle("", true);

	SetScheme("ClientScheme");
	SetMoveable(false);
	SetSizeable(false);


	SetTitleBarVisible(false);
	SetProportional(true);

	ivgui()->AddTickSignal( GetVPanel(), 100);

	LoadControlSettings("Resource/UI/RZ_Pistols.res");
	InvalidateLayout();
}

CRZPistols::~CRZPistols()
{

}

const char* CRZPistols::GetName()
{
	return PANEL_RZ_PISTOLS;
}

void CRZPistols::SetData(KeyValues *kvData)
{
}

void CRZPistols::Reset()
{
	// Load weapons
	// WeaponsForCombine() or WeaponsForRebels()
	WeaponsForCombines();
}

void CRZPistols::WeaponsForCombines()
{
	/*
	m_pButtonWeapon1 = new vgui::Button(this, "", "Colt", this, "give weapon_pistol");
	m_pButtonWeapon1->SetPos(10, 70);
	m_pButtonWeapon1->SetEnabled(true);
	m_pButtonWeapon1->SetVisible(true);
	m_pButtonWeapon1->SetSize(100, 25);
	//*/
	// load from .kv file
}

void CRZPistols::WeaponsForRebels()
{
	// load from .ctx or .txt file
}

void CRZPistols::SetParent( vgui::VPANEL parent )
{
	BaseClass::SetParent( parent );
}

vgui::VPANEL CRZPistols::GetVPanel()
{
	return BaseClass::GetVPanel();
}

bool CRZPistols::IsVisible()
{
	return BaseClass::IsVisible();
}

void CRZPistols::Update()
{
	
}

void CRZPistols::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	// etc
}

void CRZPistols::OnKeyCodePressed(vgui::KeyCode code)
{
	bool my_keycode_button_pressed = false;

	// check pressed 1..9
	if ( my_keycode_button_pressed )
	{
		// on press 1..9 select weapon
	}
	else
	{
		BaseClass::OnKeyCodePressed(code);
	}
}

void CRZPistols::ShowPanel( bool show )
{
	if ( BaseClass::IsVisible() == show )
		return;

	if ( show ) 
	{
		Activate();
		SetMouseInputEnabled(true);

		// test & see it

		
	}
	else
	{
		SetVisible(false);
		SetMouseInputEnabled(false);
	}

	m_pViewPort->ShowBackGround(show);
}


void CRZPistols::OnCommand( const char *command )
{
	if ( Q_stricmp( command, "vguicancel" ) )
	{
		engine->ClientCmd( const_cast<char *>(command) );
	}
	Close();
	BaseClass::OnCommand(command);
}



