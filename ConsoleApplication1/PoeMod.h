#pragma once
#include <windows.H>
#include <iostream>
#include "ModFilter.h"
#include "Colors.h"
//#include "PoeItem.h"

using namespace std;

enum class ItemType { Undefined, Map, Flask, Weapon, Equipable, Jewel, Amulet, Ring, Belt };

TypeModFilterArray* GetFilterArray(ItemType type);

double ReadValuePlus(string text);
double ReadValuePlusPercent(string text);
double ReadValuePercent(string text);
double ReadValueTo(string text);

class PoeMod
{
private:
	string _modText;
	//double _modValue;
	DWORD _color = Gray;
	double _modweight = 0;
public:

	PoeMod(string modLine);
	PoeMod(string modLine, ItemType type);
	PoeMod() {};
	void SetWeightAndColor(string modLine, TypeModFilterArray modarray);
	~PoeMod() {};
	double GetModweight() const { return _modweight; }
	void SetModweight(double val) 
	{
		cout << val << "\n";
		_modweight = val; 
	}
	std::string GetModText() const { return _modText; }
	void SetModText(std::string val) { _modText = val; }
};

