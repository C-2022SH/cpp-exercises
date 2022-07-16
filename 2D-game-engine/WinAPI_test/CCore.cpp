#include "pch.h"
#include "CCore.h"

#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"
#include "CCollisionManager.h"
#include "CEventManager.h"
#include "CGameManager.h"
#include "CCamera.h"

#include "SelectGDI.h"

#include "CResourceManager.h"
#include "CScene.h"
#include "CTexture.h"
#include "CPlayer.h"


CCore::CCore() :
	m_hwnd(0),
	m_ptResolution{},
	m_hDC(0),
	m_hBit(0),
	m_memDC(0),
	m_arrBrush{},
	m_arrPen{}
{

}

CCore::~CCore()
{
	ReleaseDC(m_hwnd, m_hDC);
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)BRUSH_TYPE::END; i++)
	{
		DeleteObject(m_arrBrush[i]);
	}

	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
}

int CCore::initalize(HWND _hwnd, POINT _ptResolution)
{
	m_hwnd = _hwnd;
	m_ptResolution = _ptResolution;

	RECT rt = {0, 0, m_ptResolution.x, m_ptResolution.y};

	// �ػ󵵿� �°� ������ ����
	// �ػ� : ��ü�� �׷��� �� �ִ� ����
	
	// SetWindowPos �� �ػ� + �޴� + ���� ���� ��� �����ϴ� ũ��
	// ���� ���� �װ� ������ ����� ���Ѵ�
	// ������ ���ڴ� �޴��� ���� ����
	// �갡 ���� rt �� �����ؼ� ���� �����Ѵ�!!
	// ���� ���� �ʹ� ū ��� �̷� ������ ����Ѵ�. �ּ���� _Inout_
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hwnd, nullptr, 150, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hwnd);

	// ���� ���۸� �뵵�� ��Ʈ�ʰ� DC ����
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP holdBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	// DC �⺻���� ��ȯ��. �ʿ� �����Ƿ� ����
	DeleteObject(holdBit);

	// ���� ����ϴ� ��, �귯�� ����
	CreateBrushPen();
	
	// manager �ʱ�ȭ
	CPathManager::GetInstance()->initialize();
	CTimeManager::GetInstance()->initialize();
	CKeyManager::GetInstance()->initialize();
	CSceneManager::GetInstance()->initialize();
	CCollisionManager::GetInstance()->initialize();

	return S_OK;
}



void CCore::progress()
{
	// ================
	//  manager update
	// ================
	CTimeManager::GetInstance()->update();
	CKeyManager::GetInstance()->update();
	CCamera::GetInstance()->update();


	// ================
	//   scene update
	// ================
	CSceneManager::GetInstance()->update();

	// �浹 üũ
	CCollisionManager::GetInstance()->update();


	// ================
	//    rendering
	// ================
	// �켱 ���ۿ� �׸���

	// ȭ�� �����
	// ����Ʈ ��� �̹���
	SelectGDI _brush(m_memDC, BRUSH_TYPE::BACKGROUND);
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
	
	// �� �̹���
	CTexture* pTexture = CSceneManager::GetInstance()->GetCurrentScene()->GetMap();

	// �ʵ� ������ ��ġ�� �׷��� ��
	Vector2 vRenderCam = CCamera::GetInstance()->GetRenderPosition(Vector2(m_ptResolution) * -0.5f);

	BitBlt(m_memDC, (int)vRenderCam.x, (int)vRenderCam.y, pTexture->Width(), pTexture->Height(),
		pTexture->GetDC(), 0, 0, SRCCOPY);

	// �� �̹���
	// ���� �ʰ� ����͵�! ���� �տ� �׷��� �����ϱ� �ڿ�������
	CSceneManager::GetInstance()->render(m_memDC);
	
	// hp �׸���
	//RECT rectArea = { 10,10,100,100 };
	//CPlayer* pPlayer = (CPlayer*) CSceneManager::GetInstance()->GetCurrentScene()->GetPlayerObject();
	//wstring strHP = std::to_wstring(pPlayer->GetPlayerHP());
	//DrawText(m_memDC, strHP.c_str(), -1, &rectArea, DT_LEFT);

	// ���� DC �� �Ű� �׷� �ش�.
	// ������, ������� ��ġ, ������ �׸�, ������ ��ġ
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_memDC, 0, 0, SRCCOPY);


	// ================
	//  delayed events
	// ================
	CEventManager::GetInstance()->update();
}


void CCore::CreateBrushPen()
{
	// brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BACKGROUND] = CreateSolidBrush(RGB(180, 150, 230));
	m_arrBrush[(UINT)BRUSH_TYPE::YELLOW] = CreateSolidBrush(RGB(254, 248, 189));

	// pen
	m_arrPen[(UINT)PEN_TYPE::HOLLOW] = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 2, RGB(229, 71, 103));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 2, RGB(167, 219, 76));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(50, 50, 240));
	m_arrPen[(UINT)PEN_TYPE::PURPLE] = CreatePen(PS_SOLID, 4, RGB(174, 130, 188));
}
