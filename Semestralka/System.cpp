#include "System.h"
#include "structures\array\array.h"
#include "structures\list\linked_list.h"
#include "structures\table\sorting\quick_sort.h"
#include "CityGrounding.h"
#include "structures/table/sorting/quick_sort.h"

#include <sstream>
#include <ratio>
#include <iostream>
#include <fstream>


System::System()
{
	mPopulationInformation = new BinarySearchTree<string, PopulationData*>(); // mesta s poctami obyvatelov
	mCitiesWithSoilInformations = new UnsortedSequenceTable<string, CityWithSoil*>(); // mesta s rozlahami pozemkov
	mCitiesWithRegions = new SortedSequenceTable<string, CityGrounding*>(); // okresy s mestami
	mDistricts = new SortedSequenceTable<string, DistrictData*>(); // kraje s udajmi o rozlohe
	mUnsortedDistricts = new UnsortedSequenceTable<string, DistrictData*>(); // cisto na triediace ucely
}


System::~System()
{
	for (TableItem<string, PopulationData*> *item : *mPopulationInformation) {
		delete item->accessData();
	}
	for (TableItem<string, CityWithSoil*> *item : *mCitiesWithSoilInformations) {
		delete item->accessData();
	}
	for (TableItem<string, CityGrounding*> *item : *mCitiesWithRegions) {
		delete item->accessData();
	}
	for (TableItem<string, DistrictData*> *item : *mDistricts) {
		delete item->accessData();
	}
	//for (TableItem<string, DistrictData*> *item : *mUnsortedDistricts) {
	//	delete item->accessData();
	//}
	delete mPopulationInformation;
	delete mCitiesWithRegions;
	delete mDistricts;
	delete mCitiesWithSoilInformations;
	delete mUnsortedDistricts;
	mPopulationInformation = nullptr;
	mCitiesWithSoilInformations = nullptr;
	mCitiesWithRegions = nullptr;
	mDistricts = nullptr;
	mUnsortedDistricts = nullptr;
}

void System::showCityInformationsInRange(string cityName, int yearSince, int yearTo)
{
	CityWithSoil *wantedCity = nullptr;
	for (TableItem<string, CityWithSoil*> *city : *mCitiesWithSoilInformations) {
		if (city->getKey() == cityName) {
			//wantedCity = new CityWithSoil((*city->accessData()));
			//delete wantedCity;
			//wantedCity = nullptr;
			wantedCity = city->accessData();
		}
	}

	if (wantedCity->justInitialized()) {
		cout << "City -> " << cityName << " <- not found." << endl;
		return;
	}

	int totalArea = 0;
	int agricularSoilArea = 0;
	int orchardParkArea = 0; // ovocne sady
	int waterArea = 0;
	int forestArea = 0;
	int buildUpArea = 0;

	//bool loopCondition = true;
	int tmpYearSince = yearSince;
	while (tmpYearSince <= yearTo)
	{
		cout << "==================================================================" << endl;
		cout << "Year: " << tmpYearSince << endl;
		if (!wantedCity->mTotalArea->isEmpty()) {
			UnsortedSequenceTable<int, int> table = wantedCity->getTotalArea();
			int value = table[tmpYearSince];
			totalArea = value;
			cout << "Total area: " << value << " (m2)"<< endl;
		}

		if (!wantedCity->mAgriculturalSoilAll->isEmpty()) {
			UnsortedSequenceTable<int, int> table = wantedCity->getAgriculturalSoilAll();
			int value = table[tmpYearSince];
			cout << "Total agricular area: " << value << " (m2)" << endl;
		}

		if (!wantedCity->mAgriculturalSoilOrchard->isEmpty()) {
			UnsortedSequenceTable<int, int> table = (*wantedCity->mAgriculturalSoilOrchard);
			int value = table[tmpYearSince];
			cout << "Total orchard area: " << value << " (m2)" << endl;
		}

		if (!wantedCity->mNonAgriculturalSoilWaterArea->isEmpty()) { // water area : total area
			UnsortedSequenceTable<int, int> table = (*wantedCity->mNonAgriculturalSoilWaterArea);
			int value = table[tmpYearSince];
			double ratio = calculateRatio(totalArea, value);
			cout << "Water area vs Total area ratio:  " << ratio << " %" << endl;
		}

		if (!wantedCity->mNonAgriculturalSoilForestArea->isEmpty()) { // forest area : total area
			UnsortedSequenceTable<int, int> table = (*wantedCity->mNonAgriculturalSoilForestArea);
			int value = table[tmpYearSince];
			double ratio = calculateRatio(totalArea, value);
			cout << "Forest area vs Total area ratio:  " << ratio << " %" << endl;
		}

		if (!wantedCity->mNonAgriculturalSoilBuildUpArea->isEmpty()) { // build up area : total area
			UnsortedSequenceTable<int, int> table = (*wantedCity->mNonAgriculturalSoilBuildUpArea);
			int value = table[tmpYearSince];
			double ratio = calculateRatio(totalArea, value);
			cout << "Forest area vs Total area ratio:  " << ratio << " %" << endl;
		}

		tmpYearSince++;
	}
	//delete wantedCity;
}

void System::orderCitiesByTotalArea(int year)
{
	cout << "Pick type of ordering: 1 - ascending, 2 - descending" << endl;
	int sortType;
	cin >> sortType;

	switch (sortType)
	{
	case 1: {
		QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareTotalArea);
		sort->sortTotalAreaASC(*mCitiesWithSoilInformations, year);
		showCities();

		cout << "Ordered by 2 a) - ascending" << endl;
		break;
	}
	
	case 2: { // zostupne
		QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareTotalArea);
		sort->sortTotalAreaDESC(*mCitiesWithSoilInformations, 2000);
		showCities();
		cout << "Ordered by 2 a) - descending" << endl;
		break;
	}
	default:
		cout << "Wrong type of ordering!" << endl;
		break;
	}
}

void System::orderCitiesByIncrementBuildUpAreaToTotalArea(int year)
{
	cout << "Pick type of ordering: 1 - ascending, 2 - descending" << endl;
	int sortType;
	cin >> sortType;

	switch (sortType)
	{
	case 1: { //vzostupne
		QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareIncreasment);
		sort->sortIncrementTotal_vs_BuildASC(*mCitiesWithSoilInformations, year);
		showCities();
		cout << "Ordered by 2 b) - ascending" << endl;
		cout << "Done :)" << endl;
		break;
	}

	case 2: { //zostupne
		QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareIncreasment);
		sort->sortIncrementTotal_vs_BuildDESC(*mCitiesWithSoilInformations, year);
		showCities();
		cout << "Ordered by 2 b) - descending" << endl;
		cout << "Done :)" << endl;
		break;
	}
	default:
		cout << "Wrong type of ordering!" << endl;
		break;
	}
}

