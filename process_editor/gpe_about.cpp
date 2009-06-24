#include "gpe_about.h"

GPEAboutDialog::GPEAboutDialog() : GPEAboutDialog_fb(NULL)
{
}

GPEAboutDialog::~GPEAboutDialog()
{
}

void GPEAboutDialog::OnButtonOk( wxCommandEvent& event )
{
	EndModal(0);
}

