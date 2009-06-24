///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gpe_window_fb__
#define __gpe_window_fb__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GPEWindow_fb
///////////////////////////////////////////////////////////////////////////////
class GPEWindow_fb : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
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
		virtual void OnMenuSave( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuSaveAs( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuImportGDS( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuExit( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListBoxLayersClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListBoxLayersToggled( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonAddClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonRemoveClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLayerChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnColourChangedLayer( wxColourPickerEvent& event ){ event.Skip(); }
		virtual void OnLayerChangeSpin( wxSpinEvent& event ){ event.Skip(); }
		virtual void OnButtonApply( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		GPEWindow_fb( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GDS2X Process Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,500 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		~GPEWindow_fb();
	
};

#endif //__gpe_window_fb__