void System::orderCitiesByDifferenceBetweenTotalAndAgriculturalArea(int firstYear, int secondYear)
{
	cout << "Pick type of ordering: 1 - ascending, 2 - descending" << endl;
	int sortType;
	cin >> sortType;

	switch (sortType)
	{
	case 1: { //vzostupne
		QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareDifference);
		sort->sortDifferenceTotal_vs_AgricularASC(*mCitiesWithSoilInformations, firstYear, secondYear);
		showCities();
		cout << "Ordered by 2 c) - ascending" << endl;
		break;
	}

	case 2: { //zostupne
		QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareDifference);
		sort->sortDifferenceTotal_vs_AgricularDESC(*mCitiesWithSoilInformations, firstYear, secondYear);
		showCities();
		cout << "Ordered by 2 c) - descending" << endl;
		break;
	}
	default:
		cout << "Wrong type of ordering!" << endl;
		break;
	}
}

void System::districtWithMostAndLeastTotalAreaInYear(int year)
{
	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		district->accessData()->clear(); // vycistim atributy lebo ich pouzivam na ukladanie docasnych hodnot
	}

	for (TableItem<string, CityWithSoil*> *item : *mCitiesWithSoilInformations) {
		string cityName = item->getKey();
		string districtName = "";
		int position = -2000;
		position = cityName.find(',');
		if (position >= 0) {
			districtName = cityName.substr(position + 6); // ziskam z nazvu okres
			cityName.erase(position); // odstranujem z nazvu okres
		}
		else
		{
			if (mCitiesWithRegions->containsKey(cityName)) {
				districtName = (*mCitiesWithRegions)[cityName]->mDistrictName;
			}
		}
		// pridaj spravnemu kraju hodnotu do rozlohy
		if (districtName != "") {
			(*mDistricts)[districtName]->mTotalDistrictArea += item->accessData()->getTotalArea()[year];
		}
	}

	string mostTotalAreaName = "";
	int mostTotalArea = 0;
	string leastTotalAreaName = "";
	int leastTotalArea = 0;

	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		if (district->accessData()->mTotalDistrictArea > mostTotalArea) {
			mostTotalArea = district->accessData()->mTotalDistrictArea;
			mostTotalAreaName = district->getKey();
		}
		else if (district->accessData()->mTotalDistrictArea < leastTotalArea)
		{
			leastTotalArea = district->accessData()->mTotalDistrictArea;
			leastTotalAreaName = district->getKey();
		}
	}

	cout << "District with most total area in year " << year << " is: " << mostTotalAreaName << " with area: " << mostTotalArea << endl;
	cout << "District with least total area in year " << year << " is: " << leastTotalAreaName << " with area: " << leastTotalArea << endl;

}

void System::districtWithBestAndWorstRatioTotal_VS_Agricular(int year)
{
	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		district->accessData()->clear(); // vycistim atributy lebo ich pouzivam na ukladanie docasnych hodnot
	}

	for (TableItem<string, CityWithSoil*> *item : *mCitiesWithSoilInformations) {
		string cityName = item->getKey();
		string districtName = "";
		int position = -2000;
		position = cityName.find(',');
		if (position >= 0) {
			districtName = cityName.substr(position + 6); // ziskam z nazvu okres
			cityName.erase(position); // odstranujem z nazvu okres
		}
		else
		{
			if (mCitiesWithRegions->containsKey(cityName)) {
				districtName = (*mCitiesWithRegions)[cityName]->mDistrictName;
			}
		}
		// pridaj spravnemu kraju hodnotu do rozlohy
		if (districtName != "") {
			(*mDistricts)[districtName]->mTotalDistrictArea += item->accessData()->getTotalArea()[year]; // pridava do celkovej rozlohy okresu
			(*mDistricts)[districtName]->mAgricularDistrictArea += item->accessData()->getAgriculturalSoilAll()[year]; // pridava do rozlohy ornej pody v okrese
		}
	}

	string maxRatioDistrictName = "";
	double maxRatio = 0.0;
	string minRatioDistrictName = "";
	double minRatio = 0.0;

	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		if (district->accessData()->calculateRatioTotal_VS_Agricular() > maxRatio) {
			maxRatio = district->accessData()->calculateRatioTotal_VS_Agricular();
			maxRatioDistrictName = district->getKey();
		}
		else if (district->accessData()->calculateRatioTotal_VS_Agricular() < minRatio)
		{
			minRatio = district->accessData()->calculateRatioTotal_VS_Agricular();
			minRatioDistrictName = district->getKey();
		}
	}
	
	cout << "District with max ratio total VS agricular area is: " << maxRatioDistrictName << " with ratio: " << maxRatio << endl;
	cout << "District with min ratio total VS agricular area is: " << minRatioDistrictName << " with ratio: " << minRatio << endl;
}

void System::districtWithBestAndWorstRatioTotal_VS_Forest(int year)
{
	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		district->accessData()->clear(); // vycistim atributy lebo ich pouzivam na ukladanie docasnych hodnot
	}

	for (TableItem<string, CityWithSoil*> *item : *mCitiesWithSoilInformations) {
		string cityName = item->getKey();
		string districtName = "";
		int position = -2000;
		position = cityName.find(',');
		if (position >= 0) {
			districtName = cityName.substr(position + 6); // ziskam z nazvu okres
			cityName.erase(position); // odstranujem z nazvu okres
		}
		else
		{
			if (mCitiesWithRegions->containsKey(cityName)) {
				districtName = (*mCitiesWithRegions)[cityName]->mDistrictName;
			}
		}
		// pridaj spravnemu kraju hodnotu do rozlohy
		if (districtName != "") {
			(*mDistricts)[districtName]->mTotalDistrictArea += item->accessData()->getTotalArea()[year]; // pridava do celkovej rozlohy okresu
			(*mDistricts)[districtName]->mAgricularDistrictArea += item->accessData()->getForestArea()[year]; // pridava do rozlohy lesov v okrese
		}
	}

	string maxRatioDistrictName = "";
	double maxRatio = 0.0;
	string minRatioDistrictName = "";
	double minRatio = 0.0;

	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		if (district->accessData()->calculateRatioTotal_VS_Forest() > maxRatio) {
			maxRatio = district->accessData()->calculateRatioTotal_VS_Forest();
			maxRatioDistrictName = district->getKey();
		}
		else if (district->accessData()->calculateRatioTotal_VS_Forest() < minRatio)
		{
			minRatio = district->accessData()->calculateRatioTotal_VS_Forest();
			minRatioDistrictName = district->getKey();
		}
	}

	cout << "District with max ratio total VS forest area is: " << maxRatioDistrictName << " with ratio: " << maxRatio << endl;
	cout << "District with min ratio total VS forest area is: " << minRatioDistrictName << " with ratio: " << minRatio << endl;
}

