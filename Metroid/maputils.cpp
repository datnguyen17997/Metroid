﻿#include "maputils.h"

list<BaseObject*>* GetListObjectFromFile(const string path)
{
	xml_document doc;
	list<BaseObject*>* listObj = new list<BaseObject*>();

	auto result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
		return listObj;

	auto map = doc.child("map");
	if (map == NULL)
		return listObj;

	auto mapHeight = 2 * map.attribute("tileheight").as_int() * map.attribute("height").as_int();

	xml_node objectGroup = map.child("objectgroup");
	if (objectGroup == NULL)
		return listObj;

	auto objects = objectGroup.children();
	for (auto obj : objects)
	{
		int id = obj.attribute("id").as_int();
		auto type = obj.attribute("type").as_int();

		eID enumID;
		try
		{
			enumID = (eID)type;
		}
		catch (exception e)
		{
			continue;
		}

		auto baseObj = GetObjectByType(obj, enumID, mapHeight);
		if (baseObj != nullptr)
		{
			listObj->push_back(baseObj);
		}
	}

	return listObj;
}

map<string, string> GetObjectProperties(xml_node node)
{
	map<string, string> properties;

	// Lấy kích thước của object
	properties["x"] = node.attribute("x").as_string();
	properties["y"] = node.attribute("y").as_string();
	properties["width"] = node.attribute("width").as_string();
	properties["height"] = node.attribute("height").as_string();

	// Lấy các property của object
	auto params = node.child("properties");
	if (params)
	{
		for (auto item : params)
		{
			auto key = item.attribute("name").as_string();
			auto value = item.attribute("value").as_string();
			properties[key] = value;
		}
	}

	return properties;
}

BaseObject* GetObjectByType(xml_node item, eID type, int mapHeight)
{
	switch (type)
	{
	case WALL:
		return GetWall(item, mapHeight);
		break;
	case RIPPER:
		return GetRipper(item, mapHeight);
		break;
	case WAVER:
		return GetWaver(item, mapHeight);
		break;
	default:
		return nullptr;
	}
}

BaseObject* GetWall(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	// Do tọa độ được tạo từ Tiled Map Editor là top-left, nên ta chuyển về tọa độ world (x, y)
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);
	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	auto wall = new Wall(x, y, width, height);
	wall->init();
	return wall;
}

BaseObject* GetRipper(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);
	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	auto ripper = new Ripper(x, y, false);
	ripper->init();
	return ripper;
}

BaseObject* GetWaver(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);
	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	auto waver = new Waver(x, y, false);
	waver->init();
	return waver;
}
