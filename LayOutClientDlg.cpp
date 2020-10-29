#include "stdafx.h"
#include "LayOutClientDlg.h"
#include "resource.h"
#include "ListNOMFC.h"
#include "EditNOMFC.h"
#include "DialogNoMFC.h"
#include "resource.h"
#include "..\Common64\\DataBaseRecordSet.h"
extern CGestionComptableApp *theApp;
CLayOutClientDlg::CLayOutClientDlg()
{
}


CLayOutClientDlg::~CLayOutClientDlg()
{
}

LRESULT CLayOutClientDlg::MovableWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
//		InitDialog();
		break;
	case WM_ACTUALIZE_FROM_PARENT :
	{
		RECT r;
		GetWindowRect(GetParent(m_hWnd), &r);
		POINT leftTop = { r.left, r.top };

		int Width = r.right - r.left;
		int Height = r.bottom - r.top;
		r.bottom = r.top + Height - 7;

		r.top += 66; // 50 + Taille du badeau du Layout

		r.left += 7;
		r.right = r.left + Width - 7 - 7;
		MoveWindow(m_hWnd, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_CLIENT_QUITTER:
		case IDC_RETOUR:
			EndDialog(m_hWnd, 0);
			return 1;
			break;
		case IDC_LIST_CLIENT:
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

void CLayOutClientDlg::InitDialog()
{
	m_List_Client = new CListBoxNoMFC(this->m_hWnd, IDC_LIST_CLIENT);

	m_Edit_Nom = new CEditNoMFC(m_hWnd, ID_EDIT_NOM);
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
void CLayOutClientDlg::UpdateFormulaire(int iRecord)
{
	auto sz = RecordSetClient.GetFieldValue(L"Nom", iRecord).wstringVal;
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
