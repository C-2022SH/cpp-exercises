#include "pch.h"
#include "CCollisionManager.h"

#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"


CCollisionManager::CCollisionManager() :
	m_arrCheck{}
{
}

CCollisionManager::~CCollisionManager()
{
}


void CCollisionManager::initialize()
{
}


void CCollisionManager::update()
{
	for (UINT iRow = 0; iRow < (UINT)OBJECT_TYPE::END; iRow++)
	{
		for (UINT iCol = iRow; iCol < (UINT)OBJECT_TYPE::END; iCol++)
		{
			// �� �׷��� �浹�ϵ��� �Ǿ��ִٸ� �浹 üũ
			if (m_arrCheck[iRow] & (1 << iCol))
				collisionGroup_update((OBJECT_TYPE)iRow, (OBJECT_TYPE)iCol);
		}
	}
}

void CCollisionManager::collisionGroup_update(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight)
{
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurrentScene();

	// ������ ��ȯ���� ���� ��ȯ �޴� ������ ���۷��� Ÿ������!
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	// ���� �浹 ������ �޾� �� iterator
	map<ULONGLONG, bool>::iterator iter;


	// �� �׷��� ��ü���� ���� �޾ƿ´�
	// ���� �׷쿡 �ִ� ��� �� �� üũ�ȴ�! ����
	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		CCollider* pLeftCollider = vecLeft[i]->GetCollider();

		if (pLeftCollider == nullptr)
			continue;


		for (size_t j = 0; j < vecRight.size(); j++)
		{
			CCollider* pRightCollider = vecRight[j]->GetCollider();

			// �ڱ� �ڽ��� ��쿡�� üũ ����!!
			if (pRightCollider == nullptr || vecLeft[i] == vecRight[j])
				continue;


			// �� �浹ü ���� ���̵� ����
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCollider->GetID();
			ID.iRight_id = pRightCollider->GetID();

			iter = m_mapCollisionInfo.find(ID.ID);


			// ������ ��ϵ� �Ǿ� ���� �ʴٸ� ���� �浹 ���� false
			if (iter == m_mapCollisionInfo.end())
			{
				m_mapCollisionInfo.insert(make_pair(ID.ID, false));
				iter = m_mapCollisionInfo.find(ID.ID);
			}
			

			// ==================
			// |	�浹 �׼�		|
			// ==================
			// 
			// ���� �浹 ���� ���
			if (isCollided(pLeftCollider, pRightCollider))
			{
				bool bIsTriggerAction = pLeftCollider->IsTrigger() || pRightCollider->IsTrigger();

				// �������� �浹�� ���
				if (iter->second)
				{
					// �� �� �ϳ��� ���� ������ ��� ����
					if (!vecLeft[i]->IsAlive() || !vecRight[j]->IsAlive())
					{
						// �� �� �ϳ��� Ʈ���Ŷ�� Ʈ���� ó��
						if (bIsTriggerAction)
						{
							pLeftCollider->OnTriggerExit(pRightCollider);
							pRightCollider->OnTriggerExit(pLeftCollider);
						}
						else
						{
							pLeftCollider->OnCollisionExit(pRightCollider);
							pRightCollider->OnCollisionExit(pLeftCollider);
						}

						iter->second = false;
					}

					// �ƴ� ��� �浹
					else
					{
						if (bIsTriggerAction)
						{
							pLeftCollider->OnTrigger(pRightCollider);
							pRightCollider->OnTrigger(pLeftCollider);
						}
						else
						{
							pLeftCollider->OnCollision(pRightCollider);
							pRightCollider->OnCollision(pLeftCollider);
						}
					}
				}

				// �������� �浹���� ���� ���
				else
				{
					// �� �� ���� ������ �ƴ� ��쿡�� �۵�
					if (vecLeft[i]->IsAlive() && vecRight[j]->IsAlive())
					{
						if (bIsTriggerAction)
						{
							pLeftCollider->OnTriggerEnter(pRightCollider);
							pRightCollider->OnTriggerEnter(pLeftCollider);
						}
						else
						{
							Dir4 dCollidedDir = check_collisionDir(pLeftCollider, pRightCollider);

							pLeftCollider->OnCollisionEnter(dCollidedDir, pRightCollider);
							pRightCollider->OnCollisionEnter(dCollidedDir.Reverse(), pLeftCollider);
						}

						iter->second = true;
					}
				}
			}

			// ���� �浹 ������ ���� ��� �׼�
			else
			{
				// �������� �浹�� ���
				if (iter->second)
				{
					if (pLeftCollider->IsTrigger() || pRightCollider->IsTrigger())
					{
						pLeftCollider->OnTriggerExit(pRightCollider);
						pRightCollider->OnTriggerExit(pLeftCollider);
					}
					else
					{
						pLeftCollider->OnCollisionExit(pRightCollider);
						pRightCollider->OnCollisionExit(pLeftCollider);
					}

					iter->second = false;
				}
			}
		}
	}
}


