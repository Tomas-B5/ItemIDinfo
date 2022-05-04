#pragma once

#include <windows.h>
#include <iostream>
#include "PoeMod.h"
#include <sstream>

using namespace std;

//extern enum class ItemType;// { Undefined, Map, Flask, Weapon, Equipable, Jewel, Amulet, Ring, Belt};
 enum class ItemRarity  {Undefined, Normal, Magic, Rare, Unique, Other};

//ItemType ProcessItemType(string base, string cbData);


class PoeItem
{
private:
	int ScreenX, ScreenY;

	string _name;
	string _itemBase;
	ItemType _itemType = ItemType::Undefined;
	ItemRarity _itemRarity = ItemRarity::Undefined;

	double _totalModWeight = 0;

	PoeMod _mods[18];
	int _modCount = 0;

	int _packSize = 0; //For maps only

	bool _important = false; // true = needs to be displayed, false = do no not render

	int _displayTime = 0;
	
	ItemType static ProcessItemType(string base, string cbData);
	void ProcessMods(string cbData, ItemType type);
	double CountTotalModWeight();

public:
	std::string GetName() const { return _name; }
	ItemType GetItemType() const { return _itemType; }
	ItemRarity GetItemRarity() const { return _itemRarity; }
	bool static IsItemMapType(string cbData);
	ItemType static GetItemType(string base, string cbdata);
	PoeItem();
	PoeItem(string cbData, POINT pt);

};
//std::istringstream stream(text);
//std::string line;
//while (std::getline(stream, line)) {
	// whatever
