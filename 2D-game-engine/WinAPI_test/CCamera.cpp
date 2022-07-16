#include "pch.h"
#include "CCamera.h"

#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"

#include "CObject.h"
#include "CCore.h"


CCamera::CCamera() :
	m_pTargetObject(nullptr),
	m_vOffset(Vector2(0.f, 0.f)),
	m_fFollowTime(0.3f),
	m_fProportionalConst(0.f),
	m_fFollowAccTime(0.3f)
{
	// ȭ�鿡�� ��ǥ ������(ī�޶� ����) ��ġ
	m_vCenter = (Vector2)(CCore::GetInstance()->GetResolution()) * 0.5f;
}

CCamera::~CCamera()
{
}

void CCamera::update()
{
	if (m_pTargetObject)
	{
		if (m_pTargetObject->IsAlive())
		{
			// ==========
			// �ٿ�� Ȯ��
			// ==========
			//
			float fTargetX = m_pTargetObject->GetPosition().x;
			float fTargetY = m_pTargetObject->GetPosition().y;

			// �¿� �ٿ��
			if (fTargetX < (float)m_dBound.left)
				fTargetX = (float)m_dBound.left;

			else if (fTargetX > (float)m_dBound.right)
				fTargetX = (float)m_dBound.right;

			// ���� �ٿ��
			if (fTargetY < (float)m_dBound.up)
				fTargetY = (float)m_dBound.up;

			else if (fTargetY > (float)m_dBound.down)
				fTargetY = (float)m_dBound.down;

			// ��ġ ����
			m_vLookPosition = Vector2(fTargetX, fTargetY);

			float fLength = (m_vLookPosition - m_vPrevlookPosition).Length();

			if (fLength > 5.f)
				SetLookPosition(fLength);

			CalculateDiff();
		}
		else
			m_pTargetObject = nullptr;
	}

	else
		m_vDifference = m_vCenter * -1.f;
}

void CCamera::CalculateDiff()
{
	// ���� look �� ���� look �� ���̸� �����Ͽ� ���� look �� ���Ѵ�.

	if (m_fFollowAccTime >= m_fFollowTime)
		m_vMovingLookPosition = m_vLookPosition;
	
	else
	{
		m_fFollowAccTime += fDeltaTime;

		float fDeltaSpeed = m_fProportionalConst * (m_fFollowAccTime - m_fFollowTime);
		Vector2 vLookDirNorm = (m_vLookPosition - m_vPrevlookPosition).Normalize();
		m_vMovingLookPosition = m_vPrevlookPosition + vLookDirNorm * fDeltaSpeed * fDeltaTime;
	}

	m_vDifference = m_vMovingLookPosition - m_vCenter;

	// ���� ��ġ�� ���� ��ġ��
	m_vPrevlookPosition = m_vMovingLookPosition;
}
