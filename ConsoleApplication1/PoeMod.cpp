#include "pch.h"
#include "PoeMod.h"




PoeMod::PoeMod(string modLine)
{
	_modText = modLine;

	//Maybe don't Ignore ------ so you can see implicits.
	//match with lines from mods.file
	//figure out the format.

}

PoeMod::PoeMod(string modLine, ItemType type)
{
	//Undefined, Map, Flask, Weapon, Equipable, Jewel, Amulet, Ring, Belt

	SetWeightAndColor(modLine, *GetFilterArray(type));

	switch (type)
	{
	case (ItemType::Undefined):
	{
		break;
	}
	case (ItemType::Map):
	{
		SetWeightAndColor(modLine, MapFilter);
		break;
	}
	case (ItemType::Ring):
	{
		SetWeightAndColor(modLine, RingFilter);
		break;
	}
	}
	_modText = modLine;

}


//************************************
// Method:    Search
// FullName:  PoeMod::Search
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: string modLine
// Parameter: TypeModFilterArray modarray
// ###Also sets color value and weight###
// From mod array
//************************************
void PoeMod::SetWeightAndColor(string modLine, TypeModFilterArray modarray)
{
	for (unsigned int i = 0; i < modarray._filters.size(); i++)
	{
		if (modLine.find(modarray._filters[i]._text) != string::npos)
		{
			//sets the color in which to display the mod.
			_color = modarray._filters[i]._color;

			if (modarray._filters[i]._delimType == none)
			{
				SetModweight(modarray._filters[i]._modWeight);
				return;
			}
			else if (modarray._filters[i]._delimType == DelimType::plus)
			{
				SetModweight(ReadValuePlus(modLine) * modarray._filters[i]._modWeight);
			}
			else if (modarray._filters[i]._delimType == DelimType::pluspercent)
			{
				SetModweight(ReadValuePlusPercent(modLine)* modarray._filters[i]._modWeight);
			}
			else if (modarray._filters[i]._delimType == DelimType::percent)
			{
				SetModweight(ReadValuePercent(modLine)* modarray._filters[i]._modWeight);
			}
			else if (modarray._filters[i]._delimType == DelimType::to)
			{
				SetModweight(ReadValueTo(modLine)* modarray._filters[i]._modWeight);
			}
		}
	}
}

TypeModFilterArray* GetFilterArray(ItemType type)
{
	switch (type)
	{
	case (ItemType::Undefined):
	{
		throw std::invalid_argument("Cannot get undefined filter array");
	}
	case (ItemType::Map):
	{
		return &MapFilter;
	}
	case (ItemType::Ring):
	{
		return &RingFilter;
	}
	case (ItemType::Amulet):
	{
		return &AmuletFilter;
	}
	case (ItemType::Belt):
	{
		return &BeltFilter;
	}
	case (ItemType::Jewel):
	{
		return &JewelFilter;
	}
	case (ItemType::Equipable):
	{
		return &EquipableFilter;
	}
	case (ItemType::Weapon):
	{
		return &WeaponFilter;
	}
	case (ItemType::Flask):
	{
		return &FlaskFilter;
	}
	}
	return nullptr;
}

double ReadValuePlus(string text)
{
	int pos = text.find_first_of('+');
	return stod(text.substr(pos));
}
double ReadValuePlusPercent(string text)
{
	int pos = text.find_first_of('%');
	int posplus = text.find_first_of('+');
	return stod(text.substr(posplus, pos));
}
double ReadValuePercent(string text)
{
	int pos = text.find_first_of('%');
	return stod(text.substr(0, pos));
}

double ReadValueTo(string text)
{
	//0123456789
	//Adds 1 to 44 Lightning Damage
	std::string::size_type sz;
	double minvalue, maxvalue;

	minvalue = stod(text.substr(5), &sz);
	maxvalue = stod(text.substr(5 + sz + 4));

	return (minvalue + maxvalue) / 2.0;
}