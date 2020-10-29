// GetstionComptable.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "GestionComptable.h"
#include "ClientDlg.h"
#include "FacturesDlg.h"
#include "resource.h"
#define MAX_LOADSTRING 100
CGestionComptableApp *theApp;
// Variables globales :
HINSTANCE hInst;								// instance actuelle
TCHAR szTitle[MAX_LOADSTRING];					// Le texte de la barre de titre
TCHAR szWindowClass[MAX_LOADSTRING];			// le nom de la classe de fenêtre principale

// Pré-déclarations des fonctions incluses dans ce module de code :

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: placez ici le code.
	MSG msg;
	HACCEL hAccelTable;
	RECT r;
	// Initialise les chaînes globales
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GETSTIONCOMPTABLE, szWindowClass, MAX_LOADSTRING);

	theApp = new CGestionComptableApp(hInstance, nCmdShow);
	GetWindowRect(theApp->m_hWndMain, &r);
	POINT leftTop = { r.left, r.top };
	POINT rightBottom = { r.right, r.bottom };
	ScreenToClient(theApp->m_hWndMain, &leftTop);
	ScreenToClient(theApp->m_hWndMain, &rightBottom);
	r = { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
	theApp->GetMainLayout()->Create(hInstance, theApp->m_hWndMain, r);

	theApp->VueClients.CreateWnd(L"MoveableWnd", NULL, hInst, theApp->GetMainLayout()->GetWin(), MAKEINTRESOURCE(IDD_DIALOG_CLIENT));
	theApp->VueFactures.CreateWnd(L"MoveableWnd", NULL, hInst, theApp->GetMainLayout()->GetWin(), MAKEINTRESOURCE(IDD_DIALOG_FACTURES));
	theApp->VueTva.CreateWnd(L"MoveableWnd", NULL, hInst, theApp->GetMainLayout()->GetWin(), MAKEINTRESOURCE(IDD_DIALOG_TVA));

	GetWindowRect(theApp->m_hWndMain, &r);
	SetWindowText(theApp->m_hWndMain, L"Gestion Comptable");
	int Width = r.right - r.left;
	int Height = r.bottom - r.top;

	r.bottom = Height - 66 - 7;
	r.top += 66;
	r.left += 7;
	r.right =  Width-7-7;

	theApp->GetMainLayout()->AddView(L"Clients", &theApp->VueClients);
	theApp->GetMainLayout()->AddView(L"Factures", &theApp->VueFactures);
	theApp->GetMainLayout()->AddView(L"Tva", &theApp->VueTva);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GETSTIONCOMPTABLE));

	// Boucle de messages principale :
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}