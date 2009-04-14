#include "gpe_app.h"
#include "gpe_window.h"

bool gpe_app::OnInit()
{
	GPEWindow *frame = new GPEWindow();
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
}

IMPLEMENT_APP(gpe_app)
