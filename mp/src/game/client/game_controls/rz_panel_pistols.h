//==============================================================================//
//
// Purpose: 
// 
//==============================================================================//


#ifndef RZ_PANEL_PISTOLS_H
#define RZ_PANEL_PISTOLS_H

#ifdef _WIN32
#pragma once
#endif


#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>

#include <game/client/iviewport.h>

#include <vgui/KeyCode.h>
#include <utlvector.h>


class CRZPistols : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CRZPistols, vgui::Frame );

public:
	CRZPistols( IViewPort *pViewPort );
	virtual ~CRZPistols();
	virtual const char* GetName(void);
	virtual void SetData(KeyValues *kvData );
	virtual void Reset(void);
	virtual void Update();
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void );
  	virtual bool IsVisible();
	virtual void SetParent( vgui::VPANEL parent );
	
protected:
	// Loads weapons

	void WeaponsForRebels(void);
	void WeaponsForCombines(void);

	void OnCommand( const char *command );

	// VGUI2 overrides
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void OnKeyCodePressed(vgui::KeyCode code);

	IViewPort	*m_pViewPort;
	vgui::Button	*m_pButtonWeapon1;
};





#endif // RZ_PANEL_PISTOLS_H