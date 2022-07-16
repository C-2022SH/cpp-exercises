#pragma once


struct tEvent
{
	EVENT_TYPE	eEvent;		// �̺�Ʈ Ÿ��
	// �÷����� ���� �ڷ���(�޸� ũ��)�� �ٲ�� �ڷ���
	DWORD_PTR	lParam;		// ������ ������Ʈ �ּ�
	DWORD_PTR	pParam;		// ��Ÿ ����
};

class CEventManager
{
	SINGLETON(CEventManager);

private:
	vector<tEvent> m_vecEvent;
	vector<CObject*> m_vecDead;

public:
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }

private:
	void ExecuteEvent(const tEvent& _event);


public:
	void update();
};

