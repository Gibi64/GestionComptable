#include "stdafx.h"
#include "LayoutTvaDlg.h"
#include "Tools.h"
#include "TimeUtils.h"
#include <ctime>

#include "resource.h"
extern CGestionComptableApp* theApp;
CLayOutTvaDlg::CLayOutTvaDlg()
{
}
CLayOutTvaDlg::~CLayOutTvaDlg()
{
}
LRESULT CLayOutTvaDlg::MovableWindowProc( UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
//		InitDialog();
		break;
	case WM_ACTUALIZE_FROM_PARENT:
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

		case IDCANCEL:
			EndDialog(m_hWnd, 0);
			return 0;
			break;
		case IDOK:
			EndDialog(m_hWnd, 1);
			return 0;
			break;
		case IDC_LIST_ANNEE:
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				m_Annee = _wtoi(m_List_Annee->GetText(m_List_Annee->GetCurSel()).data());
				UpdateGrid();
			}
			break;


		default:
			break;
		}
	default:
		break;
	}
	}
	return 0;
}
void CLayOutTvaDlg::InitDialog()
{
	RECT r;
	GetWindowRect(GetDlgItem(m_hWnd, IDC_GRID_FACTURE), &r);
	POINT leftTop = { r.left, r.top };
	POINT rightBottom = { r.right, r.bottom };
	ScreenToClient(m_hWnd, &leftTop);
	ScreenToClient(m_hWnd, &rightBottom);
	r = { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
	m_List_Annee = new CListBoxNoMFC(m_hWnd, IDC_LIST_ANNEE);
	m_edit_premier_semestre = new CEditNoMFC(m_hWnd, IDC_EDIT_PREMIER_SEMESTRE);
	m_edit_deuxieme_semestre = new CEditNoMFC(m_hWnd, IDC_EDIT_DEUXIEME_SEMESTRE);
	m_edit_reliquat= new CEditNoMFC(m_hWnd, IDC_EDIT_RELIQUAT);
	m_Grid_Facture.Create(theApp->GetInstance(), m_hWnd, r);
	m_Grid_Facture.m_FixedCols = 0;
	m_Grid_Facture.SetCols(5);
	m_Grid_Facture.SetColWidth(0, 30);
	m_Grid_Facture.SetColWidth(1, 80);
	m_Grid_Facture.SetColWidth(2, 200);
	m_Grid_Facture.SetColWidth(3, 60);
	m_Grid_Facture.SetColWidth(4, 60);

	m_Grid_Facture.SetRows(1);
	time_t t = time(0);
	tm today;
	today = *gmtime(&t);
	for (auto year = 2015; year < 2030; year++)
	{
		m_List_Annee->AddString(CTools::Format(L"%4d", year));
	}
	int iAnnee = m_List_Annee->FindStringExact(CTools::Format(L"%4d", today.tm_year + 1900));
	m_List_Annee->SetCurSel(iAnnee);
	m_Annee = _wtoi(m_List_Annee->GetText(iAnnee).data());
	UpdateGrid();
	m_Grid_Facture.Show(SW_SHOW);
	m_Grid_Facture.Invalidate(TRUE);

}
void CLayOutTvaDlg::UpdateGrid()
{
	wstring szList = L"select \"Facturation Description Produit/Prestation\".\"N°\",\"Factures\".\"Description pour Client\",\"Date de paiement\",SUM(\"Quantite\"*\"PU brut\") as \"SumBrut\" , Sum(\"Quantite\" * \"Remise %\"*\"PU brut\"/100) as \"SumRemise\" from \"Facturation Description Produit/Prestation\",\"Factures\"";
	szList += L"where \"Factures\".\"Facture avec TVA\"=true AND (\"Encaissement\"=true OR \"Date de paiement\" is not null) AND \"Facturation Description Produit/Prestation\".\"Facture\"=\"Factures\".\"N°\" GROUP BY \"Factures\".\"N°\",\"Facturation Description Produit/Prestation\".\"N°\"";
	szList += L" ORDER BY \"Date de paiement\"";
	SQLRequest(theApp->m_Database_Channel, (wchar_t*)szList.data(), &record_facture);
	m_Grid_Facture.ResetContent();
	record_facture.MoveFirst();
	int iLine = 1;
	m_Grid_Facture.SetText(0, 0, L"N°");
	m_Grid_Facture.SetText(0, 1, L"Date");
	m_Grid_Facture.SetText(0, 2, L"Description");
	m_Grid_Facture.SetText(0, 3, L"Montant");
	m_Grid_Facture.SetText(0, 4, L"Tva");

	time_t tAnnee_Tva_Debut = CTimeUtils::YearMonthDayToTime(1, 1, m_Annee);

	time_t tAnnee_Tva_Fin = CTimeUtils::YearMonthDayToTime(31, 12, m_Annee); 

	time_t tEcheance_Premier_Semestre = CTimeUtils::YearMonthDayToTime(16, 06, m_Annee);

	time_t tEcheance_Deuxieme_Semestre = CTimeUtils::YearMonthDayToTime(16, 12, m_Annee);
	time_t tEcheance_Reliquat = CTimeUtils::YearMonthDayToTime(17, 12, m_Annee-1);


	double Tva_Premier_Semestre = 0;
	double Tva_Deuxieme_Semestre = 0;
	double Tva_Reliquat = 0;
	while (!record_facture.IsEOF())
	{
		auto date_paiement = record_facture.GetCurrentFieldValue(L"Date de paiement").dateVal;
		if (date_paiement >= tEcheance_Reliquat && date_paiement <= tAnnee_Tva_Fin)
		{
			double Tva = 0.2 * (record_facture.GetCurrentFieldValue(L"SumBrut").dblVal - record_facture.GetCurrentFieldValue(L"SumRemise").dblVal);

			if (date_paiement >= tAnnee_Tva_Debut && date_paiement <= tAnnee_Tva_Fin)
			{
				m_Grid_Facture.SetRows(iLine + 1);

				m_Grid_Facture.SetText(iLine, 0, CTools::Format(L"%0d", record_facture.GetCurrentFieldValue(L"N°").iVal));
				m_Grid_Facture.SetText(iLine, 1, CTimeUtils::TimetToString(record_facture.GetCurrentFieldValue(L"Date de paiement").dateVal));

				m_Grid_Facture.SetText(iLine, 2, CTools::Format(L"%ls", record_facture.GetCurrentFieldValue(L"Description pour Client").wstringVal.data()));
				m_Grid_Facture.SetText(iLine, 3, CTools::Format(L"%0.2f", record_facture.GetCurrentFieldValue(L"SumBrut").dblVal - record_facture.GetCurrentFieldValue(L"SumRemise").dblVal));
				m_Grid_Facture.SetText(iLine, 4, CTools::Format(L"%0.2f", Tva));
				if (date_paiement >= tAnnee_Tva_Debut && date_paiement <= tEcheance_Premier_Semestre)
				{
					Tva_Premier_Semestre += Tva;
				}
				else if (date_paiement >= tEcheance_Premier_Semestre + 1 && date_paiement <= tEcheance_Deuxieme_Semestre)
				{
					Tva_Deuxieme_Semestre += Tva;
				}
				iLine++;

			}
			else
				Tva_Reliquat += Tva;
		}
		record_facture.MoveNext();

	}
	m_edit_premier_semestre->SetText(CTools::Format(L"%0.2f", Tva_Premier_Semestre));
	m_edit_deuxieme_semestre->SetText(CTools::Format(L"%0.2f", Tva_Deuxieme_Semestre));
	m_edit_reliquat->SetText(CTools::Format(L"%0.2f", Tva_Reliquat));

}
void CLayOutTvaDlg::OnActivate()
{
	UpdateGrid();
	CVue::OnActivate();
}
