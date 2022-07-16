#include "pch.h"
#include "CBullet.h"

#include "CTimeManager.h"
#include "CResourceManager.h"
#include "CCollider.h"
#include "CTexture.h"
#include "CAnimator.h"


CBullet::CBullet(OBJECT_TYPE _eType, UINT _iDeal, wstring _strName) :
	CObject(_eType),
	m_iDeal(_iDeal),
	m_fSpeed(40.f),
	m_vDir(Vector2(1.f, 0.f)),
	m_fAccTime(0.f),
	m_fFlyDuration(2.f)
{
	SetType(_eType);
	m_strName = _strName;
	m_vDir.Normalize();

	// ����ƽ, �߷�
	SetStatic(false);
	SetGravity(false);

	// =========
	//  �ݶ��̴�
	// =========
	CreateCollider(true);
	GetCollider()->SetScale(Vector2(3.f, 10.f));

	// ===========
	//  �ִϸ��̼�
	// ===========
	// 
	// �ؽ��� �ε�
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"bullet", L"texture\\bullet.bmp");

	// �Ѿ� �ִϸ����� ����
	CreateAnimator();

	GetAnimator()->CreateAnimation(L"bullet blink", m_pTexture, Vector2(0.f, 0.f), Vector2(13.f, 13.f), Vector2(13.f, 0.f), 0.1f, 2, Vector2(0.f, -15.f));

	GetAnimator()->PlayAnimation(L"bullet blink", true);
}

CBullet::~CBullet()
{
}

void CBullet::update()
{
	Vector2 vPos = GetPosition();

	// ���� �������ٰ� õõ�� ������
	if (m_fAccTime <= m_fFlyDuration)
		m_fAccTime += fDeltaTime;
	else
	{
		if (IsAlive())
			DeleteEventObject(this);
	}
		
	vPos += m_vDir * m_fSpeed * m_fSpeed * (m_fFlyDuration - m_fAccTime) * fDeltaTime;

	

	SetPosition(vPos);
	GetAnimator()->update();
}

void CBullet::OnTriggerEnter(CCollider* _pOther)
{
	CObject* pCollidedObject = _pOther->GetOwnerObject();

	if (GetType() == OBJECT_TYPE::BULLET_PLAYER && pCollidedObject->GetType() == OBJECT_TYPE::MONSTER)
	{
		DeleteEventObject(this);
	}
}
