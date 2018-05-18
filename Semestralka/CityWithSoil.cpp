#include "CityWithSoil.h"



CityWithSoil::CityWithSoil()
{
	mTotalArea = new UnsortedSequenceTable<int, int>();
	mAgriculturalSoilAll = new UnsortedSequenceTable<int, int>();
	mAgriculturalSoilCropLand = new UnsortedSequenceTable<int, int>();
	mAgriculturalSoilHop = new UnsortedSequenceTable<int, int>();
	mAgriculturalSoilVineyard = new UnsortedSequenceTable<int, int>();
	mAgriculturalSoilGarden = new UnsortedSequenceTable<int, int>();
	mAgriculturalSoilOrchard = new UnsortedSequenceTable<int, int>();
	mAgriculturalSoilGrassArea = new UnsortedSequenceTable<int, int>();
	mNonAgriculturalSoilAll = new UnsortedSequenceTable<int, int>();
	mNonAgriculturalSoilForestArea = new UnsortedSequenceTable<int, int>();
	mNonAgriculturalSoilWaterArea = new UnsortedSequenceTable<int, int>();
	mNonAgriculturalSoilBuildUpArea = new UnsortedSequenceTable<int, int>();
	mNonAgriculturalSoilOtherArea = new UnsortedSequenceTable<int, int>();
}

CityWithSoil::CityWithSoil(const CityWithSoil & newCity)
{
	//mName = newCity.mName;
	mTotalArea = new UnsortedSequenceTable<int, int>(*newCity.mTotalArea);
	mAgriculturalSoilAll = new UnsortedSequenceTable<int, int>(*newCity.mAgriculturalSoilAll);
	mAgriculturalSoilCropLand = new UnsortedSequenceTable<int, int>(*newCity.mAgriculturalSoilCropLand);
	mAgriculturalSoilHop = new UnsortedSequenceTable<int, int>(*newCity.mAgriculturalSoilHop);
	mAgriculturalSoilVineyard = new UnsortedSequenceTable<int, int>(*newCity.mAgriculturalSoilVineyard);
	mAgriculturalSoilGarden = new UnsortedSequenceTable<int, int>(*newCity.mAgriculturalSoilGarden);
	mAgriculturalSoilOrchard = new UnsortedSequenceTable<int, int>(*newCity.mAgriculturalSoilOrchard);
	mAgriculturalSoilGrassArea = new UnsortedSequenceTable<int, int>(*newCity.mAgriculturalSoilGrassArea);
	mNonAgriculturalSoilAll = new UnsortedSequenceTable<int, int>(*newCity.mNonAgriculturalSoilAll);
	mNonAgriculturalSoilForestArea = new UnsortedSequenceTable<int, int>(*newCity.mNonAgriculturalSoilForestArea);
	mNonAgriculturalSoilWaterArea = new UnsortedSequenceTable<int, int>(*newCity.mNonAgriculturalSoilWaterArea);
	mNonAgriculturalSoilBuildUpArea = new UnsortedSequenceTable<int, int>(*newCity.mNonAgriculturalSoilBuildUpArea);
	mNonAgriculturalSoilOtherArea = new UnsortedSequenceTable<int, int>(*newCity.mNonAgriculturalSoilOtherArea);
}


CityWithSoil::~CityWithSoil()
{
	delete mTotalArea;
	delete mAgriculturalSoilAll;
	delete mAgriculturalSoilCropLand;
	delete mAgriculturalSoilHop;
	delete mAgriculturalSoilVineyard;
	delete mAgriculturalSoilGarden;
	delete mAgriculturalSoilOrchard;
	delete mAgriculturalSoilGrassArea;
	delete mNonAgriculturalSoilAll;
	delete mNonAgriculturalSoilForestArea;
	delete mNonAgriculturalSoilWaterArea;
	delete mNonAgriculturalSoilBuildUpArea;
	delete mNonAgriculturalSoilOtherArea;
}

UnsortedSequenceTable<int, int> CityWithSoil::getTotalArea()
{
	return *mTotalArea;
}

UnsortedSequenceTable<int, int> CityWithSoil::getAgriculturalSoilAll()
{
	return *mAgriculturalSoilAll;
}

UnsortedSequenceTable<int, int> CityWithSoil::getBuildUpArea()
{
	return *mNonAgriculturalSoilBuildUpArea;
}

UnsortedSequenceTable<int, int> CityWithSoil::getForestArea()
{
	return *mNonAgriculturalSoilForestArea;
}

UnsortedSequenceTable<int, int> CityWithSoil::getOrchardArea()
{
	return *mNonAgriculturalSoilOtherArea;
}

double CityWithSoil::buildUpArea_VS_TotalAreaRatioToPreviousYear(int year)
{
	if (year == 1996) {
		//return (*mTotalArea)[year];
		return 100.0;
	}
	double totalArea = static_cast<double>((*mTotalArea)[year]);
	double buildUpArea = static_cast<double>((*mNonAgriculturalSoilBuildUpArea)[year]);
	double ratioNow = (buildUpArea / totalArea) * 100;

	double totalAreaPreviousYear = static_cast<double>((*mTotalArea)[year - 1]);
	double buildUpAreaPreviousYear = static_cast<double>((*mNonAgriculturalSoilBuildUpArea)[year - 1]);
	double ratioPreviousYear = (buildUpAreaPreviousYear / totalAreaPreviousYear) * 100.0;
	
	return ratioNow - ratioPreviousYear;
}

double CityWithSoil::differenceBetweenTotalAndAgriculturalAreaInTimePeriod(int firstYear, int secondYear)
{// total vs orna
	double totalAreaFirst = static_cast<double>((*mTotalArea)[firstYear]);
	double agricularAreaFirst = static_cast<double>((*mAgriculturalSoilAll)[firstYear]);
	double ratioFirst = calculateRatio(totalAreaFirst, agricularAreaFirst);

	double totalAreaSecond = static_cast<double>((*mTotalArea)[secondYear]);
	double agricularAreaSecond = static_cast<double>((*mAgriculturalSoilAll)[secondYear]);
	double retioSecond = calculateRatio(totalAreaSecond, agricularAreaSecond);

	return ratioFirst - retioSecond;
}

double CityWithSoil::calculateRatio(double higherNumber, double lowerNumber)
{
	double result = (lowerNumber / higherNumber) * 100.0;
	return result;
}

bool CityWithSoil::justInitialized()
{
	if (mTotalArea == 0) {
		return true;
	}
	else
	{
		return false;
	}
}
