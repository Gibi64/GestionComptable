#include "stdafx.h"
#include "PrintPreview.h"
#include "PrintFactureDlg.h"
CPrintPreview::CPrintPreview()
{
	m_Vertical_Margin = 2;
	m_Left_Margin = 2;
	m_Horz_Shift = 0;
	m_Vert_Shift = 0;
}
CPrintPreview::~CPrintPreview()
{

}
int CPrintPreview::Create(HINSTANCE hInstance, HWND hWndParent, RECT Rect)
{
	CVue::CreateWnd(L"Preview", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, hInstance, hWndParent, Rect);

	CreateScrollBars();
	double Convert = GetDeviceCaps(GetDC(m_hWnd), LOGPIXELSX);
	m_HorzBar.SetScrollRange(0, Convert * 21 / 2.54 );
	m_VertBar.SetScrollRange(0, Convert * 29.54 / 2.54);

	return 1;

}

LRESULT CPrintPreview::VueWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT r;
	GetWindowRect(GetWin(), &r);
	POINT leftTop = { r.left,r.top };
	ScreenToClient(GetWin(), &leftTop);
	POINT rightBottom = { r.right,r.bottom };
	ScreenToClient(GetWin(), &rightBottom);

	r.left = leftTop.x;
	r.top = leftTop.y;
	r.right = rightBottom.x - 17;
	r.bottom = rightBottom.y - 17;

	int i, j;
	std::wstring s;
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC pdc = BeginPaint(GetWin(), &ps);
		HDC dcMem = CreateCompatibleDC(pdc);
		HBITMAP memBM = CreateCompatibleBitmap(pdc, r.right - r.left, r.bottom - r.top);
		if (memBM)
		{
			SelectObject(dcMem, memBM);
			DrawPreview(dcMem, r);
			BitBlt(pdc, r.left, r.top, r.right - r.left, r.bottom - r.top,
				dcMem, 0, 0, SRCCOPY);
			DeleteDC(dcMem);
			DeleteObject(memBM);
		}
		EndPaint(GetWin(), &ps);
		break;
	}
	case WM_SCROLL_THUMB_HORZ:
	{
		DWORD wP = MAKELONG(SB_THUMBPOSITION, wParam);
		OnHScroll(wP, 0);
	}
	break;
	case WM_SCROLL_THUMB_VERT:
	{
		DWORD wP = MAKELONG(SB_THUMBPOSITION, wParam);
		OnVScroll(wP, 0);
	}
	break;
	case WM_SCROLL_UP:
	{
		OnVScroll(wParam, lParam);
		break;
	}

	case WM_SCROLL_DOWN:
	{
		OnVScroll(wParam, lParam);
		break;
	}
	case WM_SCROLL_LEFT:
	{
		OnHScroll(wParam, lParam);
		break;
	}
	case WM_SCROLL_RIGHT:
	{
		OnHScroll(wParam, lParam);
		break;
	}
	case WM_HSCROLL:
	{
		OnHScroll(wParam, lParam);
		break;
	}
	case WM_VSCROLL:
	{
		OnVScroll(wParam, lParam);
		break;
	}


		Invalidate(FALSE);
		break;
	default:
		CVue::VueWindowProc(msg, wParam, lParam);

	}


	return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

