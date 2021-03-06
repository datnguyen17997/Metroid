﻿#include "Player.h"
#include "SceneManager.h"

Player::Player() : BaseObject(eID::PLAYER)
{
}

Player::~Player()
{
}

void Player::init()
{
	__hook(&InputController::__eventkeyPressed, _input, &Player::onKeyPressed);
	__hook(&InputController::__eventkeyReleased, _input, &Player::onKeyReleased);

	_sprite = SpriteManager::getInstance()->getSprite(eID::PLAYER);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::PLAYER, "normal"));
	_sprite->setZIndex(1.0f);

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;
	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);

	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.07f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::PLAYER, "normal", NULL);

	_animations[eStatus::NORMAL | eStatus::ATTACKING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL | eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "normal", NULL);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::PLAYER, "walk_right_01", "walk_right_02", "walk_right_03", NULL);

	_animations[eStatus::JUMPING | eStatus::NORMAL] = new Animation(_sprite, 0.07f);
	_animations[eStatus::JUMPING | eStatus::NORMAL]->addFrameRect(eID::PLAYER, "jump", NULL);

	_animations[eStatus::FALLING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::FALLING]->addFrameRect(eID::PLAYER, "normal", NULL);

	_animations[eStatus::LOOKING_UP] = new Animation(_sprite, 0.07f);
	_animations[eStatus::LOOKING_UP]->addFrameRect(eID::PLAYER, "look_up", NULL);

	_animations[eStatus::ROLLING_DOWN] = new Animation(_sprite, 0.07f);
	_animations[eStatus::ROLLING_DOWN]->addFrameRect(eID::PLAYER, "roll_down_01", "roll_down_02", "roll_down_03", "roll_down_04", NULL);

	_animations[eStatus::DIE] = new Animation(_sprite, 0.5f, false);
	_animations[eStatus::DIE]->addFrameRect(eID::PLAYER, "roll_down_01", "roll_down_02", "roll_down_03", "roll_down_04", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::ATTACKING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::LOOKING_UP | eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "look_up", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::RUNNING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::LOOKING_UP | eStatus::RUNNING]->addFrameRect(eID::PLAYER, "run_look_up_01", "run_look_up_02", "run_look_up_03", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::RUNNING | eStatus::ATTACKING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::LOOKING_UP | eStatus::RUNNING | eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "run_look_up_01", "run_look_up_02", "run_look_up_03", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::RUNNING | eStatus::JUMPING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::LOOKING_UP | eStatus::RUNNING | eStatus::JUMPING]->addFrameRect(eID::PLAYER, "jump", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::JUMPING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::LOOKING_UP | eStatus::JUMPING]->addFrameRect(eID::PLAYER, "jump_look_up", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::JUMPING | eStatus::ATTACKING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::LOOKING_UP | eStatus::JUMPING | eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "jump_look_up", NULL);

	_animations[eStatus::RUNNING | eStatus::LOOKING_UP | eStatus::JUMPING | eStatus::ATTACKING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::RUNNING | eStatus::LOOKING_UP | eStatus::JUMPING | eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "jump_look_up", NULL);

	_animations[eStatus::ROLLING_DOWN | eStatus::RUNNING] = new Animation(_sprite, 0.01f);
	_animations[eStatus::ROLLING_DOWN | eStatus::RUNNING]->addFrameRect(eID::PLAYER, "roll_down_01", "roll_down_02", "roll_down_03", "roll_down_04", NULL);

	_animations[eStatus::RUNNING | eStatus::JUMPING] = new Animation(_sprite, 0.04f);
	_animations[eStatus::RUNNING | eStatus::JUMPING]->addFrameRect(eID::PLAYER, "run_jump_01", "run_jump_02", "run_jump_03", "run_jump_04", NULL);

	_animations[eStatus::RUNNING | eStatus::JUMPING | eStatus::ATTACKING] = new Animation(_sprite, 0.05f);
	_animations[eStatus::RUNNING | eStatus::JUMPING | eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "jump_attack", NULL);

	_animations[eStatus::ATTACKING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "normal", NULL);

	_animations[eStatus::ATTACKING | eStatus::RUNNING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::ATTACKING | eStatus::RUNNING]->addFrameRect(eID::PLAYER, "run_attack_01", "run_attack_02", "run_attack_03", NULL);

	_animations[eStatus::JUMPING | eStatus::ATTACKING] = new Animation(_sprite, 0.07f);
	_animations[eStatus::JUMPING | eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "jump_attack", NULL);

	this->setOrigin(GVector2(0.5f, 0.0f));
	this->setStatus(eStatus::NORMAL);

	this->_currentAnimateIndex = NORMAL;
	this->_isRevive = false;

	// Info có tọa độ top-left
	_info = new Info();
	_info->init();
	_info->setLife(2);
	_info->setEnergy(10);

	this->resetValues();
}

