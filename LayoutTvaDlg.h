#pragma once
#include "C:\Local\Softwares\Common64Win\DialogNoMFC.h"

#include "C:\Local\Softwares\Common64Win\MovableWnd.h"
#include "C:\Local\Softwares\Common64Win\EditNoMFC.h"
#include "C:\Local\Softwares\Common64Win\EditNoMFC.h"
#include "C:\Local\Softwares\Common64Win\GridNoMFC.h"
class CLayOutTvaDlg :
    public CMovableWnd
{
public:
    CLayOutTvaDlg();
    ~CLayOutTvaDlg();
    LRESULT MovableWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
    void InitDialog();
    void OnActivate();
private:
    CGridNoMFC  m_Grid_Facture;
    CListBoxNoMFC *m_List_Annee;
    void UpdateGrid();
    CDataBaseRecordSet record_facture;
    int m_Annee;
    CEditNoMFC* m_edit_premier_semestre;
    CEditNoMFC* m_edit_deuxieme_semestre;
    CEditNoMFC* m_edit_reliquat;

};

