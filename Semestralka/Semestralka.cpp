#include "stdafx.h"
#include "structures\heap_monitor.h"
#include "structures\table\unsorted_sequence_table.h"
#include "System.h"



#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace structures;
using namespace std;

int main()
{
	initHeapMonitor();

	System *system = new System();
	//system->loadDataResidentsInCities();
	system->loadDataSoilInformations();
	system->loadDataRegions();
	//system->showCities();

	cout << "1 - show city information in year range." << endl
		<< "2 - order cities by total area in year." << endl
		<< "3 - order cities by increment of build up area to total area in year." << endl
		<< "4 - order cities by difference change ratio total area vs agricular area between two years." << endl
		<< "5 - show district with max and min total area in year." << endl
		<< "6 - show district with max and min ratio total VS agricular area." << endl
		<< "7 - show district with max and min ratio total VS forest area." << endl
		<< "8 - sort district by difference between orchard area and total area between two years." << endl
		<< "9 - sort district by difference between agricular area and total area between two years." << endl
		<< "10 - - sort district by difference between forest area and total area between two years." <<
		endl;
		
	bool loopCondition = true;
	while (loopCondition) {
		int input;
		cout << "Insert number acording to desired function: ";
		cin >> input;
		switch (input) {
			case 333: { // testing case
				system->testSort();
				break;
			}
			case 1: 
			{
				cout << "Enter name of city you want to show: " << endl;
				string cityName;
				cin >> cityName;

				cout << "Enter year SINCE(included): " << endl;
				int yearSince;
				cin >> yearSince;

				cout << "Enter year TO(included): " << endl;
				int yearTo;
				cin >> yearTo;

				system->showCityInformationsInRange(cityName, yearSince, yearTo);
				break;
			}
			case 2: {
				cout << "Enter the year: ";
				int year;
				cin >> year;
				system->orderCitiesByTotalArea(year);
				break;
			}
			case 3: {
				cout << "Enter the year: ";
				int year;
				cin >> year;
				system->orderCitiesByIncrementBuildUpAreaToTotalArea(year);
				break;
			}
			case 4: {
				cout << "Enter the first year: ";
				int firstYear;
				cin >> firstYear;

				cout << "Enter the second year: ";
				int secondYear;
				cin >> secondYear;
				system->orderCitiesByDifferenceBetweenTotalAndAgriculturalArea(firstYear, secondYear);
				break;
			}
			case 5: {
				cout << "Enter the year: ";
				int year;
				cin >> year;
				system->districtWithMostAndLeastTotalAreaInYear(year);
				break;
			}
			case 6: {
				cout << "Enter the year: ";
				int year;
				cin >> year;
				system->districtWithBestAndWorstRatioTotal_VS_Agricular(year);
				break;
			}
			case 7: {
				cout << "Enter the year: ";
				int year;
				cin >> year;
				system->districtWithBestAndWorstRatioTotal_VS_Forest(year);
				break;
			}
			case 8: {
				cout << "Enter the first year: ";
				int firstYear;
				cin >> firstYear;

				cout << "Enter the second year: ";
				int secondYear;
				cin >> secondYear;
				system->sortDistrictByDifferenceOrchardAndTotalArea(firstYear, secondYear);
				break;
			}
			case 9: {
				cout << "Enter the first year: ";
				int firstYear;
				cin >> firstYear;

				cout << "Enter the second year: ";
				int secondYear;
				cin >> secondYear;
				system->sortDistrictByDifferenceAgricularAndTotalArea(firstYear, secondYear);
				break;
			}
			case 10: {
				cout << "Enter the first year: ";
				int firstYear;
				cin >> firstYear;

				cout << "Enter the second year: ";
				int secondYear;
				cin >> secondYear;
				system->sortDistrictByDifferenceForestAndTotalArea(firstYear, secondYear);
				break;
			}
			case 112: {
				loopCondition = false;
				break;
			}
			default: {
				cout << "You entered wrong number of function" << endl;
				break;
			}
		}
	}

	delete system;
	system = nullptr;
}


///*
//* Quick Sort Algorithm
//* Language: C++
//* Created by: Harish R
//*/
//
//#include<iostream>
//
//using namespace std;
//
//int partition(int *a, int m, int n)
//{
//	int i, j, pindex, pivot;
//	pindex = m;
//	pivot = a[n];
//	for (i = m; i<n; i++)
//	{
//		if (a[i] <= pivot)
//		{
//			swap(a[pindex], a[i]);
//			pindex++;
//		}
//	}
//	swap(a[pindex], a[n]);
//	return pindex;
//}
//
//int quicksort(int *a, int m, int n)
//{
//	int index;
//	if (m >= n)
//		return 0;
//	{
//		index = partition(a, m, n);
//		quicksort(a, m, index - 1);
//		quicksort(a, index + 1, n);
//	}
//}
//
//int main()
//{
//	int a[] = { 7,2,1,6,8,5,3,4 };
//	int i;
//	quicksort(a, 0, 7);
//	cout << "After Sorting" << endl;
//	for (i = 0; i<8; i++)
//		cout << a[i] << endl;
//
//	cout << "End" << endl;
//}

