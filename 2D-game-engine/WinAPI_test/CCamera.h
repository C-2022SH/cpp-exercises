#pragma once


class CObject;
class CCore;

class CCamera
{
	SINGLETON(CCamera);

private:
	Vector2		m_vCenter;				// ���� ������ ���� - �� ���� ī�޶� ���͸� �ΰڴ�
	Dir4		m_dBound;				// ī�޶� �ٿ����
	Vector2		m_vLookPosition;		// ī�޶� ���� �ִ� ��ġ
	Vector2		m_vMovingLookPosition;	// ���� �� ��ġ
	Vector2		m_vPrevlookPosition;	// ī�޶� ���� �����ӿ� ���� �ִ� ��ġ
	Vector2		m_vOffset;				// ī�޶� ������ - �� ���� ���Ͷ�� �Ѵ�. (ex: ���� 0.f, 0.f)

	CObject*	m_pTargetObject;		// ī�޶� ���� ������Ʈ
	Vector2		m_vDifference;			// ī�޶� ��ġ�� �ػ� �߾� ��ġ�� ����

	float		m_fFollowTime;			// ī�޶� ��ġ �����ϴ� �ð�
	float		m_fProportionalConst;	// �ӵ� ��ʻ��
	float		m_fFollowAccTime;		// ���󰡴� ���� ������ �ð�


public:
	void SetLookPosition(const float& _fMoveDist)
	{
		m_fProportionalConst = -2.f * _fMoveDist / (m_fFollowTime * m_fFollowTime);
		m_fFollowAccTime = 0.f;
	}

	void SetTarget(CObject* _pTarget) { m_pTargetObject = _pTarget; }
	void SetBound(Dir4 _dBound) { m_dBound = _dBound; }

	Vector2 GetCenter() { return m_vCenter; }
	Vector2 GetLookPosition() { return m_vMovingLookPosition; }
	Vector2 GetRenderPosition(Vector2 _vPos) { return _vPos - m_vDifference; }
	Vector2 GetRealPosition(Vector2 _vRenderPos) { return _vRenderPos + m_vDifference; }


public:
	void update();


private:
	void CalculateDiff();
};

