#pragma once
#include "City.h"

#include <string>

using namespace std;

class CityGrounding
{
public:
	CityGrounding();
	~CityGrounding();

	//int mOrderNumber;
	int mCityCode;
	//string mName; uz ma od predka
	int mDistrictCode;
	string mDistrictName;
	int mCountyCode;
	string mCountyName;
	int mRegionCode;
	string mRegionName;

};

