﻿#include "TextSprite.h"

TextSprite::TextSprite(eID type, string text, GVector2 position) : BaseObject(type)
{
	this->_type = type;
	this->_text = text;
	_position = position;
}

TextSprite::~TextSprite()
{
}

void TextSprite::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(_type);
	this->rows = _sprite->getTextureHeight() / _sprite->getFrameHeight();
	this->_columns = _sprite->getTextureWidth() / _sprite->getFrameWidth();
	this->_framewidth = _sprite->getFrameWidth();
}

void TextSprite::update(float deltatime)
{
}

void TextSprite::draw(LPD3DXSPRITE spriteHandle)
{
	this->draw(spriteHandle, nullptr);
}

void TextSprite::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	int i = 0;
	int index = 0;

	// Sprite đánh thứ tự index theo 0-based
	for each (char c in this->_text)
	{
		if (c >= 'A' && c <= 'Z')
		{
			// Thứ tự của char kể từ 'A'
			// VD: A => index = 0; B => index = 1
			index = ((int)c) - 'A';
		}
		else if (c >= 'a' && c <= 'z')
		{
			index = ((int)c) - 'a';
		}
		else if (c >= '0' && c <= '9')
		{
			index = (((int)c) - '0') + 26;
		}
		else if (c == '.')
		{
			index = 36;
		}
		else if (c == '-')
		{
			index = 44;
		}
		else if (c == ':')
		{
			index = 45;
		}
		else if (c == ' ')
		{
			index = 53;
		}

		_sprite->setIndex(index);
		_sprite->setPosition(GVector2(_position.x + _sprite->getFrameWidth() * i, _position.y));

		if (viewport == nullptr)
		{
			_sprite->render(spriteHandle);
		}
		else
		{
			_sprite->render(spriteHandle, viewport);
		}

		i++;
	}
}

void TextSprite::release()
{
}

void TextSprite::setString(string text)
{
	this->_text = text;
}

GVector2 TextSprite::getPosition()
{
	return _position;
}

void TextSprite::setPosition(GVector2 position)
{
	_position = position;
}
