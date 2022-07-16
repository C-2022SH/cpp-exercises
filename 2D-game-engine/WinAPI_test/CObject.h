#pragma once

// �Ź� ���� ������ Ư����! ������� ��� ����
#include "CCamera.h"

class CCollider;
class CTexture;
class CAnimator;

class CObject
{
private:
	// basic info
	OBJECT_TYPE	m_eType;
	Vector2		m_vPosition;
	Vector2		m_vScale;

	int			m_iHP;
	int			m_iDeal;


	bool		m_bIsAlive;

	// component
	CCollider*		m_pCollider;
	CAnimator*		m_pAnimator;

	bool		m_bIsStatic;	// ������ �������� �ʴ´� == �ݶ��̴��� ������� �ʴ´�.
	bool		m_bIsGravity;	// �߷��� ������ �޴´�. ����Ʈ true

	// gravity
	float		m_fGravity;


protected:
	wstring		m_strName;
	CTexture*	m_pTexture;

	Dir4		m_dCollidedDir;


public:
	const wstring& GetName() { return m_strName; }
	OBJECT_TYPE GetType() { return m_eType; }
	Vector2 GetPosition() { return m_vPosition; }
	Vector2 GetScale() { return m_vScale; }
	CTexture* GetTexture() { return m_pTexture; }
	int GetHP() { return m_iHP; }
	int GetDeal() { return m_iDeal; }

	void SetType(OBJECT_TYPE _eType) { m_eType = _eType; }
	void SetPosition(Vector2 _vPos) { m_vPosition = _vPos; }
	void SetScale(Vector2 _vScale) { m_vScale = _vScale; }
	void SetHP(int _iHP) { m_iHP = _iHP; }
	void SetDeal(int _iDeal) { m_iDeal = _iDeal; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }

	void SetStatic(bool _bIsStatic) { m_bIsStatic = _bIsStatic; }
	void SetGravity(bool _bIsGravity) { m_bIsGravity = _bIsGravity; }

	void SetCollidedDir(Dir4 _dDir) { m_dCollidedDir = _dDir; }
	Dir4 GetCollidedDir() { return m_dCollidedDir; }

	bool IsAlive() { return m_bIsAlive; }
	bool IsStatic() { return m_bIsStatic; }
	bool IsGravity() { return m_bIsGravity; }

	void CreateCollider(bool _bIsTrigger);

	void CreateAnimator();


public:
	virtual void update() = 0;

	virtual void gravityUpdate() final;

	// final : ���⼭ ������ �� �������̶�� ��. �� �̻� �������̵� �� �� ������!
	virtual void finalUpdate() final;
	virtual void render(HDC _dc);

	// �浹�� ��� ó��
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	virtual void OnTriggerEnter(CCollider* _pOther) {};
	virtual void OnTrigger(CCollider* _pOther) {};
	virtual void OnTriggerExit(CCollider* _pOther) {};

	virtual CObject* Clone() = 0;	// �ڱ� �ڽ��� ���� ������Ʈ�� �ǵ��� �ش�.


private:
	void component_render(HDC _dc);
	void SetDead() { m_bIsAlive = false; }


public:
	CObject();
	CObject(OBJECT_TYPE _eType);

	// ���������
	CObject(const CObject& _origin);

	virtual ~CObject();


	friend class CEventManager;
};