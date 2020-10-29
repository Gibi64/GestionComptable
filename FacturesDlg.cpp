#include "stdafx.h"
#include "FacturesDlg.h"
#include "resource.h"
#include "TimeUtils.h"
#include "..\Common64\tools.h"
#include <ctime>
extern CGestionComptableApp * theApp;
CFacturesDlg::CFacturesDlg()
{
}


CFacturesDlg::~CFacturesDlg()
{
	delete m_Client;
	delete m_Commande;
	delete m_Date_de_facturation;
	delete m_Date_de_paiement;
	delete m_Date_Echeance;
	delete m_Encaissement;
	delete m_List_Annee;
	delete m_Mode_de_paiement;
	delete m_Numero_Commande;
	delete m_Reference_Client;
	delete m_remise;
	delete m_sumbrut;
	delete m_Tva;
}

void CFacturesDlg::InitDialog()
{

	///////////////////////////////////////////////////////////// Controls //////////////////////////////////////////////
	RECT r;
	auto hWnd_Grille = GetDlgItem(m_hWnd, IDC_GRILLE);
	GetWindowRect(hWnd_Grille, &r);
	POINT leftTop = { r.left, r.top };
	POINT rightBottom = { r.right, r.bottom };
	ScreenToClient(m_hWnd, &leftTop);
	ScreenToClient(m_hWnd, &rightBottom);
	r = { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
	GridFacture.Create(theApp->GetInstance(), m_hWnd, r);
	GridFacture.m_FixedCols = 0;
	GridFacture.SetColWidth(0, 30);
	GridFacture.SetColWidth(1, 70);
	GridFacture.SetColWidth(2, 100);
	GridFacture.SetColWidth(3, 60);
	GridFacture.Show(SW_SHOW);
	GridFacture.Invalidate(TRUE);

	m_List_Annee = new CListBoxNoMFC(m_hWnd, IDC_LIST_ANNEE);
	m_Client = new CComboNoMFC(m_hWnd, IDC_COMBO_CLIENT);
	m_Reference_Client = new CEditNoMFC(m_hWnd, ID_EDIT_REFERENCE_CLIENT);
	m_Numero_Commande = new CEditNoMFC(m_hWnd, IDC_EDIT_NUEMRO_COMMANDE);
	m_Date_de_facturation = new CEditNoMFC(m_hWnd, ID_EDIT_DATE_DE_FACTURATION);
	m_Date_Echeance = new CEditNoMFC(m_hWnd, ID_EDIT_DATE_D_ECHEANCE);
	m_Date_de_paiement = new CEditNoMFC(m_hWnd, ID_EDIT_DATE_DE_PAIEMENT);
	m_sumbrut = new CEditNoMFC(m_hWnd, ID_EDIT_MONTANT);
	m_remise = new CEditNoMFC(m_hWnd, ID_EDIT_REMISE_GLOBALE);

	m_Tva = new CCheckButtonNoMFC(m_hWnd, IDC_CHECK_FACTURE_AVEC_TVA);
	m_Encaissement = new CCheckButtonNoMFC(m_hWnd, IDC_CHECK_ENCAISSEMENT);
	m_Mode_de_paiement = new CComboNoMFC(m_hWnd, IDC_COMBO_MODE_DE_PAIEMENT);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	time_t t = time(0);
	tm today;
	today = *gmtime(&t);
	for (auto year = 2015; year < 2030; year++)
	{
		m_List_Annee->AddString(CTools::Format(L"%4d", year));
	}
	int iAnnee = m_List_Annee->FindStringExact(CTools::Format(L"%4d", today.tm_year + 1900));
	m_List_Annee->SetCurSel(iAnnee);

	CDataBaseRecordSet rCommande;
	SQLRequest(theApp->m_Database_Channel, L"SELECT * FROM \"Commandes\"", &rCommande);
	m_Commande = new CComboNoMFC(m_hWnd, IDC_COMBO_COMMANDE);
	while (!rCommande.IsEOF())
	{
		m_Commande->AddString(rCommande.GetCurrentFieldValue(L"Objet").wstringVal);
		rCommande.MoveNext();
	}

	SQLRequest(theApp->m_Database_Channel, L"SELECT * FROM \"Clients\"", &rCommande);
	rCommande.MoveFirst();
	while (!rCommande.IsEOF())
	{
		m_Client->AddString(rCommande.GetCurrentFieldValue(L"Nom").wstringVal);
		rCommande.MoveNext();
	}

	SQLRequest(theApp->m_Database_Channel, L"SELECT * FROM \"Mode de Paiement\"", &rCommande);
	rCommande.MoveFirst();

	while (!rCommande.IsEOF())
	{
		m_Mode_de_paiement->AddString(rCommande.GetCurrentFieldValue(L"Mode").wstringVal);
		rCommande.MoveNext();
	}
	Affiche_Annee(iAnnee);
}
	
void CFacturesDlg::UpdateFormulaire(int iRecord)
{
	m_Reference_Client->SetText(RecordSetClient.GetFieldValue(L"Description pour Client",iRecord).wstringVal);
	m_Numero_Commande->SetText(RecordSetClient.GetFieldValue(L"numcom", iRecord).wstringVal);
	m_Date_de_facturation->SetText(CTimeUtils::TimetToString(RecordSetClient.GetFieldValue(L"Date de Facturation", iRecord).dateVal));
	m_Date_de_paiement->SetText(CTimeUtils::TimetToString(RecordSetClient.GetFieldValue(L"Date de paiement", iRecord).dateVal));
	m_Date_Echeance->SetText(CTimeUtils::TimetToString(RecordSetClient.GetFieldValue(L"Date Echeance", iRecord).dateVal));

	m_sumbrut->SetText(CTools::Format(L"%.2f",RecordSetClient.GetFieldValue(L"sumbrut", iRecord).dblVal));
	m_remise->SetText(CTools::Format(L"%.2f", RecordSetClient.GetFieldValue(L"sumremise", iRecord).dblVal));

	m_Tva->SetCheck(RecordSetClient.GetFieldValue(L"Facture avec TVA", iRecord).boolVal);
	m_Encaissement->SetCheck(RecordSetClient.GetFieldValue(L"Encaissement", iRecord).boolVal);

	int iCommande = m_Commande->FindStringExact(RecordSetClient.GetFieldValue(L"Objet", iRecord).wstringVal);
	m_Commande->SetCurSel(iCommande);

	int iClient = m_Client->FindStringExact(RecordSetClient.GetFieldValue(L"Nom", iRecord).wstringVal);
	m_Client->SetCurSel(iClient);

	int iMode = m_Mode_de_paiement->FindStringExact(RecordSetClient.GetFieldValue(L"mode", iRecord).wstringVal);
	m_Mode_de_paiement->SetCurSel(iClient);
}
LRESULT CFacturesDlg::DialogDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hWnd;
	switch (msg)
	{
	case WM_INITDIALOG:
		InitDialog();
		break;
	case 30001 :
		UpdateFormulaire(wParam-1);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case ID_CLIENT_QUITTER:
			case IDC_RETOUR:
			EndDialog(m_hWnd, 0);
			return 1;
			break;

			case IDC_LIST_ANNEE:
				if (HIWORD(wParam )== LBN_SELCHANGE)
				{
					int iAnnee = _wtoi(m_List_Annee->GetText(m_List_Annee->GetCurSel()).data());
					Affiche_Annee(iAnnee);
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

void CFacturesDlg::Affiche_Annee(int iAnnee)
{
	GridFacture.ResetContent();
	////////////////////////////////////////////////// Requete SQL des factures //////////////////////////////////////////////////
	wstring WichFields = L"Cli.\"N°\" as NumeroClient,Fa.\"N°\",Co.\"Objet\",Co.\"N°\" as NumCom,Fa.\"Description pour Client\",\"Date de Facturation\", SUM(\"Quantite\"*\"PU brut\") as Sumbrut,";
	WichFields += L"Co.\"Reference Client\",";
	WichFields += L"SUM(\"Quantite\"*\"Remise %\" *\"PU brut\"/100) as SumRemise,Cli.\"Nom\",";
	WichFields += L"Cli.\"Adresse\",Cli.\"Code Postal\",Cli.\"Ville\",Cli.\"Correspondant\",";
	WichFields += L"\"Date de paiement\",\"Date Echeance\",\"Facture avec TVA\",\"Mode de Paiement\" as Mode,\"Encaissement\"";

	wstring FromTable = L" FROM \"Factures\" as Fa, \"Facturation Description Produit/Prestation\" as Item, ";
	FromTable += L"\"Commandes\" as Co,\"Clients\" as Cli,\"Mode de Paiement\" as Mo";

	wstring	Filtre = L" WHERE Item.\"Facture\"=Fa.\"N°\"";
	Filtre += L" AND Fa.\"Commande\"=Co.\"N°\"";
	Filtre += L" AND Co.\"Reference Client\"=Cli.\"N°\"";
	Filtre += L" AND Mo.\"N°\" = Fa.\"Mode de Paiement\"";
	//		& " AND date_part('Year',Fa.\"Date de Facturation\")=" & Annee
	auto FiltreAnnee = CTools::Format(L" AND date_part('Year',Fa.\"Date de Facturation\")=%ls", m_List_Annee->GetText(m_List_Annee->GetCurSel()).data());
	Filtre += FiltreAnnee;
	wstring	GroupBy = L" GROUP BY Cli.\"N°\",Co.\"N°\",Fa.\"N°\",Cli.\"Nom\",Cli.\"Adresse\",Co.\"Objet\",Cli.\"Code Postal\",Cli.\"Ville\",Cli.\"Correspondant\",Mo.\"Mode\"";

	wstring	OrderBy = L" ORDER BY \"Date de Facturation\" DESC";

	wstring Requete = L" SELECT " + WichFields + FromTable + Filtre + GroupBy + OrderBy;


	SQLRequest(theApp->m_Database_Channel, (wchar_t *)Requete.data(), &RecordSetClient);
	RecordSetClient.MoveFirst();
	auto Line = 1;
	GridFacture.SetText(0, 0, L"N°");
	GridFacture.SetText(0, 1, L"Date");
	GridFacture.SetText(0, 2, L"Client");
	GridFacture.SetText(0, 3, L"Objet");
	GridFacture.SetText(0, 4, L"Montant (€)");
	while (!RecordSetClient.IsEOF())
	{
		GridFacture.SetRows(Line+1);

		auto szNom = RecordSetClient.GetCurrentFieldValue(L"N°").wstringVal;
		GridFacture.SetText(Line, 0, szNom);
		time_t tFacturation = RecordSetClient.GetCurrentFieldValue(L"Date de Facturation").dateVal;
		szNom = CTimeUtils::TimetToString(tFacturation);
		GridFacture.SetText(Line, 1, szNom);

		szNom = RecordSetClient.GetCurrentFieldValue(L"Nom").wstringVal;
		GridFacture.SetText(Line, 2, szNom);

		szNom = RecordSetClient.GetCurrentFieldValue(L"Objet").wstringVal;
		GridFacture.SetText(Line, 3, szNom);

		szNom = CTools::Format(L"%.2f", RecordSetClient.GetCurrentFieldValue(L"sumbrut").dblVal);
		GridFacture.SetText(Line, 4, szNom);
		RecordSetClient.MoveNext();
		Line++;
	}
	m_Tva->SetCheck(false);
	m_Encaissement->SetCheck(false);
	m_Client->SetCurSel(-1);
	m_Commande->SetCurSel(-1);
	m_Date_de_facturation->SetText(L"");
	m_Date_de_paiement->SetText(L"");
	m_Date_Echeance->SetText(L"");
	m_Mode_de_paiement->SetCurSel(-1);
	m_Numero_Commande->SetText(L"");
	m_Reference_Client->SetText(L"");
	m_sumbrut->SetText(L"");
	m_remise->SetText(L"");

}
