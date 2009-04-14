#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include "gdsparse.h"
#include "gpe_window.h"

GPEWindow::GPEWindow() : GPEWindow_fb(NULL)
{
	m_config = new GDSConfig();
	m_process = new GDSProcess();
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
