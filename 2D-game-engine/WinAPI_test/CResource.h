#pragma once


class CResource
{
private:
	wstring	m_strKey;			// ���ҽ��� ���� ���̵� ���ڿ�
	wstring m_strRelativePath;	// ���ҽ��� ��� ��� (���� ���� �������� ���)

public:
	wstring& GetKey() { return m_strKey; }
	wstring& GetRelativePath() { return m_strRelativePath; }

	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

public:
	CResource();
	virtual ~CResource();
};

