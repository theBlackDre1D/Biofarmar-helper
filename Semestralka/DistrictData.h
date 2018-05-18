#pragma once
class DistrictData
{
public:
	DistrictData();
	~DistrictData();

	int mTotalAreaFirstYear;
	int mTotalAreaSecondYear;
	int mOtherAreaFirstYear;
	int mOtherAreaSecondYear;

	int mDistrictCode;
	int mOrchardDistrictArea;
	int mTotalDistrictArea;
	int mForestDistrictArea;
	int mAgricularDistrictArea;

	void clear();
	double calculateRatioTotal_VS_Agricular();
	double calculateRatioTotal_VS_Forest();
	double calculateRatio(double higherNumber, double lowerNumber);
	double differenceBetweenRatios(); // total ratio and other ratio depends on use case
};

