#include <stdio.h>
#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <pthread.h>
#include <mutex>
#include "Bitset.h"



using namespace std;

// Prototypes:
int askUserForInteger(string, const int);			// asks the user to input a number

void presentMenu(void);
void printError(string, const int);

void runProblem1(const int, const int);
void populateMatrix(const int, char**);
void makeBinFile(const int);
void makeCheckFile(void);
void changeElement(const int, const int, const int, char **);

bool isProblem1Done(const int);
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
	
	
	// Variables:
	int  i_size       = 0;
	int  i_numThreads = 0;
	int  i_choice     = 0;

	string s_N_MSG    = "Please enter a positive integer for the size of matrix to use (<=10000): ";
	string s_M_MSG    = "Please enter a positive integer for the number of threads to use (<=100): ";
	string s_CHOICE   = "Enter your choice (1, 2 or 3): ";
	
	// Input check
	if(argc > 0)
	{
		i_size = atoi(argv[0]);

		if(i_size < 1)
		{
			i_size = askUserForInteger(s_N_MSG, i_MAX_N);
		} // end if
	} // end if
	else
	{
		i_size = askUserForInteger(s_N_MSG, i_MAX_N);
		i_num = i_size;
	} // end else
	
	srand(unsigned(time(NULL)));
	
	
	for(;;)
	{
		presentMenu();
		i_choice = askUserForInteger(s_CHOICE, i_MAX_CHOICE);
		
		if(i_choice == 3)
		{
			cout << endl << "Bye." << endl;
			break;
		} // end if
		if(i_choice == 1)
		{
			i_unchanged = 0;
			i_numThreads = askUserForInteger(s_M_MSG, i_MAX_THREADS);
			try
			{
				runProblem1(i_size, i_numThreads);
			} // end try
			catch(exception)
			{
				cout << "Problem 1 could not be finished because an infinite loop was encountered." << endl;
			} // end catch
		} // end elif
		else if(i_choice == 2)
		{
			i_unchanged = 0;
			i_numThreads = askUserForInteger(s_M_MSG, i_MAX_THREADS);
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
	
	Bitset* set = new Bitset(s_FILENAME);
	
	for(int i = 4; i < 10; i++)
	{
		cout << "Bit " << i << " is " << (*set)[i] << " before run." << endl;
	}

	
	int kill = 0;
	do
	{
		pthread_t threads[i_M];
		
		for(int i = 0; i < i_M; i++)
		{
			pthread_create(&threads[i], NULL, threadStart, (void*)set);
		} // end for
		
		kill++;
		
		for(int i = 0; i < i_M; i++)
		{
			pthread_join(threads[i], NULL);
		} // end for
		
	} while(kill < 1);//!isProblem1Done(i_N));//kill < 10 && !isProblem1Done(i_N));
	
	for(int i = 4; i < 10; i++)
	{
		cout << "Bit " << i << " is " << (*set)[i] << " should be 1." << endl;
	}	
	
	delete set;
} // end method runProblem1

void* threadStart(void* args)
{
	Bitset * set = (Bitset*)args;
	
	int i = rand() % i_num,
		j = rand() % i_num;
	int max = i_num * i_num;
	
	//for(int i = 0; i < set->getBytes(); i++)//cont->N; i++)
	//{
		//mtx.lock();
		//for(int j = 0; j < 8; j++)//< cont->N; j++)
		//{
			//cout << (*set)(i,j);
			//if(j == 3)
			//{
				// for k, k-n (above), k+1 (right), k-1 (left), k+n (below)
				//        (k-n) +/- 1 (diag up), (k+n) +/- 1 (diag down)
				//cout << " ";
			//}
		//}
		//cout << endl;
		//mtx.unlock();
	//}
	
	int bit = (i*i_num) + j;
	
	int above      = bit - i_num,
		below      = bit + i_num,
		right      = (j < i_num-1? bit + 1: -1),
		left       = (j > 0? bit - 1: -1),
		aboveRight = (j < i_num-1? above + 1: -1),
		aboveLeft  = (j > 0? above - 1: -1),
		belowRight = (j < i_num-1? below + 1: -1),
		belowLeft  = (j > 0? below - 1: -1);
	int zeroes     = 0,
		ones 	   = 0;
	
	for(int i = 0; i < 9; i++)
	{
		try
		{
			
		}
	}
	
	
	
	if(above >= 0)
	{
		if(set[above] == 1)
		{
			ones++;
		}
		else
		{
			zeroes++;
		}
		if(aboveRight != -1)
		{
			if(set[aboveRight] == 1)
			{
				ones++;
			}
			else
			{
				zeroes++;
			}
		}
		if(aboveLeft != -1)
		{
			if(set[aboveLeft] == 1)
			{
				ones++;
			}
			else
			{
				zeroes++;
			}
		}
	}
	if(below < max)
	{
		if(set[above] == 1)
		{
			ones++;
		}
		else
		{
			zeroes++;
		}
		if(aboveRight != -1)
		{
			if(set[aboveRight] == 1)
			{
				ones++;
			}
			else
			{
				zeroes++;
			}
		}
		if(aboveLeft != -1)
		{
			if(set[aboveLeft] == 1)
			{
				ones++;
			}
			else
			{
				zeroes++;
			}
		}
	}

	mtx.lock();
	
	mtx.unlock();
} // end method threadStart

void makeBinFile(const int i_N)
{
	remove(s_FILENAME.c_str());
	fstream file(s_FILENAME.c_str(), ios::out | ios::in | ios::app);
	
	if(file.is_open() && !file.bad())
	{
		// populate file with random numbers
		for(int i = 0; i < i_N*i_N; i++)
		{
			char temp = rand() % 255;
			file << temp;
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

void populateMatrix(const int i_N, char ** ca_matrix)
{
	char temp;
	ifstream file(s_FILENAME.c_str());
	
	if(file.is_open() && !file.bad())
	{
		int i = 0, j = 0;
		
		while(file >> temp)
		{
			ca_matrix[i][j] = temp;
			
			if(i == i_N-1 && j == i_N-1)
			{
				break;
			} // end if
			else if(j == i_N-1)
			{
				i++;
				j = 0;
			} // end elif
			else
			{
				j++;
			} // end else
		} // end while
	} // end if
} // end method populateMatrix

void changeElement(const int i_N, const int I, const int J, char ** ca_matrix)
{
	int temp = 0;
	int num  = 0;

	if(I == 0)
	{
		if(J == 0)
		{
			temp += (int)ca_matrix[I  ][J+1];
			temp += (int)ca_matrix[I+1][J  ];
			temp += (int)ca_matrix[I+1][J+1];
			if(temp >= 2)
			{
				ca_matrix[I][J] = (char)1;
			} // end if
			else
			{
				ca_matrix[I][J] = (char)0;
			} // end else
		}
		else if(J == i_N-1)
		{
			temp += (int)ca_matrix[I  ][J-1];
			temp += (int)ca_matrix[I+1][J  ];
			temp += (int)ca_matrix[I+1][J-1];
			if(temp >= 2)
			{
				ca_matrix[I][J] = (char)1;
			} // end if
			else
			{
				ca_matrix[I][J] = (char)0;
			} // end else			
		}
		else
		{
			temp += (int)ca_matrix[I  ][J-1];
			temp += (int)ca_matrix[I+1][J-1];
			temp += (int)ca_matrix[I  ][J+1];
			temp += (int)ca_matrix[I+1][J  ];
			temp += (int)ca_matrix[I+1][J+1];
			if(temp >= 2)
			{
				ca_matrix[I][J] = (char)1;
			} // end if
			else
			{
				ca_matrix[I][J] = (char)0;
			} // end else
		}
	}
	else if(I == i_N-1)
	{
		if(J == 0)
		{
			
		}
		else if(J == i_N-1)
		{
			
		}
		else
		{
			
		}
	}
	else
	{
		if(J == 0)
		{
			
		}
		else if(J == i_N-1)
		{
			
		}
		else
		{
			
		}
	}
}


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

bool isProblem1Done(const int i_N)
{
	ifstream file(s_FILENAME.c_str());
	ifstream check(s_CHECKFILE.c_str());
	char temp1, temp2;
	bool b_out = true;
	bool b_changed = false;
	
	cout << "called isProblem1Done. " << endl;	
	
	while(file >> temp1 && check >> temp2)
	{
		//cout << "temp1: " << temp1 << " temp2: " << temp2 << endl;
		
		if(b_out && temp1 == 1)
		{
			b_out = false;
		}
		if(temp1 != temp2)
		{
			i_unchanged = 0;
			b_changed = true;
			
			if(!b_out) break;
		} // end if
	} // end if
	
	if(b_changed)
	{
		cout << "Change detected." << endl;
		makeCheckFile();
		return b_out;
	} // endl
	
	if(i_unchanged != 0)
	{
		i_unchanged++;
		cout << "No changes were made to file " << s_FILENAME << " during the last iteration." << endl;
	} // end if
	if(i_unchanged == 10)
	{
		cout << "The file was not changed for 10 iterations. Stopping." << endl;
		throw new exception();
	} // end if
	
	return b_out;
} // end method isProblem1Done

int askUserForInteger(string s_MSG, const int i_MAX)
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
		if(i_value > 0 && i_value <= i_MAX)
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