#pragma once
#include "C:\Local\Softwares\Common64Win\DialogNoMFC.h"
#include "C:\Local\Softwares\Common64Win\ListNoMFC.h"

#include "PrintPreview.h"


class CPrintFactureDlg :
    public CDialogNoMFC
{
private:
    int m_Record_Number;
    CListBoxNoMFC *m_List_Printers;
    CDataBaseRecordSet recordSetClient, recordCommand, recordSetFacture, recordSetCommande;
    void SetPrinterList();
    void SetMapsAndListsFromXml();
    double DefaultColumnWidth;
    double DefaultRowHeight;
    void PrintInvoice();
public:

    CPrintFactureDlg();
    CPrintFactureDlg(int Record_Number);

    ~CPrintFactureDlg();
    LRESULT DialogDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void InitDialog();
    CPrintPreview m_PreView;

    map<int, double> MapOfColumnWidth;
    map<int, double>MapOfRowHeight;
    map<wstring, wstring>MapOfCellText;
    map<wstring, sFont*> MapOfCellFont;
    list<sBorder*> ListOfBorder;

    int Column;
    static int GetColumnNumberFromAZ(wstring Range )
    {
        int Column = 0;
        int iRange = Range.find_first_of(L"123456789");
        if (iRange == string::npos) iRange = Range.length();
        for (int i = 0; i<iRange ; i++)
        {
            Column = Column * 26 + (Range[i] - 'A') + 1;
        }
        return Column;
    }
   
};

