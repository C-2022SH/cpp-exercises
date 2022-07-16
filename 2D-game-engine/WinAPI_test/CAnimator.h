#pragma once


class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnimations;	// ��� �ִϸ��̼�
	CAnimation*					m_pCurAnimation;	// ���� ��� ���� �ִϸ��̼�
	CObject*					m_pOwner;			// �ִϸ����͸� ������ ������Ʈ
	bool						m_bRepeat;			// �ݺ���� ����


public:
	CObject* GetOwnerObject() { return m_pOwner; }

public:
	void update();
	void render(HDC _dc);

	void CreateAnimation(const wstring& _strName, CTexture* _pTexture, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vStep,
					float _fDuration, UINT _iFrameCount, Vector2 _vOffset = Vector2(0.f, 0.f));
	CAnimation* FindAnimation(const wstring& _strName);
	void PlayAnimation(const wstring& _strName, bool _bRepeat);


public:
	CAnimator(CObject* _owner);
	CAnimator(CAnimator& _origin, CObject* _owner);
	~CAnimator();

};

