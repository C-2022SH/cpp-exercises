#include "pch.h"
#include "CAnimation.h"

#include "CTimeManager.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCollider.h"

#include "CCamera.h"


CAnimation::CAnimation() :
	m_strName{},
	m_pAnimator(nullptr),
	m_pTexture(nullptr),
	m_iCurFrame(0),
	m_fAccTime(0.f),
	m_bIsFinishedUpdate(false),
	m_bIsFinished(false)
{
}

CAnimation::~CAnimation()
{
}


void CAnimation::CreateAnimation(CTexture* _pTexture, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vStep, float _fDuration,
						UINT _iFrameCount, Vector2 _vOffset)
{
	m_pTexture = _pTexture;

	tAnimFrame _frame = {};

	for (UINT i = 0; i < _iFrameCount; i++)
	{
		_frame.fDuration = _fDuration;
		_frame.vSlice = _vSliceSize;
		_frame.vLeftTop = _vLeftTop + _vStep * (float)i;
		_frame.vOffset = _vOffset;

		m_vecFrame.push_back(_frame);
	}
}

void CAnimation::update()
{
	// �ִϸ��̼��� �� �� ��� ������ ����
	if (m_bIsFinished)
		return;

	m_fAccTime += fDeltaTime;

	if (m_fAccTime >= m_vecFrame[m_iCurFrame].fDuration)
	{
		m_fAccTime -= m_vecFrame[m_iCurFrame].fDuration;

		++m_iCurFrame;

		if (m_iCurFrame == m_vecFrame.size())
		{
			m_iCurFrame = 0;
			m_bIsFinishedUpdate = true;
			return;
		}
	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bIsFinished)
		return;

	if (m_bIsFinishedUpdate)
		m_bIsFinished = true;

	CObject* pOwner = m_pAnimator->GetOwnerObject();
	Vector2 vPos = pOwner->GetPosition() + m_vecFrame[m_iCurFrame].vOffset;	// offset ��ġ��ŭ �߰�

	vPos = CCamera::GetInstance()->GetRenderPosition(vPos);	// ������ ��ġ

	Vector2 vScale = m_vecFrame[m_iCurFrame].vSlice;	// ���� �׸� �̹����� �����̽� ������(ũ��)
	//pOwner->GetCollider()->SetScale(Vector2(40.f, 40.f));

	GdiTransparentBlt(_dc,
					int(vPos.x - vScale.x * 0.5f),
					int(vPos.y - vScale.y * 0.5f),
					(int)vScale.x, (int)vScale.y,
					m_pTexture->GetDC(),
					int(m_vecFrame[m_iCurFrame].vLeftTop.x),	// �׸� �̹����� �»�� ����
					int(m_vecFrame[m_iCurFrame].vLeftTop.y),
					(int)vScale.x, (int)vScale.y,
					RGB(170,150,230));	// ����ȭ�� ��
}
