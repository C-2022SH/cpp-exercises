#include "pch.h"
#include "CResourceManager.h"

#include "CPathManager.h"
#include "CResource.h"
#include "CTexture.h"


CResourceManager::CResourceManager()
{

}

CResourceManager::~CResourceManager()
{
	Delete_Map<wstring, CResource*> (m_mapTexture);
}


CTexture* CResourceManager::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{	
	// �̹� �ε��� �ؽ��İ� ���� ���
	CTexture* pTexture = FindTexture(_strKey);

	if (pTexture != nullptr)
		return pTexture;


	// �ε��� �ؽ��İ� ���� ���
	wstring strFilePath = CPathManager::GetInstance()->GetContentsPath();
	strFilePath += _strRelativePath;

	pTexture = new CTexture;
	pTexture->Load(strFilePath);

	pTexture->SetKey(_strKey);
	pTexture->SetRelativePath(_strRelativePath);

	m_mapTexture.insert(make_pair(_strKey, pTexture));

	return pTexture;
}


CTexture* CResourceManager::FindTexture(const wstring& _strKey)
{
	map<wstring, CResource*>::iterator iter = m_mapTexture.find(_strKey);
	
	// ã�� ����� ���� ���
	if (iter == m_mapTexture.end())
		return nullptr;

	return (CTexture*)iter->second;
}
