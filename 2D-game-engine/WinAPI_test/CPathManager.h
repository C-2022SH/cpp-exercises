#pragma once
class CPathManager
{
	SINGLETON(CPathManager);

private:
	wchar_t		m_szContentPath[255];	// ������ �ִ�ġ ���� ���� ����

public:
	void initialize();

public:
	const wchar_t* GetContentsPath() { return m_szContentPath; }
};

