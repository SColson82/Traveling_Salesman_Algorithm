/*
     Title:     
     Author:    
     Date:      
     Purpose:   Traveling Salesman - find the lowest cost
                tour when traveling from US to 8 other countries
                and then back to US.
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include "GraphMatrix.h"
using namespace std;

const int SIZE = 10;
const string COUNTRY_CODES[SIZE]={"AU", "BR", "CA", "CN", "GL", "IT", "NA", "RU", "US", "US"};

/*
	Use the following structure to store each tour & its cost
	You will create an array of Tour variables in your program
*/
struct Tour
{
	string tour[SIZE];
	int cost;
};

//Function Prototypes
int searchCountryCode(string);
GraphMatrix* readFileMakeMatrix();
void printStringArray(string* arr, int size);

int main()
{	
	Tour *tourOptions = new Tour[40320];
	//read in the flight information from the file and then create the weight matrix
	GraphMatrix * matrix = readFileMakeMatrix();
	string * countries = new string[SIZE-2];
	
	cout << "\n\n*************************COUNTRIES*******************\n";
	for(int x=0; x < SIZE-2; x++)
	{
		countries[x] = COUNTRY_CODES[x];
		cout << countries[x] << endl;
	}
	
	
	//generate all possible tours (starting & ending with "US") using lexicographic permute algorithm
	//you will need to call your lexicographic function, sending the modified countries array with the 8 country codes
	
	cout << "\n\n*************************SOLUTION*******************\n";
	//find the lowest cost tour and print it out (including the cost)
	
	
	cout << "\nHappy Traveling!\n";
	
	//don't forget to release anything that was dynamically allocated!
	
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
int searchCountryCode(string)
{

}

/*
	Title: readFileMakeMatrix
	Purpose:  this function will read the data in flights.txt and create an
	adjacency matrix with the couuntry codes.  The matrix values will be the
	cost of the flights.
*/
GraphMatrix* readFileMakeMatrix()
{
	ifstream inFile;
	GraphMatrix * matrix = new GraphMatrix(SIZE-1);
	cout << "\nCreated the matrix.";
	string country1, country2;
	int price;
	inFile.open("flights.txt");
	cout << "\nReading from flights.txt\n";
	if(inFile)
	{
		while(inFile >> country1)
		{
			inFile >> country2;
			inFile >> price;
			//add price to graph matrix
			matrix->addEdge(searchCountryCode(country1), searchCountryCode(country2), price);
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
void printStringArray(string* arr, int size)
{
	for(int x=0; x<size; x++)
	{
		cout << arr[x] << " ";
	}
	cout << endl;
}
