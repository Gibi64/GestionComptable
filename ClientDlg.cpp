#include "stdafx.h"
#include "ClientDlg.h"
#include "GestionComptableApp.h"
#include "resource.h"
extern CGestionComptableApp * theApp;
//static CClientDlg *m_pDlg;

CClientDlg::CClientDlg()
{
}


CClientDlg::~CClientDlg()
{
//	delete m_List_Client;
}


LRESULT CClientDlg::DialogDlgProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hWnd;
	switch (msg)
	{
		case WM_INITDIALOG:
			InitDialog();
			break;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_CLIENT_QUITTER:
				case IDC_RETOUR :
					EndDialog(m_hWnd, 0);
					return 1;
					break;
				case IDC_LIST_CLIENT :
				{
					if (HIWORD(wParam) == LBN_SELCHANGE)
					{
						int iRecord = m_List_Client->GetCurSel();
						if (iRecord != -1)
						{
							UpdateFormulaire(iRecord);
						}
					}
				}
					break;

				default:
					break;
			}
			break;
		}
	}
	return 0;
}
void CClientDlg::InitDialog()
{
	m_List_Client = new CListBoxNoMFC(this->m_hWnd, IDC_LIST_CLIENT);

	m_Edit_Nom = new CEditNoMFC(m_hWnd,ID_EDIT_NOM);
	m_Edit_Adresse = new CEditNoMFC(m_hWnd, ID_EDIT_ADRESSE);
	m_Edit_Ville = new CEditNoMFC(m_hWnd, ID_EDIT_VILLE);
	m_Edit_Code_Postal = new CEditNoMFC(m_hWnd, ID_EDIT_CODE_POSTAL);
	m_Edit_Correspondant = new CEditNoMFC(m_hWnd, ID_EDIT_CORRESPPONDANT);

	SQLRequest(theApp->m_Database_Channel, L"SELECT * FROM \"Clients\" ORDER BY \"Nom\" ASC", &RecordSetClient);
	RecordSetClient.MoveFirst();
	while (!RecordSetClient.IsEOF())
	{
		auto szNom = RecordSetClient.GetCurrentFieldValue(L"Nom").wstringVal;
		m_List_Client->AddString(szNom);
		RecordSetClient.MoveNext();
	}

}
void CClientDlg::UpdateFormulaire(int iRecord)
{
	auto sz = RecordSetClient.GetFieldValue(L"Nom",iRecord).wstringVal;
	m_Edit_Nom->SetText(sz);

	sz = RecordSetClient.GetFieldValue(L"Adresse", iRecord).wstringVal;
	m_Edit_Adresse->SetText(sz);

	sz = RecordSetClient.GetFieldValue(L"Ville", iRecord).wstringVal;
	m_Edit_Ville->SetText(sz);

	sz = RecordSetClient.GetFieldValue(L"Code Postal", iRecord).wstringVal;
	m_Edit_Code_Postal->SetText(sz);

	sz = RecordSetClient.GetFieldValue(L"Correspondant", iRecord).wstringVal;
	m_Edit_Correspondant->SetText(sz);

}
