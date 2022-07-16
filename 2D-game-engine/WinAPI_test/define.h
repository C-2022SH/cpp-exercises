#pragma once

#include <wingdi.h>


#define SINGLETON(type) public: static type* GetInstance()\
						{\
							static type mgr;\
							return &mgr;\
						}\
						private: type(); ~type();

#define fDeltaTime CTimeManager::GetInstance()->GetfDeltaTime()
#define DeltaTime CTimeManager::GetInstance()->GetDeltaTime()

#define CLONE(type) type* Clone() { return new type(*this); }

#define CHECK_KEY(key, state) CKeyManager::GetInstance()->GetKeyState(key) == state
#define KEY_HOLD(key) CHECK_KEY(key, KEY_STATE::HOLD)
#define KEY_TAP(key) CHECK_KEY(key, KEY_STATE::TAP)
#define KEY_AWAY(key) CHECK_KEY(key, KEY_STATE::AWAY)
#define KEY_NONE(key) CHECK_KEY(key, KEY_STATE::NONE)

#define MOUSE_POS CKeyManager::GetInstance()->GetMousePosition()

#define CHECK_ONCLICK(key, position) KEY_TAP(key) && MOUSE_POS.x <= position.right && MOUSE_POS.x >= position.left\
								&& MOUSE_POS.y <= position.down && MOUSE_POS.y >= position.up

#define X_AXIS Vector2(1.f, 0.f)
#define Y_AXIS Vector2(0.f, 1.f)


//#define DIR_UP Vector2(0.f, -1.f)
//#define DIR_DOWN Vector2(0.f, 1.f)
//#define DIR_LEFT Vector2(-1.f, 0.f)
//#define DIR_RIGHT Vector2(1.f, 0.f)

// =============
//  ����׿� �ڵ�
// =============
//
#define DRAW_LOG(dc, r, g, b, pos_x, pos_y, str, down) \
	SetBkMode(dc, TRANSPARENT);\
	SetTextColor(dc, RGB(r, g, b));\
	RECT rectArea = { int(pos_x - 30.f), int(pos_y + down - 20.f),\
						int(pos_x + 30.f), int(pos_y + down) };\
	DrawText(_dc, str.c_str(), -1, &rectArea, DT_CENTER);


// �� ����
enum class SCENE_TYPE
{
	TOOL,
	START,
	WHITE_01,
	VIOLET_01,

	END
};

// �� ���� ������Ʈ �з�
enum class OBJECT_TYPE
{
	// ������ ����
	// ���ڰ� Ŭ���� ���� �ö�´�.
	DEFAULT,
	BULLET_PLAYER,
	BULLET_MONSTER,
	GROUND,
	GENERATOR,
	PORTAL,
	PLAYER,
	MONSTER,

	END = 32
};

enum class UI_TYPE
{
	DEFAULT,
	BUTTON,
	TEXT,
	BAR,

	END = 32
};


// ����ó���� ����
enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	CHANGE_SCENE,

	END
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BACKGROUND,
	YELLOW,
	END
};


enum class PEN_TYPE
{
	HOLLOW,
	RED,
	GREEN,
	BLUE,
	PURPLE,
	END
};