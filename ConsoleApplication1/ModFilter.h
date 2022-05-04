#ifndef ModFilter_h__
#define ModFilter_h__
#pragma once

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
struct ModFilterInfo;

class TypeModFilterArray
{
public:
	double _minWeight = 0;
	vector<ModFilterInfo> _filters;
	
};

bool ReadFilters();
bool ReadSingleFilter(TypeModFilterArray &filterarray, const char* file);
bool ReadSingleFilterMap(TypeModFilterArray &filterarray, const char* file);

const char MapFilterCFG[] = "Cfg\\Maps.cfg";
const char RingFilterCFG[] = "Cfg\\Rings.cfg";
const char AmuletFilterCFG[] = "Cfg\\Amulets.cfg";
const char WeaponFilterCFG[] = "Cfg\\Weapons.cfg";
const char JewelFilterCFG[] = "Cfg\\Jewels.cfg";
const char BeltFilterCFG[] = "Cfg\\Belts.cfg";
const char FlaskFilterCFG[] = "Cfg\\Flasks.cfg";
const char UniquesFilterCFG[] = "Cfg\\Uniques.cfg";



extern TypeModFilterArray MapFilter;
extern TypeModFilterArray RingFilter;
extern TypeModFilterArray AmuletFilter;
extern TypeModFilterArray BeltFilter;
extern TypeModFilterArray JewelFilter;
extern TypeModFilterArray FlaskFilter;
extern TypeModFilterArray WeaponFilter;
extern TypeModFilterArray EquipableFilter;

enum  DelimType {none, plus, percent, pluspercent, to};

struct ModFilterInfo 
{
	string _text;
	DWORD _color;
	DelimType _delimType = 	none;
	double _modWeight;
	//int min value to display???
	// maybe enum with how to find value inside a string.

	ModFilterInfo(string text, DWORD color, double modweight, DelimType delimtype)
	{
		_text = text;
		_color = color;
		_modWeight = modweight;
		_delimType = delimtype;
	}
	ModFilterInfo(string text, DWORD color)
	{
		_text = text;
		_color = color;
		_modWeight = 0;
	}
};




#endif // ModFilter_h__