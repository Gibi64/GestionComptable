#include "stdafx.h"
#include <winspool.h>
#include "PrintFactureDlg.h"
#include "..\Common64\\XmlParser.h"
#include "..\Common64\\Tools.h"
#include "..\Common64\AdoDatabase.h"
#include "TimeUtils.h"
#include<iostream>
#include<fstream>
#include<sstream>
extern CGestionComptableApp* theApp;
CPrintFactureDlg::CPrintFactureDlg()
{

}
CPrintFactureDlg::CPrintFactureDlg(int Record_Number)
{
	m_Record_Number = Record_Number;
}
CPrintFactureDlg::~CPrintFactureDlg()
{
	for (auto it = ListOfBorder.begin(); it != ListOfBorder.end(); it++ )
		delete (*it);
	ListOfBorder.clear();
	MapOfCellFont.clear();
}
LRESULT CPrintFactureDlg::DialogDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hWnd;
	switch (msg)
	{
	case WM_INITDIALOG:
		InitDialog();
		break;
	case WM_PAINT:
		break;
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

		case IDC_BUTTON_IMPRIMER:
			PrintInvoice();
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

void CPrintFactureDlg::InitDialog()
{


/////////////////////////////////////////////////////////////////////////////////////////////// Read the xml document ////////////////////////////
	SetPrinterList();
	SetMapsAndListsFromXml();
	wstring szRequest;
	szRequest = CTools::Format(L"SELECT * FROM \"Factures\" WHERE \"N°\"=%d", m_Record_Number);
	SQLRequest(theApp->m_Database_Channel,(wchar_t *)szRequest.data(),&recordSetFacture);
	if (recordSetFacture.IsEOF())
	{
		MessageBox(NULL, L"Cette facture n'existe pas", L"Error", MB_OK);
		EndDialog(m_hWnd, 1);
	}
	sFont* pFont = new sFont;
	pFont->Family = L"Calibri";
	pFont->size = 11;
	pFont->bBold = false;
	pFont->bItalic = false;

	sFont* pFontBold = new sFont;
	pFontBold->Family = L"Arial";
	pFontBold->size = 9;
	pFontBold->bBold = true;
	pFontBold->bItalic = false;

	sFont* pFontFacture = new sFont;
	pFontFacture->Family = L"Arial";
	pFontFacture->size = 18;
	pFontFacture->bBold = true;
	pFontFacture->bItalic = false;


	auto NumCommande = recordSetFacture.GetFieldValue(L"Commande", 0).iVal;
	szRequest = CTools::Format(L"SELECT * FROM \"Commandes\" WHERE \"N°\"=%d", NumCommande);

	SQLRequest(theApp->m_Database_Channel, (wchar_t *)szRequest.data(), &recordSetCommande);
	auto Numero_Client = recordSetCommande.GetFieldValue(L"Reference Client", 0).iVal;
	
	szRequest = CTools::Format(L"SELECT * FROM \"Clients\" WHERE \"N°\"=%d", Numero_Client);
	SQLRequest(theApp->m_Database_Channel, (wchar_t*)szRequest.data(), &recordSetClient);
	auto sz = recordSetClient.GetFieldValue(L"Nom", 0).wstringVal;
	MapOfCellText[L"F4"] = sz;
	MapOfCellFont[L"F4"] = pFont;
	MapOfCellText[L"F5"] = recordSetClient.GetFieldValue(L"Adresse", 0).wstringVal;
	MapOfCellFont[L"F5"] = pFont;

	int CodePostal = recordSetClient.GetFieldValue(L"Code Postal", 0).iVal;
	if (CodePostal < 0) CodePostal += 65536;
	MapOfCellText[L"F6"] = CTools::Format(L"%d", CodePostal) + L" " + recordSetClient.GetFieldValue(L"Ville", 0).wstringVal;
	MapOfCellFont[L"F6"] = pFont;
	sz = recordSetClient.GetFieldValue(L"Correspondant", 0).wstringVal;
	if (sz.find_first_of(L"\r\n") != string::npos)
	{
		auto l = sz.find_first_of(L"\r\n");
		MapOfCellText[L"F7"] = sz.substr(0, l);
		MapOfCellFont[L"F7"] = pFont;
		MapOfCellText[L"F8"] = sz.substr(l+2);
		MapOfCellFont[L"F8"] = pFont;

	}

	sz = recordSetFacture.GetFieldValue(L"Description pour Client", 0).wstringVal;
	int RowObj = 12;

	for (;;)
	{
		auto iLast = sz.find_first_of(L"\r\n");
		if (iLast == string::npos)
		{
			iLast = sz.length();
		}
		MapOfCellText[CTools::Format(L"B%d", RowObj)] = sz.substr(0, iLast);
		MapOfCellFont[CTools::Format(L"B%d", RowObj)] = pFont;
		RowObj++;
		iLast += 2;
		if (iLast > sz.length()) break;
		sz = sz.substr(iLast);
	}


	MapOfCellText[L"F3"] = CTimeUtils::TimetToString(recordSetFacture.GetFieldValue(L"Date de Facturation", 0).dateVal);
	MapOfCellFont[L"F3"] = pFont;

	MapOfCellText[L"C31"] = CTimeUtils::TimetToString(recordSetFacture.GetFieldValue(L"Date Echeance", 0).dateVal);
	MapOfCellFont[L"C31"] = pFontBold;
	auto Year = CTimeUtils::GetYear(recordSetFacture.GetFieldValue(L"Date de Facturation", 0).dateVal);
	MapOfCellText[L"C10"] = CTools::Format(L"%d/%d",Year,m_Record_Number);
	MapOfCellFont[L"C10"] = pFontFacture;

	szRequest = CTools::Format(L"SELECT \"Mode\" FROM \"Mode de Paiement\" WHERE \"N°\"=%d", recordSetFacture.GetFieldValue(L"Mode de Paiement",0).iVal);
	SQLRequest(theApp->m_Database_Channel, (wchar_t*)szRequest.data(), &recordCommand);
	MapOfCellText[L"C32"] = recordCommand.GetFieldValue(L"Mode", 0).wstringVal;
	MapOfCellFont[L"C32"] = pFont;
	szRequest = CTools::Format(L"SELECT * FROM \"Facturation Description Produit/Prestation\" WHERE \"Facture\"=%d", m_Record_Number);
	SQLRequest(theApp->m_Database_Channel, (wchar_t*)szRequest.data(), &recordCommand);
	double Sum_Pu_Brut = 0;
	double Sum_Remise = 0;

	RowObj = 18;
	while (!recordCommand.IsEOF())
	{
		auto szRowCell = CTools::Format(L"B%d", RowObj);
		MapOfCellText[szRowCell] = recordCommand.GetCurrentFieldValue(L"Description").wstringVal;
		MapOfCellFont[szRowCell] = pFont;

		szRowCell = CTools::Format(L"C%d", RowObj);
		int Quantite = recordCommand.GetCurrentFieldValue(L"Quantite").iVal;
		MapOfCellText[szRowCell] = CTools::Format(L"%d",Quantite);
		MapOfCellFont[szRowCell] = pFont;

		szRowCell = CTools::Format(L"D%d", RowObj);
		double Brut = recordCommand.GetCurrentFieldValue(L"PU brut").dblVal;
		MapOfCellText[szRowCell] = CTools::Format(L"%.2lf €", Brut);
		Sum_Pu_Brut += Brut*Quantite;
		MapOfCellFont[szRowCell] = pFont;

		szRowCell = CTools::Format(L"E%d", RowObj);
		double Pourcent_Remise = recordCommand.GetCurrentFieldValue(L"Remise %").dblVal;
		MapOfCellText[szRowCell] = CTools::Format(L"%.2lf", Pourcent_Remise);
		Sum_Remise += Brut * Quantite * Pourcent_Remise/100;
		MapOfCellFont[szRowCell] = pFont;

		szRowCell = CTools::Format(L"F%d", RowObj);
		MapOfCellText[szRowCell] = CTools::Format(L"%.2lf €", Brut - Pourcent_Remise * Brut / 100);
		MapOfCellFont[szRowCell] = pFont;

		szRowCell = CTools::Format(L"G%d", RowObj);
		MapOfCellText[szRowCell] = CTools::Format(L"%.2lf €", (Brut - Pourcent_Remise * Brut / 100) * Quantite);
		MapOfCellFont[szRowCell] = pFont;


		RowObj++;
		recordCommand.MoveNext();
	}

	MapOfCellText[L"G25"] = CTools::Format(L"%.2lf €", Sum_Pu_Brut);
	MapOfCellFont[L"G25"] = pFont;

	MapOfCellText[L"G26"] = CTools::Format(L"%.2lf", Sum_Remise/Sum_Pu_Brut*100);
	MapOfCellFont[L"G26"] = pFont;

	MapOfCellText[L"G27"] = CTools::Format(L"%.2lf €", Sum_Pu_Brut - Sum_Remise);
	MapOfCellFont[L"G27"] = pFont;

	MapOfCellText[L"G28"] = CTools::Format(L"%.2lf €", 0.2 * ( Sum_Pu_Brut - Sum_Remise));
	MapOfCellFont[L"G28"] = pFont;

	MapOfCellText[L"G29"] = CTools::Format(L"%.2lf €", 1.2 * (Sum_Pu_Brut - Sum_Remise));
	MapOfCellFont[L"G29"] = pFont;



	RECT r;
	auto hWnd_Preview = GetDlgItem(m_hWnd, IDC_PRINT_PREVIEW);
	GetWindowRect(hWnd_Preview, &r);
	POINT leftTop = { r.left, r.top };
	POINT rightBottom = { r.right, r.bottom };
	ScreenToClient(m_hWnd, &leftTop);
	ScreenToClient(m_hWnd, &rightBottom);
	r = { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
	m_PreView.m_pMapOfColumnWidth = &MapOfColumnWidth;
	m_PreView.m_pMapOfRowHeight = &MapOfRowHeight;
	m_PreView.m_pMapOfCellText = &MapOfCellText;
	m_PreView.m_pMapOfCellFont = &MapOfCellFont;
	m_PreView.m_pListOfBorder = &ListOfBorder;
	m_PreView.DefaultColumnWidth = DefaultColumnWidth;
	m_PreView.DefaultRowHeight = DefaultRowHeight;
	m_PreView.Create(theApp->GetInstance(), m_hWnd, r);
}

void CPrintFactureDlg::SetPrinterList()
{
	m_List_Printers = new CListBoxNoMFC(m_hWnd, IDC_LIST_PRINTERS);
	PRINTER_INFO_5* pi;
	DWORD bufferSize = 0;
	DWORD bufferNeeded = 0;
	DWORD Entries = 0;

	bool bEnum;

	bEnum = EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 5, NULL, bufferSize, &bufferNeeded, &Entries);

	LPBYTE pBuffer = new BYTE[bufferNeeded];
	bufferSize = bufferNeeded;
	bEnum = EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 5, pBuffer, bufferSize, &bufferNeeded, &Entries);
	pi = (PRINTER_INFO_5*)pBuffer;
	for (auto iPrn = 0; iPrn < Entries; iPrn++, pi++)
		m_List_Printers->AddString(pi->pPrinterName);
	delete[] pBuffer;

}

