#pragma once
#include "City.h"
#include "structures\table\unsorted_sequence_table.h"

#include <string>

using namespace structures;

class CityWithSoil /*: public City*/
{
public:
	CityWithSoil();
	CityWithSoil(string name);
	CityWithSoil(const CityWithSoil &newCity);
	~CityWithSoil();

	//prvy int je kluc ktory reprezentuje rok (1996 - 2017) a data su pocet v m2 pre dany rok.
	UnsortedSequenceTable<int, int> *mTotalArea;
	UnsortedSequenceTable<int, int> *mAgriculturalSoilAll;
	UnsortedSequenceTable<int, int> *mAgriculturalSoilCropLand;
	UnsortedSequenceTable<int, int> *mAgriculturalSoilHop;
	UnsortedSequenceTable<int, int> *mAgriculturalSoilVineyard;
	UnsortedSequenceTable<int, int> *mAgriculturalSoilGarden;
	UnsortedSequenceTable<int, int> *mAgriculturalSoilOrchard;
	UnsortedSequenceTable<int, int> *mAgriculturalSoilGrassArea;
	UnsortedSequenceTable<int, int> *mNonAgriculturalSoilAll;
	UnsortedSequenceTable<int, int> *mNonAgriculturalSoilForestArea;
	UnsortedSequenceTable<int, int> *mNonAgriculturalSoilWaterArea;
	UnsortedSequenceTable<int, int> *mNonAgriculturalSoilBuildUpArea;
	UnsortedSequenceTable<int, int> *mNonAgriculturalSoilOtherArea;

	UnsortedSequenceTable<int, int> getTotalArea();
	UnsortedSequenceTable<int, int> getAgriculturalSoilAll();
	UnsortedSequenceTable<int, int> getBuildUpArea();
	UnsortedSequenceTable<int, int> getForestArea();
	UnsortedSequenceTable<int, int> getOrchardArea();

	double buildUpArea_VS_TotalAreaRatioToPreviousYear(int year);
	double differenceBetweenTotalAndAgriculturalAreaInTimePeriod(int firstYear, int secondYear);
	double calculateRatio(double firstNumber, double secondNumber);

	bool justInitialized();
};

