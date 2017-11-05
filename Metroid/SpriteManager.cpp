﻿#include "SpriteManager.h"
#include "pugixml/pugixml.hpp"

SpriteManager* SpriteManager::_instance = nullptr;

SpriteManager::SpriteManager(void)
{
	// do nothing.
}

SpriteManager::~SpriteManager(void)
{
	for (auto spr = _listSprite.begin(); spr != _listSprite.end(); ++spr)
	{
		spr->second->release(); // release image
		delete spr->second; // delete sprite
	}
	if (_listSprite.empty() == false)
		_listSprite.clear(); // remove all from MAP
}

void SpriteManager::loadResource(LPD3DXSPRITE spriteHandle)
{
	/* if you have any image, load them with this format */
	// [psedue code]
	// sp = new SPRITE(...)
	// this->_listSprite.insert(pair<eID, Sprite*>(eID::ENUMOBJECT, sp));

	Sprite* pSprite = NULL;

	pSprite = new Sprite(spriteHandle, L"Resources//Images//mainmenu.png");
	this->_listSprite[eID::MAIN_MENU] = pSprite;

	pSprite = new Sprite(spriteHandle, L"Resources//Fonts//fontEx.png", 30, 10);
	this->_listSprite[eID::FONTEX] = pSprite;

	pSprite = new Sprite(spriteHandle, L"Resources//Fonts//fontFull.png", 54, 6);
	this->_listSprite[eID::FONTFULL] = pSprite;

	pSprite = new Sprite(spriteHandle, L"Resources//Images//life.png");
	this->_listSprite[eID::LIFE_ICON] = pSprite;

	pSprite = new Sprite(spriteHandle, L"Resources//Images//player.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::PLAYER, pSprite));
	this->loadSpriteInfo(eID::PLAYER, "Resources//Images//player.txt");

	// Đọc file xml để tạo đối tượng sprite
	pSprite = loadXMLDoc(spriteHandle, L"Resources//Maps//test.tmx");
	pSprite->setOrigin(VECTOR2ZERO);
	pSprite->setScale(2.0f);
	this->_listSprite[eID::MAP_METROID] = pSprite;
}

Sprite* SpriteManager::loadXMLDoc(LPD3DXSPRITE spritehandle, LPWSTR path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path, pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		OutputDebugString(L"Khong tim thay file");
		return nullptr;
	}

	auto tileset_node = doc.child("map").child("tileset");
	int tilecount = tileset_node.attribute("tilecount").as_int();
	int columns = tileset_node.attribute("columns").as_int();
	auto image = tileset_node.child("image");

	// Tìm tên file.
	// Cắt từ chuỗi path ra để tìm thư mục.
	// Sau đó ghép với tên file ảnh được lấy từ file xml để load ảnh.
	string filename = image.attribute("source").as_string();		// get filename from xml node
	wstring L_filename = wstring(filename.begin(), filename.end());	// convert to wstring.
	wstring strpath = wstring(path);								// convert to wstring.
	int index = strpath.find_last_of(L'//');						// cut to find path
	strpath = strpath.substr(0, index);
	strpath += L"/" + L_filename;									// concat string.  Final string is strpath.

	return new Sprite(spritehandle, (LPWSTR)strpath.c_str(), tilecount, columns);
}

Sprite* SpriteManager::getSprite(eID id)
{
	Sprite* it = this->_listSprite.find(id)->second;
	return new Sprite(*it); // get the copy version of Sprite
}

RECT SpriteManager::getSourceRect(eID id, string name)
{
	//return _sourceRectList[id].at(name);
	return _sourceRectList[id][name];
}

void SpriteManager::loadSpriteInfo(eID id, const char* fileInfoPath)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");

	if (file)
	{
		while (!feof(file))
		{
			RECT rect;
			char name[100];
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d", &name, &rect.left, &rect.top, &rect.right, &rect.bottom);

			_sourceRectList[id][string(name)] = rect;
		}
	}

	fclose(file);
}

void SpriteManager::releaseSprite(eID id)
{
	Sprite* it = this->_listSprite.find(id)->second;
	delete it; // delete the sprite only, dont release image
	this->_listSprite.erase(id); // erase function only remove the pointer from MAP, dont delete it.
}

void SpriteManager::releaseTexture(eID id)
{
	Sprite* spr = this->_listSprite.find(id)->second;
	spr->release(); // release image
	delete spr;
	this->_listSprite.erase(id); // erase function only remove the pointer from MAP, dont delete it.
}

SpriteManager* SpriteManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new SpriteManager();
	return _instance;
}

void SpriteManager::release()
{
	delete _instance; // _instance is static attribute, only static function can delete it.
	_instance = nullptr;
}

/*
	HOW TO USE:
	spritemanager is basic class to manage load and release sprite and texture.
	It load all image you have, if you want to use sprite any where, just getSprite from SpriteManager.
	if you dont want you use this image any more, you can call release.

	everytime use this, you have to call SpriteManager::getInstance() first.
	this is Singleton format.
	Seemore about Singleton: http://www.stdio.vn/articles/read/224/singleton-pattern

	you can call directly: SpriteManager::getInstance()->DOSOMETHING()
	or reference it:
		 	SpriteManager* _spritemanager;
			...
			_spritemanager = SpriteManager::getInstance();
			...
			_spritemanager->DOSOMETHING()

	Next, you should call loadresource(LPD3DXSPRITE spriteHandle) in Game::loadresource().
	It will load all your image from file to memory.
	Remember: Insert your code to loadresource to load image from file.

	If you want object reference to Sprite. call:
		SpriteManager::getInstance()->getSprite(eID::OBJECT_ID)
		what is eID::OBJECT_ID ?  
		in define.h you can insert element to eID.

	OK. Now you have sprite to draw.

	If you dont want to use this sprite any more, call releaseSprite or releaseTexture
	they often are called in object::release
		
	Call SpriteManager::release() in game:release() to clean all memory.
*/
