#include <stdio.h>
#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
//#include <pthread.h>
#include <mutex>
#include "Bitset.h"



using namespace std;

// Prototypes:
int askUserForInteger(string, const int, const int);	// asks the user to input a number

void presentMenu(void);
void printError(string, const int);

void runProblem1(const int, const int);
void makeBinFile(const int);
void makeCheckFile(void);
bool getNewValue(int i, int j, Bitset * set);
bool areFilesEqual(ifstream& file1, ifstream& file2);

bool isProblem1Done();
void* threadStart(void*);

void runProblem2(const int, const int);


// Globals:
const string s_FILENAME  = "stuff.bin";
const string s_CHECKFILE = "check.bin";
int i_unchanged;
mutex mtx;
int i_num;


int main(int argc, char **argv)
{
	// Constants:
	const int i_MAX_N       = 10000;
	const int i_MAX_THREADS = 100;
	const int i_MAX_CHOICE  = 3;
	const int i_MIN_CHOICE  = 1;
	const int i_MIN_THREADS = 1;
	const int i_MIN_N       = 2;
	
	
	// Variables:
	int  i_size       = 0;
	int  i_numThreads = 0;
	int  i_choice     = 0;

	string s_N_MSG    = "Please enter a positive integer for the size of matrix to use (2 <= n <= 10000): ";
	string s_M_MSG    = "Please enter a positive integer for the number of threads to use (1 <= m <= 100): ";
	string s_CHOICE   = "Enter your choice (1, 2 or 3): ";
	
	// Input check
	if(argc > 0)
	{
		i_size = atoi(argv[0]);

		if(i_size < 1)
		{
			i_size = askUserForInteger(s_N_MSG, i_MIN_N, i_MAX_N);\
		} // end if
	} // end if
	else
	{
		i_size = askUserForInteger(s_N_MSG, i_MIN_N, i_MAX_N);
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
			i_unchanged = 0;
			i_numThreads = askUserForInteger(s_M_MSG, i_MIN_THREADS, i_MAX_THREADS);
			i_num = i_size;
			runProblem1(i_size, i_numThreads);
		} // end elif
		else if(i_choice == 2)
		{
			i_unchanged = 0;
			i_numThreads = askUserForInteger(s_M_MSG, i_MIN_THREADS, i_MAX_THREADS);
			runProblem2(i_size, i_numThreads);
		} // end elif
		else
		{
			printError("You must chose either 1, 2 or 3.", i_choice);
		} // end else
	} // end for
	
	return EXIT_SUCCESS;
} // end method Main


void runProblem1(const int i_N, const int i_M)
{
	// Variables:
	makeBinFile(i_N);
	makeCheckFile();
	
	Bitset* set = new Bitset(s_FILENAME, i_num*i_num);

	do
	{
		/*
		pthread_t threads[i_M];
		
		for(int i = 0; i < i_M; i++)
		{
			pthread_create(&threads[i], NULL, threadStart, (void*)set);
		} // end for
		for(int i = 0; i < i_M; i++)
		{
			pthread_join(threads[i], NULL);
		} // end for
		*/

		threadStart((void*)set);

		if (i_unchanged == 100)
		{
			cout << "The file has not changed in 100 iterations. Stopping." << endl;
			break;
		} // end if

	} while(!isProblem1Done());
	
	delete set;
} // end method runProblem1


void* threadStart(void* args)
{
	Bitset * set = (Bitset*)args;
	
	int i = rand() % i_num,
		j = rand() % i_num;
	
	try
	{
		bool newValue = getNewValue(i, j, set);

		mtx.lock();
		set->setBit(i, j, newValue);
		set->writeSetToFile(s_FILENAME);
		cout << set << endl;
		mtx.unlock();
	} // end try
	catch (exception)
	{
		exit(EXIT_FAILURE);
	} // end catch
} // end method threadStart


bool getNewValue(int i, int j, Bitset * set)
{
	int max = i_num * i_num;

	int zeroes = 0,
		ones = 0;

	if (i < (i_num-1))
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

	if (j < (i_num - 1))
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

	if (i < (i_num - 1) && j < (i_num - 1))
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

	if (i > 0 && j < (i_num - 1))
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

	if (i < (i_num - 1) && j > 0)
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


void makeBinFile(const int i_N)
{
	remove(s_FILENAME.c_str());
	fstream file(s_FILENAME.c_str(), ios::out | ios::in | ios::app);
	
	int stop = ceil(static_cast<double>((i_N * i_N)/8.0));

	if(file.is_open() && !file.bad())
	{
		// populate file with random numbers
		for(int i = 0; i < stop; i++)
		{
			char temp = rand() % 255; // generate random numbers 8 at a time
			file << temp;			  // any extra will just be ignored
		} // end for
	} // end if
} // end method makeBinFile


void makeCheckFile(void)
{
	ifstream file(s_FILENAME.c_str());
	remove(s_CHECKFILE.c_str());
	ofstream check("check.bin");
	
	char temp;
	
	while(file >> temp)
	{
		check << temp;
	} // end while
} // end method makeCheckFile


void runProblem2(const int i_N, const int i_M)
{
	
} // end method runProblem2


void printError(string s_msg, const int i_choice)
{
	cout << "Error. Your input of \"" << i_choice << "\" did not work." << endl;
	cout << s_msg << endl;
} // end method printError


void presentMenu(void)
{
	cout << endl;
	cout << "1) Run problem 1" << endl;
	cout << "2) Run problem 2" << endl;
	cout << "3) Exit" << endl;
} // end method presentMenu


bool isProblem1Done()
{
	ifstream file(s_FILENAME.c_str(), ios::binary);
	ifstream check(s_CHECKFILE.c_str(), ios::binary);

	char temp1, temp2;

	bool b_unchanged = areFilesEqual(file,check);

	file.seekg(0);
	check.close();
	
	if (!b_unchanged)
	{
		Bitset* a = new Bitset(s_FILENAME, i_num*i_num);

		bool done = a->allZeroes();
		bool done1 = a->allOnes();

		if (done || done1)
		{
			cout << "Problem 1 finished." << endl;
			file.close();
			return true;
		} // end if
	} // end if

	if(!b_unchanged)
	{
		i_unchanged = 0;
		file.close();
		makeCheckFile();
		return false;
	} // endl
	else
	{
		i_unchanged++;
		file.close();
		return false;
	} // end if
} // end method isProblem1Done


bool areFilesEqual(ifstream& file1, ifstream& file2)
{
	const size_t BLOCKSIZE = 4096;
	      size_t remaining;

	ifstream::pos_type size;

	size = file1.seekg(0, ifstream::end).tellg();
	file1.seekg(0, ifstream::beg);
		
	remaining = size;


	while (remaining)
	{
		char buffer1[BLOCKSIZE], 
			 buffer2[BLOCKSIZE];
		size_t buffer_size = min(BLOCKSIZE, remaining);

		file1.read(buffer1, buffer_size);
		file2.read(buffer2, buffer_size);

		if (0 != memcmp(buffer1, buffer2, buffer_size))
		{
			return false;
		} // end if

		remaining -= buffer_size;
	} // end while

	return true;
} // end method areFilesEqual                         


int askUserForInteger(string s_MSG, const int i_MIN, const int i_MAX)
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
			cout << "Invalid input. Try again." << endl;
			cin.clear();
		} // end if
	} // end for
} // end method askUserForSize