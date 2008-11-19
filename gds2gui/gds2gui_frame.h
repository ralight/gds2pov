#ifndef GDS2GUI_FRAME_H
#define GDS2GUI_FRAME_H

#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/statbmp.h>
#include <wx/treectrl.h>

enum {
	ID_CellTree = 1,
	ID_Exit,
	ID_Map,
};

class Gds2GuiFrame : public wxFrame
{
private:
	wxTreeCtrl *m_celltree;
	wxButton *buttonAcquire;
	wxComboBox *comboMap;
	wxImage *image;
	wxGLCanvas *glcanvas;
	bool acquiring;
	int *red_map, *green_map, *blue_map;

	void InitData(void);
	void DrawFrame(void);

public:
	Gds2GuiFrame(const wxString& title);

	void OnAcquire(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnInitDialog(wxInitDialogEvent& event);
	void OnMapCombo(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnThreadEnd(wxCommandEvent& event);
	void OnDataAcquisition(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif
