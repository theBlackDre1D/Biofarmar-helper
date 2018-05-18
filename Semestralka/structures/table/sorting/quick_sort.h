#pragma once

#include "sort.h"
#include "../unsorted_sequence_table.h"

namespace structures
{

	/// <summary> Triedenie Quick sort. </summary>
	/// <typeparam name = "K"> Kluc prvkov v tabulke. </typepram>
	/// <typeparam name = "T"> Typ dat ukladanych v tabulke. </typepram>
	template <typename K, typename T>
	class QuickSort : public Sort<K, T>
	{
	public:
		/// <summary> Utriedi tabulku triedenim Quick sort. </summary>
		/// <param name = "table"> NonortedSequenceTable, ktoru ma utriedit. </param>
		QuickSort();
		QuickSort(int(*cmp)(const T &item1, const T &item2, int year));
		QuickSort(int(*cmp)(const T &item1, const T &item2, int year1, int year2));
		QuickSort(int(*cmp)(const T &item1, const T &item2));

		void sort(UnsortedSequenceTable<K, T>& table) override;
		void sortTotalAreaASC(UnsortedSequenceTable<K, T>& table, int year);
		void sortTotalAreaDESC(UnsortedSequenceTable<K, T>& table, int year);

		void sortIncrementTotal_vs_BuildASC(UnsortedSequenceTable<K, T>& table, int year);
		void sortIncrementTotal_vs_BuildDESC(UnsortedSequenceTable<K, T>& table, int year);

		void sortDifferenceTotal_vs_AgricularASC(UnsortedSequenceTable<K, T>& table, int year1, int year2);
		void sortDifferenceTotal_vs_AgricularDESC(UnsortedSequenceTable<K, T>& table, int year1, int year2);

		void sortDistrictsASC(UnsortedSequenceTable<K, T>& table);
		void sortDistrictsDESC(UnsortedSequenceTable<K, T>& table);