void System::sortDistrictByDifferenceOrchardAndTotalArea(int firstYear, int secondYear)
{
	cout << "Choose type of ordering: 1 - ascending 2 - descending" << endl;
	int type;
	cin >> type;
	
	if (type > 2 || type < 1) {
		cout << "Wrong type of ordering!" << endl;
		return;
	}

	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		district->accessData()->clear(); // vycistim atributy lebo ich pouzivam na ukladanie docasnych hodnot
	}

	// fill unsorted districts with proper data
	for (TableItem<string, CityWithSoil*> *item : *mCitiesWithSoilInformations) {
		string cityName = item->getKey();
		//cout << cityName << endl;
		string districtName = "";
		int position = -2000;
		position = cityName.find(',');
		if (position >= 0) {
			districtName = cityName.substr(position + 6); // ziskam z nazvu okres
			//districtName[0] = toupper(districtName[0]);
			cityName.erase(position); // odstranujem z nazvu okres
		}
		else
		{
			if (mCitiesWithRegions->containsKey(cityName)) {
				districtName = (*mCitiesWithRegions)[cityName]->mDistrictName;
			}
		}

		if (districtName != "") {
			// pridaj spravnemu kraju hodnotu do rozlohy
			try
			{
				int totalAreaFirstYear = item->accessData()->getTotalArea()[firstYear];
				int totalAreaSecondyear = item->accessData()->getTotalArea()[secondYear];

				(*mUnsortedDistricts)[districtName]->mTotalAreaFirstYear += totalAreaFirstYear;
				(*mUnsortedDistricts)[districtName]->mTotalAreaSecondYear += totalAreaSecondyear;

				cout << "District: " << districtName << " total area first year yet: " << (*mUnsortedDistricts)[districtName]->mTotalAreaFirstYear << endl;

				// ak mesto ma ovocne sady tak ich pripocitaj do hodnot rozlohy ovocnych sadov
				if (item->accessData()->mAgriculturalSoilOrchard->size() > 0) {
					int orchardAreaFirstYear = item->accessData()->getOrchardArea()[firstYear];
					int orchardAreaSecondYear = item->accessData()->getOrchardArea()[secondYear];

					(*mUnsortedDistricts)[districtName]->mOtherAreaFirstYear += orchardAreaFirstYear;
					(*mUnsortedDistricts)[districtName]->mOtherAreaSecondYear += orchardAreaSecondYear;

					cout << "District: " << districtName << " orchard area yet: " << (*mUnsortedDistricts)[districtName]->mTotalAreaFirstYear << endl;

				}
			}
			catch (const std::exception&)
			{
				cout << "Error catch: ----->" << item->getKey() << endl;
			}

		}
	}

	QuickSort<string, DistrictData*> *sort = new QuickSort<string, DistrictData*>(compareByRatioTotalVSOther);
	if (type == 1) {
		sort->sortDistrictsASC(*mUnsortedDistricts);
	}
	else
	{
		sort->sortDistrictsDESC(*mUnsortedDistricts);
	}
	// showing districts
	for (TableItem<string, DistrictData*> *district : *mUnsortedDistricts) {
		cout << district->getKey() << " --> " << district->accessData()->differenceBetweenRatios() << endl;
		//cout << district->getKey() << endl;
	}

	if (type == 1) {
		cout << "4 a) Ascending order." << endl;
	}
	else if (type == 2)
	{
		cout << "4 a) Descending order." << endl;
	}

	delete sort;
}

void System::sortDistrictByDifferenceAgricularAndTotalArea(int firstYear, int secondYear)
{
	cout << "Choose type of ordering: 1 - ascending 2 - descending" << endl;
	int type;
	cin >> type;

	if (type > 2 || type < 1) {
		cout << "Wrong type of ordering!" << endl;
		return;
	}

	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		district->accessData()->clear(); // vycistim atributy lebo ich pouzivam na ukladanie docasnych hodnot
	}

	// fill unsorted districts with proper data
	for (TableItem<string, CityWithSoil*> *item : *mCitiesWithSoilInformations) {
		string cityName = item->getKey();
		string districtName = "";
		int position = -2000;
		position = cityName.find(',');
		if (position >= 0) {
			districtName = cityName.substr(position + 6); // ziskam z nazvu okres
			//toupper(districtName[0]);
			cityName.erase(position); // odstranujem z nazvu okres
		}
		else
		{
			if (mCitiesWithRegions->containsKey(cityName)) {
				districtName = (*mCitiesWithRegions)[cityName]->mDistrictName;
			}
		}

		if (districtName != "") {
			try
			{
				// pridaj spravnemu kraju hodnotu do rozlohy
				int totalAreaFirstYear = item->accessData()->getTotalArea()[firstYear];
				int totalAreaSecondyear = item->accessData()->getTotalArea()[secondYear];

				(*mUnsortedDistricts)[districtName]->mTotalAreaFirstYear += totalAreaFirstYear;
				(*mUnsortedDistricts)[districtName]->mTotalAreaSecondYear += totalAreaSecondyear;

				// ak mesto ma ovocne sady tak ich pripocitaj do hodnot rozlohy ovocnych sadov
				if (item->accessData()->mAgriculturalSoilOrchard->size() > 0) {
					int agricularAreaFirstYear = item->accessData()->getAgriculturalSoilAll()[firstYear];
					int agricularAreaSecondYear = item->accessData()->getAgriculturalSoilAll()[secondYear];

					(*mUnsortedDistricts)[districtName]->mOtherAreaFirstYear += agricularAreaFirstYear;
					(*mUnsortedDistricts)[districtName]->mOtherAreaSecondYear += agricularAreaSecondYear;
				}
			}
			catch (const std::exception&)
			{
				cout << "Error catch: ----->" << item->getKey() << endl;
			}
		}
	}

	QuickSort<string, DistrictData*> *sort = new QuickSort<string, DistrictData*>(compareByRatioTotalVSOther);
	if (type == 1) {
		sort->sortDistrictsASC(*mUnsortedDistricts);
	}
	else
	{
		sort->sortDistrictsDESC(*mUnsortedDistricts);
	}
	// showing districts
	for (TableItem<string, DistrictData*> *district : *mUnsortedDistricts) {
		//cout << district->getKey() << endl;
		cout << district->getKey() << " --> " << district->accessData()->differenceBetweenRatios() << endl;
	}

	if (type == 1) {
		cout << "4 b) Ascending order." << endl;
	}
	else if (type == 2)
	{
		cout << "4 b) Descending order." << endl;
	}

	delete sort;
}

