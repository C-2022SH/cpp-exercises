#pragma once

// ������ ����ȭ
// ��� ����
// 1. ���� ������ �������� ���� Ű�� ���� ������ �̺�Ʈ�� ��������. (������ async �� ������� �ʾƵ��ȴ�)
// 2. Ű �Է� �̺�Ʈ�� ó���Ѵ�. (ex: tap, hold, away ...)


enum class KEY_STATE
{
	NONE,	// �������� ���ݵ� �Է����� ����
	TAP,	// �������� �Է����� �ʴٰ� ���� ó������ �Է���
	HOLD,	// �������� ���ݵ� ��� �Է� ��
	AWAY,	// ���������� �ԷµǾ����� ����� �Է����� ����
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	CLICK_LEFT,
	CLICK_RIGHT,

	LAST
};

struct tKeyInfo
{
	KEY_STATE	eState;		// Ű �Է� ����
	bool		bPrevPush;	// ���� �����ӿ��� ���ȴ��� ����
};

class CKeyManager
{
	SINGLETON(CKeyManager);

private:
	vector<tKeyInfo>	m_vKey;
	Vector2				m_vCurrMousePosition;

public:
	void initialize();
	void update();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vKey[(int)_eKey].eState; }
	Vector2 GetMousePosition() { return m_vCurrMousePosition; }
};

