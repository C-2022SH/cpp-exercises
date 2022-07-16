#pragma once


class CCore
{
	SINGLETON(CCore);

private:
	HWND	m_hwnd;			// ������ �ڵ�
	POINT	m_ptResolution;	// ���� ������ �ػ�
	HDC		m_hDC;			// ���� �����쿡 �׸� DC (�����쿡�� �����ϴ� ����)

	HBITMAP	m_hBit;
	HDC		m_memDC;		// �纻�� DC

	// ���� ����ϴ� GDI Object
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];


public:
	int initalize(HWND _hwnd, POINT _ptResolution);
	void progress();


private:
	void CreateBrushPen();


public:
	HWND GetMainHwnd() { return m_hwnd; };
	HDC GetMainDC() { return m_hDC; };
	POINT GetResolution() { return m_ptResolution; };
	HBRUSH GetBrush(BRUSH_TYPE _eBrush) { return m_arrBrush[(UINT)_eBrush]; }
	HPEN GetPen(PEN_TYPE _ePen) { return m_arrPen[(UINT)_ePen]; }
};