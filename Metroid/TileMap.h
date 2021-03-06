﻿#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "TileSet.h"
#include "define.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "pugixml\pugixml.hpp"
using namespace std;
using namespace pugi;

class TileMap
{
public:
	// Tạo TileMap từ file XML (@spriteId: ID của TileMap)
	static TileMap* LoadFromFile(const string path, eID spriteId);

	// Lấy kich thước Map.
	GVector2 getWorldSize();

	TileMap();
	~TileMap();

	void draw(LPD3DXSPRITE, Viewport*);

	int worldWidth();
	int worldHeight();

private:
	int _frameWidth;	// độ rộng một frame (độ rộng của tile)
	int _frameHeight;	// độ cao một frame (độ cao của tile)

	// Kích thước Map tính theo số ô Tile (cột, dòng)
	GVector2 _mapSize;

	// Ma trận Tile ID.
	// ID trong ma trận này được so sánh với ID của Tile trong TileSet.listTiles
	int** _mapIndex;

	// TileSet chứa listTiles.
	TileSet* _tileSet;

	// Đọc dữ liệu danh sách các Tile ID từ file XML
	void getElementMatrixIndex(xml_node& node);
};

#endif // !__TILEMAP_H__