void System::sortDistrictByDifferenceForestAndTotalArea(int firstYear, int secondYear)
{
	cout << "Choose type of ordering: 1 - ascending 2 - descending" << endl;
	int type;
	cin >> type;

	if (type > 2 || type < 1) {
		cout << "Wrong type of ordering!" << endl;
		return;
	}

	for (TableItem<string, DistrictData*> *district : *mDistricts) {
		district->accessData()->clear(); // vycistim atributy lebo ich pouzivam na ukladanie docasnych hodnot
	}

	// fill unsorted districts with proper data
	for (TableItem<string, CityWithSoil*> *item : *mCitiesWithSoilInformations) {
		string cityName = item->getKey();
		string districtName = "";
		int position = -2000;
		position = cityName.find(',');
		if (position >= 0) {
			districtName = cityName.substr(position + 6); // ziskam z nazvu okres
			//toupper(districtName[0]);
			cityName.erase(position); // odstranujem z nazvu okres
		}
		else
		{
			if (mCitiesWithRegions->containsKey(cityName)) {
				districtName = (*mCitiesWithRegions)[cityName]->mDistrictName;
			}
		}

		if (districtName != "") {
			try
			{
				// pridaj spravnemu kraju hodnotu do rozlohy
				int totalAreaFirstYear = item->accessData()->getTotalArea()[firstYear];
				int totalAreaSecondyear = item->accessData()->getTotalArea()[secondYear];

				(*mUnsortedDistricts)[districtName]->mTotalAreaFirstYear += totalAreaFirstYear;
				(*mUnsortedDistricts)[districtName]->mTotalAreaSecondYear += totalAreaSecondyear;

				// ak mesto ma ovocne sady tak ich pripocitaj do hodnot rozlohy ovocnych sadov
				if (item->accessData()->mAgriculturalSoilOrchard->size() > 0) {
					int forestAreaFirstYear = item->accessData()->getForestArea()[firstYear];
					int forestAreaSecondYear = item->accessData()->getForestArea()[secondYear];

					(*mUnsortedDistricts)[districtName]->mOtherAreaFirstYear += forestAreaFirstYear;
					(*mUnsortedDistricts)[districtName]->mOtherAreaSecondYear += forestAreaSecondYear;
				}
			}
			catch (const std::exception&)
			{
				cout << "Error catch: ----->" << item->getKey() << endl;
			}
		}
	}

	QuickSort<string, DistrictData*> *sort = new QuickSort<string, DistrictData*>(compareByRatioTotalVSOther);
	if (type == 1) {
		sort->sortDistrictsASC(*mUnsortedDistricts);
	}
	else
	{
		sort->sortDistrictsDESC(*mUnsortedDistricts);
	}
	// showing districts
	for (TableItem<string, DistrictData*> *district : *mUnsortedDistricts) {
		//cout << district->getKey() << endl;
		cout << district->getKey() << " --> " << district->accessData()->differenceBetweenRatios() << endl;
	}

	if (type == 1) {
		cout << "4 c) Ascending order." << endl;
	}
	else if (type == 2)
	{
		cout << "4 c) Descending order." << endl;
	}
	delete sort;
}

