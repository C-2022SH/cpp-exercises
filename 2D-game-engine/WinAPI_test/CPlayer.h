#pragma once
#include "CObject.h"

class CBar;

class CPlayer :
    public CObject
{

private:
    wstring m_strCurrAnim;          // ���� �÷��� ���� �ִϸ��̼�

    float   m_fPlayerSpeed;
    float   m_fJumpHeight;
    float   m_fJumpHalfDuration;    // �ö󰡴� �� �ɸ��� �ð�
    float   m_fJumpSpeed;           // �ʴ� �̵� �ӵ�
    float   m_fJumpAccTime;

    bool    m_bIsJumping;
    float   m_fIsMovingRight;       // 1 �̸� ������ -1 �̸� ����! �������ͷ� ���� �� ���ϵ��� +- 1.f �� �����Ѵ�.

    Dir4    m_dBound;


public:
    Dir4 GetBound() { return m_dBound; }
    void SetBound(Dir4 _dBound) { m_dBound = _dBound; }

private:
    void CreateBullet(float _bMovingRight);


public:
    virtual void update();
    virtual void render(HDC _dc);

    virtual void OnCollisionEnter(CCollider* _pOther);

    // ���� ����!
    virtual CObject* Clone() { return nullptr; }
    CPlayer(const CPlayer& _origin) = delete;

public:
    CPlayer(Vector2 _vPos, bool _bIsStatic, wstring _strName = L"player", Vector2 _vCollider = Vector2(30.f, 30.f), bool _bIsTrigger = false);

    /*
    * �ڽ���Ʈ ��������� ���� ����ٸ� �̷��� �����!
    * 
    * CPlayer(const CPlayer& _origin) :
    *   CObject(_origin)
    * {}
    */

    ~CPlayer();
};

