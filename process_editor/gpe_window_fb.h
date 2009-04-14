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
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
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
		wxTextCtrl* m_textCtrl1;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrl3;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrl4;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrl5;
		wxCheckBox* m_checkBox1;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxColourPickerCtrl* m_colourPicker1;
		wxStaticText* m_staticText8;
		wxCheckBox* m_checkBox2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuImportGDS( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListBoxLayersClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListBoxLayersToggled( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonAddClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonRemoveClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnColourChangedLayer( wxColourPickerEvent& event ){ event.Skip(); }
		
	
	public:
		GPEWindow_fb( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GDS2X Process Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,400 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		~GPEWindow_fb();
	
};

#endif //__gpe_window_fb__
