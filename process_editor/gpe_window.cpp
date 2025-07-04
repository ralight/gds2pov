#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include "gdsparse.h"
#include "gpe_window.h"
#include "gpe_about.h"

GPEWindow::GPEWindow() : GPEWindow_fb(NULL)
{
	m_process = new GDSProcess();
	m_process_path = wxT("");
	m_defaultSaveDir = wxT("");
	m_defaultSaveFile = wxT("");
	m_layerIsDirty = false;
	m_fileIsDirty = false;
	m_selectedLayer = -1;
}

GPEWindow::~GPEWindow()
{
	delete m_process;
}

bool GPEWindow::DoFileSave( bool forceNewFilename )
{
	if(m_process_path == wxT("") || forceNewFilename){
		wxFileDialog *fileDialog = new wxFileDialog(this, wxT("Choose a file"),
				m_defaultSaveDir, m_defaultSaveFile, wxT("All files (*.*;*)|*.*;*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if(fileDialog->ShowModal() == wxID_OK){
			m_defaultSaveDir = fileDialog->GetDirectory();
			m_defaultSaveFile = fileDialog->GetFilename();
			m_process_path = fileDialog->GetPath();
		}else{
			return false;
		}
	}

	std::string filename;
	filename = (char *)m_process_path.char_str();
	m_process->Save(filename);
	m_fileIsDirty = false;

	return true;
}

void GPEWindow::OnButtonAdd( wxCommandEvent& event )
{
	ProcessLayer *layer;

	layer = m_process->AddLayer(0, 0);

	int item = m_checkListBoxLayers->Append(wxString::FromAscii(layer->Name.c_str()));
	m_checkListBoxLayers->Check(item, true);
}

void GPEWindow::OnButtonApply( wxCommandEvent& event )
{
	int selected = m_checkListBoxLayers->GetSelection();
	if(selected != wxNOT_FOUND){
		SaveLayer(selected);
		SetLayerDirtyState(false);
		m_fileIsDirty = true;
	}
}

void GPEWindow::OnButtonDown( wxCommandEvent& event )
{
	int selected = m_checkListBoxLayers->GetSelection();
	if(selected != wxNOT_FOUND && selected != (int)m_checkListBoxLayers->GetCount()-1){
		wxString s0, s1;
		bool b0, b1;

		s0 = m_checkListBoxLayers->GetString(selected);
		s1 = m_checkListBoxLayers->GetString(selected+1);

		b0 = m_checkListBoxLayers->IsChecked(selected);
		b1 = m_checkListBoxLayers->IsChecked(selected+1);

		m_checkListBoxLayers->SetString(selected+1, s0);
		m_checkListBoxLayers->SetString(selected, s1);

		m_checkListBoxLayers->Check(selected+1, b0);
		m_checkListBoxLayers->Check(selected, b1);

		m_process->SwapLayers(selected, selected+1);

		m_selectedLayer++;
		m_checkListBoxLayers->SetSelection(selected+1);

		if(m_selectedLayer > 0){
			m_buttonUp->Enable(true);
		}else{
			m_buttonUp->Enable(false);
		}
		if(m_selectedLayer < (int)m_checkListBoxLayers->GetCount() - 1){
			m_buttonDown->Enable(true);
		}else{
			m_buttonDown->Enable(false);
		}
	}
}

void GPEWindow::OnButtonRemove( wxCommandEvent& event )
{
	int selected = m_checkListBoxLayers->GetSelection();
	int saveSelected;

	if(selected != wxNOT_FOUND){
		m_textCtrlName->Enable(false);
		m_textCtrlLayer->Enable(false);
		m_textCtrlDatatype->Enable(false);
		m_textCtrlHeight->Enable(false);
		m_textCtrlThickness->Enable(false);
		m_colourPickerLayer->Enable(false);
		m_spinCtrlTransparency->Enable(false);
		m_checkBoxMetal->Enable(false);

		m_buttonRemove->Enable(false);

		SetLayerDirtyState(false);
		m_fileIsDirty = true;

		if(selected == (int)m_checkListBoxLayers->GetCount()-1){
			saveSelected = m_selectedLayer - 1;
		}else{
			saveSelected = m_selectedLayer;
		}
		m_process->RemoveLayer(selected);
		m_checkListBoxLayers->Delete(selected);
		m_checkListBoxLayers->SetSelection(saveSelected);
		OnCheckListBoxLayersClick(event);
	}
}

void GPEWindow::OnButtonUp( wxCommandEvent& event )
{
	int selected = m_checkListBoxLayers->GetSelection();
	if(selected != wxNOT_FOUND && selected != 0){
		wxString s0, s1;
		bool b0, b1;

		s0 = m_checkListBoxLayers->GetString(selected-1);
		s1 = m_checkListBoxLayers->GetString(selected);

		b0 = m_checkListBoxLayers->IsChecked(selected-1);
		b1 = m_checkListBoxLayers->IsChecked(selected);

		m_checkListBoxLayers->SetString(selected, s0);
		m_checkListBoxLayers->SetString(selected-1, s1);

		m_checkListBoxLayers->Check(selected, b0);
		m_checkListBoxLayers->Check(selected-1, b1);

		m_process->SwapLayers(selected, selected-1);

		m_selectedLayer--;
		m_checkListBoxLayers->SetSelection(selected-1);

		if(m_selectedLayer > 0){
			m_buttonUp->Enable(true);
		}else{
			m_buttonUp->Enable(false);
		}
		if(m_selectedLayer < (int)m_checkListBoxLayers->GetCount() - 1){
			m_buttonDown->Enable(true);
		}else{
			m_buttonDown->Enable(false);
		}
	}
}

void GPEWindow::OnCheckListBoxLayersClick( wxCommandEvent& event )
{
	if(m_layerIsDirty && m_selectedLayer >= 0 && m_selectedLayer < (int)m_checkListBoxLayers->GetCount()){
		wxMessageDialog *msgDialog = new wxMessageDialog(this, wxT("Layer modified. Apply changes?"), wxT("Warning"), wxYES_NO | wxCANCEL);
		switch(msgDialog->ShowModal()){
			case wxID_YES:
				SaveLayer(m_selectedLayer);
				break;
			case wxID_NO:
				break;
			case wxID_CANCEL:
				m_checkListBoxLayers->Select(m_selectedLayer);
				delete msgDialog;
				return;
				break;
		}
		delete msgDialog;
	}
	int selected = m_checkListBoxLayers->GetSelection();
	if(selected != wxNOT_FOUND){
		ProcessLayer *layer = m_process->GetLayer(selected);
		if(layer){
			m_textCtrlName->SetValue(wxString::FromAscii(layer->Name.c_str()));
			m_textCtrlLayer->SetValue(wxString::Format(wxT("%d"), layer->Layer));
			m_textCtrlDatatype->SetValue(wxString::Format(wxT("%d"), layer->Datatype));
			m_textCtrlHeight->SetValue(wxString::Format(wxT("%f"), layer->Height));
			m_textCtrlThickness->SetValue(wxString::Format(wxT("%f"), layer->Thickness));
			m_spinCtrlTransparency->SetValue(100 * layer->Filter);
			m_checkBoxMetal->SetValue(layer->Metal);

			m_colourPickerLayer->SetColour(wxColour(255 * layer->Red, 255 * layer->Green, 255 * layer->Blue));

			m_textCtrlName->Enable(true);
			m_textCtrlLayer->Enable(true);
			m_textCtrlDatatype->Enable(true);
			m_textCtrlHeight->Enable(true);
			m_textCtrlThickness->Enable(true);
			m_colourPickerLayer->Enable(true);
			m_spinCtrlTransparency->Enable(true);
			m_checkBoxMetal->Enable(true);

			m_buttonRemove->Enable(true);

			SetLayerDirtyState(false);
		}
	}
	m_selectedLayer = selected;
	if(selected > 0){
		m_buttonUp->Enable(true);
	}else{
		m_buttonUp->Enable(false);
	}
	if(selected < (int)m_checkListBoxLayers->GetCount() - 1){
		m_buttonDown->Enable(true);
	}else{
		m_buttonDown->Enable(false);
	}
}

void GPEWindow::OnCheckListBoxLayersToggled( wxCommandEvent& event )
{
	int selected = event.GetSelection();
	if(selected != wxNOT_FOUND){
		ProcessLayer *layer = m_process->GetLayer(selected);
		if(layer){
			layer->Show = m_checkListBoxLayers->IsChecked(selected);
		}
	}
}

void GPEWindow::OnCloseEvent( wxCloseEvent& event )
{
	bool veto;

	veto = TryFileSave();

	if(!event.CanVeto()){
		/* Must destroy the window */
		Destroy();
	}else{
		if(veto){
			event.Veto(true);
		}else{
			Destroy();
		}
	}
}

void GPEWindow::OnColourChangedLayer( wxColourPickerEvent& event )
{
	int selected = m_checkListBoxLayers->GetSelection();
	if(selected != wxNOT_FOUND){
		SetLayerDirtyState(true);
		/*
		ProcessLayer *layer = m_process->GetLayer(selected);

		wxColour colour = m_colourPickerLayer->GetColour();

		layer->Red = colour.Red() / 255.0;
		layer->Green = colour.Green() / 255.0;
		layer->Blue = colour.Blue() / 255.0;
		*/
	}
}

void GPEWindow::OnLayerChange( wxCommandEvent& event )
{
	SetLayerDirtyState(true);
}

void GPEWindow::OnLayerChangeSpin( wxSpinEvent& event )
{
	SetLayerDirtyState(true);
}

void GPEWindow::OnMenuFileExit( wxCommandEvent& event )
{
	Close();
}

void GPEWindow::OnMenuFileImportGDS( wxCommandEvent& event )
{
	wxFileDialog *fileDialog = new wxFileDialog(this);

	fileDialog->SetWildcard(wxT("GDS files (*.gds;*.gds2;*.gdsii)|*.gds;*.gds2;*.gdsii|All files (*.*;*)|*.*;*"));

	if(fileDialog->ShowModal() == wxID_OK){
		/************ Open GDS2 file and parse ****************/

		FILE *iptr;
		iptr = fopen((char *)fileDialog->GetPath().char_str(), "rb");
		if(iptr){
			m_process_path = wxT("");
			m_defaultSaveDir = wxT("");
			m_defaultSaveFile = wxT("");
			m_selectedLayer = -1;

			class GDSParse *Parser = new class GDSParse(m_process, true);
			if(!Parser->Parse(iptr)){
				ProcessLayer *layer;

				m_checkListBoxLayers->Clear();
				for(unsigned int i = 0; i < m_process->LayerCount(); i++){
					layer = m_process->GetLayer(i);
					layer->Show = true;
					int item = m_checkListBoxLayers->Append(wxString::FromAscii(layer->Name.c_str()));
					m_checkListBoxLayers->Check(item, true);
				}
				m_fileIsDirty = true;
			}else{
				wxMessageDialog *msgDialog = new wxMessageDialog(this, wxT("Unable to parse GDS file."), wxT("Error"), wxOK | wxICON_ERROR);
				msgDialog->ShowModal();
				delete msgDialog;
			}
			delete Parser;
		}else{
			wxMessageDialog *msgDialog = new wxMessageDialog(this, wxT("Unable to open GDS file."), wxT("Error"), wxOK | wxICON_ERROR);
			msgDialog->ShowModal();
			delete msgDialog;
		}
	}

	delete fileDialog;
}

void GPEWindow::OnMenuFileNew( wxCommandEvent& event )
{
	bool veto;
	veto = TryFileSave();

	if(!veto){
		if(m_process){
			delete m_process;
			m_process = NULL;
		}
		m_process = new GDSProcess();
		m_checkListBoxLayers->Clear();
		m_process_path = wxT("");
		m_defaultSaveDir = wxT("");
		m_defaultSaveFile = wxT("");
		m_selectedLayer = -1;
		m_fileIsDirty = false;

		m_textCtrlName->Enable(false);
		m_textCtrlLayer->Enable(false);
		m_textCtrlDatatype->Enable(false);
		m_textCtrlHeight->Enable(false);
		m_textCtrlThickness->Enable(false);
		m_colourPickerLayer->Enable(false);
		m_spinCtrlTransparency->Enable(false);
		m_checkBoxMetal->Enable(false);
	}
}

void GPEWindow::OnMenuFileOpen( wxCommandEvent& event )
{
	bool veto;
	veto = TryFileSave();

	if(!veto){
		if(m_process){
			delete m_process;
			m_process = NULL;
		}
		m_process = new GDSProcess();

		wxFileDialog *fileDialog = new wxFileDialog(this);

		fileDialog->SetWildcard(wxT("Process files (*.*;*)|*.*;*"));

		if(fileDialog->ShowModal() == wxID_OK){
			m_process_path.Printf(wxT("%s"), (char *)fileDialog->GetPath().char_str());

			std::string filename;

			filename = std::string(m_process_path.mb_str());

			m_process->Parse(filename);
			if(m_process->IsValid()){

				m_checkListBoxLayers->Clear();

				ProcessLayer *layer;
				for(unsigned int i = 0; i < m_process->LayerCount(); i++){
					layer = m_process->GetLayer(i);
					int item = m_checkListBoxLayers->Append(wxString::FromAscii(layer->Name.c_str()));
					m_checkListBoxLayers->Check(item, true);
				}
				m_fileIsDirty = false;
			}else{
				wxMessageDialog *msgDialog = new wxMessageDialog(this, wxT("Unable to parse process file."), wxT("Error"), wxOK | wxICON_ERROR);
				msgDialog->ShowModal();
				delete msgDialog;
			}
		}
	}
}

void GPEWindow::OnMenuFileSave( wxCommandEvent& event )
{
	DoFileSave(false);
}

void GPEWindow::OnMenuFileSaveAs( wxCommandEvent& event )
{
	DoFileSave(true);
}

void GPEWindow::OnMenuHelpAbout( wxCommandEvent& event )
{
	GPEAboutDialog *dialog = new GPEAboutDialog();
	dialog->ShowModal();
	delete dialog;
}

void GPEWindow::SaveLayer(int number)
{
	ProcessLayer *layer = m_process->GetLayer(number);

	if(layer){
		layer->Name = m_textCtrlName->GetValue().ToUTF8();

		long val;

		if(m_textCtrlLayer->GetValue().ToLong(&val)){
			layer->Layer = val;
		}else{
			// FIXME
		}
		if(m_textCtrlDatatype->GetValue().ToLong(&val)){
			layer->Datatype = val;
		}else{
			// FIXME
		}

		double dval;
		if(m_textCtrlThickness->GetValue().ToDouble(&dval)){
			layer->Thickness = dval;
		}else{
			// FIXME
		}

		if(m_textCtrlHeight->GetValue().ToDouble(&dval)){
			layer->Height = dval;
		}else{
			// FIXME
		}

		wxColour colour = m_colourPickerLayer->GetColour();


		layer->Red = colour.Red() / 255.0;
		layer->Green = colour.Green() / 255.0;
		layer->Blue = colour.Blue() / 255.0;

		layer->Filter = m_spinCtrlTransparency->GetValue() / 100.0;

		layer->Metal = m_checkBoxMetal->IsChecked();
	}
}

void GPEWindow::SetLayerDirtyState(bool state)
{
	m_buttonApply->Enable(state);
	m_layerIsDirty = state;

	m_fileIsDirty = true;
}

bool GPEWindow::TryFileSave(void)
{
	bool veto = false;

	if(m_fileIsDirty){
		wxMessageDialog *msgDialog = new wxMessageDialog(this, wxT("File modified. Save changes?"), wxT("Warning"), wxYES_NO | wxCANCEL);
		switch(msgDialog->ShowModal()){
			case wxID_YES:
				if(!DoFileSave(false)){
					/* User cancelled file selection dialog */
					veto = true;
				}
			case wxID_NO:
				break;
			case wxID_CANCEL:
				veto = true;
				break;
		}
		delete msgDialog;
	}

	return veto;
}
