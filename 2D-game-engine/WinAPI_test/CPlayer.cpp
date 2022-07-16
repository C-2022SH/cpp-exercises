#include "pch.h"
#include "CPlayer.h"

#include "CSceneManager.h"
#include "CScene.h"

#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"

#include "CBullet.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CTexture.h"

#include "CAnimation.h"


CPlayer::CPlayer(Vector2 _vPos, bool _bIsStatic, wstring _strName, Vector2 _vCollider, bool _bIsTrigger) :
	CObject(OBJECT_TYPE::PLAYER),
	m_fIsMovingRight(1.f),
	m_fPlayerSpeed(200.f),
	m_fJumpHeight(350.f),
	m_fJumpHalfDuration(0.6f),
	m_fJumpSpeed(m_fJumpHeight / m_fJumpHalfDuration),
	m_fJumpAccTime(0.f),
	m_bIsJumping(true),
	m_strCurrAnim(L"player stop")
{
	// �÷��̾� ������Ʈ �̸�, ��ġ, ũ�� ����
	m_strName = _strName;
	SetPosition(_vPos);
	SetScale(Vector2(80.f, 80.f));

	// ü��, ����
	SetHP(10);
	SetDeal(2);

	// ����ƽ ����
	SetStatic(_bIsStatic);


	// =========
	//  �ݶ��̴�
	// =========
	CreateCollider(_bIsTrigger);
	GetCollider()->SetScale(Vector2(_vCollider.x, _vCollider.y * 0.5f));
	// GetCollider()->SetOffsetPosition(Vector2(0.f, -_vCollider.y * 0.5f));

	// ===========
	//  �ִϸ��̼�
	// ===========
	// 
	// �ؽ��� �ε�
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"player texture", L"texture\\playerTextures.bmp");

	// �÷��̾� �ִϸ����� ����
	CreateAnimator();

	// ����
	GetAnimator()->CreateAnimation(L"player stop", m_pTexture,
					Vector2(3.f, 640.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.2f, 5, Vector2(0.f, -15.f));

	// �������� �ȱ�
	GetAnimator()->CreateAnimation(L"player walk_left", m_pTexture,
					Vector2(84.f, 158.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.1f, 7, Vector2(0.f, -15.f));

	// ���������� �ȱ�
	GetAnimator()->CreateAnimation(L"player walk_right", m_pTexture,
					Vector2(84.f, 0.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.1f, 7, Vector2(0.f, -15.f));

	// ���������� ����
	GetAnimator()->CreateAnimation(L"player jump_left", m_pTexture,
					Vector2(151.f, 800.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.2f, 6, Vector2(0.f, -15.f));

	// ���������� ����
	GetAnimator()->CreateAnimation(L"player jump_right", m_pTexture,
					Vector2(165.f, 720.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.2f, 6, Vector2(0.f, -15.f));


//	GetAnimator()->PlayAnimation(L"player stop", true);

	// ������ ���� ������ ����
	//CAnimation* pAnim = GetAnimator()->FindAnimation(_strAnimName);
	//pAnim->GetFrame(0).vOffset = Vector2(0.f, -20.f);
}


CPlayer::~CPlayer()
{
}

void CPlayer::update()
{
	// ===============
	//  ����/�ִϸ��̼�
	// ===============
	//
	if (!IsStatic())
	{
		Vector2 vPos = GetPosition();

		// �ִϸ��̼�
		m_strCurrAnim = L"player stop";
		bool bAnimRepeat = true;
	

		if (KEY_HOLD(KEY::RIGHT))
		{
			// ������ �� ��� �ִϸ��̼�/���� ������ �ٲپ� �ش�.
			m_strCurrAnim = L"player walk_right";
			m_fIsMovingRight = 1.f;

			// ���� ���� ���� ��� �̵�
			if (m_dCollidedDir.right <= 0)
			{
				vPos.x += m_fPlayerSpeed * fDeltaTime;

				// �ʿ��� ��� ��� ����� ���ϵ��� ��ġ ����
				if (vPos.x > (float)m_dBound.right)
					vPos.x = (float)m_dBound.right;
			}
		}

		if (KEY_HOLD(KEY::LEFT))
		{
			m_strCurrAnim = L"player walk_left";
			m_fIsMovingRight = -1.f;

			if (m_dCollidedDir.left <= 0)
			{
				vPos.x -= m_fPlayerSpeed * fDeltaTime;

				// �ʿ��� ��� ��� ����� ���ϵ��� ��ġ ����
				if (vPos.x < (float)m_dBound.left)
					vPos.x = (float)m_dBound.left;
			}
		}

		if (KEY_HOLD(KEY::UP))
		{
			m_strCurrAnim = L"player walk_right";

			if (m_dCollidedDir.up <= 0)
			{
				vPos.y -= m_fPlayerSpeed * fDeltaTime;

				// �ʿ��� ��� ��� ����� ���ϵ��� ��ġ ����
				if (vPos.y < (float)m_dBound.up)
					vPos.y = (float)m_dBound.up;
			}
		}

		if (KEY_HOLD(KEY::DOWN))
		{
			m_strCurrAnim = L"player walk_left";
			m_fIsMovingRight = -1.f;

			if (m_dCollidedDir.down <= 0)
			{
				vPos.y += m_fPlayerSpeed * fDeltaTime;

				// �ʿ��� ��� ���: ����� ���ϵ��� ��ġ ����
				if (vPos.y > (float)m_dBound.down)
					vPos.y = (float)m_dBound.down;
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

					if (m_fIsMovingRight)
						m_strCurrAnim = L"player jump_right";
					else
						m_strCurrAnim = L"player jump_left";
				}
			}
		}

		SetPosition(vPos);

		if (KEY_TAP(KEY::A))
			CreateBullet(m_fIsMovingRight);

		if (GetAnimator())
		{
			GetAnimator()->PlayAnimation(m_strCurrAnim, bAnimRepeat);

			GetAnimator()->update();
		}
	}
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	// =============
	//    HP ����
	// =============
	//
	CObject* pCollidedObj = _pOther->GetOwnerObject();

	if (pCollidedObj->GetType() == OBJECT_TYPE::MONSTER)
	{
		int iHP = GetHP() - pCollidedObj->GetDeal();

		if (iHP < 0)
			iHP = 0;

		SetHP(iHP);
	}
}

void CPlayer::render(HDC _dc)
{
	CObject::render(_dc);

	// =============
	//  ����׿� �ڵ�
	// =============
	//
	Vector2 vPos = CCamera::GetInstance()->GetRenderPosition(GetPosition());
	wstring strHP = std::to_wstring(GetHP());

	DRAW_LOG(_dc, 100, 150, 180, vPos.x, vPos.y, strHP, -60.f);
}

void CPlayer::CreateBullet(float _bMovingRight)
{
	// �Ѿ� ��ġ
	Vector2 vPos = GetPosition();
	vPos.x += _bMovingRight * GetScale().x * 0.5f;

	// �Ѿ� ����
	CBullet* pBullet = new CBullet(OBJECT_TYPE::BULLET_PLAYER, GetDeal());

	pBullet->SetPosition(vPos);
	pBullet->SetDirection(Vector2(_bMovingRight, 0.f));

	CreatEventObject(pBullet);
}