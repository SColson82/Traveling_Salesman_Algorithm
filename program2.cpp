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

// Customer header file
#include "GraphMatrix.h"
using namespace std;

// Constant size for country permutations
const int SIZE = 10;
// Initialize array of country codes
const string COUNTRY_CODES[SIZE] = {"AU", "BR", "CA", "CN", "GL", "IT", "NA", "RU", "US", "US"};

/*
	Structure to store each tour & its cost
	An array of Tour variables will be created later
*/
struct Tour
{	
	// Array of string to store the tour
	string tour[SIZE];
	// Stores the cost of the tour
	int cost;
};

// Function Prototypes 
// Search for given country code
int searchCountryCode(string);
// Reads text file and creates matrix
GraphMatrix *readFileMakeMatrix();
// Prints string array
void printStringArray(string *arr, int size);
// Generates country permutations in lexicographic order
void lexicographicCountryPermute(string *countries, int size, Tour *tourOptions, GraphMatrix *matrix);
// Saves specific tour
void saveTour(Tour *tourOptions, string *tour, int cost, int &currentIndex);
// Finds the lowest cost tour
void findLowest(Tour *tourOptions);

// Main function
int main()
{	
	// Creates an array of tour objects
	Tour *tourOptions = new Tour[40320];
	// Reads in the flight information from the file and then creates the weight matrix
	GraphMatrix *matrix = readFileMakeMatrix();

	// Creates array to store country codes
	string *countries = new string[SIZE - 2];

	// Formatting for readability
	cout << "\n\n*************************COUNTRIES*******************\n";
	
	for (int x = 0; x < SIZE - 2; x++)
	{	// Populate the countries array
		countries[x] = COUNTRY_CODES[x];
		// Print country codes
		cout << countries[x] << endl;
	}

	// generate all possible tours (starting & ending with "US") using lexicographic permute algorithm
	lexicographicCountryPermute(countries, SIZE - 2, tourOptions, matrix);

	// Formatting for readability
	cout << "*************************SOLUTION*******************\n";

	// find the lowest cost tour and print it out (including the cost)
	findLowest(tourOptions);
	
	// Print statement for user experience
	cout << "\nHappy Traveling!\n";

	// Releases objects that have been dynamically allocated
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
	int left = 0;
	// Set the ending search parameter
	int right = SIZE - 1; // Excluding US and we only
	// want to search n - 1.
	while (left <= right)
	{ // find the mid-point
		int middle = left + (right - left) / 2;

		// Check to verify if the mid-point is
		// the country code we are looking for.
		if (country == COUNTRY_CODES[middle])
		{
			// If so, return that value
			return middle;
		}
		// Else determine if we need to look in the
		// left or right half
		else if (country < COUNTRY_CODES[middle])
		{
			// If we need to search the left we reset
			// the ending search parameter
			right = middle - 1;
		}
		else
		{
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
	Returns: a pointer to a GraphMatrix object. 
*/
GraphMatrix *readFileMakeMatrix()
{	
	// Declare input file stream
	ifstream inFile;
	// Create object with SIZE - 1 vertices
	GraphMatrix *matrix = new GraphMatrix(SIZE - 1);
	// Message confirming creation to user
	cout << "\nCreated the matrix.";
	string country1, country2;
	int price;

	// Opens flights.txt for reading
	inFile.open("flights.txt");
	// Confirms file is being read to user
	cout << "\nReading from flights.txt\n";
	if (inFile)
	{
		while (inFile >> country1)
		{
			// Reads second country code
			inFile >> country2;
			// Reads price of flight
			inFile >> price;

			// add price to graph matrix (edge)
			matrix->addEdge(searchCountryCode(country1), searchCountryCode(country2), price);
			// Confirm to user that edge has been added
			cout << "\nAdded edge from " << country1 << " to " << country2 << " with cost of $" << price;
		}
	}
	else
		// Error statement in the event file is unable to be opened
		cout << "\nSorry, I am unable to open the file.\n";
	// Close file
	inFile.close();
	// Header
	cout << "\n\nFLIGHT PRICE GRAPH MATRIX\n";
	// Prints matrix to screen
	matrix->printGraph();
	cout << endl;
	// Returns pointer to matrix
	return matrix;
}

/*
	Title: printStringArray
	Purpose:  this function will print the array of strings with a space
		between each string
	Args: Pointer to an array of strings, size of the array.
*/
void printStringArray(string *arr, int size)
{
	for (int x = 1; x < size - 1; x++)
	{
		cout << arr[x] << " ";
	}
}

/*
	Title: lexicographicCountryPermute
	Purpose: generate all country permutations using the lexicographic permutation algorithm
	Args:
		Pointer to an array of strings.
		Integer size of the array.
		Pointer to an array of Tour structures.
		Pointer to GraphMatrix object.
*/
void lexicographicCountryPermute(string *countries, int size, Tour *tourOptions, GraphMatrix *matrix)
{
	// Header for readability
	cout << "\n\nLEXICOGRAPHIC ALGORITHM\n\n";
	// Set start point for storing indices
	int currentIndex = 0;
	// Create an array to store the current tour
	string *currentTour = new string[SIZE];
	// Set first element to US
	currentTour[0] = "US";
	// Set final element to US
	currentTour[SIZE - 1] = "US";
	
	for (int i = 1; i <= size; i++)
	{	// Initialize current country codes
		currentTour[i] = countries[i - 1];
	}
	// Set flag to stop loop when all permutations 
	// have been generated
	bool isDone = false;
	// Initialize counter
	int permutationCount = 0;

	while (!isDone)
	{	// Formatting to start permutation readout at 1.
		cout << permutationCount + 1 << ": ";
		// Print current tour
		printStringArray(currentTour, SIZE);
		// Set cost to 0
		int cost = 0;
		for (int i = 0; i <= size; i++)
		{	// Get starting country
			int fromIndex = searchCountryCode(currentTour[i]);
			// Get ending country
			int toIndex = searchCountryCode(currentTour[i + 1]);
			// Get cost of the flight
			int flightCost = matrix->getWeight(fromIndex, toIndex);
			// Display flightCost
			cout << flightCost << " ";
			// Calculate cost
			cost += matrix->getWeight(fromIndex, toIndex);
		}
		// Save current tour
		saveTour(tourOptions, currentTour, cost, currentIndex);
		// Declare the total tour cost.
		cout << " has tour weight: " << cost << endl;
		// Increment counter
		permutationCount++;
		// Initialize variable for swapping
		int k = -1;
		for (int i = 1; i < size; i++)
		{
			// Find largest index where value is less than the next value
			if (currentTour[i] < currentTour[i + 1])
				// Set k to that index
				k = i;
		}
		// If k is still -1 then all permutations have been generated.
		if (k == -1)
		{	// Set exit flag
			isDone = true;
		}
		else
		{	// Initialize another swapping variable
			int l = -1;
			for (int i = 1; i <= size; i++)
			{	// Find the value that needs to be swapped with the above.
				if (currentTour[k] < currentTour[i])
					// Set l to that index
					l = i;
			}
			// Make the swap
			swap(currentTour[k], currentTour[l]);

			for (int i = k + 1, j = size; i < j; i++, j--)
			{	// Reverse the order of elements from k+1 to the end.
				swap(currentTour[i], currentTour[j]);
			}
		}
	}
	cout << endl
		 << endl;
	// Deallocate memory for current tour array
	delete[] currentTour;
}
/*
	Title: saveTour
	Purpose: Save a tour and its cost into the tourOptions array.
	Args:
		Pointer to an array of Tour structures.
		Pointer to an array of strings for the current tour.
		Integer for the cost of the current tour.
		Reference to an integer representing the current index in the tourOptions array.
*/
void saveTour(Tour *tourOptions, string *tour, int cost, int &currentIndex)
{	// Store cost of the current tour in the tourOptions array
	tourOptions[currentIndex].cost = cost; 
	for (int i = 0; i < SIZE; i++)
	{	// Copy the elements of the current tour into the tourOptions array
		tourOptions[currentIndex].tour[i] = tour[i];
	}
	// Increment currentIndex for the next tour to be saved.
	currentIndex++;
}
/*
	Title: findLowest
	Purpose: Tracks the tour with the current lowest cost and displays it.
	Args:
		Pointer to an array of Tour structures.
*/
void findLowest(Tour *tourOptions)
{	// Initialize lowestCost variable with the cost of the fist tour
	int lowestCost = tourOptions[0].cost;
	// Variable to store the index of the current lowest cost.
	int index = 0;
	// Loop through all tours
	for (int i = 1; i < 40320; i++)
	{	// Check if cost of current tour is lower than has been previously saved.
		if (tourOptions[i].cost < lowestCost)
		{	// Update with new lower cost.
			lowestCost = tourOptions[i].cost;
			// Update the index
			index = i;
			// Print the information for the current lowest cost tour.
			cout << "New Lowest is: " << lowestCost << " with tour US ";
			printStringArray(tourOptions[index].tour, SIZE);
			cout << "US" << endl;
		}
	}
	// Print statement for the final lowest cost tour. 
	cout << "\n\n***************************************************************" << endl;
	cout << "The tour with the lowest cost of $" << tourOptions[index].cost << " is US ";
	printStringArray(tourOptions[index].tour, SIZE);
	cout << "US";
}