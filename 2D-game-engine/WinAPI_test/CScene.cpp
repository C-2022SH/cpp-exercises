#include "pch.h"
#include "CScene.h"

#include "CCore.h"

#include "CCollisionManager.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CTexture.h"

#include "CUI.h"
#include "CText.h"


CScene::CScene() :
	m_arrObj{},
	m_strName{},
	m_pMapImage(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			delete m_arrObj[i][j];
		}
	}

	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrUI[i].size(); j++)
		{
			delete m_arrUI[i][j];
		}
	}
}


void CScene::update()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			if (m_arrObj[i][j]->IsAlive())
			{
				m_arrObj[i][j]->update();
				m_arrObj[i][j]->gravityUpdate();
			}
		}
	}

	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrUI[i].size(); j++)
		{
			m_arrUI[i][j]->update();
		}
	}
}

// component update
// UI ������Ʈ�� �ݶ��̴��� �����Ƿ� �������� �ʴ´�
void CScene::finalUpdate()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// �浹 ó���� �������ؾ� �ϱ� ������ dead ������Ʈ�� final update �� ����
			m_arrObj[i][j]->finalUpdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if ((*iter)->IsAlive())
			{
				(*iter)->render(_dc);
				iter++;
			}

			else
				iter = m_arrObj[i].erase(iter);
		}
	}

	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrUI[i].size(); j++)
		{
			m_arrUI[i][j]->render(_dc);
		}
	}
}


void CScene::Enter()
{
	// ============
	//  �ٿ�� ���
	// ============
	//
	// ������ â ����, ���� / 2
	int pHalfResX = ((int)CCore::GetInstance()->GetResolution().x) / 2;
	int pHalfResY = ((int)CCore::GetInstance()->GetResolution().y) / 2;

	// �� ����, ����
	int iMapWidth = m_pMapImage->Width();
	int iMapHeight = m_pMapImage->Height();

	// ī�޶� ����
	int vCameraCenterX = (int)CCamera::GetInstance()->GetCenter().x;
	int vCameraCenterY = (int)CCamera::GetInstance()->GetCenter().y;

	// �����¿� �ٿ�� ���
	int iLeftBound = -vCameraCenterX;
	int iUpperBound = -vCameraCenterY;
	int iRightBound = iMapWidth - vCameraCenterX;
	int iLowerBound = iMapHeight - vCameraCenterY;

	// ���� ȭ�麸�� ���� ��� ����ó��
	// ���ΰ� ª�� ���
	if (pHalfResX * 2 >= iMapWidth)
	{
		iLeftBound = -pHalfResX;
		iRightBound = pHalfResX;
	}

	// ���ΰ� ª�� ���
	else if (pHalfResY * 2 >= iMapHeight)
	{
		iUpperBound = -pHalfResY;
		iLowerBound = pHalfResY;
	}
	

	// ============
	//  �ٿ�� ����
	// ============
	//
	// �÷��̾�
	CPlayer* pPlayer = CGameManager::GetInstance()->GetPlayerObject();

	if (pPlayer)
	{
		Dir4 dPlayerBound = Dir4(iLeftBound, iUpperBound, iRightBound, iLowerBound);

		// �÷��̾� Ư�� �ٿ�� �ݿ�
		pPlayer->SetBound(dPlayerBound + pPlayer->GetBound());

		// ī�޶�
		CCamera::GetInstance()->SetBound(dPlayerBound + Dir4(pHalfResX, pHalfResY, -pHalfResX, -pHalfResY));
	}
}

void CScene::Exit()
{
	DeleteAllObjects();
	DeleteAllUI();
	CCollisionManager::GetInstance()->ReleaseCollider();
}


void CScene::AddObject(CObject* _pObj)
{
	OBJECT_TYPE eType = _pObj->GetType();
	m_arrObj[(UINT)eType].push_back(_pObj);
}

void CScene::AddUI(CUI* _pUI)
{
	UI_TYPE eType = _pUI->GetType();
	m_arrUI[(UINT)eType].push_back(_pUI);
}

void CScene::ShowMessage(wstring _strMsg)
{
	CText* pText = new CText(L"Wonderful!", Dir4(-100, -20, 100, 20));

}


void CScene::DeleteObjectsByType(OBJECT_TYPE _eType)
{
	Delete_Vector<CObject*> (m_arrObj[(UINT)_eType]);
}


void CScene::DeleteUIByType(UI_TYPE _eType)
{
	Delete_Vector<CUI*> (m_arrUI[(UINT)_eType]);
}

void CScene::DeleteAllObjects()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		DeleteObjectsByType((OBJECT_TYPE)i);
	}
}

void CScene::DeleteAllUI()
{
	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		DeleteUIByType((UI_TYPE)i);
	}
}