void System::loadDataResidentsInCities()
{
	// loading residents in cities
	ifstream residentsFile("ObyvatelstvoPodlaTrvalehoPobytu.csv");
	if (!residentsFile.is_open()) {
		cout << "ERROR WHILE OPENING FILE" << endl;
	}
	else
	{
		string cityName;
		string mTo5YearsBothSex;
		string mTo5YearsMale;
		string mTo5YearsFemale;

		string m5_9BothSex;
		string m5_9Male;
		string m5_9Female;

		string m10_14BothSex;
		string m10_14Male;
		string m10_14female;

		string m15_19BothSex;
		string m15_19Male;
		string m15_19Female;

		string m20_24BothSex;
		string m20_24Male;
		string m20_24Female;

		string m25_29BothSex;
		string m25_29Male;
		string m25_29Female;

		string m30_34BothSex;
		string m30_34Male;
		string m30_34Female;

		string m35_39BothSex;
		string m35_39Male;
		string m35_39Female;

		string m40_44BothSex;
		string m40_44Male;
		string m40_44Female;

		string m45_49BothSex;
		string m45_49male;
		string m45_49Female;

		string m50_54BothSex;
		string m50_54male;
		string m50_54Female;

		string m55_59BothSex;
		string m55_59male;
		string m55_59Female;

		string m60_64BothSex;
		string m60_64male;
		string m60_64Female;

		string m65_69BothSex;
		string m65_69Male;
		string m65_69Female;

		string m70_74BothSex;
		string m70_74Male;
		string m70_74Female;

		string m75_79BothSex;
		string m75_79Male;
		string m75_79Female;

		string m80_84BothSex;
		string m80_84Male;
		string m80_84Female;

		string m85_89BothSex;
		string m85_89Male;
		string m85_89Female;

		string m90_94BothSex;
		string m90_94Male;
		string m90_94Female;

		string m95_99BothSex;
		string m95_99Male;
		string m95_99Female;

		string m100AndMoreBothSex;
		string m100AndMoreMale;
		string m100AndMoreFemale;

		bool firstLine = true;
		while (residentsFile.good()) {
			getline(residentsFile, cityName, ';');
			if (firstLine) {
				cityName.erase(0, 3); // lebo nejake blbe znaky su na prvom riadku a kazdy dalsi riadok vzdy zacina s \n znakom
				firstLine = false;
			}
			else
			{
				cityName.erase(0, 1);
			}
			getline(residentsFile, mTo5YearsBothSex,';');
			getline(residentsFile, mTo5YearsMale, ';');
			getline(residentsFile, mTo5YearsFemale, ';');

			getline(residentsFile, m5_9BothSex, ';');
			getline(residentsFile, m5_9Male, ';');
			getline(residentsFile, m5_9Female, ';');

			getline(residentsFile, m10_14BothSex, ';');
			getline(residentsFile, m10_14Male, ';');
			getline(residentsFile, m10_14female, ';');

			getline(residentsFile, m15_19BothSex, ';');
			getline(residentsFile, m15_19Male, ';');
			getline(residentsFile, m15_19Female, ';');

			getline(residentsFile, m20_24BothSex, ';');
			getline(residentsFile, m20_24Male, ';');
			getline(residentsFile, m20_24Female, ';');

			getline(residentsFile, m25_29BothSex, ';');
			getline(residentsFile, m25_29Male, ';');
			getline(residentsFile, m25_29Female, ';');

			getline(residentsFile, m30_34BothSex, ';');
			getline(residentsFile, m30_34Male, ';');
			getline(residentsFile, m30_34Female, ';');

			getline(residentsFile, m35_39BothSex, ';');
			getline(residentsFile, m35_39Male, ';');
			getline(residentsFile, m35_39Female, ';');

			getline(residentsFile, m40_44BothSex, ';');
			getline(residentsFile, m40_44Male, ';');
			getline(residentsFile, m40_44Female, ';');

			getline(residentsFile, m45_49BothSex, ';');
			getline(residentsFile, m45_49male, ';');
			getline(residentsFile, m45_49Female, ';');

			getline(residentsFile, m50_54BothSex, ';');
			getline(residentsFile, m50_54male, ';');
			getline(residentsFile, m50_54Female, ';');

			getline(residentsFile, m55_59BothSex, ';');
			getline(residentsFile, m55_59male, ';');
			getline(residentsFile, m55_59Female, ';');

			getline(residentsFile, m60_64BothSex, ';');
			getline(residentsFile, m60_64male, ';');
			getline(residentsFile, m60_64Female, ';');

			getline(residentsFile, m65_69BothSex, ';');
			getline(residentsFile, m65_69Male, ';');
			getline(residentsFile, m65_69Female, ';');

			getline(residentsFile, m70_74BothSex, ';');
			getline(residentsFile, m70_74Male, ';');
			getline(residentsFile, m70_74Female, ';');

			getline(residentsFile, m75_79BothSex, ';');
			getline(residentsFile, m75_79Male, ';');
			getline(residentsFile, m75_79Female, ';');

			getline(residentsFile, m80_84BothSex, ';');
			getline(residentsFile, m80_84Male, ';');
			getline(residentsFile, m80_84Female, ';');

			getline(residentsFile, m85_89BothSex, ';');
			getline(residentsFile, m85_89Male, ';');
			getline(residentsFile, m85_89Female, ';');

			getline(residentsFile, m90_94BothSex, ';');
			getline(residentsFile, m90_94Male, ';');
			getline(residentsFile, m90_94Female, ';');

			getline(residentsFile, m95_99BothSex, ';');
			getline(residentsFile, m95_99Male, ';');
			getline(residentsFile, m95_99Female, ';');

			getline(residentsFile, m100AndMoreBothSex, ';');
			getline(residentsFile, m100AndMoreMale, ';');
			getline(residentsFile, m100AndMoreFemale, ';');

			PopulationData *data = new PopulationData();
			data->mTo5YearsBothSex = stoi(mTo5YearsBothSex);
			data->mTo5YearsMale = stoi(mTo5YearsMale);
			data->mTo5YearsFemale = stoi(mTo5YearsFemale);

			data->m5_9BothSex = stoi(m5_9BothSex);
			data->m5_9Male = stoi(m5_9Male);
			data->m5_9Female = stoi(m5_9Female);

			data->m10_14BothSex = stoi(m10_14BothSex);
			data->m10_14Male = stoi(m10_14Male);
			data->m10_14female = stoi(m10_14female);

			data->m15_19BothSex = stoi(m15_19BothSex);
			data->m15_19Male = stoi(m15_19Male);
			data->m15_19Female = stoi(m15_19Female);

			data->m20_24BothSex = stoi(m20_24BothSex);
			data->m20_24Male = stoi(m20_24Male);
			data->m20_24Female = stoi(m20_24Female);

			data->m25_29BothSex = stoi(m25_29BothSex);
			data->m25_29Male = stoi(m25_29Male);
			data->m25_29Female = stoi(m25_29Female);

			data->m30_34BothSex = stoi(m30_34BothSex);
			data->m30_34Male = stoi(m30_34Male);
			data->m30_34Female = stoi(m30_34Female);

			data->m35_39BothSex = stoi(m35_39BothSex);
			data->m35_39Male = stoi(m35_39Male);
			data->m35_39Female = stoi(m35_39Female);

			data->m40_44BothSex = stoi(m40_44BothSex);
			data->m40_44Male = stoi(m40_44Male);
			data->m40_44Female = stoi(m40_44Female);

			data->m45_49BothSex = stoi(m45_49BothSex);
			data->m45_49male = stoi(m45_49male);
			data->m45_49Female = stoi(m45_49Female);

			data->m50_54BothSex = stoi(m50_54BothSex);
			data->m50_54male = stoi(m50_54male);
			data->m50_54Female = stoi(m50_54Female);

			data->m55_59BothSex = stoi(m55_59BothSex);
			data->m55_59male = stoi(m55_59male);
			data->m55_59Female = stoi(m55_59Female);

			data->m60_64BothSex = stoi(m60_64BothSex);
			data->m60_64male = stoi(m60_64male);
			data->m60_64Female = stoi(m60_64Female);

			data->m65_69BothSex = stoi(m65_69BothSex);
			data->m65_69Male = stoi(m65_69Male);
			data->m65_69Female = stoi(m65_69Female);

			data->m70_74BothSex = stoi(m70_74BothSex);
			data->m70_74Male = stoi(m70_74Male);
			data->m70_74Female = stoi(m70_74Female);

			data->m75_79BothSex = stoi(m75_79BothSex);
			data->m75_79Male = stoi(m75_79Male);
			data->m75_79Female = stoi(m75_79Female);

			data->m80_84BothSex = stoi(m80_84BothSex);
			data->m80_84Male = stoi(m80_84Male);
			data->m80_84Female = stoi(m80_84Female);

			data->m85_89BothSex = stoi(m85_89BothSex);
			data->m85_89Male = stoi(m85_89Male);
			data->m85_89Female = stoi(m85_89Female);

			data->m90_94BothSex = stoi(m90_94BothSex);
			data->m90_94Male = stoi(m90_94Male);
			data->m90_94Female = stoi(m90_94Female);

			data->m95_99BothSex = stoi(m95_99BothSex);
			data->m95_99Male = stoi(m95_99Male);
			data->m95_99Female = stoi(m95_99Female);

			data->m100AndMoreBothSex = stoi(m100AndMoreBothSex);
			data->m100AndMoreMale = stoi(m100AndMoreMale);
			data->m100AndMoreFemale = stoi(m100AndMoreFemale);

			try
			{
				mPopulationInformation->insert(cityName, data);
			}
			catch (const std::exception&)
			{
				delete data;
				cout << "Item with key: " << cityName << " already exists." << endl;
			}
		}

	}
}

