/*
	 Title:     program2.cpp
	 Author:    Sharon Colson
	 Date:      November 2023
	 Purpose:   Traveling Salesman - find the lowest cost
				tour when traveling from US to 8 other countries
				and then back to US.
*/

// Library dependencies
#include <iostream>
#include <fstream>
#include <cmath>

#include "GraphMatrix.h"
using namespace std;

// Array of constant size for country permutations
const int SIZE = 10;
const string COUNTRY_CODES[SIZE] = {"AU", "BR", "CA", "CN", "GL", "IT", "NA", "RU", "US", "US"};

/*
	Use the following structure to store each tour & its cost
	You will create an array of Tour variables in your program
*/
struct Tour
{
	string tour[SIZE];
	int cost;
};

// Function Prototypes
int searchCountryCode(string);
GraphMatrix *readFileMakeMatrix();
void printStringArray(string *arr, int size);
void lexicographicCountryPermute(string *countries, int size,Tour *tourOptions, GraphMatrix* matrix);
void saveTour(Tour* tourOptions, string* tour, int cost, int& currentIndex);
void findLowest(Tour* tourOptions);

int main()
{
	Tour *tourOptions = new Tour[40320];
	// read in the flight information from the file and then create the weight matrix
	GraphMatrix * matrix = readFileMakeMatrix();

	string *countries = new string[SIZE - 2];

	cout << "\n\n*************************COUNTRIES*******************\n";
	for (int x = 0; x < SIZE - 2; x++)
	{
		countries[x] = COUNTRY_CODES[x];
		cout << countries[x] << endl;
	}

	// generate all possible tours (starting & ending with "US") using lexicographic permute algorithm
	// you will need to call your lexicographic function, sending the modified countries array with the 8 country codes
	// lexicographicCountryPermute(countries, SIZE - 2, tourOptions, matrix);

	

	cout << "\n\n*************************SOLUTION*******************\n";
	lexicographicCountryPermute(countries, SIZE - 2, tourOptions, matrix);
	// find the lowest cost tour and print it out (including the cost)
	findLowest(tourOptions);
	cout << "\nHappy Traveling!\n";

	// don't forget to release anything that was dynamically allocated!
	delete[] countries;
	delete[] tourOptions;
	delete matrix;

	return 0;
}
/*
	Function: searchCountryCode
	Parameters: a string with the country code like "BR"
	Returns: an integer representing this country's index in the GraphMatrix.
	For example, if the parameter is "BR", it should return index 1.  If the parameter is "CA", it should return index 3
	It is returning the index value in the COUNTRY_CODES array.
	Implement this search function with the binary search algorithm!
*/
int searchCountryCode(string country)
{
	// Set the starting search parameter
	//cout << "At line SearchCountryCode: 83."<< endl;
	int left = 0;
	// Set the ending search parameter
	int right = SIZE - 1; // Excluding US and we only
	// want to search n - 1.
	while (left <= right)
	{ // find the mid-point
		int middle = left + (right - left) / 2;
	//	cout << "middle = " << middle;
		// Check to verify if the mid-point is
		// the country code we are looking for.
		if (country == COUNTRY_CODES[middle])
		{
	//		cout << "IF";
			return middle;
		}
		// else determine if we need to look in the
		// left or right half
		else if (country < COUNTRY_CODES[middle])
			{
	//		cout << "else if";
			// If we need to search the left we reset
			// the ending search parameter
			right = middle - 1;
			}
		else
			{
	//		cout << "else";
			// If we need to search the right we
			// reset the beginning search parameter.
			left = middle + 1;
			}
	}
	return -1; // Country code not found.
}

/*
	Title: readFileMakeMatrix
	Purpose:  this function will read the data in flights.txt and create an
	adjacency matrix with the country codes.  The matrix values will be the
	cost of the flights.
*/
GraphMatrix *readFileMakeMatrix()
{
	ifstream inFile;
	GraphMatrix *matrix = new GraphMatrix(SIZE - 1);
	cout << "\nCreated the matrix.";
	string country1, country2;
	int price;
	inFile.open("flights.txt");
	cout << "\nReading from flights.txt\n";
	if (inFile)
	{
		while (inFile >> country1)
		{
			
			inFile >> country2;
			
			inFile >> price;
	//		cout << "At line 131";
			// add price to graph matrix
			matrix->addEdge(searchCountryCode(country1), searchCountryCode(country2), price);
	//		cout << "At line 134";
			cout << "\nAdded edge from " << country1 << " to " << country2 << " with cost of $" << price;
		}
	}
	else
		cout << "\nSorry, I am unable to open the file.\n";
	inFile.close();
	cout << "\n\nFLIGHT PRICE GRAPH MATRIX\n";
	matrix->printGraph();
	cout << endl;
	return matrix;
}

/*
	Title: printStringArray
	Purpose:  this function will print the array of strings with a space
		between each string
*/
void printStringArray(string *arr, int size)
{
	for (int x = 0; x < size; x++)
	{
		cout << arr[x] << " ";
	}
	// cout << endl;
}

/*
	Title: lexicographicCountryPermute
	Purpose: generate all country permutations using the lexicographic permutation algorithm
*/
void lexicographicCountryPermute(string *countries, int size, Tour *tourOptions, GraphMatrix* matrix)
{

	cout << "\n\n\nLEXICOGRAPHIC ALGORITHM\n";
	int currentIndex = 0;

	string* currentTour = new string[SIZE];

	currentTour[0]= "US";
	currentTour[SIZE - 1] = "US";

	for(int i = 1; i <= size; i++)
	{
		currentTour[i] = countries[i - 1];
	
	}

	bool isDone = false;
	int permutationCount = 0;

	while(!isDone)
	{
		int cost = 0;
		for (int i =0; i<SIZE-1; i++)
		{
			int fromIndex = searchCountryCode(currentTour[i]);
			int toIndex = searchCountryCode(currentTour[i + 1]);
			cost += matrix->getWeight(fromIndex, toIndex);
		}
		
		saveTour(tourOptions, currentTour, cost, currentIndex);

		cout << permutationCount + 1 << ": ";
		printStringArray(currentTour, SIZE);
		cout << "has tour weight: " << cost << endl;

		permutationCount++;

		int k = -1;
		for (int i = 1; i < size; i++)
		{

			if (currentTour[i] < currentTour[i+1])
				k = i;
		}
		if (k==-1)
		{
			isDone = true;
		}
		else
		{
			int l = -1;
			for (int i = 1; i <= size; i++)
			{
				if(currentTour[k]< currentTour[i])
				l = i;
			}
			swap(currentTour[k], currentTour[l]);

			for(int i = k + 1, j = size; i<j; i++, j--)
			{
				swap(currentTour[i], currentTour[j]);
			}
		}
	}
	cout << endl
		 << endl;

	delete[] currentTour;
}

void saveTour(Tour* tourOptions, string* tour, int cost, int& currentIndex)
{
	tourOptions[currentIndex].cost = cost;
	for (int i = 0; i < SIZE; i++)
	{
		tourOptions[currentIndex].tour[i]=tour[i];
	}
	currentIndex++;
}

void findLowest(Tour* tourOptions)
{
	int lowestCost = tourOptions[0].cost;
	int index = 0;
	for (int i = 1; i < 40320; i++)
	{
		if(tourOptions[i].cost < lowestCost)
		{
			lowestCost = tourOptions[i].cost;
			index = i;
		}
	}
	cout << "New Lowest is: " << lowestCost << " with tour ";
	printStringArray(tourOptions[index].tour, SIZE);
}