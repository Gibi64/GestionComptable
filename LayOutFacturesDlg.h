#pragma once
#include "C:\Local\Softwares\Common64Win\GridNoMFC.h"
#include "C:\Local\Softwares\Common64Win\EditNoMFC.h"
#include "C:\Local\Softwares\Common64Win\CheckButtonNoMFC.h"
#include "C:\Local\Softwares\Common64Win\ComboNoMFC.h"
#include "C:\Local\Softwares\Common64Win\ButtonNoMFC.h"

#include "C:\Local\Softwares\Common64Win\MovableWnd.h"
class CLayOutFacturesDlg :
	public CMovableWnd
{
public:
	CLayOutFacturesDlg();
	~CLayOutFacturesDlg();
	virtual void InitDialog();
	LRESULT MovableWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);

	CDataBaseRecordSet RecordSetClient;
	void UpdateFormulaire(int iRecord);
private:
	int m_Record_Number;
	CButtonNoMFC *m_Button_dupliquer;
	CButtonNoMFC *m_Button_enregister;
	CButtonNoMFC *m_Button_supprimer;
	CButtonNoMFC* m_Button_imprimer;

	CListBoxNoMFC* m_List_Annee;
	CGridNoMFC GridFacture;
	CEditNoMFC * m_Reference_Client;
	CEditNoMFC * m_Numero_Commande;
	CEditNoMFC * m_Date_de_facturation;
	CEditNoMFC * m_Date_de_paiement;
	CEditNoMFC * m_Date_Echeance;
	CEditNoMFC * m_sumbrut;
	CEditNoMFC * m_remise;


	CCheckButtonNoMFC *m_Tva;
	CCheckButtonNoMFC *m_Encaissement;

	CComboNoMFC *m_Client;
	CComboNoMFC *m_Commande;
	CComboNoMFC *m_Mode_de_paiement;
	void Affiche_Annee(int iAnnee);

	void OnDupliquer();
	void OnSupprimer();
	void OnEnregistrer();
	void OnImprimer();

};