void CPrintPreview::DrawPreview(HDC pdc, RECT& rcBounds)
{

	HPEN pPen;
	pPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(pdc,pPen);
	HBRUSH pWhite = CreateSolidBrush(RGB(255,255,255));
	FillRect(pdc, &rcBounds, pWhite);

	for (auto it = m_pMapOfCellText->begin(); it != m_pMapOfCellText->end(); it++)
	{
		auto pFont = (*m_pMapOfCellFont)[(*it).first];
		int nWeight = FW_NORMAL;
		if (pFont->bBold) nWeight = FW_BOLD;
		HFONT FontText;
		FontText = CreateFont(
			-MulDiv(pFont->size, GetDeviceCaps(pdc, LOGPIXELSY), 72),             // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			nWeight,                 // nWeight
			pFont->bItalic,            // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			(LPCTSTR)pFont->Family.data());
		SelectObject(pdc, FontText);
		auto Range = (*it).first;
		auto Point_Text = GetPosition(Range,pdc);
		SetTextAlign(pdc, TA_LEFT | TA_BOTTOM);
		TextOut(pdc, Point_Text.x+3, Point_Text.y-3,(*it).second.data(), (*it).second.length());
		DeleteObject(FontText);
	}
	for (auto it = m_pListOfBorder->begin(); it != m_pListOfBorder->end(); it++)
	{
		auto ptFrom = GetPosition((*it)->FromRange, pdc);
		ptFrom.y -=GetCellSize((*it)->FromRange,pdc).y;
		auto ptTo = GetPosition((*it)->ToRange, pdc);
		ptTo.x+= GetCellSize((*it)->ToRange, pdc).x;

		MoveToEx(pdc, ptFrom.x, ptFrom.y, NULL);
		LineTo(pdc, ptTo.x, ptFrom.y);
		LineTo(pdc, ptTo.x, ptTo.y);
		LineTo(pdc, ptFrom.x, ptTo.y);
		LineTo(pdc, ptFrom.x, ptFrom.y);
	}
	DeleteObject(pPen);
}
POINT CPrintPreview::GetPosition(wstring Range,HDC pdc)
{
	int iRange;
	auto Multx = GetDeviceCaps(pdc, LOGPIXELSX);
	auto Multy = GetDeviceCaps(pdc, LOGPIXELSY);

	int Column = CPrintFactureDlg::GetColumnNumberFromAZ(Range);
	double x = 0 ,y = 0;
	iRange = Range.find_first_of(L"123456789");
	

	int Row = _wtoi(Range.substr(iRange).data());
	for (auto iRow = 1; iRow <= Row; iRow++)
	{
		if (m_pMapOfRowHeight->count(iRow))
		{
			y += (*m_pMapOfRowHeight)[iRow];
		}
		else
		{
			y += DefaultRowHeight;
		}
	}

	for (auto iColumn = 1; iColumn < Column; iColumn++)
	{
		if (m_pMapOfColumnWidth->count(iColumn))
		{
			x += (*m_pMapOfColumnWidth)[iColumn];
		}
		else
		{
			x += DefaultColumnWidth;
		}
	}
	x = m_Horz_Shift + x * Multx + m_Left_Margin * Multx / 2.54;
	y = -m_Vert_Shift + y * Multy + m_Vertical_Margin * Multy / 2.54;
	return { (int) x,(int) y };
}
POINT CPrintPreview::GetCellSize(wstring Range, HDC pdc)
{
	int iRange;
	auto Multx = GetDeviceCaps(pdc, LOGPIXELSX);
	auto Multy = GetDeviceCaps(pdc, LOGPIXELSY);

	int Column = CPrintFactureDlg::GetColumnNumberFromAZ(Range);
	double x = 0, y = 0;
	iRange = Range.find_first_of(L"123456789");

	if (m_pMapOfRowHeight->count(Column))
	{
		x += (*m_pMapOfColumnWidth)[Column];
	}
	else
	{
		x += DefaultColumnWidth;
	}
	int Row = _wtoi(Range.substr(iRange).data());
	if (m_pMapOfRowHeight->count(Row))
	{
		y += (*m_pMapOfRowHeight)[Row];
	}
	else
	{
		y += DefaultRowHeight;
	}
	x =  x * Multx ;
	y =  y * Multy ;
	return { (int)x,(int)y };

}
void CPrintPreview::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case SB_LINEUP:
		if (m_Horz_Shift > 0)
		{
			m_Horz_Shift-=10;
			m_HorzBar.SetCurrentPosition(m_Horz_Shift);
			Invalidate(FALSE);
		}
		break;
	case SB_LINEDOWN:
		if (m_Horz_Shift < m_HorzBar.GetMaxPos())
		{
			m_Horz_Shift-=10;
			m_HorzBar.SetCurrentPosition(m_Horz_Shift);
			Invalidate(FALSE);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_Horz_Shift = HIWORD(wParam);

		m_HorzBar.SetCurrentPosition(m_Horz_Shift);
		Invalidate(FALSE);
		break;
	}

}
void CPrintPreview::OnVScroll(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case SB_LINEUP:
		if (m_Vert_Shift > 0)
		{
			m_Vert_Shift-=10;
			m_VertBar.SetCurrentPosition(m_Vert_Shift);
			Invalidate(FALSE);
		}
		break;
	case SB_LINEDOWN:
		if (m_Vert_Shift <= m_VertBar.GetMaxPos())
		{
			m_Vert_Shift+=10;
			m_VertBar.SetCurrentPosition(m_Vert_Shift);
			Invalidate(FALSE);

		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_Vert_Shift = HIWORD(wParam);
		m_VertBar.SetCurrentPosition(m_Vert_Shift);
		Invalidate(FALSE);
		break;
	}
}