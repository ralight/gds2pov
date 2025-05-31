///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gpe_window_fb.h"

///////////////////////////////////////////////////////////////////////////

GPEAboutDialog_fb::GPEAboutDialog_fb( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxBORDER_SUNKEN );
	m_panel1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_staticText19 = new wxStaticText( m_panel1, wxID_ANY, wxT("GPE"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	m_staticText19->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );

	bSizer4->Add( m_staticText19, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxSHAPED, 5 );

	m_staticText18 = new wxStaticText( m_panel1, wxID_ANY, wxT("Process file editor for gds2pov"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer4->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer4->Add( 0, 20, 0, wxEXPAND, 5 );

	m_staticText10 = new wxStaticText( m_panel1, wxID_ANY, wxT("Developed by:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	m_staticText10->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );

	bSizer4->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticText11 = new wxStaticText( m_panel1, wxID_ANY, wxT("Roger Light"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer4->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer4->Add( 0, 20, 0, wxEXPAND, 5 );

	m_staticText12 = new wxStaticText( m_panel1, wxID_ANY, wxT("Homepage:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );

	bSizer4->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panel1->SetSizer( bSizer4 );
	m_panel1->Layout();
	bSizer4->Fit( m_panel1 );
	bSizer3->Add( m_panel1, 1, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_buttonOk = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );

	m_buttonOk->SetDefault();
	bSizer3->Add( m_buttonOk, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();

	// Connect Events
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEAboutDialog_fb::OnButtonOk ), NULL, this );
}

GPEAboutDialog_fb::~GPEAboutDialog_fb()
{
	// Disconnect Events
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEAboutDialog_fb::OnButtonOk ), NULL, this );

}

GPEWindow_fb::GPEWindow_fb( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemNew;
	m_menuItemNew = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("&New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemNew );

	wxMenuItem* m_menuItemOpen;
	m_menuItemOpen = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("&Open...") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemOpen );

	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSave );

	wxMenuItem* m_menuItemSaveAs;
	m_menuItemSaveAs = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("Save &As...") ) + wxT('\t') + wxT("Shift+Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSaveAs );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuItemImportGDS;
	m_menuItemImportGDS = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("Import GDS...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemImportGDS );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("E&xit") ) + wxT('\t') + wxT("Alt+F4"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemExit );

	m_menubar1->Append( m_menuFile, wxT("&File") );

	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuHelp, wxID_ANY, wxString( wxT("&About...") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemAbout->SetBitmaps( wxNullBitmap );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemAbout->SetBitmap( wxNullBitmap );
	#endif
	m_menuHelp->Append( m_menuItemAbout );

	m_menubar1->Append( m_menuHelp, wxT("&Help") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxArrayString m_checkListBoxLayersChoices;
	m_checkListBoxLayers = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkListBoxLayersChoices, 0 );
	bSizer3->Add( m_checkListBoxLayers, 1, wxALL|wxEXPAND, 5 );

	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 2, 2, 0, 0 );

	m_buttonUp = new wxButton( this, wxID_ANY, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonUp->Enable( false );

	gSizer2->Add( m_buttonUp, 0, wxALL, 5 );

	m_buttonDown = new wxButton( this, wxID_ANY, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonDown->Enable( false );

	gSizer2->Add( m_buttonDown, 0, wxALL, 5 );

	m_buttonAdd = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_buttonAdd, 0, wxALL, 5 );

	m_buttonRemove = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonRemove->Enable( false );

	gSizer2->Add( m_buttonRemove, 0, wxALL, 5 );


	bSizer3->Add( gSizer2, 0, wxEXPAND, 5 );


	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlName->Enable( false );

	fgSizer1->Add( m_textCtrlName, 1, wxALL|wxEXPAND, 5 );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Layer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlLayer = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlLayer->Enable( false );

	fgSizer1->Add( m_textCtrlLayer, 1, wxALL|wxEXPAND, 5 );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Datatype"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlDatatype = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlDatatype->Enable( false );

	fgSizer1->Add( m_textCtrlDatatype, 1, wxALL|wxEXPAND, 5 );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Y-Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer1->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlHeight = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlHeight->Enable( false );

	fgSizer1->Add( m_textCtrlHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlThickness = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlThickness->Enable( false );

	fgSizer1->Add( m_textCtrlThickness, 0, wxALL|wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_colourPickerLayer = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_colourPickerLayer->Enable( false );

	fgSizer1->Add( m_colourPickerLayer, 0, wxALL, 5 );

	m_staticText81 = new wxStaticText( this, wxID_ANY, wxT("Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	fgSizer1->Add( m_staticText81, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_spinCtrlTransparency = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	m_spinCtrlTransparency->Enable( false );

	fgSizer1->Add( m_spinCtrlTransparency, 0, wxALL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	m_checkBoxMetal = new wxCheckBox( this, wxID_ANY, wxT("Metal Layer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxMetal->Enable( false );

	fgSizer1->Add( m_checkBoxMetal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonApply = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonApply->Enable( false );

	fgSizer1->Add( m_buttonApply, 0, wxALL, 5 );


	bSizer1->Add( fgSizer1, 2, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GPEWindow_fb::OnCloseEvent ) );
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuFileNew ), this, m_menuItemNew->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuFileOpen ), this, m_menuItemOpen->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuFileSave ), this, m_menuItemSave->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuFileSaveAs ), this, m_menuItemSaveAs->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuFileImportGDS ), this, m_menuItemImportGDS->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuFileExit ), this, m_menuItemExit->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuHelpAbout ), this, m_menuItemAbout->GetId());
	m_checkListBoxLayers->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersClick ), NULL, this );
	m_checkListBoxLayers->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersToggled ), NULL, this );
	m_buttonUp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonUp ), NULL, this );
	m_buttonDown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonDown ), NULL, this );
	m_buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonAdd ), NULL, this );
	m_buttonRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonRemove ), NULL, this );
	m_textCtrlName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlLayer->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlDatatype->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlHeight->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlThickness->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_colourPickerLayer->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GPEWindow_fb::OnColourChangedLayer ), NULL, this );
	m_spinCtrlTransparency->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GPEWindow_fb::OnLayerChangeSpin ), NULL, this );
	m_spinCtrlTransparency->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_checkBoxMetal->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_buttonApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonApply ), NULL, this );
}

GPEWindow_fb::~GPEWindow_fb()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GPEWindow_fb::OnCloseEvent ) );
	m_checkListBoxLayers->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersClick ), NULL, this );
	m_checkListBoxLayers->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersToggled ), NULL, this );
	m_buttonUp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonUp ), NULL, this );
	m_buttonDown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonDown ), NULL, this );
	m_buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonAdd ), NULL, this );
	m_buttonRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonRemove ), NULL, this );
	m_textCtrlName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlLayer->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlDatatype->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlHeight->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_textCtrlThickness->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_colourPickerLayer->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GPEWindow_fb::OnColourChangedLayer ), NULL, this );
	m_spinCtrlTransparency->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GPEWindow_fb::OnLayerChangeSpin ), NULL, this );
	m_spinCtrlTransparency->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_checkBoxMetal->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnLayerChange ), NULL, this );
	m_buttonApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonApply ), NULL, this );

}
