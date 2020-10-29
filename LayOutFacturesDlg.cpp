#include "stdafx.h"
#include "LayOutFacturesDlg.h"
#include "FacturesDlg.h"
#include "resource.h"
#include "TimeUtils.h"
#include "FactureItemDlg.h"
#include "..\Common64\tools.h"
#include "..\Common64\xmlparser.h"
#include "..\Common64\TranslateDOM.h"
#include "PrintFactureDlg.h"
#include <ctime>
extern CGestionComptableApp * theApp;

CLayOutFacturesDlg::CLayOutFacturesDlg()
{
}

CLayOutFacturesDlg::~CLayOutFacturesDlg()
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

void CLayOutFacturesDlg::InitDialog()
{
	///////////////////////////////////////////////////////////// boutons ///////////////////////////////////////////////

	m_Button_dupliquer = new CButtonNoMFC(m_hWnd,IDC_BUTTON_DUPLIQUER);
	m_Button_enregister = new CButtonNoMFC(m_hWnd, IDC_BUTTON_ENREGISTRER);
	m_Button_supprimer = new CButtonNoMFC(m_hWnd, IDC_BUTTON_SUPPRIMER);
	m_Button_imprimer = new CButtonNoMFC(m_hWnd, IDC_BUTTON_IMPRIMER);

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

void CLayOutFacturesDlg::UpdateFormulaire(int iRecord)
{
	m_Record_Number = iRecord;
	m_Reference_Client->SetText(RecordSetClient.GetFieldValue(L"Description pour Client", iRecord).wstringVal);
	m_Numero_Commande->SetText(RecordSetClient.GetFieldValue(L"numcom", iRecord).wstringVal);
	m_Date_de_facturation->SetText(CTimeUtils::TimetToString(RecordSetClient.GetFieldValue(L"Date de Facturation", iRecord).dateVal));
	m_Date_de_paiement->SetText(CTimeUtils::TimetToString(RecordSetClient.GetFieldValue(L"Date de paiement", iRecord).dateVal));
	m_Date_Echeance->SetText(CTimeUtils::TimetToString(RecordSetClient.GetFieldValue(L"Date Echeance", iRecord).dateVal));

	m_sumbrut->SetText(CTools::Format(L"%.2f", RecordSetClient.GetFieldValue(L"sumbrut", iRecord).dblVal));
	m_remise->SetText(CTools::Format(L"%.2f", RecordSetClient.GetFieldValue(L"sumremise", iRecord).dblVal));

	m_Tva->SetCheck(RecordSetClient.GetFieldValue(L"Facture avec TVA", iRecord).boolVal);
	m_Encaissement->SetCheck(RecordSetClient.GetFieldValue(L"Encaissement", iRecord).boolVal);

	int iCommande = m_Commande->FindStringExact(RecordSetClient.GetFieldValue(L"Objet", iRecord).wstringVal);
	m_Commande->SetCurSel(iCommande);

	int iClient = m_Client->FindStringExact(RecordSetClient.GetFieldValue(L"Nom", iRecord).wstringVal);
	m_Client->SetCurSel(iClient);

	int iMode = m_Mode_de_paiement->FindStringExact(RecordSetClient.GetFieldValue(L"mode", iRecord).wstringVal);
	m_Mode_de_paiement->SetCurSel(iClient);

//	if (RecordSetClient.GetFieldValue(L"Encaissement", iRecord).iVal)
	if(!m_Date_de_paiement->GetText().empty())
	{
		m_Reference_Client->SetReadOnly(TRUE);
		m_Date_de_facturation->SetReadOnly(TRUE);
		m_Date_de_paiement->SetReadOnly(TRUE);
		m_Date_Echeance->SetReadOnly(TRUE);
		m_remise->SetReadOnly(TRUE);
		m_Client->Enable(FALSE);
		m_Commande->Enable(FALSE);
		m_Mode_de_paiement->Enable(FALSE);
		m_Tva->Enable(FALSE);
		m_Encaissement->Enable(FALSE);
		m_Button_dupliquer->Enable(true);
		m_Button_enregister->Enable(FALSE);
		m_Button_supprimer->Enable(FALSE);
		m_Button_imprimer->Enable(TRUE);

	}
	else
	{
		m_Reference_Client->SetReadOnly(FALSE);
		m_Date_de_facturation->SetReadOnly(FALSE);
		m_Date_de_facturation->SetReadOnly(FALSE);
		m_Date_de_paiement->SetReadOnly(FALSE);
		m_Date_Echeance->SetReadOnly(FALSE);
		m_remise->SetReadOnly(FALSE);

		m_Client->Enable(TRUE);
		m_Commande->Enable(TRUE);
		m_Mode_de_paiement->Enable(TRUE);
		m_Tva->Enable(TRUE);
		m_Encaissement->Enable(TRUE);
		m_Button_dupliquer->Enable(TRUE);
		m_Button_enregister->Enable(TRUE);
		m_Button_supprimer->Enable(TRUE);
		m_Button_imprimer->Enable(TRUE);


	}
}
LRESULT CLayOutFacturesDlg::MovableWindowProc( UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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

	case WM_INITDIALOG:
//		InitDialog();
		break;
	case 30001:
		UpdateFormulaire(wParam - 1);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_RETOUR:
			EndDialog(m_hWnd, 0);
			return 1;
			break;
		case IDC_BUTTON_DUPLIQUER:
			OnDupliquer();
			break;
		case IDC_BUTTON_SUPPRIMER:
			OnSupprimer();
			break;
		case IDC_BUTTON_ENREGISTRER:
			OnEnregistrer();
			break;
		case IDC_BUTTON_IMPRIMER:
			OnImprimer();
			break;

		case IDC_LIST_ANNEE:
			if (HIWORD(wParam) == LBN_SELCHANGE)
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

void CLayOutFacturesDlg::Affiche_Annee(int iAnnee)
{
	GridFacture.ResetContent();
	m_Record_Number = -1;

	////////////////////////////////////////////////// Requete SQL des factures //////////////////////////////////////////////////
	wstring WichFields = L"Cli.\"N°\" as NumeroClient,Fa.\"N°\",Co.\"Objet\",Co.\"N°\" as NumCom,Fa.\"Description pour Client\",\"Date de Facturation\", SUM(\"Quantite\"*\"PU brut\") as Sumbrut,";
	WichFields += L"Co.\"Reference Client\",Mo.\"N°\" as numode,";
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
	wstring	GroupBy = L" GROUP BY Cli.\"N°\",Co.\"N°\",Fa.\"N°\",Cli.\"Nom\",Cli.\"Adresse\",Co.\"Objet\",Cli.\"Code Postal\",Cli.\"Ville\",Cli.\"Correspondant\",Mo.\"Mode\",Mo.\"N°\"";

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
		GridFacture.SetRows(Line + 1);

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

	m_Reference_Client->SetReadOnly(TRUE);
	m_Date_de_facturation->SetReadOnly(TRUE);
	m_Date_de_paiement->SetReadOnly(TRUE);
	m_Date_Echeance->SetReadOnly(TRUE);
	m_remise->SetReadOnly(TRUE);
	m_Client->Enable(FALSE);
	m_Commande->Enable(FALSE);
	m_Mode_de_paiement->Enable(FALSE);
	m_Tva->Enable(FALSE);
	m_Encaissement->Enable(FALSE);

	m_Button_dupliquer->Enable(FALSE);
	m_Button_enregister->Enable(FALSE);
	m_Button_imprimer->Enable(FALSE);

}

void CLayOutFacturesDlg::OnDupliquer()
{
	CDataBaseRecordSet LesItemACopier;
	CDataBaseRecordSet Insert;
	SQLRequest(theApp->m_Database_Channel, L"SELECT \"N°\" FROM \"Factures\"", &Insert);
	int OldInvoiceNumber = RecordSetClient.GetFieldValue(L"N°", m_Record_Number).iVal;
	int iMax = 0;
	while (!Insert.IsEOF())
	{
		auto iCurrent = Insert.GetCurrentFieldValue(L"N°").iVal;
		iMax = max(iCurrent, iMax);
		Insert.MoveNext();
	}
	iMax++;
	m_Numero_Commande->GetText();
	wstring szInsert = L"INSERT INTO \"Factures\"";
//	szInsert += L"(\"N°\", \"Commande\", \"Date de Facturation\", \"Facture avec TVA\", \"Date de paiement\", \"Description pour Client\", \"Encaissement\", \"Date Echeance\", \"Remise globale\", \"Mode de Paiement\") ";
	szInsert += L"(\"N°\", \"Commande\", \"Date de Facturation\", \"Facture avec TVA\", \"Description pour Client\", \"Encaissement\",\"Date Echeance\", \"Remise globale\", \"Mode de Paiement\") ";

	szInsert += L" VALUES (";
	time_t t = time(0);
	tm today = *gmtime(&t);

	szInsert += CTools::Format(L"%-d,", iMax);
	szInsert += m_Numero_Commande->GetText() + L",";
	szInsert += CTools::Format(L"'%04d-%02d-%02d',",today.tm_year+1900,today.tm_mon + 1,today.tm_mday );
	if (RecordSetClient.GetFieldValue(L"Facture avec TVA", m_Record_Number).boolVal)
		szInsert += L"true,";
	else
		szInsert += L"false,";
	szInsert += L"'" + m_Reference_Client->GetText() + L"',";
	szInsert += L"false,";
	szInsert += L"'"+m_Date_Echeance->GetText() + L"',";
	szInsert += m_remise->GetText() + L",";
	CFactureItemDlg dlg(iMax,OldInvoiceNumber);
	int iReturn = DialogBoxParam(theApp->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_ITEMS_FACTURE), GetWin(), (DLGPROC)CDialogNoMFC::EntryDialogDlgProc, (LPARAM) &dlg);
	szInsert += CTools::Format(L"%d);", RecordSetClient.GetFieldValue(L"numode", m_Record_Number).iVal);
	if (iReturn)
	{
		SQLRequest(theApp->m_Database_Channel, (wchar_t *)szInsert.data(), &Insert);
		int iAnnee = _wtoi(m_List_Annee->GetText(m_List_Annee->GetCurSel()).data());
		Affiche_Annee(iAnnee);


		//		search the new record in the recordset
		RecordSetClient.MoveFirst();
		int iSelectedRow = 0;
		while (!RecordSetClient.IsEOF())
		{
			if (RecordSetClient.GetCurrentFieldValue(L"N°").iVal == iMax)
			{
				GridFacture.m_Select.LigneMin = iSelectedRow + 1;
				GridFacture.m_Select.LigneMax = iSelectedRow + 1;
				GridFacture.m_Select.ColMin = 0;
				GridFacture.m_Select.ColMax = 5;
				break;
			}
			iSelectedRow++;
			RecordSetClient.MoveNext();
		}
		UpdateFormulaire(iSelectedRow);
		m_Record_Number = iSelectedRow;
		m_Button_dupliquer->Enable(true);
	}
	else
	{
		szInsert = CTools::Format(L"DELETE FROM \"Facturation Description Produit/Prestation\" WHERE \"Facture\"=%d", iMax);
		SQLRequest(theApp->m_Database_Channel, (wchar_t *)szInsert.data(), &Insert);
	}
	m_Button_dupliquer->Enable(false);
	m_Button_enregister->Enable(true);



}

void CLayOutFacturesDlg::OnSupprimer()
{
	auto szDel = CTools::Format(L"DELETE FROM \"Facturation Description Produit/Prestation\" WHERE \"Facture\"=%d;", RecordSetClient.GetFieldValue(L"N°", m_Record_Number).iVal);
	SQLRequest(theApp->m_Database_Channel, (wchar_t *)szDel.data(), NULL);
	szDel = CTools::Format(L"DELETE FROM \"Factures\" WHERE \"N°\"=%d;", RecordSetClient.GetFieldValue(L"N°", m_Record_Number).iVal);
	SQLRequest(theApp->m_Database_Channel, (wchar_t *)szDel.data(), NULL);
	m_Button_dupliquer->Enable(FALSE);
	m_Button_enregister->Enable(FALSE);
	m_Button_supprimer->Enable(FALSE);
	m_Button_imprimer->Enable(FALSE);

	int iAnnee = _wtoi(m_List_Annee->GetText(m_List_Annee->GetCurSel()).data());
	m_Record_Number = -1;
	Affiche_Annee(iAnnee);

}

void CLayOutFacturesDlg::OnEnregistrer()
{
	auto iNum = RecordSetClient.GetFieldValue(L"N°", m_Record_Number).iVal;
	auto szEcheance = m_Date_Echeance->GetText();
	auto szEcheance_Jour = szEcheance.substr(0, 2);
	auto szEcheance_mois = szEcheance.substr(3, 2);
	auto szEcheance_Annee = szEcheance.substr(6, 4);
	szEcheance = szEcheance_Annee + L"-" + szEcheance_mois + L"-" + szEcheance_Jour;

	auto szFacturation = m_Date_de_facturation->GetText();
	auto szszFacturation_Jour = szFacturation.substr(0, 2);
	auto szFacturation_mois = szFacturation.substr(3, 2);
	auto szFacturation_Annee = szFacturation.substr(6, 4);
	szFacturation = szFacturation_Annee + L"-" + szFacturation_mois + L"-" + szszFacturation_Jour;

	
	auto szPaiement = m_Date_de_paiement->GetText();
	if (!szPaiement.empty())
	{
		auto szPaiement_Jour = szPaiement.substr(0, 2);
		auto szPaiement_mois = szPaiement.substr(3, 2);
		auto szPaiement_annee = szPaiement.substr(6, 4);
		szPaiement = L"'" + szPaiement_annee + L"-" + szPaiement_mois + L"-" + szPaiement_Jour + L"'";
	}
	else
		szPaiement = L"Null";
	wstring szEncaissement;
	if (m_Encaissement->GetCheck()) szEncaissement = L"true";
	else szEncaissement = L"false";

	wstring szTva;
	if (m_Tva->GetCheck()) szTva = L"true";
	else szTva = L"false";

	wstring szRequest;
	CDataBaseRecordSet recordCommand;
	auto iSelPaiement = m_Mode_de_paiement->GetCurSel();
	auto szMode = m_Mode_de_paiement->GetText(iSelPaiement);
	szRequest = L"SELECT \"N°\" FROM \"Mode de Paiement\" WHERE \"Mode\"='" + szMode + L"'";
	SQLRequest(theApp->m_Database_Channel, (wchar_t *)szRequest.data(), &recordCommand);
	auto iNumPaiement = recordCommand.GetFieldValue(L"N°", 0).iVal;

	szRequest = L"UPDATE \"Factures\"";
	szRequest+= L"SET  \"Commande\" = " + m_Numero_Commande->GetText() + L" , ";
	szRequest += L"\"Date de Facturation\" = '" +  szFacturation + L"', ";
	szRequest += L"\"Facture avec TVA\" = " + szTva +L" , ";
	szRequest += L"\"Date de paiement\" = " + szPaiement + L", ";
	szRequest += L"\"Description pour Client\" = '" + m_Reference_Client->GetText() +L"' , ";
	szRequest += L"\"Encaissement\" = " + szEncaissement + L", ";
	szRequest += L"\"Date Echeance\" = '" + szEcheance + L"', ";
	szRequest += L"\"Remise globale\" = " + m_remise->GetText() + L" , ";
	szRequest += CTools::Format(L"\"Mode de Paiement\" = %d",iNumPaiement);
	szRequest += CTools::Format(L" WHERE \"N°\"=%d",iNum);
	SQLRequest(theApp->m_Database_Channel, (wchar_t*)szRequest.data(), &recordCommand);

}
void CLayOutFacturesDlg::OnImprimer()
{

	auto iNum = RecordSetClient.GetFieldValue(L"N°", m_Record_Number).iVal;
	CPrintFactureDlg dlg(iNum);
	int iReturn = DialogBoxParam(theApp->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_PRINT_FACTURE), GetWin(), (DLGPROC)CDialogNoMFC::EntryDialogDlgProc, (LPARAM)&dlg);
}


