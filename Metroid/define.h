﻿#ifndef __FRAMEWORK__
#define __FRAMEWORK__

#define _USE_MATH_DEFINES

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <dsound.h>

#include <windows.h>
#include <exception>
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include "utils.h"
using namespace std;

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 450
#define SCALE_FACTOR 2.0f

#define COLOR_WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// màu trắng
#define COLOR_KEY D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)		// màu khi mà load hình nó sẽ bỏ qua thành trong suốt

// ID của object
enum eID
{
	PLAYER = 0,
	INTRO = 1,
	FONT = 2,

	LIFE_ICON = 3,
	ENEGY_ICON = 4,

	MAP_METROID = 5,

	WALL = 20,

	ENEMY = 50,
	RIPPER = 51,
	WAVER = 52,
};

// Trạng thái của object
enum eStatus
{
	NORMAL = 0,
	MOVING_LEFT = (1 << 0),
	MOVING_RIGHT = (1 << 1),
	JUMPING = (1 << 2),
	FALLING = (1 << 3),
	RUNNING = (1 << 4),
	LOOKING_UP = (1 << 5),
	ROLLING_DOWN = (1 << 6),
	ATTACKING = (1 << 7),
	//BEING_HIT = (1 << 8),
	DIE = (1 << 9),
	DESTROY = (1 << 10),
};

// Hướng bị va chạm của object
enum eDirection
{
	NONE = 0,
	TOP = 1,
	LEFT = 2,
	RIGHT = 4,
	BOTTOM = 8,
	ALL = (TOP | BOTTOM | LEFT | RIGHT),
};

// ID của Sound
enum eSoundId
{
	INTRO_SCENE,
	PLAY_SCENE,
	OVER_SCENE,
};

typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;

#define VECTOR2ZERO GVector2(0.0f, 0.0f)
#define VECTOR2ONE  GVector2(1.0f, 1.0f)

#ifndef SAFE_DELETE

#define SAFE_DELETE(p) \
if(p) \
{\
	delete (p); \
	p = nullptr; \
} \

#endif // !SAFE_DELETE

#endif // !__FRAMEWORK__
