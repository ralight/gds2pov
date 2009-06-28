///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gpe_window_fb__
#define __gpe_window_fb__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/checklst.h>
#include <wx/textctrl.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GPEAboutDialog_fb
///////////////////////////////////////////////////////////////////////////////
class GPEAboutDialog_fb : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText18;
		
		wxStaticText* m_staticText10;
		wxStaticText* m_staticText11;
		
		wxStaticText* m_staticText12;
		wxButton* m_buttonOk;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonOk( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		GPEAboutDialog_fb( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 291,257 ), long style = wxDEFAULT_DIALOG_STYLE );
		~GPEAboutDialog_fb();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GPEWindow_fb
///////////////////////////////////////////////////////////////////////////////
class GPEWindow_fb : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menuFile;
		wxMenu* m_menuHelp;
		wxCheckListBox* m_checkListBoxLayers;
		wxButton* m_buttonUp;
		wxButton* m_buttonDown;
		wxButton* m_buttonAdd;
		wxButton* m_buttonRemove;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlName;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrlLayer;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrlDatatype;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_textCtrlHeight;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrlThickness;
		wxStaticText* m_staticText7;
		wxColourPickerCtrl* m_colourPickerLayer;
		wxStaticText* m_staticText81;
		wxSpinCtrl* m_spinCtrlTransparency;
		
		wxCheckBox* m_checkBoxMetal;
		
		wxButton* m_buttonApply;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseEvent( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnMenuFileNew( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuFileOpen( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuFileSave( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuFileSaveAs( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuFileImportGDS( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuFileExit( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuHelpAbout( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListBoxLayersClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListBoxLayersToggled( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonUp( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonDown( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonAdd( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonRemove( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLayerChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnColourChangedLayer( wxColourPickerEvent& event ){ event.Skip(); }
		virtual void OnLayerChangeSpin( wxSpinEvent& event ){ event.Skip(); }
		virtual void OnButtonApply( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		GPEWindow_fb( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GDS2X Process Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,500 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		~GPEWindow_fb();
	
};

#endif //__gpe_window_fb__
