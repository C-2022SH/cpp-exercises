#pragma once


class CCollider;


#define COLL_BOUND 0.7f


union COLLIDER_ID
{
	struct
	{
		UINT	iLeft_id;
		UINT	iRight_id;
	};
	ULONGLONG ID;
};


class CCollisionManager
{
	SINGLETON(CCollisionManager);

private:
	map<ULONGLONG, bool> m_mapCollisionInfo;				// �ݶ��̴� �� ���� ������ �浹 ����
	UINT				 m_arrCheck[(UINT)OBJECT_TYPE::END];	// �� ��(�迭���)�� ��Ʈ�������� collision �� ���� ���� ������ ���θ� Ȯ���Ѵ�.


public:
	void initialize();
	void update();
	
public:
	void CheckGroup(OBJECT_TYPE eLeft, OBJECT_TYPE eRight);
	void ReleaseCollider() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJECT_TYPE::END); }	// �浹 üũ ���� ����

private:
	void collisionGroup_update(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight);
	bool isCollided(CCollider* _pLeft, CCollider* _pRight);

	bool check_axisCollision(CCollider* _pTarget, CCollider* _pOther);
	Dir4 check_collisionDir(CCollider* _pLeft, CCollider* _pRight);
	void order_vertices(Vector2* _arrVertices, CCollider* _pTarget);
};