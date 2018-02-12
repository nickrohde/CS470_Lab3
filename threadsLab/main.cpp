#include <stdio.h>
#include <iostream>
#include <fstream>
#include <random>
#include <pthread.h>
#include <mutex>
#include <string.h>
#include <iomanip>
#include "Bitset.h"
#include "Sorting.h"

// Defines:
#ifndef _MIN
#define _MIN(a,b) (a < b? a : b)
#endif // !_MIN
#ifndef _MAX
#define _MAX(a,b) (a > b? a : b)
#endif // !_MAX

// Namespace:
using namespace std;

// Prototypes:
int askUserForInteger(string&, const int, const int);

void presentMenu(void);
void printError(string&, const int);

// Problem1:
void runProblem1(const int, const int);
void makeBinFileP1(const int);
void* threadStart(void*);

bool getNewValue(const int, const int, Bitset*);
bool isProblem1Done(void);

// Problem2:
void runProblem2(const int, const int);
void* threadStart2(void*);
void makeTextFileP2(const int, double*);
void generateRandomDoublesInRange(const double, const double, double*, const int);
void readArrayFromFile(double *);

bool isProblem2Done(double*);


// Global constants:
const string s_FILENAME  = "problem1.bin";
const string s_FILENAME2 = "problem2.txt";
const double d_RANDMAX   =  1000.0;
const double d_RANDMIN   = -1000.0;

// Global variables:
mutex mtx;
int i_N;


int main(int argc, char **argv)
{
	// Constants:
	const int i_MAX_N       = 10000;
	const int i_MAX_THREADS = 100;
	const int i_MAX_CHOICE  = 3;
	const int i_MIN_CHOICE  = 1;
	const int i_MIN_THREADS = 1;
	const int i_MIN_N       = 1;
	
	
	// Variables:
	int  i_numThreads = 0;
	int  i_choice     = 0;

	string s_N_MSG         = "Please enter a positive integer for the size of matrix to use (1 <= n <= 10000): ";
	string s_M_MSG         = "Please enter a positive integer for the number of threads to use (1 <= m <= 100): ";
	string s_CHOICE        = "Enter your choice (1, 2, or 3): ";
	string s_ERROR         = "You must chose either 1, 2, or 3.";
	
	i_N = -1;

	if(argc > 0)
	{
		i_N = atoi(argv[0]);

		if(i_N < 1)
		{
			i_N = askUserForInteger(s_N_MSG, i_MIN_N, i_MAX_N);\
		} // end if
	} // end if
	else
	{
		i_N = askUserForInteger(s_N_MSG, i_MIN_N, i_MAX_N);
	} // end else
	
	srand(unsigned(time(NULL)));
	
	for(;;)
	{
		presentMenu();
		i_choice = askUserForInteger(s_CHOICE, i_MIN_CHOICE, i_MAX_CHOICE);
		
		if(i_choice == 3)
		{
			cout << endl << "Bye." << endl;
			break;
		} // end if
		if(i_choice == 1)
		{
			i_numThreads = askUserForInteger(s_M_MSG, i_MIN_THREADS, i_MAX_THREADS);
			runProblem1(i_N, i_numThreads);
		} // end elif
		else if(i_choice == 2)
		{
			i_numThreads = askUserForInteger(s_M_MSG, i_MIN_THREADS, i_MAX_THREADS);
			runProblem2(i_N, i_numThreads);
		} // end elif
		else
		{
			printError(s_ERROR, i_choice);
		} // end else
	} // end for
	
	return EXIT_SUCCESS;
} // end method Main


void runProblem1(const int i_N, const int i_M)
{
	// Variables:
	makeBinFileP1(i_N);
	
	Bitset* set = new Bitset(s_FILENAME, i_N*i_N);
	
	do
	{
		pthread_t threads[i_M];
		
		for (int i = 0; i < i_M; i++)
		{
			pthread_create(&threads[i], NULL, threadStart, (void*)set);
		} // end for
		for(int i = 0; i < i_M; i++)
		{
			pthread_join(threads[i], NULL);
		} // end for
		
		threadStart((void*)set);

		cout << set << endl;
	} while(!isProblem1Done());

	delete set;
} // end method runProblem1


void* threadStart(void* args)
{
	Bitset * set = (Bitset*)args;
	
	int i = rand() % i_N,
		j = rand() % i_N;
	
	try
	{
		bool newValue = getNewValue(i, j, set);

		mtx.lock();
		set->setBit(i, j, newValue);
		set->writeSetToFile(s_FILENAME);
		mtx.unlock();
	} // end try
	catch (exception)
	{
		exit(EXIT_FAILURE);
	} // end catch

	return NULL;
} // end method threadStart


bool getNewValue(const int i, const int j, Bitset * set)
{
	int zeroes = 0,
		ones   = 0;

	if (i < (i_N -1))
	{
		if ((*set)(i + 1, j))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	} // end if

	if (j < (i_N - 1))
	{
		if ((*set)(i, j + 1))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	}

	if (i > 0)
	{
		if ((*set)(i - 1, j))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	}

	if (j > 0)
	{
		if ((*set)(i, j - 1))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	}

	if (i < (i_N - 1) && j < (i_N - 1))
	{
		if ((*set)(i + 1, j + 1))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	}

	if (i > 0 && j > 0)
	{
		if ((*set)(i - 1, j - 1))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	}

	if (i > 0 && j < (i_N - 1))
	{
		if ((*set)(i - 1, j + 1))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	}

	if (i < (i_N - 1) && j > 0)
	{
		if ((*set)(i + 1, j - 1))
		{
			ones++;
		} // end if
		else
		{
			zeroes++;
		} // end else
	}

	if (zeroes >= ones)
	{
		return false;
	} // end if
	else
	{
		return true;
	} // end else		

} // end method getNewValue


