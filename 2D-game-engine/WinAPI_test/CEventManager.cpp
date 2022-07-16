#include "pch.h"
#include "CEventManager.h"

#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"


CEventManager::CEventManager()
{

}

CEventManager::~CEventManager()
{

}


void CEventManager::ExecuteEvent(const tEvent& _event)
{
	switch (_event.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		CObject* pNewObj = (CObject*)_event.lParam;

		CSceneManager::GetInstance()->GetCurrentScene()->AddObject(pNewObj);
	}
		break;

	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam: object address
		// ������Ʈ�� dead ���·� ����
		// ���� ���� ������Ʈ ��Ƶα�
		CObject* pDeadObject = (CObject*)_event.lParam;

		// �ߺ� ���� ����
		if (pDeadObject->IsAlive())
		{
			pDeadObject->SetDead();
			m_vecDead.push_back(pDeadObject);
		}
	}
		break;

	case EVENT_TYPE::CHANGE_SCENE:
		// lParam: scene type
		CSceneManager::GetInstance()->ChangeScene((SCENE_TYPE)_event.lParam, (int)_event.pParam);
		break;
	}
}


void CEventManager::update()
{
	// ====================================
	// ���� �����ӿ��� ����� �� ������Ʈ ���̱�
	// ====================================
	for (size_t i = 0; i < m_vecDead.size(); i++)
	{
		if(m_vecDead[i])
			delete m_vecDead[i];
	}

	m_vecDead.clear();


	// ==========
	// �̺�Ʈ ó��
	// ==========
	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		ExecuteEvent(m_vecEvent[i]);
	}

	m_vecEvent.clear();
}