#include "ModFilter.h"

TypeModFilterArray MapFilter;
TypeModFilterArray RingFilter;
TypeModFilterArray AmuletFilter;
TypeModFilterArray BeltFilter;
TypeModFilterArray JewelFilter;
TypeModFilterArray FlaskFilter;
TypeModFilterArray WeaponFilter;
TypeModFilterArray EquipableFilter;

bool ReadFilters()
{
	if (!ReadSingleFilterMap(MapFilter, MapFilterCFG))
		return false;
	if (!ReadSingleFilter(RingFilter, RingFilterCFG))
		return false;
	if (!ReadSingleFilter(AmuletFilter, AmuletFilterCFG))
		return false;

	cout << "RingFilter._filters.size(): " << RingFilter._filters.size() << "\n";
	cout << "RingFilter._minWeight: " << RingFilter._minWeight << "\n";
	cout << "MapFilter._filters.size(): " << MapFilter._filters.size() << "\n";
	cout << "MapFilter._minWeight: " << MapFilter._minWeight << "\n";
	return true;
}

bool ReadSingleFilter(TypeModFilterArray &filterarray, const char* file)
{
	cout << "Reading" << "\n";
	bool _success = true;
	std::string delimiter = ";";
	std::ifstream _configFile(file);
	if (_configFile.is_open()) 
	{
		std::string line;
		getline(_configFile, line);
		//Read minWeight=
		if (line.find("minWeight=") != string::npos)
		{
			//printf(line.c_str());
			//printf(line.substr(10, 10).c_str());
			filterarray._minWeight = stod(line.substr(10),nullptr);
			printf("minWeight = %f\n", filterarray._minWeight);
		}
		else
			return false;

		//Text/Delim/Color/Weight value
		while (getline(_configFile, line)) {
			istringstream f(line);
			string s;
			string modtext;
			DWORD color;
			DelimType delimtype = none;
			double modWeight;

			//Name
			getline(f, s, ';');
			cout << s << "\n";
			modtext = s;

			//Delimiter type
			getline(f, s, ';');
			cout << s << "\n";
			if (s == "+")
				delimtype = DelimType::plus;
			else if (s == "+%")
				delimtype = DelimType::pluspercent;
			else if (s == "%")
				delimtype = DelimType::percent;
			else if (s == "to")
				delimtype = DelimType::to;

			//Color
			getline(f, s, ';');
			cout << s << "\n";
			color = stoul(s,nullptr,0);
			printf("%x\n", color);
			//Weight
			getline(f, s, ';');
			cout << s << "\n";
			modWeight = stod(s,  0);
			printf("%f\n", modWeight);

			//add to array
			filterarray._filters.push_back(ModFilterInfo{ modtext, color,modWeight,delimtype });

		}
		_configFile.close();
	}
	return _success;
}

bool ReadSingleFilterMap(TypeModFilterArray &filterarray, const char* file)
{
	bool _success = true;
	std::string delimiter = ";";
	std::ifstream _configFile(file);
	if (_configFile.is_open()) {
		std::string line;
		while (getline(_configFile, line)) {
			// using printf() in all tests for consistency
			int delimLoc = line.find(";");
			string modText = line.substr(0, delimLoc);
			DWORD color = std::strtoul(line.substr(delimLoc + 1, 10).c_str(), NULL, 16);

			printf("%s\n", modText.c_str());
			//printf("%s\n", line.substr(delimLoc+1, 10).c_str());
			printf("%x\n", color);
			filterarray._filters.push_back(ModFilterInfo{ modText, color });

		}
		_configFile.close();
	}
	return _success;
}