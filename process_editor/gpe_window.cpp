#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include "gdsparse.h"
#include "gpe_window.h"

GPEWindow::GPEWindow() : GPEWindow_fb(NULL)
{
	m_config = new GDSConfig();
	m_process = new GDSProcess();
	m_process_path = wxT("");
	m_defaultSaveDir = wxT("");
	m_defaultSaveFile = wxT("");
	m_layerIsDirty = false;
	m_selectedLayer = -1;
}

GPEWindow::~GPEWindow()
{
	delete m_config;
	delete m_process;
}

void GPEWindow::OnMenuImportGDS( wxCommandEvent& event )
{
	wxFileDialog *fileDialog = new wxFileDialog(this);

	fileDialog->SetWildcard(wxT("GDS files (*.gds;*.gds2;*.gdsii)|*.gds;*.gds2;*.gdsii|All files (*.*;*)|*.*;*"));

	if(fileDialog->ShowModal() == wxID_OK){
		/************ Open GDS2 file and parse ****************/

		printf("File is '%s'\n", (char*)(fileDialog->GetPath().char_str()));
		FILE *iptr;
		iptr = fopen((char *)fileDialog->GetPath().char_str(), "rb");
		if(iptr){
			class GDSParse *Parser = new class GDSParse(m_config, m_process, true);
			if(!Parser->Parse(iptr)){
				ProcessLayer *layer;

				for(unsigned int i = 0; i < m_process->LayerCount(); i++){
					layer = m_process->GetLayer(i);
					layer->Show = true;
					printf("%s - %d:%d\n", layer->Name.c_str(), layer->Layer, layer->Datatype);
					int item = m_checkListBoxLayers->Append(wxString::FromAscii(layer->Name.c_str()));
					m_checkListBoxLayers->Check(item, true);
				}
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

void GPEWindow::OnButtonApply( wxCommandEvent& event )
{
	int selected = m_checkListBoxLayers->GetSelection();
	if(selected != wxNOT_FOUND){
		SaveLayer(selected);
		SetLayerDirtyState(false);
	}
}

void GPEWindow::OnMenuSave( wxCommandEvent& event )
{
	if(m_process_path == wxT("")){
		OnMenuSaveAs(event);
	}else{
		std::string filename;
		filename = (char *)m_process_path.char_str();
		m_process->Save(filename);
	}
}

void GPEWindow::OnMenuSaveAs( wxCommandEvent& event )
{
	wxFileDialog *fileDialog = new wxFileDialog(this, wxT("Choose a file"),
			m_defaultSaveDir, m_defaultSaveFile, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if(fileDialog->ShowModal() == wxID_OK){
		m_defaultSaveDir = fileDialog->GetDirectory();
		m_defaultSaveFile = fileDialog->GetFilename();
		m_process_path = fileDialog->GetPath();

		std::string filename;
		filename = (char *)m_process_path.char_str();
		m_process->Save(filename);
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
			m_textCtrlThickness->SetValue(wxString::Format(wxT("%f"), layer->Thickness));

			m_colourPickerLayer->SetColour(wxColour(255 * layer->Red, 255 * layer->Green, 255 * layer->Blue));

			m_textCtrlName->Enable(true);
			m_textCtrlLayer->Enable(true);
			m_textCtrlDatatype->Enable(true);
			m_textCtrlThickness->Enable(true);
			m_colourPickerLayer->Enable(true);

			SetLayerDirtyState(false);
		}
	}
	m_selectedLayer = selected;
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

void GPEWindow::SetLayerDirtyState(bool state)
{
	m_buttonApply->Enable(state);
	m_layerIsDirty = state;

	printf("SetLayerDirtyState(%d)\n", state);
}

void GPEWindow::OnLayerChange( wxCommandEvent& event )
{
	SetLayerDirtyState(true);
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

//		layer->Height = m_textCtrlLayer->GetValue()->;

		wxColour colour = m_colourPickerLayer->GetColour();


		layer->Red = colour.Red() / 255.0;
		layer->Green = colour.Green() / 255.0;
		layer->Blue = colour.Blue() / 255.0;
	}
}