bool CCollisionManager::isCollided(CCollider* _pLeft, CCollider* _pRight)
{
	return check_axisCollision(_pLeft, _pRight) && check_axisCollision(_pRight, _pLeft);
}


bool CCollisionManager::check_axisCollision(CCollider* _pTarget, CCollider* _pOther)
{
	Vector2 vCenterDiff = _pTarget->GetFinalPosition() - _pOther->GetFinalPosition();

	// x��
	Vector2 vBaseAxis = _pTarget->GetRotation();

	Vector2 vBaseRelative = _pTarget->GetDirVector(1.f, 1.f);
	Vector2 vOtherRelative = _pOther->GetDirVector(1.f, 1.f);

	float fTest = abs(vCenterDiff.Dot(vBaseAxis)) - abs(vBaseRelative.Dot(vBaseAxis));

	if ((fTest >= abs(_pOther->GetDirVector(1.f, 1.f).Dot(vBaseAxis)))
		&& (fTest >= abs(_pOther->GetDirVector(-1.f, 1.f).Dot(vBaseAxis))))
		return false;


	// y ��
	vBaseAxis.Reverse();

	if (vBaseAxis.x != 0 && vBaseAxis.y != 0)
	{
		vBaseAxis.x = -vBaseAxis.x;
	}

	fTest = abs(vCenterDiff.Dot(vBaseAxis)) - abs(vBaseRelative.Dot(vBaseAxis));

	if ((fTest >= abs(_pOther->GetDirVector(1.f, 1.f).Dot(vBaseAxis)))
		&& (fTest >= abs(_pOther->GetDirVector(-1.f, 1.f).Dot(vBaseAxis))))
		return false;

	return true;
}


