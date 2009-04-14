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
}

GPEWindow::~GPEWindow()
{
	delete m_config;
	delete m_process;
}

void GPEWindow::OnMenuImportGDS( wxCommandEvent& event )
{
	wxFileDialog *fileDialog = new wxFileDialog(this);

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
					printf("%s - %d:%d\n", layer->Name.c_str(), layer->Layer, layer->Datatype);
					m_checkListBoxLayers->Append(wxString::FromAscii(layer->Name.c_str()));
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
	wxColour colour = m_colourPickerLayer->GetColour();

	printf("Red: %f\n", colour.Red() / 255.0);
	printf("Green: %f\n", colour.Green() / 255.0);
	printf("Blue: %f\n", colour.Blue() / 255.0);
}

void GPEWindow::OnCheckListBoxLayersClick( wxCommandEvent& event )
{
	int selected = m_checkListBoxLayers->GetSelection();
	if(selected != wxNOT_FOUND){
		ProcessLayer *layer = m_process->GetLayer(selected);
		if(layer){
			m_textCtrlName->SetValue(wxString::FromAscii(layer->Name.c_str()));
			m_textCtrlLayer->SetValue(wxString::Format(wxT("%d"), layer->Layer));
			m_textCtrlDatatype->SetValue(wxString::Format(wxT("%d"), layer->Datatype));
			m_textCtrlThickness->SetValue(wxString::Format(wxT("%f"), layer->Thickness));

			m_colourPickerLayer->SetColour(wxColour(255 * layer->Red, 255 * layer->Green, 255 * layer->Blue));
		}
	}
}

