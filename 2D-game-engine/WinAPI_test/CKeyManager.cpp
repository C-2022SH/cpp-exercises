#include "pch.h"
#include "CKeyManager.h"

#include "CCore.h"


// �������� ���� ���� Ű���� ������Ű�� �迭
int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',

	VK_MENU,	//ALT,
	VK_CONTROL,	//CTRL,
	VK_LSHIFT,	//LSHIFT,
	VK_SPACE,	//SPACE,
	VK_RETURN,	//ENTER,
	VK_ESCAPE,	//ESC,
	VK_LBUTTON,	//CLICK_LEFT,
	VK_RBUTTON	//CLICK_RIGHT,

	//LAST
};

CKeyManager::CKeyManager()
{}

CKeyManager::~CKeyManager()
{}

void CKeyManager::initialize()
{
	for (int i = 0; i < (int)KEY::LAST; i++)
	{
		m_vKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void CKeyManager::update()
{
	/*
	�����찡 ���� ���� ���
	* ���� ��Ŀ�� �� �����츦 Ȯ���ϰ� ���� ���� �̷��� ������ �ҷ��ͼ�,
	* HWND hMainWnd = CCore::GetInstance()->GetMainHwnd();
	* �Ʒ� hWnd �� ������ �ƴ����� üũ�ϸ� �ȴ�
	*/

	HWND hWnd = GetFocus();		// ���� ��Ŀ�� �Ǿ� �ִ� ������

	// ������ ��Ŀ�� Ȯ��
	if (hWnd != nullptr)
	{
		// ���콺 ��ġ ���
		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);
		ScreenToClient(CCore::GetInstance()->GetMainHwnd(), &ptMousePos);

		m_vCurrMousePosition = Vector2(ptMousePos);


		// i : ���� ������ ���� Ű ��
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			// Ű�� ���ȴٸ�
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// ������ �����־��ٸ�
				if (m_vKey[i].bPrevPush)
					m_vKey[i].eState = KEY_STATE::HOLD;
				else
					m_vKey[i].eState = KEY_STATE::TAP;

				m_vKey[i].bPrevPush = true;
			}
			// Ű�� �� ���ȴٸ�
			else
			{
				if (m_vKey[i].bPrevPush)
					m_vKey[i].eState = KEY_STATE::AWAY;
				else
					m_vKey[i].eState = KEY_STATE::NONE;

				m_vKey[i].bPrevPush = false;
			}
		}
	}
	
	// ������ ��Ŀ�� ���� ����
	else
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			m_vKey[i].bPrevPush = false;

			if (m_vKey[i].eState == KEY_STATE::TAP || m_vKey[i].eState == KEY_STATE::HOLD)
				m_vKey[i].eState = KEY_STATE::AWAY;

			else if (m_vKey[i].eState == KEY_STATE::AWAY)
				m_vKey[i].eState = KEY_STATE::NONE;
		}
	}
}
