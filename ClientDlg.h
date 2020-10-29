#pragma once
#include "stdafx.h"
#include "ListNOMFC.h"
#include "EditNOMFC.h"
#include "DialogNoMFC.h"
#include "resource.h"
#include "..\Common64\\DataBaseRecordSet.h"
class CClientDlg : public CDialogNoMFC
{
public:
	CClientDlg();
	~CClientDlg();
	CListBoxNoMFC *m_List_Client;
	CEditNoMFC *m_Edit_Nom;
	CEditNoMFC *m_Edit_Adresse;
	CEditNoMFC *m_Edit_Ville;
	CEditNoMFC *m_Edit_Code_Postal;
	CEditNoMFC *m_Edit_Correspondant;

private:
	HWND m_hWnd;
	virtual void InitDialog();
	virtual LRESULT DialogDlgProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam);
	CDataBaseRecordSet RecordSetClient;
	void UpdateFormulaire(int iRecord);

};

