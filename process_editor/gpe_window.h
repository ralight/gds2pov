#ifndef __gpe_window__
#define __gpe_window__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/frame.h>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gpe_window_fb.h"

///////////////////////////////////////////////////////////////////////////////
/// Class GPEWindow
///////////////////////////////////////////////////////////////////////////////
class GPEWindow : public GPEWindow_fb
{
	private:
		GDSConfig *m_config;
		GDSProcess *m_process;
		wxString m_process_path;
		wxString m_defaultSaveDir;
		wxString m_defaultSaveFile;
		bool m_layerIsDirty;
		bool m_fileIsDirty;
		int m_selectedLayer;
	
	protected:
		
		bool DoFileSave( bool forceNewFilename );
		void OnButtonApply( wxCommandEvent& event );
		void OnCheckListBoxLayersClick( wxCommandEvent& event );
		void OnCheckListBoxLayersToggled( wxCommandEvent& event );
		void OnCloseEvent( wxCloseEvent& event );
		void OnColourChangedLayer( wxColourPickerEvent& event );
		void OnLayerChange( wxCommandEvent& event );
		void OnLayerChangeSpin( wxSpinEvent& event );
		void OnMenuFileExit( wxCommandEvent& event );
		void OnMenuFileImportGDS( wxCommandEvent& event );
		void OnMenuFileNew( wxCommandEvent& event );
		void OnMenuFileOpen( wxCommandEvent& event );
		void OnMenuFileSave( wxCommandEvent& event );
		void OnMenuFileSaveAs( wxCommandEvent& event );
		void OnMenuHelpAbout( wxCommandEvent& event );
		void SaveLayer(int number);
		void SetLayerDirtyState(bool state);
		bool TryFileSave(void);

	public:
		GPEWindow();
		~GPEWindow();
};

#endif //__gpe_window__
