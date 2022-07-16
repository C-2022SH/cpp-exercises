#include "pch.h"
#include "CPathManager.h"


CPathManager::CPathManager() :
	m_szContentPath{}
{

}

CPathManager::~CPathManager()
{

}


void CPathManager::initialize()
{
	// �Ӽ� > ��� > �۾� ���͸� ���� ��θ� ���� ��ο� �Ȱ��� ������ �ش�.
	GetCurrentDirectory(255, m_szContentPath);

	// �� �ܰ� ���� ������ �̵��ϱ� ���Ͽ� ���ڿ��� �߰��� �߶��ش�.
	int iLength = (int)wcslen(m_szContentPath);

	for (int i = iLength - 1; i >= 0; i--)
	{
		if (m_szContentPath[i] == '\\')
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\contents\\");
}