Dir4 CCollisionManager::check_collisionDir(CCollider* _pLeft, CCollider* _pRight)
{
	// ������ ����
	Vector2 arrLeftVertices[4];
	Vector2 arrRightVertices[4];

	order_vertices(arrLeftVertices, _pLeft);
	order_vertices(arrRightVertices, _pRight);

	// ===========================
	//  ���� �������θ� �浹�ϴ� ���
	// ===========================
	// 
	// --- ����(����) ---
	// 
	// ���� �ֻ�ܰ� ����� ���ϴ��� �����ϴٸ� - ���θ� �浹
	if (abs(arrLeftVertices[1].y - arrRightVertices[3].y) < COLL_BOUND)
		return Dir4(0, 1, 0, 0);

	// ���� ���ϴܰ� ����� �ֻ���� �����ϴٸ� - �Ʒ��θ� �浹
	else if (abs(arrLeftVertices[3].y - arrRightVertices[1].y) < COLL_BOUND)
		return Dir4(0, 0, 0, 1);

	// --- ����(�¿�) ---
	// 
	// ���� �ֿ�ܰ� ����� ���´��� �����ϴٸ� - ���������θ� �浹
	if (abs(arrLeftVertices[2].x - arrRightVertices[0].x) < COLL_BOUND)
		return Dir4(0, 0, 1, 0);

	// ���� ���´ܰ� ����� �ֿ���� �����ϴٸ� - �������θ� �浹
	else if (abs(arrLeftVertices[0].x - arrRightVertices[2].x) < COLL_BOUND)
		return Dir4(1, 0, 0, 0);


	// ========================
	//  �� �������� �浹�ϴ� ���
	// ========================
	// 
	Dir4 dCollidedDir = Dir4();

	// --- ����(����) ---
	//
	// ���� ���´ܰ� �ֿ�� ���̿� ����� �ֿ���� �ִ�.
	if (arrLeftVertices[0].x > arrRightVertices[0].x && arrLeftVertices[2].x > arrRightVertices[2].x)
	{
		// ���� ���´��� ����� �ֿ�ܺ��� �Ʒ��� �ִٸ� - ������ �浹
		if (arrLeftVertices[0].y > arrRightVertices[2].y)
			dCollidedDir += Dir4(0, 1, 0, 0);

		// �ƴ϶�� - �Ʒ����� �浹
		else
			dCollidedDir += Dir4(0, 0, 0, 1);
	}

	// ���� ���´ܰ� �ֿ�� ���̿� ����� ���´��� �ִ�.
	else if (arrLeftVertices[0].x < arrRightVertices[0].x && arrLeftVertices[2].x < arrRightVertices[2].x)
	{
		// ���� �ֿ���� ����� ���´ܺ��� �Ʒ��� �ִٸ� - ������ �浹
		if (arrLeftVertices[2].y > arrRightVertices[0].y)
			dCollidedDir += Dir4(0, 1, 0, 0);

		// �ƴ϶�� �Ʒ����� - �浹
		else
			dCollidedDir += Dir4(0, 0, 0, 1);
	}

	// ����� �¿� ���� ������ ũ��. (���� ��� �ȿ� �� �� �ִ�)
	else if (arrRightVertices[0].x < arrLeftVertices[0].x && arrRightVertices[2].x > arrLeftVertices[2].x)
	{
		float fDX = arrRightVertices[0].x - arrRightVertices[2].x;

		// �밢���� ������ ��� �׳� y ���� ��
		if (fDX == 0)
		{
			// ����� �߽��� ������ �Ʒ��ʿ� �ִٸ� - �Ʒ����� �浹
			if (_pLeft->GetFinalPosition().y < _pRight->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 0, 0, 1);

			// ����� �߽��� ������ ���ʿ� �ִٸ� - ������ �浹
			else
				dCollidedDir += Dir4(0, 1, 0, 0);
		}

		// �ƴ� ��� �����Լ� ���� �������� ��
		else
		{
			float fSlope = (arrRightVertices[0].y - arrRightVertices[2].y) / fDX;

			// ���� �߽��� �밢������ ���� �ִٸ� - �Ʒ����� �浹
			if ((fSlope * (_pLeft->GetFinalPosition().x - _pRight->GetFinalPosition().x) + _pRight->GetFinalPosition().y)
				> _pLeft->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 0, 0, 1);
			
			// ���� �߽��� �밢������ �Ʒ��� �ִٸ� - ������ �浹
			else
				dCollidedDir += Dir4(0, 1, 0, 0);
		}
	}

	// ���� �¿� ���� ��뺸�� ũ��. (��밡 �� �ȿ� �� �� �ִ�)
	else
	{
		float fDX = arrLeftVertices[0].x - arrLeftVertices[2].x;

		// �밢���� ������ ��� �׳� y ���� ��
		if (fDX == 0)
		{
			// ����� �߽��� ������ �Ʒ��ʿ� �ִٸ� - �Ʒ����� �浹
			if (_pLeft->GetFinalPosition().y < _pRight->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 0, 0, 1);

			// ����� �߽��� ������ ���ʿ� �ִٸ� - ������ �浹
			else
				dCollidedDir += Dir4(0, 1, 0, 0);
		}

		// �ƴ� ��� �����Լ� ���� �������� ��
		else
		{
			float fSlope = (arrLeftVertices[0].y - arrLeftVertices[2].y) / fDX;

			// ����� �߽��� �밢������ ���� �ִٸ� - ������ �浹
			if ((fSlope * (_pRight->GetFinalPosition().x - _pLeft->GetFinalPosition().x) + _pLeft->GetFinalPosition().y)
				> _pRight->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 1, 0, 0);

			// ����� �߽��� �밢������ �Ʒ��� �ִٸ� - �Ʒ����� �浹
			else
				dCollidedDir += Dir4(0, 0, 0, 1);
		}
	}


	// --- ����(�¿�) ---
	//
	// ���� �ֻ�ܰ� ���ϴ� ���̿� ����� �ֻ���� �ִ�.
	if (arrLeftVertices[1].y < arrRightVertices[1].y && arrLeftVertices[3].y < arrRightVertices[3].y)
	{
		// ���� ���ϴ��� ����� �ֻ�ܺ��� �����ʿ� �ִٸ� - ���ʿ��� �浹
		if (arrLeftVertices[3].x > arrRightVertices[1].x)
			dCollidedDir += Dir4(1, 0, 0, 0);

		// �ƴ϶�� - �����ʿ��� �浹
		else
			dCollidedDir += Dir4(0, 0, 1, 0);
	}

	// ���� �ֻ�ܰ� ���ϴ� ���̿� ����� ���ϴ��� �ִ�.
	else if (arrLeftVertices[1].y < arrRightVertices[1].y && arrLeftVertices[3].y < arrRightVertices[3].y)
	{
		// ���� �ֻ���� ����� ���ϴܺ��� �����ʿ� �ִٸ� - �����ʿ��� �浹
		if (arrLeftVertices[1].x > arrRightVertices[3].x)
			dCollidedDir += Dir4(1, 0, 0, 0);

		// �ƴ϶�� - ���ʿ��� �浹
		else
			dCollidedDir += Dir4(0, 0, 1, 0);
	}

	// ����� ���� ���� ������ ũ��. (���� ��� �ȿ� �� �� �ִ�)
	else if (arrRightVertices[1].y < arrLeftVertices[1].y && arrRightVertices[3].y > arrLeftVertices[3].y)
	{
		float fDY = arrRightVertices[1].y - arrRightVertices[3].y;

		// �밢���� ������ ��� �׳� x ���� ��
		if (fDY == 0)
		{
			// ����� �߽��� ������ �����ʿ� �ִٸ� - �����ʿ��� �浹
			if (_pLeft->GetFinalPosition().x < _pRight->GetFinalPosition().x)
				dCollidedDir += Dir4(0, 0, 1, 0);

			// ����� �߽��� ������ ���ʿ� �ִٸ� - ���ʿ��� �浹
			else
				dCollidedDir += Dir4(1, 0, 0, 0);
		}

		// �ƴ� ��� �����Լ� ���� �������� ��
		else
		{
			float fSlope = (arrRightVertices[1].x - arrRightVertices[3].x) / fDY;

			// ���� �߽��� �밢������ ���ʿ� �ִٸ� - �����ʿ��� �浹
			if ((fSlope * (_pLeft->GetFinalPosition().y - _pRight->GetFinalPosition().y) + _pRight->GetFinalPosition().x)
				> _pLeft->GetFinalPosition().x)
				dCollidedDir += Dir4(0, 0, 1, 0);

			// ���� �߽��� �밢������ �����ʿ� �ִٸ� - ���ʿ��� �浹
			else
				dCollidedDir += Dir4(1, 0, 0, 0);
		}
	}

	// ���� ���� ���� ��뺸�� ũ��. (��밡 �� �ȿ� �� �� �ִ�)
	else
	{
		float fDY = arrLeftVertices[1].y - arrLeftVertices[3].y;

		// �밢���� ������ ��� �׳� x ���� ��
		if (fDY == 0)
		{
			// ����� �߽��� ������ �����ʿ� �ִٸ� - �����ʿ��� �浹
			if (_pLeft->GetFinalPosition().x < _pRight->GetFinalPosition().x)
				dCollidedDir += Dir4(0, 0, 1, 0);

			// ����� �߽��� ������ ���ʿ� �ִٸ� - ���ʿ��� �浹
			else
				dCollidedDir += Dir4(1, 0, 0, 0);
		}

		// �ƴ� ��� �����Լ� ���� �������� ��
		else
		{
			float fSlope = (arrLeftVertices[1].x - arrLeftVertices[3].x) / fDY;

			// ����� �߽��� �밢������ ���ʿ� �ִٸ� - ���ʿ��� �浹
			if ((fSlope * (_pRight->GetFinalPosition().y - _pLeft->GetFinalPosition().y) + _pLeft->GetFinalPosition().x)
			> _pRight->GetFinalPosition().x)
				dCollidedDir += Dir4(1, 0, 0, 0);

			// ����� �߽��� �밢������ �����ʿ� �ִٸ� - �����ʿ��� �浹
			else
				dCollidedDir += Dir4(0, 0, 1, 0);
		}
	}

	return dCollidedDir;
}

