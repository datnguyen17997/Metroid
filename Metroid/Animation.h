﻿#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include <stdarg.h>
#include "define.h"
#include "Sprite.h"
#include "IComponent.h"
#include "SpriteManager.h"

class Animation : public IComponent
{
public:
	/*
	Tạo animation từ spritesheet
		@spriteSheet: spritesheet animation
		@timeAnimate: thời gian chuyển từng frame
	Sau khi tạo bằng contructor này phải addFrameRect để có frame để chuyển động.
	*/
	Animation(Sprite* spriteSheet, float timeAnimate = 0.0f, bool loop = true);

	/*
	Tạo animation từ spritesheet có frame đều nhau
		@spriteSheet: spritesheet của animation
		@totalFrames: tổng số frame
		@cols: số cột
		@timeAnimate: thời gian chuyển từng frame
	Sau khi tạo bằng contructor này, danh sách frameRect sẽ tự động được add
	*/
	Animation(Sprite* spriteSheet, int totalFrames, int cols, float timeAnimate = 0.0f);

	~Animation();

	// Chuyển qua frame kế tiếp
	void nextFrame();

	// Set frame index cụ thể
	void setIndex(int index);

	// Lấy giá trị index hiện tại
	int getIndex();

	// Update animation
	void update(float dt);

	// Vẽ animation
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);

	// Set thời gian chuyển frame (lưu ý: thời gian tính theo giây)
	void setTimeAnimate(float time);
	
	// Lấy giá trị thời gian chuyển frame
	float getTimeAnimate();

	// Bắt đầu chuyển frame
	void start();

	// Dừng chuyển frame
	void stop();

	void canAnimate(bool can);
	bool isAnimate();
	void addFrameRect(RECT rect);
	void addFrameRect(float left, float top, int width, int height);
	void addFrameRect(float left, float top, float right, float bottom);

	/*
	Tạo nhiều frameRect liên tiếp nhau, không giới hạn tham số, kết thúc bằng tham số NULL
		@id: ID của sprite
		@firstRectName,...: các tên của frameRect
	*/
	void addFrameRect(eID id, char* firstRectName, ...);

	void setLoop(bool isLoop);
	bool isLoop();

	// Chạy lại animation (@from: index frame muốn bắt đầu)
	void restart(int from = 0);

	/*
	Animate từ frame này đến frame khác cụ thể
		@from: frame bắt đầu
		@to: frame kết thúc
		@loop: có lặp hay ko
		Giá trị from/to là thứ tự của frame add vô, tính từ 0.
	*/
	void animateFromTo(int from, int to, bool loop = true);

	// Tạo hiệu ứng chớp cho sprite, sau khi set enable = true, phải đặt giá trị thay đổi ở hàm setValueFlashes
	void enableFlashes(bool enable);

	// Gán giá trị chớp tắt (@value: giá trị thay đổi để chớp, từ 0 đến < 1 (giá trị bình thường là 1))
	void setValueFlashes(float value);

	void setColorFlash(D3DXCOLOR color);
	D3DXCOLOR getColorFlash();

private:
	int _startFrame;
	int _endFrame;
	int _totalFrames;
	int _index;				// số thứ tự frame

	float _timeAnimate;		// thời gian chuyển giữa các frame
	float _timer;

	bool _canAnimate;
	bool _isLoop;

	Sprite* _spriteSheet;	// reference đến sprite
	vector<RECT> _frameRectList;	// danh sách các frameRect
	RECT _currentRect;		// frameRect hiện tại

	bool _canFlashes;
	float _valueFlashes;
	D3DXCOLOR _flashColor;
};

#endif // !__ANIMATION_H__
