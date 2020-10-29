#pragma once
#include "stdafx.h"
#include "MovableWnd.h"
#include "ListNOMFC.h"
#include "EditNOMFC.h"

class CLayOutClientDlg :
	public CMovableWnd
{
public:
	CLayOutClientDlg();
	~CLayOutClientDlg();
	LRESULT MovableWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CListBoxNoMFC *m_List_Client;
	CEditNoMFC *m_Edit_Nom;
	CEditNoMFC *m_Edit_Adresse;
	CEditNoMFC *m_Edit_Ville;
	CEditNoMFC *m_Edit_Code_Postal;
	CEditNoMFC *m_Edit_Correspondant;
	virtual void InitDialog();
	CDataBaseRecordSet RecordSetClient;
	void UpdateFormulaire(int iRecord);
};

