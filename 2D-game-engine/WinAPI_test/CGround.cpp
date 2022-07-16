#include "pch.h"
#include "CGround.h"

#include "CResourceManager.h"

#include "CCollider.h"
#include "CTexture.h"

// �� - �⺻ ������
CGround::CGround(Vector2 _vPos, Vector2 _vScale, GROUND_TYPE _eType) :
	CObject(OBJECT_TYPE::GROUND),
	m_arrScale{},
	m_arrLeftTop{}
{
	// Ÿ�Կ� ���� �̸�, �ؽ��� ����
	if (_eType == GROUND_TYPE::WALL_SOLID)
	{
		m_strName = L"solid wall";
		m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"ground", L"texture\\wall.bmp");
	}
	else
		m_strName = L"transparent ground";

	// ������Ʈ ��ġ, ������ ����
	SetPosition(_vPos);
	SetScale(_vScale);

	// ����ƽ, �߷�
	SetStatic(true);
	SetGravity(false);

	// =========
	//  �ݶ��̴�
	// =========
	CreateCollider(false);
	GetCollider()->SetScale(_vScale);
}

// �����ϱ� ���� ����
CGround::CGround(Vector2 _vPos, GROUND_TYPE _eType, bool _bIsStatic, wstring _strName, bool _bIsTrigger) :
	CObject(OBJECT_TYPE::GROUND),
	m_eGroundType(_eType)
{
	InitLeftTop();
	InitScale();

	// ������Ʈ �̸� ����
	m_strName = _strName;

	// ������Ʈ ��ġ ����
	SetPosition(_vPos);

	// ����ƽ, �߷�
	SetStatic(_bIsStatic);
	SetGravity(false);


	// =========
	//  �ݶ��̴�
	// =========
	//
	CreateCollider(_bIsTrigger);
	GetCollider()->SetScale(m_arrScale[(UINT)m_eGroundType]);


	// ===========
	//   �ؽ���
	// ===========
	// 
	// �ؽ��� �ε�
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"ground", L"texture\\ground.bmp");

	// �̹����� �µ��� ũ�� ����
	SetScale(m_arrScale[(UINT)m_eGroundType]);
}

CGround::~CGround()
{
}


void CGround::update()
{
}

void CGround::render(HDC _dc)
{
	if (m_pTexture)
	{
		Vector2 vPos = GetPosition();
		vPos = CCamera::GetInstance()->GetRenderPosition(vPos);	// ������ ��ġ

		Vector2 vScale = GetScale();
		Vector2 vLeftTop = m_arrLeftTop[(UINT)m_eGroundType];


		GdiTransparentBlt(_dc,
			int(vPos.x - vScale.x * 0.5f),
			int(vPos.y - vScale.y * 0.5f),
			(int)vScale.x, (int)vScale.y,
			m_pTexture->GetDC(),
			(int)vLeftTop.x, (int)vLeftTop.y,
			(int)vScale.x, (int)vScale.y,
			RGB(170, 150, 230));	// ����ȭ�� ��
	}

	CObject::render(_dc);
}