// _arrVertices �� �ε����� [0 ���´�, 1 �ֻ��, 2 �ֿ��, 3 ���ϴ�] �� �ǹ��Ѵ�.
void CCollisionManager::order_vertices(Vector2* _arrVertices, CCollider* _pTarget)
{
	// ===========================
	//  ���� ���� �ش� ������ ã�´�.
	// ===========================
	// 
	// ���� �����ʿ� �ִ� ���� �������� �Ѵ�.
	// ȸ������ 0 ~ 90 �� ������ ��� �߽����κ��� [1.f, 1.f] ������ ���� ���� �����ʿ� ���� Ȯ���� �����Ƿ� �ʱⰪ�� �̷��� �����Ѵ�.
	float fHoriMax_x = 1.f;
	float fHoriMax_y = 1.f;

	// �̿��ϴ� ���� ������ ������ ũ�ٸ� (�°� ������� �شܰ��̶�� ��)
	if (abs(_pTarget->GetDirVector(fHoriMax_x, fHoriMax_y).x) < abs(_pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y).x))
	{
		// �̿��ϴ� ���� ����� �Ѵ�.
		fHoriMax_y *= -1.f;
	}

	// ��� ���� �������ٸ� (== ��� ���� ���� ������ ���̾��ٸ�)
	if (abs(_pTarget->GetDirVector(fHoriMax_x, fHoriMax_y).x) != _pTarget->GetDirVector(fHoriMax_x, fHoriMax_y).x)
	{
		// �밢�� ������ ���� ����� �Ѵ�.
		fHoriMax_x *= -1.f;
		fHoriMax_y *= -1.f;
	}

	// ���� ���� �ش� ���� ��ǥ�� �����Ѵ�.
	_arrVertices[2] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(fHoriMax_x, fHoriMax_y);
	_arrVertices[0] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(-fHoriMax_x, -fHoriMax_y);


	// ===========================
	//  ���� ���� �ش� ������ ã�´�.
	// ===========================
	// 
	// �Ʊ� ���ϴ� ���� �ĺ��� ã������ ���ʿ������� ���縦 �����Ѵ�.
	// ��� ���� �������ٸ� (== ��� ���� ���� ������ ���̾��ٸ�)
	if (abs(_pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y).y) != _pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y).y)
	{
		// ����� ����� �Ѵ�.
		fHoriMax_x *= -1.f;
		fHoriMax_y *= -1.f;
	}

	// ���� ���� �ش� ���� ��ǥ�� �����Ѵ�.
	_arrVertices[1] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(-fHoriMax_x, fHoriMax_y);
	_arrVertices[3] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y);
}


void CCollisionManager::CheckGroup(OBJECT_TYPE eLeft, OBJECT_TYPE eRight)
{
	/*
	 ��Ʈ�� �����ʺ��� ī��Ʈ�Ѵ�. (�б� ���϶��)
	 0 (8,4,2,1,0)
	 1 (8,4,2,1,0)
	 �̷� ������!
	 
	 ���� ����� �»�� �κи� ����Ѵ�. �� <= ��
	*/
	
	UINT iRow = (UINT)eLeft;
	UINT iCol = (UINT)eRight;

	if (iRow > iCol)
	{
		iCol = (UINT)eLeft;
		iRow = (UINT)eRight;
	}

	// ������ �ְ� ������ ����.
	m_arrCheck[iRow] ^= (1 << iCol);
}