void Player::update(float deltatime)
{
	if (_info->getEnergy() <= 0)
	{
		this->die();
		_protectTime = 0;
	}

	if (_protectTime > 0)
	{
		_protectTime -= deltatime;
	}

	this->_info->update(deltatime);

	this->checkPosition();
	this->updateStatus(deltatime);
	this->updateCurrentAnimateIndex();

	_animations[_currentAnimateIndex]->update(deltatime);

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Player::updateInput(float dt)
{
	// Dùng event
}

void Player::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_protectTime > 0)
		_animations[_currentAnimateIndex]->enableFlashes(true);
	else
		_animations[_currentAnimateIndex]->enableFlashes(false);

	_animations[_currentAnimateIndex]->draw(spriteHandle, viewport);
	_info->draw(spriteHandle, viewport);
}

void Player::release()
{
	for (auto it = _animations.begin(); it != _animations.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_animations.clear();

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();

	SAFE_DELETE(_sprite);
	SAFE_DELETE(_info);

	if (_input != nullptr)
		__unhook(_input);
}

void Player::updateStatus(float dt)
{
	if (this->isInStatus(eStatus::DIE))
	{
		// Lưu lại vị trí chết để set vị trí hồi sinh
		this->_revivePosition = GVector2(this->getPosition());

		if (_info->getLife() < 0)
			return;

		if (!_animations[eStatus::DIE]->isAnimate())
			this->revive();
	}

	if ((this->getStatus() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT)
	{
		this->moveLeft();
	}
	else if ((this->getStatus() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)
	{
		this->moveRight();
	}
	else if ((this->getStatus() & eStatus::FALLING) == eStatus::FALLING)
	{
		this->falling();
	}
	else if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING)
	{
		this->standing();
	}
	
	if ((this->getStatus() & eStatus::ATTACKING) == eStatus::ATTACKING)
	{
		if ((this->getStatus() & eStatus::ROLLING_DOWN) == eStatus::ROLLING_DOWN)
			this->bomb();
		else
			this->shoot();
	}
}

void Player::updateCurrentAnimateIndex()
{
	_currentAnimateIndex = this->getStatus();

	if (!((_currentAnimateIndex & eStatus::ROLLING_DOWN) == eStatus::ROLLING_DOWN) && (_currentAnimateIndex & eStatus::FALLING) == eStatus::FALLING)
	{
		_currentAnimateIndex = eStatus::FALLING;
	}

	if ((_currentAnimateIndex & eStatus::ROLLING_DOWN) == eStatus::ROLLING_DOWN)
	{
		_currentAnimateIndex = eStatus::ROLLING_DOWN;
	}

	if ((_currentAnimateIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT || ((_currentAnimateIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT))
	{
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex | eStatus::RUNNING);
	}

	if (this->isInStatus(eStatus::DIE))
	{
		_currentAnimateIndex = eStatus::DIE;
	}
}

void Player::onKeyPressed(KeyEventArg* keyEvent)
{
	if (this->isInStatus(eStatus::DIE))
		return;

	switch (keyEvent->keyCode)
	{
	case DIK_LEFT:
	{
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->addStatus(eStatus::MOVING_LEFT);
		break;
	}
	case DIK_RIGHT:
	{
		this->removeStatus(eStatus::MOVING_LEFT);
		this->addStatus(eStatus::MOVING_RIGHT);
		break;
	}
	case DIK_UP:
	{	if (!this->isInStatus(eStatus::ROLLING_DOWN) && !this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING))
		{
			this->addStatus(eStatus::LOOKING_UP);
		}
		else
		{
			this->removeStatus(eStatus::ROLLING_DOWN);
		}
		break;
	}
	case DIK_DOWN:
	{
		if (!this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING)) 
			this->addStatus(eStatus::ROLLING_DOWN);
		break;
	}
	case DIK_X:
	{
		if (!this->isInStatus(eStatus::ROLLING_DOWN) && ((this->isInStatus(eStatus::NORMAL) || this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT))))
			this->jump();

		if (this->isInStatus(eStatus::ROLLING_DOWN))
			this->removeStatus(eStatus::ROLLING_DOWN);

		break;
	}
	case DIK_Z:
	{
		this->addStatus(eStatus::ATTACKING);
		break;
	}
	default:
		break;
	}
}

