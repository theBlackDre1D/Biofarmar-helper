#pragma once
#include "structures\table\unsorted_sequence_table.h"
#include "structures\table\sorted_sequence_table.h"
#include "structures\table\binary_search_tree.h"
#include "PopulationData.h"
#include "structures\list\linked_list.h"
#include "CityWithSoil.h"
#include "CityGrounding.h"
#include "DistrictData.h"

#include <string>

using namespace structures;
using namespace std;

class System
{
public:
	System();
	~System();

	BinarySearchTree<string, PopulationData*> *mPopulationInformation;
	UnsortedSequenceTable<string, CityWithSoil*> *mCitiesWithSoilInformations;
	SortedSequenceTable<string, CityGrounding*> *mCitiesWithRegions;
	SortedSequenceTable<string, DistrictData*> *mDistricts;
	UnsortedSequenceTable<string, DistrictData*> *mUnsortedDistricts;

	void loadDataResidentsInCities();
	void loadDataSoilInformations();
	void loadDataRegions();
	void showCityInformationsInRange(string cityName, int yearSince, int yearTo);
	void orderCitiesByTotalArea(int year);
	void orderCitiesByIncrementBuildUpAreaToTotalArea(int year);
	void orderCitiesByDifferenceBetweenTotalAndAgriculturalArea(int firstYear, int secondYear);
	void districtWithMostAndLeastTotalAreaInYear(int year);
	void districtWithBestAndWorstRatioTotal_VS_Agricular(int year);
	void districtWithBestAndWorstRatioTotal_VS_Forest(int year);

	void sortDistrictByDifferenceOrchardAndTotalArea(int firstYear, int secondYear);
	void sortDistrictByDifferenceAgricularAndTotalArea(int firstYear, int secondYear);
	void sortDistrictByDifferenceForestAndTotalArea(int firstYear, int secondYear);


	void showCities();
	void testSort();

	//sorting
	static int compareTotalArea(CityWithSoil* const& city1, CityWithSoil* const& city2, int year);
	static int compareIncreasment(CityWithSoil* const& city1, CityWithSoil* const& city2, int year);
	static int compareDifference(CityWithSoil* const& city1, CityWithSoil* const& city2, int year1, int year2);
	static int compareByRatioTotalVSOther(DistrictData* const& district1, DistrictData* const& district2);

private:
	
	int decideRowName(string row);

	void fillATotal(CityWithSoil* newCity, ifstream& file);
	void fillACropLand(CityWithSoil* newCity, ifstream& file);
	void fillAHop(CityWithSoil* newCity, ifstream& file);
	void fillAVineyard(CityWithSoil* newCity, ifstream& file);
	void fillAGarden(CityWithSoil* newCity, ifstream& file);
	void fillAOrchard(CityWithSoil* newCity, ifstream& file);
	void fillAGrassArea(CityWithSoil* newCity, ifstream& file);
	void fillNTotal(CityWithSoil* newCity, ifstream& file);
	void fillNForest(CityWithSoil* newCity, ifstream& file);
	void fillNWater(CityWithSoil* newCity, ifstream& file);
	void fillNBuildUp(CityWithSoil* newCity, ifstream& file);
	void fillNOther(CityWithSoil* newCity, ifstream& file);

	double calculateRatio(double firstNumber, double secondNumber);
	// loading regions
	void loadDistricts();
};

