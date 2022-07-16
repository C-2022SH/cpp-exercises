#pragma once

// �ð� ����ȭ �Ŵ���


class CTimeManager
{
	SINGLETON(CTimeManager);

private:
	// FPS; deltaTime
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurrCount;
	LARGE_INTEGER	m_llFrequency;

	double			m_dDeltaTime;	// ������ ������ �ð�
	double			m_dAccum;		// deltaTime ������
	UINT			m_iCallCount;	// �ʴ� ȣ�� Ƚ��
	UINT			m_iFPS;			// �ʴ� ȣ�� Ƚ���� 1�� �� ����

public:
	void initialize();
	void update();

public:
	double GetDeltaTime() { return m_dDeltaTime; }
	float GetfDeltaTime() { return (float)m_dDeltaTime; }
};

