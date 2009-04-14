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
	
	protected:
		
		void OnMenuImportGDS( wxCommandEvent& event );

	public:
		GPEWindow();
		~GPEWindow();
};

#endif //__gpe_window__
