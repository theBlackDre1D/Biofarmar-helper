#include "DistrictData.h"



DistrictData::DistrictData()
{
	mDistrictCode = 0;
	mOrchardDistrictArea = 0;
	mTotalDistrictArea = 0;
	mForestDistrictArea = 0;
	mAgricularDistrictArea = 0;
	mTotalAreaFirstYear = 0;
	mTotalAreaSecondYear = 0;
	mOtherAreaFirstYear = 0;
	mOtherAreaSecondYear = 0;
}


DistrictData::~DistrictData()
{
}

void DistrictData::clear()
{
	mOrchardDistrictArea = 0;
	mTotalDistrictArea = 0;
	mForestDistrictArea = 0;
	mAgricularDistrictArea = 0;
	mTotalAreaFirstYear = 0;
	mTotalAreaSecondYear = 0;
	mOtherAreaFirstYear = 0;
	mOtherAreaSecondYear = 0;
}

double DistrictData::calculateRatioTotal_VS_Agricular()
{
	return static_cast<double>(mAgricularDistrictArea) / static_cast<double>(mTotalDistrictArea);
}

double DistrictData::calculateRatioTotal_VS_Forest()
{
	return static_cast<double>(mTotalDistrictArea) / static_cast<double>(mForestDistrictArea);
}

double DistrictData::calculateRatio(double higherNumber, double lowerNumber)
{
	return (lowerNumber / higherNumber) * 100.0;
}

double DistrictData::differenceBetweenRatios()
{
	if (mOtherAreaFirstYear == 0 && mOtherAreaSecondYear == 0) {
		return 0.0;
	}
	double firstYearRatio = (static_cast<double>(mOtherAreaFirstYear) / static_cast<double>(mTotalAreaFirstYear)) * 100.0;
	double secondYearRatio = (static_cast<double>(mOtherAreaSecondYear) / static_cast<double>(mTotalAreaSecondYear)) * 100.0;
	double result = firstYearRatio - secondYearRatio;
	return result;
}
