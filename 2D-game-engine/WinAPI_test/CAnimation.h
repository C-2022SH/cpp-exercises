#pragma once


class CAnimator;
class CTexture;

struct tAnimFrame
{
	Vector2	vLeftTop;
	Vector2	vSlice;
	Vector2	vOffset;
	float	fDuration;
};

class CAnimation
{
private:
	wstring				m_strName;
	CAnimator*			m_pAnimator;
	CTexture*			m_pTexture;
	int					m_iCurFrame;			// �ִϸ��̼��� ���� ������
	float				m_fAccTime;				// �ִϸ��̼� ���� ��� �ð�
	bool				m_bIsFinishedUpdate;	// ������Ʈ���� finish ȣ���
	bool				m_bIsFinished;			// �ִϸ��̼� �� �� ����� ����Ǿ����� �˸�

	vector<tAnimFrame>	m_vecFrame;


private:
	void SetName(const wstring& _strName) { m_strName = _strName; }


public:
	bool IsFinished() { return m_bIsFinished; }
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; }
	
	void SetFrame(int _iFrame)
	{
		m_bIsFinishedUpdate = false;
		m_bIsFinished = false;
		m_iCurFrame = _iFrame;
	}

	void CreateAnimation(CTexture* _pTexture, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vStep, float _fDuration,
					UINT _iFrameCount, Vector2 _Offset);


public:
	void update();
	void render(HDC _dc);

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

