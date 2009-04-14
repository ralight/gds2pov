///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gpe_window_fb.h"

///////////////////////////////////////////////////////////////////////////

GPEWindow_fb::GPEWindow_fb( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItemNew;
	m_menuItemNew = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("&New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemNew );
	
	wxMenuItem* m_menuItemOpen;
	m_menuItemOpen = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("&Open...") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemOpen );
	
	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemSave );
	
	wxMenuItem* m_menuItemSaveAs;
	m_menuItemSaveAs = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Save &As...") ) + wxT('\t') + wxT("Shift+Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemSaveAs );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItemImportGDS;
	m_menuItemImportGDS = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Import GDS...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemImportGDS );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("E&xit") ) + wxT('\t') + wxT("Alt+F4"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemExit );
	
	m_menubar1->Append( m_menu1, wxT("&File") );
	
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
	gSizer2->Add( m_buttonRemove, 0, wxALL, 5 );
	
	bSizer3->Add( gSizer2, 0, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Layer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl2, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Datatype"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl3, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl4 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl4, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl5 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl5, 0, wxALL|wxEXPAND, 5 );
	
	m_checkBox1 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	
	fgSizer1->Add( m_checkBox1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourPicker1 = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer1->Add( m_colourPicker1, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer1->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_checkBox2 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer1->Add( m_checkBox2, 0, wxALL, 5 );
	
	bSizer1->Add( fgSizer1, 2, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( m_menuItemSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuSave ) );
	this->Connect( m_menuItemSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuSaveAs ) );
	this->Connect( m_menuItemImportGDS->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuImportGDS ) );
	m_checkListBoxLayers->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersClick ), NULL, this );
	m_checkListBoxLayers->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersToggled ), NULL, this );
	m_buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonAddClick ), NULL, this );
	m_buttonRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonRemoveClick ), NULL, this );
	m_colourPicker1->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GPEWindow_fb::OnColourChangedLayer ), NULL, this );
}

GPEWindow_fb::~GPEWindow_fb()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuSave ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnMenuImportGDS ) );
	m_checkListBoxLayers->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersClick ), NULL, this );
	m_checkListBoxLayers->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( GPEWindow_fb::OnCheckListBoxLayersToggled ), NULL, this );
	m_buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonAddClick ), NULL, this );
	m_buttonRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GPEWindow_fb::OnButtonRemoveClick ), NULL, this );
	m_colourPicker1->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GPEWindow_fb::OnColourChangedLayer ), NULL, this );
}
