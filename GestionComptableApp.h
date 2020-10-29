#pragma once
#include <Windows.h>
#include "..\Common64Win\LayoutWnd.h"
#include "..\Common64Win\EasyApplication.h"
#include "LayOutClientDlg.h"
#include "LayOutFacturesDlg.h"
#include "LayOutTvaDlg.h"


class CGestionComptableApp	: public CEasyApplication
{
private:

	CLayoutWnd m_Main_LayOut;

public:
	CLayoutWnd * GetMainLayout()
	{
		return &m_Main_LayOut;
	}
	CGestionComptableApp();
	CGestionComptableApp(HINSTANCE hInst,int nCmdShow);
	~CGestionComptableApp();
	int m_Database_Channel;
	LRESULT ApplicationWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
	CLayOutClientDlg VueClients;
	CLayOutFacturesDlg VueFactures;
	CLayOutTvaDlg VueTva;

	CVue VueCommandes;
};