void CPrintFactureDlg::SetMapsAndListsFromXml()
{
	CXmlParserDocument DocFactureTemplate;
	std::ifstream file("Facture.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	size_t n = content.length();
	wchar_t* pwch = CTools::charToWchar(content.data());
	DocFactureTemplate.Text = pwch;
	delete[] pwch;
	DocFactureTemplate.Parse();
	auto pNode_Facture = DocFactureTemplate.first_node()->first_node(L"Facture");
	auto pNode_Columns = pNode_Facture->first_node(L"Columns");
	DefaultColumnWidth = pNode_Columns->GetValueDouble(L"Width") / 2.54;
	for (auto pNode_Column = pNode_Columns->first_node(L"Column"); pNode_Column; pNode_Column = pNode_Column->next_sibling())
	{
		int ColumnNumber = GetColumnNumberFromAZ(pNode_Column->GetValue(L"Id"));
		MapOfColumnWidth[ColumnNumber] = pNode_Column->GetValueDouble(L"Width") / 2.54;
	}

	auto pNode_Rows = pNode_Facture->first_node(L"Rows");
	DefaultRowHeight = pNode_Rows->GetValueDouble(L"Height") / 2.54;
	for (auto pNode_Row = pNode_Rows->first_node(L"Row"); pNode_Row; pNode_Row = pNode_Row->next_sibling())
	{
		MapOfRowHeight[pNode_Row->GetValueInt(L"Id")] = pNode_Row->GetValueDouble(L"Height") / 2.54;
	}

	auto pNode_Cells = pNode_Facture->first_node(L"Cells");
	for (auto pNode_Cell = pNode_Cells->first_node(L"Cell"); pNode_Cell; pNode_Cell = pNode_Cell->next_sibling())
	{
		MapOfCellText[pNode_Cell->GetValue(L"Id")] = pNode_Cell->GetValue(L"Text");
		sFont* pFont = new sFont;
		pFont->Family = pNode_Cell->GetValue(L"Font");
		pFont->bBold = pNode_Cell->GetValue(L"Bold") == L"true";
		pFont->bItalic = pNode_Cell->GetValue(L"Italic") == L"true";
		pFont->size = pNode_Cell->GetValueInt(L"FontSize");
		MapOfCellFont[pNode_Cell->GetValue(L"Id")] = pFont;
	}
	auto pNode_Borders = pNode_Facture->first_node(L"Borders");
	for (auto pNode_Border = pNode_Borders->first_node(L"Border"); pNode_Border; pNode_Border = pNode_Border->next_sibling())
	{
		sBorder* pBorder = new (sBorder);
		pBorder->FromRange = pNode_Border->GetValue(L"FromRange");
		pBorder->ToRange = pNode_Border->GetValue(L"ToRange");
		ListOfBorder.push_back(pBorder);
	}

}
void CPrintFactureDlg::PrintInvoice()
{
	PRINTER_INFO_2 *pInfo;
	DOCINFO di;
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = L"Facture";

	DWORD bufferNeeded;
	HANDLE phPrinter;
	auto iNum = m_List_Printers->GetCurSel();
	if (iNum != -1)
	{
		m_List_Printers->GetText(iNum);
		bool bOpen = OpenPrinter((LPWSTR) m_List_Printers->GetText(iNum).data(),&phPrinter,NULL);
		GetPrinter(phPrinter, 2, NULL, 0, &bufferNeeded);
		BYTE* pBuffer = new BYTE[bufferNeeded];
		GetPrinter(phPrinter, 2, pBuffer, bufferNeeded, &bufferNeeded);
		pInfo = (PRINTER_INFO_2*)pBuffer;
		HDC printerDC = CreateDC(L"WINSPOOL", pInfo->pPrinterName, pInfo->pPortName, NULL);
		bool bPrintingOk = StartDoc(printerDC ,&di);

		StartPage(printerDC);

		auto Mult = GetDeviceCaps(printerDC, LOGPIXELSX);
		double TailleX = 21. / 2.54 * Mult;
		double TailleY = 29.7 / 2.54 * Mult;

		RECT r = { 0, 0, TailleX, TailleY };

		m_PreView.DrawPreview(printerDC, r);

		EndDoc(printerDC);

	}
}