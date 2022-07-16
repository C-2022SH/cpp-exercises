#include "pch.h"
#include "CMonster.h"

#include "CTimeManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CCollider.h"
#include "CTexture.h"

#include "CAnimator.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CBullet.h"


CMonster::CMonster(Vector2 _vPos, wstring _strName, Vector2 _vCollider) :
	CObject(OBJECT_TYPE::MONSTER),
	m_fSpeed(120.f)
{
	m_strName = _strName;

	SetPosition(_vPos);
	SetScale(Vector2(80.f, 80.f));

	// ����, ü��
	SetHP(5);
	SetDeal(3);

	// ����ƽ ����
	SetStatic(false);

	// =========
	//  �ݶ��̴�
	// =========
	CreateCollider(false);
	GetCollider()->SetScale(Vector2(_vCollider.x, _vCollider.y * 0.5f));

	// ===========
	//  �ִϸ��̼�
	// ===========
	// 
	// �ؽ��� �ε�
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"player texture", L"texture\\playerTextures.bmp");

	// �÷��̾� �ִϸ����� ����
	CreateAnimator();

	// ����
	GetAnimator()->CreateAnimation(L"monster stop", m_pTexture, Vector2(3.f, 560.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.2f, 7, Vector2(0.f, -15.f));

	GetAnimator()->PlayAnimation(L"monster stop", true);
}

CMonster::~CMonster()
{
}


void CMonster::update()
{
	Vector2 vPlayerPos = CGameManager::GetInstance()->GetPlayerObject()->GetPosition();
	Vector2 vCurPos = GetPosition();

	if (vPlayerPos != vCurPos)
	{
		Vector2 vDirUnit = (vPlayerPos - vCurPos).Normalize();

		// ���� �̵�
		if (m_dCollidedDir.up <= 0 && vDirUnit.y < 0.f)
			vCurPos.y += vDirUnit.y * m_fSpeed * fDeltaTime;

		// �Ʒ��� �̵�
		if(m_dCollidedDir.down <= 0 && vDirUnit.y > 0.f)
			vCurPos.y += vDirUnit.y * m_fSpeed * fDeltaTime;

		// �������� �̵�
		if (m_dCollidedDir.left <= 0 && vDirUnit.x < 0.f)
			vCurPos.x += vDirUnit.x * m_fSpeed * fDeltaTime;

		// ���������� �̵�
		if (m_dCollidedDir.right <= 0 && vDirUnit.x > 0.f)
			vCurPos.x += vDirUnit.x * m_fSpeed * fDeltaTime;

		SetPosition(vCurPos);
	}

	GetAnimator()->update();
}

void CMonster::render(HDC _dc)
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

void CMonster::OnTriggerEnter(CCollider* _pOther)
{
	CObject* pCollidedObject = _pOther->GetOwnerObject();

	if (pCollidedObject->GetType() == OBJECT_TYPE::BULLET_PLAYER && pCollidedObject->IsAlive())
	{
		CBullet* pBullet = (CBullet*)pCollidedObject;

		int iHP = GetHP() - pBullet->GetDeal();

		if (iHP <= 0)
			DeleteEventObject(this);
		else
			SetHP(iHP);
	}
}