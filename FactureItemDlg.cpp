#include "stdafx.h"
#include "FactureItemDlg.h"
#include "..\Common64\Tools.h"
extern CGestionComptableApp *theApp;

CFactureItemDlg::CFactureItemDlg()
{
	m_Numero_Facture = -1;
}

CFactureItemDlg::CFactureItemDlg(int Numero,int Duplicate)
{
	m_Numero_Facture = Numero;
	m_Duplicate_From = Duplicate;
}
CFactureItemDlg::~CFactureItemDlg()
{
}
LRESULT CFactureItemDlg::DialogDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		case IDC_BUTTON_NOUVEAU:
			OnNouveau();
			break;
		case IDC_BUTTON_ENREGISTRER:
			OnEnregistrer();
			break;
		case IDCANCEL:
			EndDialog(m_hWnd, 0);
			return 0;
			break;
		case IDOK:
			EndDialog(m_hWnd, 1);
			return 0;
			break;


		default:
			break;
		}
	default :
		break;
	}
	}
	return 0;
}
void CFactureItemDlg::InitDialog()
{
	m_Edit_Description = new CEditNoMFC(m_hWnd, IDC_EDIT_DESCRIPTION);
	m_Edit_Prix_Unitaire = new CEditNoMFC(m_hWnd, IDC_EDIT_PRIX_UNITAIRE);
	m_Edit_Quantite  = new CEditNoMFC(m_hWnd, IDC_EDIT_QUANTITE);
	m_Edit_Remise = new CEditNoMFC(m_hWnd, IDC_EDIT_REMISE);

	m_Edit_Description->Enable(FALSE);
	m_Edit_Prix_Unitaire->Enable(FALSE);
	m_Edit_Quantite->Enable(FALSE);
	m_Edit_Remise->Enable(FALSE);

	m_Button_Supprimer = new CButtonNoMFC(m_hWnd,IDC_BUTTON_SUPPRIMER);
	m_Button_Nouveau = new CButtonNoMFC(m_hWnd, IDC_BUTTON_NOUVEAU);
	m_Button_Dupliquer = new CButtonNoMFC(m_hWnd, IDC_BUTTON_DUPLIQUER);
	m_Button_Enregistrer = new CButtonNoMFC(m_hWnd, IDC_BUTTON_ENREGISTRER);

	m_Button_Supprimer->Enable(FALSE);

	RECT r;
	GetWindowRect(GetDlgItem(m_hWnd, IDC_GRID_ITEM), &r);
	POINT leftTop = { r.left, r.top };
	POINT rightBottom = { r.right, r.bottom };
	ScreenToClient(m_hWnd, &leftTop);
	ScreenToClient(m_hWnd, &rightBottom);
	r = { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };

	m_Grid_Item.Create(theApp->GetInstance(), m_hWnd, r);
	m_Grid_Item.m_FixedCols = 0;
	m_Grid_Item.SetCols(4);
	m_Grid_Item.SetColWidth(0, 30);
	m_Grid_Item.SetColWidth(1, 120);
	m_Grid_Item.SetColWidth(2, 30);
	m_Grid_Item.SetColWidth(3, 50);
	m_Grid_Item.Show(SW_SHOW);
	m_Grid_Item.Invalidate(TRUE);

	UpdateGrid();

}

void CFactureItemDlg::OnNouveau()
{
	m_Button_Nouveau->Enable(FALSE);
	m_Button_Dupliquer->Enable(FALSE);
	m_Button_Enregistrer->Enable(TRUE);


	m_Edit_Description->Enable(TRUE);
	m_Edit_Prix_Unitaire->SetText(L"0.00");
	m_Edit_Quantite->SetText(L"1");
	m_Edit_Remise->SetText(L"0");

	m_Edit_Prix_Unitaire->Enable(TRUE);
	m_Edit_Quantite->Enable(TRUE);
	m_Edit_Remise->Enable(TRUE);
}

void CFactureItemDlg::OnEnregistrer()
{

	SQLRequest(theApp->m_Database_Channel, L"SELECT * FROM \"Facturation Description Produit/Prestation\"", &record_item);
	int max_num = -1;
	record_item.MoveFirst();
	while (!record_item.IsEOF())
	{
		max_num = max(max_num, record_item.GetCurrentFieldValue(L"N°").iVal);
		record_item.MoveNext();
	}

	max_num++;
	wstring szInsert = L"INSERT INTO \"Facturation Description Produit/Prestation\"";
	szInsert += L"(\"N°\",\"Facture\",\"Quantite\",\"Remise %\",\"Description\",\"PU brut\") ";
	szInsert += CTools::Format(L"VALUES(%d,%d,%ls,%ls,'%s',%ls);", max_num,m_Numero_Facture, m_Edit_Quantite->GetText().data(), m_Edit_Remise->GetText().data(), m_Edit_Description->GetText().data(), m_Edit_Prix_Unitaire->GetText().data());
	SQLRequest(theApp->m_Database_Channel, (wchar_t *)szInsert.data(), &record_item);

	UpdateGrid();

}
void CFactureItemDlg::UpdateGrid()
{

	wstring szList = CTools::Format(L"SELECT * FROM \"Facturation Description Produit/Prestation\" WHERE \"Facture\"=%d", m_Numero_Facture);
	SQLRequest(theApp->m_Database_Channel, (wchar_t *)szList.data(), &record_item);
	m_Grid_Item.ResetContent();
	record_item.MoveFirst();
	int iLine = 1;
	m_Grid_Item.SetText(0, 0, L"N°");
	m_Grid_Item.SetText(0, 1, L"Description");
	m_Grid_Item.SetText(0, 2, L"Quan");
	m_Grid_Item.SetText(0, 3, L"PU Brut");


	while (!record_item.IsEOF())
	{
		m_Grid_Item.SetText(iLine, 0, CTools::Format(L"%0d",record_item.GetCurrentFieldValue(L"N°").iVal));
		m_Grid_Item.SetText(iLine, 1, record_item.GetCurrentFieldValue(L"Description").wstringVal);
		m_Grid_Item.SetText(iLine, 2, CTools::Format(L"%0d", record_item.GetCurrentFieldValue(L"Quantite").iVal));
		m_Grid_Item.SetText(iLine, 3, CTools::Format(L"%0.2f", record_item.GetCurrentFieldValue(L"PU brut").dblVal));
		record_item.MoveNext();
	}

	m_Button_Nouveau->Enable(TRUE);
}