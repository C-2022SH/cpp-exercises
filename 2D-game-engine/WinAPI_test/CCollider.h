#pragma once


class CObject;
class CText;

class CCollider
{
private:
	static UINT		g_iNextID;

	CObject*		m_pOwner;

	// ũ��
	Vector2			m_vScale;				// �ݶ��̴� ũ��

	// ��ġ
	Vector2			m_vOffset;				// �������κ����� ������� �Ÿ�
	Vector2			m_vFinalPosition;		// ������Ʈ�� ��ġ�� �������� ���� ���� ������

	// ȸ��
	float			m_fCos;					// ���� ���Ϳ� ���� �ڻ��� ��
	float			m_fSin;					// ���� ���Ϳ� ���� ���� ��

	Vector2			m_vRotation;

	UINT			m_iID;					// ��ġ�� �ʴ� �ݶ��̴� ���� ID (�浹ü ���� �浹�� �����ϵ���)
	int				m_iCollided;			// �ε��� �ݶ��̴� ����
	bool			m_bIsTrigger;			// �浹ü�� trigger ���� collision ����


protected:
	map<CCollider*, Dir4>	m_mapCollidedDir;	// �ε��� �� �ݶ��̴��� ���� ����
												// x > 0 ������, x < 0 ����
												// y > 0 �Ʒ�, y < 0 ��
	Vector2					m_vCollidedDir;		// ��� ���� ������ ��


public:
	CObject* GetOwnerObject() { return m_pOwner; }
	Vector2 GetCollidedDir() { return m_vCollidedDir; }

	Vector2 GetOffset() { return m_vOffset; }
	Vector2 GetFinalPosition() { return m_vFinalPosition; }
	Vector2 GetScale() { return m_vScale; }

	Vector2 GetRotation() { return m_vRotation; }
	Vector2 GetDirVector(float _fHori, float _fVerti)
	{
		Vector2 vHalfScale = m_vScale * 0.5f;

		float fX = _fHori * vHalfScale.x * m_fCos - _fVerti * vHalfScale.y * m_fSin;
		float fY = _fHori * vHalfScale.x * m_fSin + _fVerti * vHalfScale.y * m_fCos;

		return Vector2(fX, fY);
	}

	Vector2 GetRotatedVertex(float _fHori, float _fVerti)
	{
		Vector2 vHalfScale = m_vScale * 0.5f;

		float fX = (_fHori * vHalfScale.x * m_fCos - _fVerti * vHalfScale.y * m_fSin) + m_vFinalPosition.x;
		float fY = (_fHori * vHalfScale.x * m_fSin + _fVerti * vHalfScale.y * m_fCos) + m_vFinalPosition.y;

		return Vector2(fX, fY);
	}
	
	UINT GetID() { return m_iID; }
	
	bool IsTrigger() { return m_bIsTrigger; }
	bool IsRotated() { if (m_fCos == 1 && m_fSin == 0) return false; else return true; }

	void SetScale(Vector2 _vScale) { m_vScale = Vector2(abs(_vScale.x), abs(_vScale.y)); }
	void SetOffset(Vector2 _vPos) { m_vOffset = _vPos; }

	void SetRotation(Vector2 _vRot)
	{
		assert(_vRot != Vector2());

		m_vRotation = _vRot;

		float fRotVecLength = _vRot.Length();

		m_fCos = _vRot.x / fRotVecLength;
		m_fSin = _vRot.y / fRotVecLength;
	}

	void SetIsTrigger(bool _bTrig) { m_bIsTrigger = _bTrig; }


public:
	// ������Ʈ�� ��ġ�� ���󰣴�.
	void finalUpdate();
	void render(HDC _dc);

public:
	// �浹�� ��� ó��
	void OnCollisionEnter(Dir4 _dCollidedDir, CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

	void OnTriggerEnter(CCollider* _pOther);
	void OnTrigger(CCollider* _pOther);
	void OnTriggerExit(CCollider* _pOther);


public:
	CCollider(CObject* _owner, bool _bIsTrigger, Vector2 _vRot = Vector2(1.f, 0.f));

	// ����Ǵ� ���
	CCollider(const CCollider& _origin, CObject* _owner);
	virtual ~CCollider();

	// �ݶ��̴��� ������ �� ����.
	CCollider& operator = (CCollider& _origin) = delete;
	//friend class CObject;
};