void System::loadDataSoilInformations()
{
	ifstream soilFile("VymeraUzemiaVyuzitiePodyTest.csv");
	if (!soilFile.is_open()) {
		cout << "ERROR WHILE OPENING FILE" << endl;
	}
	else
	{
		string cityName;
		string row1;
		string row2;
		string row3;
		string row4;
		string row5;
		string row6;
		string row7;
		string row8;
		string row9;
		string row10;
		string row11;
		string row12;
		string row13;

		string column1;
		string column2;
		string column3;
		string column4;
		string column5;
		string column6;
		string column7;
		string column8;
		string column9;
		string column10;
		string column11;
		string column12;
		string column13;
		string column14;
		string column15;
		string column16;
		string column17;
		string column18;
		string column19;
		string column20;
		string column21;
		string column22;
		
		bool firstRow = true;
		bool iterateFile = soilFile.good();
		while (iterateFile)
		{
			getline(soilFile, cityName, ';');
			if (firstRow) {
				cityName.erase(0, 3); ///////////////////// ====================== odkomentovat po testovani ! ! ! 
				firstRow = false;
			}
			else
			{
				cityName.erase(0, 1);
			}
			cityName[0] = toupper(cityName[0]); // musim lebo v datach som niekde nechtiac pri vymazuvani diatkritiky zmenil velke na male 
			cout << cityName << endl;

			CityWithSoil *newCity = new CityWithSoil();
			getline(soilFile, row1, ';'); // row1 bude vzdy splnat podmienku
			if (row1[0] == 'C') {
				getline(soilFile, column1, ';');
				newCity->mTotalArea->insert(1996, stoi(column1));
				getline(soilFile, column2, ';');
				newCity->mTotalArea->insert(1997, stoi(column2));
				getline(soilFile, column3, ';');
				newCity->mTotalArea->insert(1998, stoi(column3));
				getline(soilFile, column4, ';');
				newCity->mTotalArea->insert(1999, stoi(column4));
				getline(soilFile, column5, ';');
				newCity->mTotalArea->insert(2000, stoi(column5));
				getline(soilFile, column6, ';');
				newCity->mTotalArea->insert(2001, stoi(column6));
				getline(soilFile, column7, ';');
				newCity->mTotalArea->insert(2002, stoi(column7));
				getline(soilFile, column8, ';');
				newCity->mTotalArea->insert(2003, stoi(column8));
				getline(soilFile, column9, ';');
				newCity->mTotalArea->insert(2004, stoi(column9));
				getline(soilFile, column10, ';');
				newCity->mTotalArea->insert(2005, stoi(column10));
				getline(soilFile, column11, ';');
				newCity->mTotalArea->insert(2006, stoi(column11));
				getline(soilFile, column12, ';');
				newCity->mTotalArea->insert(2007, stoi(column12));
				getline(soilFile, column13, ';');
				newCity->mTotalArea->insert(2008, stoi(column13));
				getline(soilFile, column14, ';');
				newCity->mTotalArea->insert(2009, stoi(column14));
				getline(soilFile, column15, ';');
				newCity->mTotalArea->insert(2010, stoi(column15));
				getline(soilFile, column16, ';');
				newCity->mTotalArea->insert(2011, stoi(column16));
				getline(soilFile, column17, ';');
				newCity->mTotalArea->insert(2012, stoi(column17));
				getline(soilFile, column18, ';');
				newCity->mTotalArea->insert(2013, stoi(column18));
				getline(soilFile, column19, ';');
				newCity->mTotalArea->insert(2014, stoi(column19));
				getline(soilFile, column20, ';');
				newCity->mTotalArea->insert(2015, stoi(column20));
				getline(soilFile, column21, ';');
				newCity->mTotalArea->insert(2016, stoi(column21));
				getline(soilFile, column22, ';');
				newCity->mTotalArea->insert(2017, stoi(column22));
			}
			bool read = true;
			readRows:
			while (read) {
				getline(soilFile, row2, ';');
				if (row2 == "\n") {
					goto readRows;
				}
				int decisionRow2 = decideRowName(row2);
				if (decisionRow2 == 1) { // hospodarska spolu
					fillATotal(newCity, soilFile);
				}
				else if (decisionRow2 == 2) { // orna poda
					fillACropLand(newCity, soilFile);
				}
				else if (decisionRow2 == 3) { // chmelnice
					fillAHop(newCity, soilFile);
				}
				else if (decisionRow2 == 4) { // vinice
					fillAVineyard(newCity, soilFile);
				}
				else if (decisionRow2 == 5) { // zahrada
					fillAGarden(newCity, soilFile);
				}
				else if (decisionRow2 == 6) { // ovocny sad
					fillAOrchard(newCity, soilFile);
				}
				else if (decisionRow2 == 7) { // travnaty porast
					fillAGrassArea(newCity, soilFile);
				}
				else if (decisionRow2 == 8) { // nehospodarska spolu
					fillNTotal(newCity, soilFile);
				}
				else if (decisionRow2 == 9) { // lesny pozemok
					fillNForest(newCity, soilFile);
				}
				else if (decisionRow2 == 10) { // voda
					fillNWater(newCity, soilFile);
				}
				else if (decisionRow2 == 11) { // zastavana plocha
					fillNBuildUp(newCity, soilFile);
				}
				else if (decisionRow2 == 12) { // ostatne
					fillNOther(newCity, soilFile);
					read = false;
					//break;
				}
				else
				{
					cout << "No matching table with loaded string" << endl;
				}
			}
			try
			{
				mCitiesWithSoilInformations->insert(cityName, newCity);
			}
			catch (const std::exception&)
			{
				cout << "City with that key/ city name is already in table." << endl;
				delete newCity;
			}
			//if (cityName == "ZemplinskyBranc") {
			//	iterateFile = false;
			//}
			if (cityName == "Gajary") {
				iterateFile = false;
			}
		}
	}
}

void System::loadDataRegions()
{
	loadDistricts();
}

void System::showCities()
{
	for (TableItem<string, CityWithSoil*> *city : *mCitiesWithSoilInformations) {
		cout << city->getKey() << endl;
	}
}

void System::testSort()
{
	//QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareTotalArea);
	//sort->sortTotalAreaASC(*mCitiesWithSoilInformations, 2000);
	//showCities();
	//cout << "==============================================" << endl;
	//sort->sortTotalAreaDESC(*mCitiesWithSoilInformations, 2000);
	//showCities();
	//QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareIncreasment);
	//sort->sortIncrementTotal_vs_BuildASC(*mCitiesWithSoilInformations, 2000);
	//showCities();
	//cout << "==============================================" << endl;
	//sort->sortIncrementTotal_vs_BuildDESC(*mCitiesWithSoilInformations, 2000);
	//showCities();
	//QuickSort<string, CityWithSoil*> *sort = new QuickSort<string, CityWithSoil*>(compareDifference);
	//sort->sortDifferenceTotal_vs_AgricularASC(*mCitiesWithSoilInformations, 2000, 2008);
	//showCities();
	//cout << "==============================================" << endl;
	//sort->sortDifferenceTotal_vs_AgricularDESC(*mCitiesWithSoilInformations, 2000, 2008);
	//showCities();
	for (TableItem<string, DistrictData*> *item : *mDistricts) {
		item->getKey();
	}
}

int System::compareTotalArea(CityWithSoil* const& city1, CityWithSoil* const& city2, int year)
{
	if (city1->getTotalArea()[year] < city2->getTotalArea()[year]) {
		return -1;
	}
	else if (city1->getTotalArea()[year] > city2->getTotalArea()[year]) {
		return 1;
	}
	else
	{
		return 0;
	}
}

int System::compareIncreasment(CityWithSoil * const & city1, CityWithSoil * const & city2, int year)
{
	if (city1->buildUpArea_VS_TotalAreaRatioToPreviousYear(year) < city2->buildUpArea_VS_TotalAreaRatioToPreviousYear(year)) {
		return -1;
	}
	else if (city1->buildUpArea_VS_TotalAreaRatioToPreviousYear(year) > city2->buildUpArea_VS_TotalAreaRatioToPreviousYear(year)) {
		return 1;
	}
	else
	{
		return 0;
	}
}

int System::compareDifference(CityWithSoil * const & city1, CityWithSoil * const & city2, int year1, int year2)
{
	if (city1->differenceBetweenTotalAndAgriculturalAreaInTimePeriod(year1, year2) < city2->differenceBetweenTotalAndAgriculturalAreaInTimePeriod(year1, year2)) {
		return -1;
	}
	else if (city1->differenceBetweenTotalAndAgriculturalAreaInTimePeriod(year1, year2) > city2->differenceBetweenTotalAndAgriculturalAreaInTimePeriod(year1, year2)) {
		return 1;
	}
	else
	{
		return 0;
	}
}