void makeBinFileP1(const int i_N)
{
	remove(s_FILENAME.c_str());
	fstream file(s_FILENAME.c_str(), ios::binary | ios::in | ios::app);
	
	int stop = ceil(static_cast<double>((i_N * i_N)/8.0));

	if(file.is_open() && !file.bad())
	{
		for(int i = 0; i < stop; i++)
		{
			char temp = rand() % 256; 
			file << temp;			  
		} // end for
	} // end if
} // end method makeBinFile


void makeTextFileP2(const int i_N, double* da_arr)
{	
	remove(s_FILENAME2.c_str());
	fstream file(s_FILENAME2.c_str(), ios::out | ios::app);

	if(file.is_open() && !file.bad())
	{
		for(int i = 0; i < i_N*i_N; i++)
		{
			file << std::fixed << std::setprecision(8) << da_arr[i];
			file << '\t';
		} // end for
	} // end if
} // end method makeBinFile


void runProblem2(const int i_N, const int i_M)
{
	double* da_arr = new double[i_N*i_N];

	generateRandomDoublesInRange(d_RANDMIN, d_RANDMAX, da_arr, i_N*i_N);
	makeTextFileP2(i_N, da_arr);

	do
	{
		pthread_t threads[i_M];
		readArrayFromFile(da_arr);

		for(int i = 0; i < i_M; i++)
		{
			pthread_create(&threads[i], NULL, threadStart2, (void*)da_arr);
		} // end for
		for(int i = 0; i < i_M; i++)
		{
			pthread_join(threads[i], NULL);
		} // end for

		cout << "Array: " << endl << "[";
		for (int i = 0; i < (i_N*i_N); i++)
		{
			cout << da_arr[i] << " ";
		} // end for
		cout << "]" << endl;

		makeTextFileP2(i_N, da_arr);
	} while(!isProblem2Done(da_arr));

	delete[] da_arr;
} // end method runProblem2


void readArrayFromFile(double * da_arr)
{
	ifstream file(s_FILENAME2.c_str(), ios::in);

	double temp = 0;
	int i = 0;

	while (file >> temp) //&& (i < (i_N*i_N)))
	{
		da_arr[i] = temp;
		i++;
	} // end while
} // end method readArrayFromFile


void* threadStart2(void* args)
{	
	int i   = rand() % (i_N*i_N),
		j   = rand() % (i_N*i_N),
		run = rand() % 3,
		temp = _MAX(i, j);

	i = _MIN(i, j);
	j = temp;

	double * arr = (double*)args;
	
	try
	{
		mtx.lock();
		switch(run)
		{
			case 0:
				cout << "Quick Sort is running on the interval [" << i << ", " << j << "]" << endl;
				quicksort(arr, i, j);
			break;

			case 1:
				cout << "Bubble Sort is running on the interval [" << i << ", " << j << "]" << endl;
				bubbleSort(arr, i, j);
			break;

			case 2:
				cout << "Insertion Sort is running on the interval [" << i << ", " << j << "]" << endl;
				insertionSort(arr, i, j);
			break;
		} // end switch
		mtx.unlock();		
		
	} // end try
	catch (exception)
	{
		exit(EXIT_FAILURE);
	} // end catch

	return NULL;
} // end method threadStart


void generateRandomDoublesInRange(const double d_MIN, const double d_MAX, double* da_arr, const int i_COUNT)
{
	// stolen from http://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(d_MIN, d_MAX);

	for(int i = 0; i < i_COUNT; i++)
	{
		da_arr[i] = dis(gen);
	} // end for
} // end method generateRandomDoublesInRange


void printError(string& s_msg, const int i_choice)
{
	cout << "Error. Your input of \"" << i_choice << "\" did not work." << endl;
	cout << s_msg << endl << endl;
} // end method printError


void presentMenu(void)
{
	cout << endl;
	cout << "1) Run problem 1" << endl;
	cout << "2) Run problem 2" << endl;
	cout << "3) Exit" << endl << endl;
} // end method presentMenu


bool isProblem1Done(void)
{
	ifstream file(s_FILENAME.c_str(), ios::binary);

	Bitset* a = new Bitset(s_FILENAME, i_N*i_N);

	bool done = a->allSame();

	delete a;

	if (done)
	{
		cout << "Problem 1 finished." << endl;
		file.close();
		return true;
	} // end if
	
	return false;
} // end method isProblem1Done


bool isProblem2Done(double* da_arr)
{
	for (int i = 1; i < (i_N*i_N); i++)
	{
		if (da_arr[i - 1] > da_arr[i])
		{
			return false;
		} // end if
	} // end for

	return true;
} // end method isProblem2Done                     


int askUserForInteger(string& s_MSG, const int i_MIN, const int i_MAX)
{
	// Variables:
	int i_value = 0;
	string c;
	
	for(;;)
	{
		cout << s_MSG;
		
		getline(cin, c);
		
		i_value = atoi(c.c_str());
		
		// ensure valid input
		if(i_value >= i_MIN && i_value <= i_MAX)
		{
			return i_value;	
		} // end if
		else
		{
			cout << endl << "Invalid input. Try again." << endl;
			cout << "Input must be an integer in the range [" << i_MIN << "," << i_MAX << "]." << endl << endl; 
			cin.clear();
		} // end if
	} // end for
} // end method askUserForSize