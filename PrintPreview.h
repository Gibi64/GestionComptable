#pragma once
#include "C:\Local\Softwares\Common64Win\Vue.h"
#include "sFont.h"
class CPrintPreview :
    public CVue
{
public:
	CPrintPreview(void);
	~CPrintPreview(void);
	virtual int Create(HINSTANCE hInstance, HWND hWndParent, RECT Rect);

	virtual LRESULT VueWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void DrawPreview(HDC pdc, RECT& rcBounds);
	map<int, double>* m_pMapOfColumnWidth;
	map<int, double>* m_pMapOfRowHeight;
	map<wstring, wstring>* m_pMapOfCellText;
	map<wstring, sFont*>* m_pMapOfCellFont;
	list<sBorder*>* m_pListOfBorder;
	double DefaultColumnWidth;
	double DefaultRowHeight;
	POINT GetPosition(wstring Range,HDC pdc);
	POINT GetCellSize(wstring Range, HDC pdc);
private:
	double m_Left_Margin;
	double m_Vertical_Margin;
	double m_Horz_Shift;
	double m_Vert_Shift;

	void OnHScroll(WPARAM wParam, LPARAM lParam);
	void OnVScroll(WPARAM wParam, LPARAM lParam);
};

