#pragma once

// ���� ����
// ������ �ӵ��� ������ ���� �ʱ� ���� + ������Ʈ �ڵ尡 �����Ǹ� �� �ڵ嵵 �Ź� �����ؾ� �ϴ� ���� ����
// ��ȯ���� ����
class CObject;
class CUI;
class CTexture;


// ��� ���� ������ �����ϴ� Ŭ����
class CScene
{
private:
	// ������ ������Ʈ�� �׷� ���� ����
	// �θ������� Ÿ������ ȣ��
	//
	// ���� ������Ʈ
	vector<CObject*> m_arrObj[(UINT)OBJECT_TYPE::END];
	// UI ������Ʈ
	vector<CUI*>	 m_arrUI[(UINT)UI_TYPE::END];
	wstring		   	 m_strName;		// scene �̸�

	// ���� ���� ��
	CTexture*		m_pMapImage;	// �� �̹���
	Vector2			m_vSceneSize;	// �� ũ��

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	void SetMap(CTexture* _pMap) { m_pMapImage = _pMap; }
	CTexture* GetMap() { return m_pMapImage; }

	// ������Ʈ ����
	virtual void update();

	// ������ �۾� ������
	void finalUpdate();

	void render(HDC _dc);

	virtual void Enter();		// �� ���� ��
	virtual void Exit();		// �� ��Ż ��
	

public:
	void AddObject(CObject* _pObj);
	void AddUI(CUI* _pUI);

	const vector<CObject*>& GetGroupObject(OBJECT_TYPE _eType) { return m_arrObj[(UINT)_eType]; }

	void ShowMessage(wstring _strMsg);

protected:
	void DeleteObjectsByType(OBJECT_TYPE _eType);
	void DeleteUIByType(UI_TYPE _eType);
	void DeleteAllObjects();
	void DeleteAllUI();


public:
	CScene();
	virtual ~CScene();
};