int System::compareByRatioTotalVSOther(DistrictData * const & district1, DistrictData * const & distrct2)
{
	double first = district1->differenceBetweenRatios();
	double second = distrct2->differenceBetweenRatios();
	if (district1->differenceBetweenRatios() < distrct2->differenceBetweenRatios()) {
		return -1;
	}
	else if (district1->differenceBetweenRatios() > distrct2->differenceBetweenRatios()) {
		return 1;
	}
	else
	{
		return 0;
	}
}

int System::decideRowName(string row)
{
	if (row[0] == 'P' && row[21] == 's') { // hospodarska spolu
		return 1;
	}
	else if (row[0] == 'P' && row[21] == 'o' && row[22] == 'r') // orna poda
	{
		return 2;
	}
	else if (row[0] == 'P' && row[21] == 'c' && row[22] == 'h') { // chmelnice
		return 3;
	}
	else if (row[0] == 'P' && row[21] == 'v') { // vinice
		return 4;
	}
	else if (row[0] == 'P' && row[21] == 'z' && row[22] == 'a') { // zahrada
		return 5;
	}
	else if (row[0] == 'P' && row[21] == 'o' && row[22] == 'v') { // ovocny sad
		return 6;
	}
	else if (row[0] == 'P' && row[21] == 't') { // travnaty porast
		return 7;
	}
	else if (row[0] == 'N' && row[23] == 's') { // nehospodarska spolu
		return 8;
	}
	else if (row[0] == 'N' && row[23] == 'l') { // lesny pozemok
		return 9;
	}
	else if (row[0] == 'N' && row[23] == 'v') { // voda
		return 10;
	}
	else if (row[0] == 'N' && row[23] == 'z'/* && row[24] == 'a' && row[24] == 's'*/) { // zastavana plocha
		return 11;
	}
	else if (row[0] == 'N' && row[23] == 'o' && row[24] == 's') { // ostatne
		return 12;
	}
	else
	{
		//throw std::logic_error("Row with unexpected name");
		cout << "Unexpected row name: " << row << endl;
	}
}

void System::fillATotal(CityWithSoil* newCity, ifstream& file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mAgriculturalSoilAll->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mAgriculturalSoilAll->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mAgriculturalSoilAll->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mAgriculturalSoilAll->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mAgriculturalSoilAll->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mAgriculturalSoilAll->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mAgriculturalSoilAll->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mAgriculturalSoilAll->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mAgriculturalSoilAll->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mAgriculturalSoilAll->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mAgriculturalSoilAll->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mAgriculturalSoilAll->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mAgriculturalSoilAll->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mAgriculturalSoilAll->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mAgriculturalSoilAll->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mAgriculturalSoilAll->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mAgriculturalSoilAll->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mAgriculturalSoilAll->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mAgriculturalSoilAll->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mAgriculturalSoilAll->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mAgriculturalSoilAll->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mAgriculturalSoilAll->insert(2017, stoi(column22));
}

void System::fillACropLand(CityWithSoil * newCity, ifstream& file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mAgriculturalSoilCropLand->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mAgriculturalSoilCropLand->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mAgriculturalSoilCropLand->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mAgriculturalSoilCropLand->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mAgriculturalSoilCropLand->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mAgriculturalSoilCropLand->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mAgriculturalSoilCropLand->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mAgriculturalSoilCropLand->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mAgriculturalSoilCropLand->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mAgriculturalSoilCropLand->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mAgriculturalSoilCropLand->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mAgriculturalSoilCropLand->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mAgriculturalSoilCropLand->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mAgriculturalSoilCropLand->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mAgriculturalSoilCropLand->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mAgriculturalSoilCropLand->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mAgriculturalSoilCropLand->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mAgriculturalSoilCropLand->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mAgriculturalSoilCropLand->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mAgriculturalSoilCropLand->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mAgriculturalSoilCropLand->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mAgriculturalSoilCropLand->insert(2017, stoi(column22));
}

void System::fillAHop(CityWithSoil * newCity, ifstream& file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mAgriculturalSoilHop->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mAgriculturalSoilHop->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mAgriculturalSoilHop->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mAgriculturalSoilHop->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mAgriculturalSoilHop->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mAgriculturalSoilHop->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mAgriculturalSoilHop->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mAgriculturalSoilHop->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mAgriculturalSoilHop->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mAgriculturalSoilHop->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mAgriculturalSoilHop->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mAgriculturalSoilHop->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mAgriculturalSoilHop->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mAgriculturalSoilHop->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mAgriculturalSoilHop->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mAgriculturalSoilHop->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mAgriculturalSoilHop->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mAgriculturalSoilHop->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mAgriculturalSoilHop->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mAgriculturalSoilHop->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mAgriculturalSoilHop->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mAgriculturalSoilHop->insert(2017, stoi(column22));
}

void System::fillAVineyard(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mAgriculturalSoilVineyard->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mAgriculturalSoilVineyard->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mAgriculturalSoilVineyard->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mAgriculturalSoilVineyard->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mAgriculturalSoilVineyard->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mAgriculturalSoilVineyard->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mAgriculturalSoilVineyard->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mAgriculturalSoilVineyard->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mAgriculturalSoilVineyard->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mAgriculturalSoilVineyard->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mAgriculturalSoilVineyard->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mAgriculturalSoilVineyard->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mAgriculturalSoilVineyard->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mAgriculturalSoilVineyard->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mAgriculturalSoilVineyard->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mAgriculturalSoilVineyard->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mAgriculturalSoilVineyard->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mAgriculturalSoilVineyard->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mAgriculturalSoilVineyard->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mAgriculturalSoilVineyard->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mAgriculturalSoilVineyard->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mAgriculturalSoilVineyard->insert(2017, stoi(column22));
}

void System::fillAGarden(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mAgriculturalSoilGarden->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mAgriculturalSoilGarden->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mAgriculturalSoilGarden->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mAgriculturalSoilGarden->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mAgriculturalSoilGarden->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mAgriculturalSoilGarden->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mAgriculturalSoilGarden->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mAgriculturalSoilGarden->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mAgriculturalSoilGarden->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mAgriculturalSoilGarden->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mAgriculturalSoilGarden->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mAgriculturalSoilGarden->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mAgriculturalSoilGarden->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mAgriculturalSoilGarden->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mAgriculturalSoilGarden->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mAgriculturalSoilGarden->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mAgriculturalSoilGarden->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mAgriculturalSoilGarden->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mAgriculturalSoilGarden->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mAgriculturalSoilGarden->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mAgriculturalSoilGarden->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mAgriculturalSoilGarden->insert(2017, stoi(column22));
}

