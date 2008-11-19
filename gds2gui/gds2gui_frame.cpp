#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>

#include "gds2gui_frame.h"


BEGIN_EVENT_TABLE(Gds2GuiFrame, wxFrame)
	EVT_INIT_DIALOG(Gds2GuiFrame::OnInitDialog)
	EVT_CLOSE(Gds2GuiFrame::OnClose)
//	EVT_BUTTON(ID_Exit, Gds2GuiFrame::OnQuit)
//	EVT_BUTTON(ID_Acquire, Gds2GuiFrame::OnAcquire)
END_EVENT_TABLE()


Gds2GuiFrame::Gds2GuiFrame(const wxString& title) : wxFrame((wxFrame *)NULL, -1, title, wxDefaultPosition, wxDefaultSize)
{
	wxInitDialogEvent evt;
	OnInitDialog(evt);
}


void Gds2GuiFrame::OnClose(wxCloseEvent& WXUNUSED(event))
{
	Destroy();
}


void Gds2GuiFrame::OnInitDialog(wxInitDialogEvent& WXUNUSED(event))
{
	m_celltree = new wxTreeCtrl(this, ID_CellTree, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);

	wxBoxSizer *cellsizer = new wxBoxSizer(wxVERTICAL);

	cellsizer->Add(m_celltree, 0, wxALL, 0);

	cellsizer->SetSizeHints(this);
	SetSizer(cellsizer);
/*
	wxBoxSizer *vsizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	buttonAcquire = new wxButton(this, ID_Acquire, _T("Start Acquire"));
	hsizer->Add(buttonAcquire, 0, wxALL | wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTRE_HORIZONTAL, 5);
	hsizer->Add(new wxButton(this, ID_Exit, _T("E&xit")), 0, wxALL | wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTRE_HORIZONTAL, 5);

	//bitmap = new wxStaticBitmap(this, -1, NULL, wxDefaultPosition, wxSize(COLS*COLPIXELSIZE, ROWS*ROWPIXELSIZE), wxBORDER_SUNKEN, _T("staticBitmap"));

	glcanvas = new wxGLCanvas(this, -1, wxDefaultPosition, wxSize(200,200), 0, _T("GL Canvas"), NULL, wxNullPalette);
	
	vsizer->Add(glcanvas, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL | wxSTRETCH_NOT, 10);

	wxArrayString astr;
	astr.Add(_T("Grey"));
	astr.Add(_T("Jet"));
	astr.Add(_T("Hot"));
	astr.Add(_T("Cool"));
	astr.Add(_T("Copper"));
	astr.Add(_T("Pink"));
	astr.Add(_T("Bone"));
	astr.Add(_T("HSV"));

	comboMap = new wxComboBox(this, ID_Map, astr[0], wxDefaultPosition, wxDefaultSize, astr, wxCB_READONLY | wxCB_READONLY, wxDefaultValidator, _T("comboBox"));
	vsizer->Add(comboMap, 0,  wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTRE_HORIZONTAL, 5);
	vsizer->Add(hsizer, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 5);

	vsizer->SetSizeHints(this);
	SetSizer(vsizer);
	SetOwnBackgroundColour(wxColour(100, 0, 0, wxALPHA_OPAQUE));

	//image = new wxImage(COLS*COLPIXELSIZE, ROWS*ROWPIXELSIZE, true);

	InitData();
	DrawFrame();

	//wxBitmap bm(*image);
	//bitmap->SetBitmap(bm);
*/
}


void Gds2GuiFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}