void Player::onKeyReleased(KeyEventArg* keyEvent)
{
	if (this->isInStatus(eStatus::DIE))
		return;

	switch (keyEvent->keyCode)
	{
	case DIK_LEFT:
	{
		this->removeStatus(eStatus::MOVING_LEFT);
		this->removeStatus(eStatus::RUNNING);
		break;
	}
	case DIK_RIGHT:
	{
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->removeStatus(eStatus::RUNNING);
		break;
	}
	case DIK_UP:
	{
		this->removeStatus(eStatus::LOOKING_UP);
		break;
	}
	case DIK_DOWN:
	{
		break;
	}
	case DIK_X:
	{
		break;
	}
	case DIK_Z:
	{
		this->removeStatus(eStatus::ATTACKING);
		_info->setDebugAttack("");
		break;
	}
	default:
		break;
	}
}

void Player::resetValues()
{
	preWall = nullptr;

	this->setScale(SCALE_FACTOR);
	_movingSpeed = MOVE_SPEED;
	_protectTime = PROTECT_TIME;

	if (_isRevive)
	{
		this->setStatus(eStatus::NORMAL);
		_info->setLife(_info->getLife() - 1);
		_info->setEnergy(30);

		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::FALLING_DOWN);

		this->setPosition(_revivePosition);
		_isRevive = false;
	}

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));

	for (auto animate : _animations)
	{
		animate.second->setColorFlash(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void Player::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));

	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Player::moveLeft()
{
	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-_movingSpeed, move->getVelocity().y));
}

void Player::moveRight()
{
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}

void Player::jump()
{
	if (this->isInStatus(eStatus::JUMPING) || this->isInStatus(eStatus::FALLING))
		return;

	this->addStatus(eStatus::JUMPING);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x, JUMP_VELOCITY));

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
}

void Player::falling()
{
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
}

void Player::shoot()
{
	_info->setDebugAttack("SHOOT");
}

void Player::bomb()
{
	_info->setDebugAttack("BOMB");
}

void Player::beHit(eDirection direction)
{
	_protectTime = PROTECT_TIME;

	if (direction == NONE)
	{
		_info->setEnergy(_info->getEnergy() - 2);
		return;
	}

	//if (this->isInStatus(eStatus::BEING_HIT))
	//	return;

	//this->setStatus(eStatus::BEING_HIT);

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);

	auto move = (Movement*)this->_componentList["Movement"];

	switch (direction)
	{
	case RIGHT:
	{
		// Nếu va chạm RIGHT thì bị văng sang bên phải (player hướng sang trái)
		if (this->getScale().x > 0)
			this->setScaleX(this->getScale().x * (-1));
		move->setVelocity(GVector2(MOVE_SPEED / 2, JUMP_VELOCITY / 2));
		break;
	}
	case LEFT:
	{
		// Nếu va chạm LEFT thì bị văng sang bên trái (player hướng sang phải)
		if (this->getScale().x < 0)
			this->setScaleX(this->getScale().x * (-1));
		move->setVelocity(GVector2(-MOVE_SPEED / 2, JUMP_VELOCITY / 2));
		break;
	}
	default:
	{
		if (this->getScale().x > 0)
			move->setVelocity(GVector2(-MOVE_SPEED / 2, JUMP_VELOCITY / 2));
		else
			move->setVelocity(GVector2(MOVE_SPEED / 2, JUMP_VELOCITY / 2));
		break;
	}
	}
}

void Player::die()
{
	if (!this->isInStatus(eStatus::DIE))
		this->setStatus(eStatus::DIE);
}

void Player::revive()
{
	this->_isRevive = true;
	this->resetValues();
	_animations[eStatus::DIE]->restart();

	this->jump();
}