void System::fillAOrchard(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mAgriculturalSoilOrchard->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mAgriculturalSoilOrchard->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mAgriculturalSoilOrchard->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mAgriculturalSoilOrchard->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mAgriculturalSoilOrchard->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mAgriculturalSoilOrchard->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mAgriculturalSoilOrchard->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mAgriculturalSoilOrchard->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mAgriculturalSoilOrchard->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mAgriculturalSoilOrchard->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mAgriculturalSoilOrchard->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mAgriculturalSoilOrchard->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mAgriculturalSoilOrchard->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mAgriculturalSoilOrchard->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mAgriculturalSoilOrchard->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mAgriculturalSoilOrchard->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mAgriculturalSoilOrchard->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mAgriculturalSoilOrchard->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mAgriculturalSoilOrchard->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mAgriculturalSoilOrchard->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mAgriculturalSoilOrchard->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mAgriculturalSoilOrchard->insert(2017, stoi(column22));
}

void System::fillAGrassArea(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mAgriculturalSoilGrassArea->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mAgriculturalSoilGrassArea->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mAgriculturalSoilGrassArea->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mAgriculturalSoilGrassArea->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mAgriculturalSoilGrassArea->insert(2017, stoi(column22));
}

void System::fillNTotal(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mNonAgriculturalSoilAll->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mNonAgriculturalSoilAll->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mNonAgriculturalSoilAll->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mNonAgriculturalSoilAll->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mNonAgriculturalSoilAll->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mNonAgriculturalSoilAll->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mNonAgriculturalSoilAll->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mNonAgriculturalSoilAll->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mNonAgriculturalSoilAll->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mNonAgriculturalSoilAll->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mNonAgriculturalSoilAll->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mNonAgriculturalSoilAll->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mNonAgriculturalSoilAll->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mNonAgriculturalSoilAll->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mNonAgriculturalSoilAll->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mNonAgriculturalSoilAll->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mNonAgriculturalSoilAll->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mNonAgriculturalSoilAll->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mNonAgriculturalSoilAll->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mNonAgriculturalSoilAll->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mNonAgriculturalSoilAll->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mNonAgriculturalSoilAll->insert(2017, stoi(column22));
}

void System::fillNForest(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mNonAgriculturalSoilForestArea->insert(2017, stoi(column22));
}

void System::fillNWater(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mNonAgriculturalSoilWaterArea->insert(2017, stoi(column22));
}

void System::fillNBuildUp(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mNonAgriculturalSoilBuildUpArea->insert(2017, stoi(column22));
}

void System::fillNOther(CityWithSoil * newCity, ifstream & file)
{
	string column1;
	string column2;
	string column3;
	string column4;
	string column5;
	string column6;
	string column7;
	string column8;
	string column9;
	string column10;
	string column11;
	string column12;
	string column13;
	string column14;
	string column15;
	string column16;
	string column17;
	string column18;
	string column19;
	string column20;
	string column21;
	string column22;
	getline(file, column1, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(1996, stoi(column1));
	getline(file, column2, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(1997, stoi(column2));
	getline(file, column3, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(1998, stoi(column3));
	getline(file, column4, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(1999, stoi(column4));
	getline(file, column5, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2000, stoi(column5));
	getline(file, column6, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2001, stoi(column6));
	getline(file, column7, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2002, stoi(column7));
	getline(file, column8, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2003, stoi(column8));
	getline(file, column9, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2004, stoi(column9));
	getline(file, column10, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2005, stoi(column10));
	getline(file, column11, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2006, stoi(column11));
	getline(file, column12, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2007, stoi(column12));
	getline(file, column13, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2008, stoi(column13));
	getline(file, column14, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2009, stoi(column14));
	getline(file, column15, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2010, stoi(column15));
	getline(file, column16, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2011, stoi(column16));
	getline(file, column17, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2012, stoi(column17));
	getline(file, column18, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2013, stoi(column18));
	getline(file, column19, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2014, stoi(column19));
	getline(file, column20, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2015, stoi(column20));
	getline(file, column21, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2016, stoi(column21));
	getline(file, column22, ';');
	newCity->mNonAgriculturalSoilOtherArea->insert(2017, stoi(column22));
}

double System::calculateRatio(double higherNumber, double lowerNumber)
{
	double result = (lowerNumber * 100) / higherNumber;
	return result;
}

void System::loadDistricts()
{
	ifstream regionsFile("GroundingSR.csv");
	if (!regionsFile.is_open()) {
		cout << "ERROR WHILE OPENING FILE" << endl;
	}
	else
	{
		// nacitavaj
		string cityCode;
		string cityName;
		string districtCode;
		string districtName;
		string regionCode;
		string regionName;
		bool iterateFile = true;
		bool firstLine = true;
		int count = 1;
		while (iterateFile)
		{
			getline(regionsFile, cityCode, ';');
			if (firstLine) {
				cityCode.erase(0, 3);
				firstLine = false;
			}
			else
			{
				cityCode.erase(0, 1);
			}

			getline(regionsFile, cityName, ';');
			cityName.erase(0, 4);
			getline(regionsFile, districtCode, ';');
			getline(regionsFile, districtName, ';');
			getline(regionsFile, regionCode, ';');
			getline(regionsFile, regionName, ';');
			CityGrounding *newCity = new CityGrounding();
			DistrictData *newData = new DistrictData();
			if (mCitiesWithRegions->containsKey(cityName)) {
				cityName = cityName + "->" + districtName;
			}
			try
			{
				mDistricts->insert(districtName, newData);
				mUnsortedDistricts->insert(districtName, newData);
			}
			catch (const std::exception&)
			{
				delete newData;
				newData = nullptr;
			}
			try
			{
				cout << cityName << endl;
				newCity->mCityCode = stoi(cityCode);
				//newCity->mName = cityName;
				newCity->mDistrictCode = stoi(districtCode);
				newCity->mDistrictName = districtName;
				newCity->mRegionCode = stoi(regionCode);
				newCity->mRegionName = regionName;
				mCitiesWithRegions->insert(cityName, newCity);
			}
			catch (const std::exception&)
			{
				cout << "City with that name/key is already in table." << endl;
				cout << cityName << endl;
				delete newCity;
			}
			if (cityName == "Zilina") {
				iterateFile = false;
			}
			//count++; // v podstate reprezentuje cislo riadku ktory nacitavam
			//if (count == 90) {
			//	//iterateFile = false;
			//	actualTable = mDistrictBB;
			//}
			//else if (count == 606) {
			//	actualTable = mDistrictKE;
			//}
			//else if (count == 1067) {
			//	actualTable = mDistrictNR;
			//}
			//else if (count == 1421) {
			//	actualTable = mDistrictPO;
			//}
			//else if (count == 2086) {
			//	actualTable = mDistrictTN;
			//}
			//else if (count == 2362) {
			//	actualTable = mDistrictTT;
			//}
			//else if (count == 2613) {
			//	actualTable = mDistrictZA;
			//}
			//else if (cityName == "Zilina" || count == 2973) {
				//iterateFile = false;
				//actualTable = nullptr;
			//}
		}
	}
}
