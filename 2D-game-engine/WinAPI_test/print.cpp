

/*
* 
* WINAPI, C++
* 
* �浹ü�� �÷��̾� ��ũ��Ʈ �� �Ϻ��Դϴ�.
* 
*/


#include "pch.h"

/*
#include "CPlayer.h"

#include "CSceneManager.h"
#include "CScene.h"

#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"

#include "CBullet.h"

#include "CAnimator.h"
#include "CCollider.h"

#include "CAnimation.h"



// =============================
// 
//  ����� �� ���� �簢�� �浹ü
// 
// =============================
//
void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCollided;

	// ��� ���⿡�� �ε�������
	if (!m_pOwner->IsStatic())
	{
		Vector2 vOtherFinalPos = _pOther->GetFinalPosition();
		Vector2 vOtherScale = _pOther->GetScale();

		Dir4 dCollidedDir = Dir4();

		float fCheckBottomHit = m_vFinalPosition.y + m_vScale.y * 0.5f - (vOtherFinalPos.y - vOtherScale.y * 0.5f);
		float fCheckTopHit = vOtherFinalPos.y + vOtherScale.y * 0.5f - (m_vFinalPosition.y - m_vScale.y * 0.5f);
		float fCheckRightHit = vOtherFinalPos.x - vOtherScale.x * 0.5f - (m_vFinalPosition.x + m_vScale.x * 0.5f);
		float fCheckLeftHit = vOtherFinalPos.x + vOtherScale.x * 0.5f - (m_vFinalPosition.x - m_vScale.x * 0.5f);

		// �Ʒ����� �浹
		if (abs(fCheckBottomHit) <= 0.8f)
			dCollidedDir.down = 1;

		// ������ �浹
		if (abs(fCheckTopHit) <= 0.8f)
			dCollidedDir.up = 1;

		// �����ʿ��� �浹
		if (abs(fCheckRightHit) <= 0.8f)
			dCollidedDir.right = 1;

		// ���ʿ��� �浹
		if (abs(fCheckLeftHit) <= 0.8f)
			dCollidedDir.left = 1;


		// ���� ���� �𷺼� ������Ʈ
		Dir4 dFinalColDir = m_pOwner->GetCollidedDir() + dCollidedDir;
		m_pOwner->SetCollidedDir(dFinalColDir);

		m_mapCollidedDir.insert(make_pair(_pOther, dCollidedDir));
	}

	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	--m_iCollided;

	map<CCollider*, Dir4>::iterator iter = m_mapCollidedDir.find(_pOther);

	if (iter != m_mapCollidedDir.end())
	{
		Dir4 dFinalColDir = m_pOwner->GetCollidedDir() - iter->second;
		m_pOwner->SetCollidedDir(dFinalColDir);

		m_mapCollidedDir.erase(_pOther);
	}

	m_pOwner->OnCollisionExit(_pOther);
}




// ==================================
// 
//  2D �÷�����: �÷��̾� �̵��� ����
// 
// ==================================
//
void CPlayer::update()
{
	if (!IsStatic())
	{
		Vector2 vPos = GetPosition();

		// 1 �̸� ������ -1 �̸� ����! �������ͷ� ���� �� ���ϵ��� +- 1.f �� �����Ѵ�.
		float bIsMovingRight = 1.f;

		// �ִϸ��̼�
		m_strCurrAnim = L"player stop";
		bool bAnimRepeat = true;


		if (KEY_HOLD(KEY::RIGHT))
		{
			// ������ �� ��� �ִϸ��̼�/���� ������ �ٲپ� �ش�.
			m_strCurrAnim = L"player walk_right";

			// ���� ���� ���� ��� �̵�
			if (m_dCollidedDir.right <= 0)
				vPos.x += m_fPlayerSpeed * fDeltaTime;
		}

		if (KEY_HOLD(KEY::LEFT))
		{
			m_strCurrAnim = L"player walk_left";
			bIsMovingRight = -1.f;

			if (m_dCollidedDir.left <= 0)
				vPos.x -= m_fPlayerSpeed * fDeltaTime;
		}

		if (KEY_HOLD(KEY::UP))
		{
			m_strCurrAnim = L"player walk_right";

			if (m_dCollidedDir.up <= 0)
			{
				vPos.y -= m_fPlayerSpeed * fDeltaTime;

				// �濡�� ��� ��� ����� ���ϵ��� ��ġ ����
				if (vPos.y < 56.f)
					vPos.y = 56.f;
			}
		}

		if (KEY_HOLD(KEY::DOWN))
		{
			m_strCurrAnim = L"player walk_left";
			bIsMovingRight = -1.f;

			if (m_dCollidedDir.down <= 0)
			{
				vPos.y += m_fPlayerSpeed * fDeltaTime;

				// �濡�� ��� ���: ����� ���ϵ��� ��ġ ����
				if (vPos.y > 360.f)
					vPos.y = 360.f;
			}
		}

		// �������� ����
		if (m_dCollidedDir.up <= 0 && KEY_TAP(KEY::SPACE) && m_bIsJumping == false)
			m_bIsJumping = true;


		if (m_bIsJumping)
		{
			// �ð� ����
			m_fJumpAccTime += fDeltaTime;

			float fJumpTimePassed = m_fJumpAccTime - m_fJumpHalfDuration;

			// ���� ���� �ִ� ��� (�ö󰡾� �ϴµ� �� �ö󰡴� ���)
			// �ٷ� ������
			if (fJumpTimePassed <= 0.f && m_dCollidedDir.up > 0)
				m_fJumpAccTime = m_fJumpHalfDuration;

			// �Ʒ��� ���� �ִ� ��� (�������� �ϴµ� �� �������� ���)
			// ���� �ߴ�
			else if (fJumpTimePassed >= 0.f && m_dCollidedDir.down < 0)
			{
				m_fJumpAccTime = 0.f;
				m_bIsJumping = false;
				GetAnimator()->FindAnimation(m_strCurrAnim)->SetFrame(0);
			}

			// ����ϰ� �շ� �ִٸ� ���� ���
			else
			{
				// �ð��� ������ ��� ���� �ߴ�
				if (fJumpTimePassed >= m_fJumpHalfDuration)
				{
					m_fJumpAccTime = 0.f;
					m_bIsJumping = false;
					GetAnimator()->FindAnimation(m_strCurrAnim)->SetFrame(0);
				}
				else
				{
					vPos.y += fJumpTimePassed * m_fJumpSpeed * fDeltaTime;

					if (bIsMovingRight)
						m_strCurrAnim = L"player jump_right";
					else
						m_strCurrAnim = L"player jump_left";
				}
			}
		}

		SetPosition(vPos);

		if (KEY_TAP(KEY::A))
			CreateBullet(bIsMovingRight);

		if (GetAnimator())
		{
			GetAnimator()->PlayAnimation(m_strCurrAnim, bAnimRepeat);

			GetAnimator()->update();
		}
	}
}
*/