	private:
		void quickTotalASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year);
		void quickTotalDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year);

		void quickIncrementASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year);
		void quickIncrementDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year);

		void quickDifferenceASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year1, int year2);
		void quickDifferencetDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year1, int year2);
		//void quick(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int(*cmp)(const T& item1, const T& item2));

		void quickDistrictsASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex);
		void quickDistrictsDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex);


		int (*cmpFun_)(const T& item1, const T& item2, int year);
		int (*cmpFun2_)(const T& item1, const T& item2, int year1, int year2);
		int (*cmpFun3_)(const T& item1, const T& item2);
	};

	template<typename K, typename T>
	inline void QuickSort<K, T>::sort(UnsortedSequenceTable<K, T>& table)
	{
		//quick(table, 0, table.size() -1);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortTotalAreaASC(UnsortedSequenceTable<K, T>& table, int year)
	{
		quickTotalASC(table, 0, table.size() - 1, year);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortTotalAreaDESC(UnsortedSequenceTable<K, T>& table, int year)
	{
		quickTotalDESC(table, 0, table.size() - 1, year);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortIncrementTotal_vs_BuildASC(UnsortedSequenceTable<K, T>& table, int year)
	{
		quickIncrementASC(table, 0, table.size() - 1, year);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortIncrementTotal_vs_BuildDESC(UnsortedSequenceTable<K, T>& table, int year)
	{
		quickIncrementDESC(table, 0, table.size() - 1, year);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortDifferenceTotal_vs_AgricularASC(UnsortedSequenceTable<K, T>& table, int year1, int year2)
	{
		quickDifferenceASC(table, 0, table.size() - 1, year1, year2);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortDifferenceTotal_vs_AgricularDESC(UnsortedSequenceTable<K, T>& table, int year1, int year2)
	{
		quickDifferencetDESC(table, 0, table.size() - 1, year1, year2);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortDistrictsASC(UnsortedSequenceTable<K, T>& table)
	{
		quickDistrictsASC(table, 0, table.size() - 1);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::sortDistrictsDESC(UnsortedSequenceTable<K, T>& table)
	{
		quickDistrictsDESC(table, 0, table.size() - 1);
	}


	template<typename K, typename T>
	inline QuickSort<K, T>::QuickSort(int(*cmp)(const T &item1, const T &item2, int year)) :
		cmpFun_(cmp) 
	{
	}

	template<typename K, typename T>
	inline QuickSort<K, T>::QuickSort(int(*cmp)(const T &item1, const T &item2, int year1, int year2)) :
		cmpFun2_(cmp)
	{
	}

	template<typename K, typename T>
	inline QuickSort<K, T>::QuickSort(int(*cmp)(const T &item1, const T &item2)) :
		cmpFun3_(cmp)
	{
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::quickTotalASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year) {
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun_(table.getItemAtIndex(left).accessData(), pivot, year) < 0) { //ascending
				left++;
			}
			while (cmpFun_(table.getItemAtIndex(right).accessData(), pivot, year) > 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickTotalASC(table, startIndex, right, year);
		if (left < endIndex)
			quickTotalASC(table, left, endIndex, year);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::quickTotalDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year)
	{
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun_(table.getItemAtIndex(left).accessData(), pivot, year) > 0) { //descending
				left++;
			}
			while (cmpFun_(table.getItemAtIndex(right).accessData(), pivot, year) < 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickTotalDESC(table, startIndex, right, year);
		if (left < endIndex)
			quickTotalDESC(table, left, endIndex, year);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::quickIncrementASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year)
	{
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun_(table.getItemAtIndex(left).accessData(), pivot, year) < 0) { //ascending
				left++;
			}
			while (cmpFun_(table.getItemAtIndex(right).accessData(), pivot, year) > 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickIncrementASC(table, startIndex, right, year);
		if (left < endIndex)
			quickIncrementASC(table, left, endIndex, year);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::quickIncrementDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year)
	{
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun_(table.getItemAtIndex(left).accessData(), pivot, year) > 0) { //descending
				left++;
			}
			while (cmpFun_(table.getItemAtIndex(right).accessData(), pivot, year) < 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickIncrementDESC(table, startIndex, right, year);
		if (left < endIndex)
			quickIncrementDESC(table, left, endIndex, year);
	}
	template<typename K, typename T>
	inline void QuickSort<K, T>::quickDifferenceASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year1, int year2)
	{
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun2_(table.getItemAtIndex(left).accessData(), pivot, year1, year2) < 0) { //ascending
				left++;
			}
			while (cmpFun2_(table.getItemAtIndex(right).accessData(), pivot, year1, year2) > 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickDifferenceASC(table, startIndex, right, year1, year2);
		if (left < endIndex)
			quickDifferenceASC(table, left, endIndex, year1, year2);
	}
	template<typename K, typename T>
	inline void QuickSort<K, T>::quickDifferencetDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex, int year1, int year2)
	{
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun2_(table.getItemAtIndex(left).accessData(), pivot, year1, year2) > 0) { //descending
				left++;
			}
			while (cmpFun2_(table.getItemAtIndex(right).accessData(), pivot, year1, year2) < 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickDifferencetDESC(table, startIndex, right, year1, year2);
		if (left < endIndex)
			quickDifferencetDESC(table, left, endIndex, year1, year2);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::quickDistrictsASC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex)
	{
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun3_(table.getItemAtIndex(left).accessData(), pivot) < 0) { //ascending
				left++;
			}
			while (cmpFun3_(table.getItemAtIndex(right).accessData(), pivot) > 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickDistrictsASC(table, startIndex, right);
		if (left < endIndex)
			quickDistrictsASC(table, left, endIndex);
	}

	template<typename K, typename T>
	inline void QuickSort<K, T>::quickDistrictsDESC(UnsortedSequenceTable<K, T>& table, int startIndex, int endIndex)
	{
		T pivot = table.getItemAtIndex((startIndex + endIndex) / 2).accessData();
		int left = startIndex;
		int right = endIndex;
		do {
			while (cmpFun3_(table.getItemAtIndex(left).accessData(), pivot) > 0) { //descending
				left++;
			}
			while (cmpFun3_(table.getItemAtIndex(right).accessData(), pivot) < 0) {
				right--;
			}
			if (left <= right) {
				table.swap(left, right);
				left++;
				right--;
			}
			//notify();
		} while (left <= right);
		if (right > startIndex)
			quickDistrictsDESC(table, startIndex, right);
		if (left < endIndex)
			quickDistrictsDESC(table, left, endIndex);
	}
}