void Player::checkPosition()
{
	if (this->isInStatus(eStatus::DIE))
		return;

	auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	GVector2 viewportPosition = viewport->getPositionWorld();

	// Rớt xuống dưới viewport thì chết
	if (this->getPositionY() < viewportPosition.y - WINDOW_HEIGHT)
	{
		if (_status != eStatus::DIE)
			_status = eStatus::DIE;

		this->die();
	}
}

float Player::checkCollision(BaseObject* object, float dt)
{
	if (object->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::DIE))
		return 0.0f;
	if (this == object)
		return 0.0f;

	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	if (objectId == eID::WALL)
	{
		// Kiểm tra va chạm
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			// Dời ra xa
			float moveX, moveY;
			if (collisionBody->isColliding(object, moveX, moveY, dt))
			{
				collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
			}

			// Nếu va chạm TOP mà trừ trường hợp nhảy lên vận tốc lớn hơn 0
			// Trường hợp vận tốc y lớn hơn 0 là trường hợp nhảy từ dưới lên
			if (direction == eDirection::TOP && !(this->getVelocity().y > 0 && this->isInStatus(eStatus::JUMPING)))
			{
				auto gravity = (Gravity*)this->_componentList["Gravity"];
				gravity->setStatus(eGravityStatus::SHALLOWED);

				this->standing();
				preWall = object;
			}
		}
		else if (preWall == object) // Xét sau va chạm
		{	
			// Nếu đã đi ra khỏi hết Wall đụng trước đó thì cho rớt xuống
			auto gravity = (Gravity*)this->_componentList["Gravity"];
			gravity->setStatus(eGravityStatus::FALLING_DOWN);
			
			if (!this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING))
				this->addStatus(eStatus::FALLING);
		}
	}
	else if (objectId == RIPPER)
	{
		if (!((Ripper*)object)->isDead() && _protectTime <= 0)
		{
			if (collisionBody->checkCollision(object, direction, dt, false))
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
				beHit(direction);

				_info->setEnergy(_info->getEnergy() - 8);
			}

			//if (this->weaponCheckCollision(object, direction, dt, false))
			//{
			//	((Ripper*)object)->wasHit(1);
			//}
			//if (((Ripper*)object)->isDead())
			//	_info->AddScore(300);
		}
	}
	else if (objectId == WAVER)
	{
		if (!((Waver*)object)->isActive())
		{
			auto objPosition = object->getPosition();
			auto position = this->getPosition();
			if (getDistance(objPosition, position) < WINDOW_WIDTH / 2 && abs(position.x - objPosition.x) < WINDOW_WIDTH / 2)
			{
				((Waver*)object)->active(position.x > objPosition.x);
			}
		}

		if (((Waver*)object)->isActive())
		{
			auto objPosition = object->getPosition();
			auto position = this->getPosition();
			if (getDistance(objPosition, position) > 150 && abs(position.x - objPosition.x) > WINDOW_WIDTH / 2)
			{
				((Waver*)object)->deactive();
			}
		}

		if (!((Waver*)object)->isDead() && _protectTime <= 0)
		{
			if (collisionBody->checkCollision(object, direction, dt, false))
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
				beHit(direction);

				_info->setEnergy(_info->getEnergy() - 8);
			}

			//if (this->weaponCheckCollision(object, direction, dt, false))
			//{
			//	((Waver*)object)->wasHit(1);
			//}
			//if (((Waver*)object)->isDead())
			//	_info->AddScore(300);
		}
	}
}

void Player::setStatus(eStatus status)
{
	_status = status;
}

GVector2 Player::getPosition()
{
	return _sprite->getPosition();
}

int Player::getLifeNumber()
{
	return _info->getLife();
}

RECT Player::getBounding()
{
	int offset = 10;

	RECT bound = _sprite->getBounding();
	
	if ((_currentAnimateIndex & LOOKING_UP) == LOOKING_UP)
	{
		bound.top -= offset * 0.6f;
	}

	if ((_currentAnimateIndex & NORMAL) == NORMAL)
	{
		if (this->getScale().x > 0)
		{
			bound.right -= offset * 0.3f;
			bound.left += offset * 0.3f;
		}
		else if (this->getScale().x < 0)
		{
			bound.left += offset * 0.3f;
			bound.right -= offset * 0.3f;
		}
	}

	return bound;
}

GVector2 Player::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}
