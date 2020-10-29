#include "stdafx.h"
#include <Windows.h>
#include "GestionComptableApp.h"
#include "resource.h"
#include "DialogNoMFC.h"
#include "ClientDlg.h"
#include "FacturesDlg.h"
CGestionComptableApp::CGestionComptableApp()
{
}
CGestionComptableApp::CGestionComptableApp(HINSTANCE hInst,int nCmdShow)
{


	Initialize("gibi64.ddns.net");
//	Initialize("localhost");
	szClassName = "GestionComptable";
	szTitleName = "GestionComptable";
	m_MenuId = IDC_GETSTIONCOMPTABLE;
	
	MyInitInstance(hInst, nCmdShow);

	
	
//	m_Database_Channel = ConnectDataBase(L"DRIVER={PostgreSQL Unicode};DATABASE=GestionTest;SERVER=localhost;UID=postgres;PWD=arpege64;ByteaAsLongVarBinary=1");
	m_Database_Channel = ConnectDataBase(L"DRIVER={PostgreSQL Unicode};DATABASE=GestionComptable;SERVER=localhost;UID=postgres;PWD=arpege64;ByteaAsLongVarBinary=1");

}


CGestionComptableApp::~CGestionComptableApp()
{
}


LRESULT CGestionComptableApp::ApplicationWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (msg)
	{
	case WM_SIZE :
		SendMessage(GetMainLayout()->GetWin(), WM_ACTUALIZE_FROM_PARENT, 0, lParam);
		break;
	case WM_MOVE:
		SendMessage(GetMainLayout()->GetWin(), WM_ACTUALIZE_FROM_PARENT, 0, lParam);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Analyse les sélections de menu :
		switch (wmId)
		{
		case IDM_ABOUT:
//			DialogBox(GetInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), m_hWndMain, About);
			break;
		case IDM_EXIT:
			DestroyWindow(m_hWndMain);
			break;
		case ID_CLIENTS:
		{
			CLayOutClientDlg dlg;
			DialogBoxParam(GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_CLIENT), m_hWndMain, (DLGPROC)CDialogNoMFC::EntryDialogDlgProc, (LPARAM)&dlg);
			break;
		}
		case ID_FICHIER_FACTURES:
		{
			CLayOutFacturesDlg dlg;
			DialogBoxParam(GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_FACTURES), m_hWndMain, (DLGPROC)CDialogNoMFC::EntryDialogDlgProc, (LPARAM)&dlg);
			break;
		}
		default:
			return DefWindowProc(m_hWndMain, msg, wParam, lParam);
		}
		break;
	case WM_PAINT:
		//	
		hdc = BeginPaint(m_hWndMain, &ps);
		// TODO: ajoutez ici le code de dessin...
		EndPaint(m_hWndMain, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(m_hWndMain, msg, wParam, lParam);
	}
	return 0;
}