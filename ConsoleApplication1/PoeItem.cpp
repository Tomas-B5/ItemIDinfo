#include "pch.h"
#include "PoeItem.h"


bool PoeItem::IsItemMapType(string cbData)
{
	bool result = false;
	istringstream strstream(cbData);
	string line;
	getline(strstream, line); //First line is always item rarity
	//check if rarity is normal, magic,rare, unique
	if ((line.find("Rare", 0) != string::npos) || line.find("Unique", 0) != string::npos)
	{
		getline(strstream, line); //should be map name, not relevant here.
		getline(strstream, line); //Should move to item base.
		if (line.find(" Map", 0) != string::npos)
			result = true;
	}
	else if ((line.find("Magic", 0) != string::npos) || line.find("Normal", 0) != string::npos)
	{
		getline(strstream, line); //Should move to item base. or base + item name for magic
		if (line.find(" Map", 0) != string::npos)
			result = true;
	}
	//then check for type.
	return result;
}

PoeItem::PoeItem(string cbData, POINT pt)
{
	istringstream strstream(cbData);
	string line;
	int lines = 0;

	getline(strstream, line); //First line is always item rarity

		if (line.find("Rare", 0) != string::npos)
		_itemRarity = ItemRarity::Rare;
	else if (line.find("Magic", 0) != string::npos)
		_itemRarity = ItemRarity::Magic;
	else if (line.find("Normal", 0) != string::npos)
		{
			_itemRarity = ItemRarity::Normal;
			return;//I don't really care about white items for now? I think
		}
	else if (line.find("Unique", 0) != string::npos)
			_itemRarity = ItemRarity::Unique;
	else
		_itemRarity = ItemRarity::Other;


	getline(strstream, line); //Second line could be either item base or item name depending on the rarity.

	if ( _itemRarity == ItemRarity::Rare || _itemRarity == ItemRarity::Unique)//If item rarity is not normal or magic it's name will be 2nd line.
	{
		_name = line; //SETTING ITEM NAME
		getline(strstream, line); 

		_itemBase = line; // Its fucked on magic items name will be same as based type which will be "xxx Pagan Wand of XXX" so not ideal i guess.
	}
	else if (_itemRarity == ItemRarity::Magic)
	{
		_name = line; //SETTING ITEM NAME
		_itemBase = line; // Its fucked on magic items name will be same as based type which will be "xxx Pagan Wand of XXX" so not ideal i guess.

	}

	//Time to figure out item type.
	_itemType = ProcessItemType(_itemBase, cbData);

	
	ProcessMods(cbData, _itemType);

	cout << "_modCount: " << _modCount << "\n";

	_totalModWeight = CountTotalModWeight();
	cout << "_totalModWeight: " << _totalModWeight << "\n";

	if (_totalModWeight >= GetFilterArray(_itemType)->_minWeight)
		_important = true;

}

ItemType PoeItem::ProcessItemType(string base, string cbData)
{
	if (base.find(" Map", 0) != string::npos)
		return ItemType::Map;
	else if (base.find(" Flask", 0) != string::npos)
		return ItemType::Flask;
	else if (base.find(" Jewel", 0) != string::npos)
		return ItemType::Jewel;
	else if (base.find(" Amulet", 0) != string::npos || base.find(" Talisman", 0) != string::npos)
		return ItemType::Amulet;
	else if (base.find(" Ring", 0) != string::npos)
		return ItemType::Ring;
	else if (base.find(" Belt", 0) != string::npos || base.find("Rustic Sash", 0) != string::npos || base.find("Stygian Vise", 0) != string::npos)
		return ItemType::Belt;
	else if (cbData.find("Attacks per Second:", 0) != string::npos)
		return ItemType::Weapon;//Could also maybe get the weapon type here idk if i need it
	else
		return ItemType::Equipable;//could technically parse base types to match to boots/helm/shield/body etc. Would take a while tho to make files and etc.


}

void PoeItem::ProcessMods(string cbData, ItemType type)
{
	istringstream strstream(cbData);
	string line;
	while (line.find("Item Level:", 0) == string::npos)
	{
		getline(strstream, line);
		//cout << "1 " << line << "\n";
	}
	getline(strstream, line);//Should make it jump to --- line.
	//cout << "2 " << line << "\n";

	//cout << line << "\n";

	//Maps in 3.8 will have implicits so i have to figure out how to handle that. Only some maybe?

	while (getline(strstream, line)	)
	{
		if (line.length() > 89) // should help to drop some texts like //flask info/map info/jewel info
			// CHECK TO SEE IF THERE ARE ANY MODS THAT COULD POTENTIALY BE DROPPED BY THIS !!!
			continue;

		_mods[_modCount] = PoeMod(line,type);
		_modCount++;
	}
	/*if (_mods[_modCount].GetModText().find("--------") != string::npos)
		if (_modCount != 0)*/
}


double PoeItem::CountTotalModWeight()
{
	double result = 0;
	for (int i = 0; i < _modCount; i++)
		result += _mods[i].GetModweight();
	return result;
}

PoeItem::PoeItem()
{
	
}
