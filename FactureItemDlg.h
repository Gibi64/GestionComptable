#pragma once
#include "C:\Local\Softwares\Common64Win\DialogNoMFC.h"
#include "C:\Local\Softwares\Common64Win\EditNoMFC.h"
#include "C:\Local\Softwares\Common64Win\EditNoMFC.h"
#include "C:\Local\Softwares\Common64Win\GridNoMFC.h"
class CFactureItemDlg :
	public CDialogNoMFC
{
public:
	CFactureItemDlg();
	CFactureItemDlg(int Numero,int Duplicate);
	~CFactureItemDlg();
	LRESULT DialogDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void InitDialog();
	void OnNouveau();
	void OnDupliquer();
	void OnEnregistrer();
private:
	int m_Numero_Facture;
	int m_Duplicate_From;
	CEditNoMFC *m_Edit_Description;
	CEditNoMFC *m_Edit_Prix_Unitaire;
	CEditNoMFC *m_Edit_Quantite;
	CEditNoMFC *m_Edit_Remise;

	CGridNoMFC  m_Grid_Item;
	CButtonNoMFC * m_Button_Nouveau;
	CButtonNoMFC * m_Button_Dupliquer;
	CButtonNoMFC * m_Button_Supprimer;
	CButtonNoMFC * m_Button_Enregistrer;

	CButtonNoMFC * m_Engregistrer;
	CDataBaseRecordSet record_item;
	void UpdateGrid();
};

