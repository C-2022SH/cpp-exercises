#include "pch.h"
#include "CTimeManager.h"

#include "CCore.h"


CTimeManager::CTimeManager() :
	m_llPrevCount{},
	m_llCurrCount{},
	m_llFrequency{},
	m_dDeltaTime(0.),
	m_iCallCount(0)
{}

CTimeManager::~CTimeManager()
{}

void CTimeManager::initialize()
{
	// ���� ī��Ʈ ��; �ʴ� 100�������� ī��Ʈ
	QueryPerformanceCounter(&m_llPrevCount);

	// �ʴ� ī��Ʈ ��������
	QueryPerformanceFrequency(&m_llFrequency);
}

// �� �����ӿ� �ɸ� �ð� ���
void CTimeManager::update()
{
	QueryPerformanceCounter(&m_llCurrCount);

	// ���� �������� ī��Ʈ�� ���� �������� ī��Ʈ ���̸� �ʴ� ī��Ʈ Ƚ���� ������ - �� �� �ɷȴ��� �� �� ����!
	m_dDeltaTime = (m_llCurrCount.QuadPart - m_llPrevCount.QuadPart) / (double) m_llFrequency.QuadPart;

	// ���� ī��Ʈ �� ����
	m_llPrevCount = m_llCurrCount;


#ifdef _DEBUG
	if (m_dDeltaTime > (1. / 60.))
		m_dDeltaTime = (1. / 60.);
#endif // _DEBUG


	++m_iCallCount;
	m_dAccum += m_dDeltaTime;

	if (m_dAccum >= 1.)
	{
		m_iFPS = m_iCallCount;

		m_dAccum = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d,	DT : %f", m_iFPS, m_dDeltaTime);
		SetWindowText(CCore::GetInstance()->GetMainHwnd(), szBuffer);
	}
}
