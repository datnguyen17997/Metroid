﻿#ifndef __COLLISIONBODY__
#define __COLLISIONBODY__

#include "BaseObject.h"
#include "IComponent.h"
#include "Event.h"

// HỆ TỌA ĐỘ WOLRD (X, Y)
class CollisionBody : public IComponent
{
public:
	CollisionBody(BaseObject* target);
	~CollisionBody();

	/*
	Kiểm tra va chạm với object khác lấy được hướng va chạm, không gọi event Begin, End.
		@otherObject: object cần kiểm tra va chạm
		@direction: lấy hướng bị va chạm của otherObject
		@dt: deltatime của mỗi frame
		@isUpdatePosition:	TRUE khi muốn CollisionBody sẽ cập nhật vị trí object lại nếu object chồng lắp lên object khác
	*/
	bool checkCollision(BaseObject* otherObject, eDirection& direction, float dt, bool isUpdatePosition = true);

	void update(float dt);

	// Kiểm tra va chạm và trả về thời gian bắt đầu va chạm và hướng va chạm (AABB)
	float isCollide(BaseObject* otherObject, eDirection& direction, float dt);

	// Kiểm tra có đang va chạm rồi hay không, trả về khoảng chồng lắp nhỏ nhất
	bool isColliding(BaseObject* otherObject, float& moveX, float& moveY, float dt);

	// Kiểm tra 2 HCN có chồng lắp nhau hay không
	bool isColliding(RECT myRect, RECT otherRect);

	// Lấy Broadphase rect để kiểm tra trước vùng tiếp theo có va chạm không
	RECT getSweptBroadphaseRect(BaseObject* object, float dt);

	// Lấy hướng bị va chạm của otherObject
	eDirection getSide(BaseObject* otherObject);

	/*
	Cập nhật target position khi va chạm
		@otherObject: đối tượng va chạm
		@direction: hướng bị va chạm của otherObject
		@withVelocity:	TRUE khi kiểm va chạm với vận tốc, tham số move không cần. 
						FALSE khi va chạm bằng kiểm tra RECT, cần tham số move
		@move: khoảng chồng lấp nhỏ nhất của 2 object, dùng làm vector vận tốc để dịch chuyển ra xa
	*/
	void updateTargetPosition(BaseObject* otherObject, eDirection direction, bool withVelocity, GVector2 move = GVector2(0, 0));

private:
	BaseObject* _target;

	float _dxEntry, _dyEntry, _dxExit, _dyExit;
	float _txEntry, _tyEntry, _txExit, _tyExit;
};

#endif // !__COLLISIONBODY__
