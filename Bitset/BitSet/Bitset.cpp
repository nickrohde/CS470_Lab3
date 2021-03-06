#include "Bitset.h"
#include <string.h>
#include <fstream>

using namespace std;

Bitset::Bitset(int size)
{
	if (size <= 0)
	{
		throw new invalid_argument("Cannot construct bitset of size 0.");
	} // end if

	capacity = bitsToBytes(size);
	this->size = size;
	set = new char[capacity];

	memset(set, 0, sizeof(char)*capacity);
} // end Constructor(int)                    


Bitset::Bitset(std::string s_fileName)
{
	int temp = countCharsInFile(s_fileName);

	if (temp <= 0)
	{
		throw new invalid_argument("Cannot construct bitset of size 0.");
	} // end if

	ifstream file(s_fileName.c_str());

	capacity = temp;
	set = new char[capacity];
	size = temp * __BYTEOFFSET;

	if (file.is_open() && !file.bad())
	{
		char c;
		int counter = 0;

		while (file >> c && counter < size)
		{
			set[counter] = c;
			counter++;
		} // end while
	} // end if

	file.close();

}// end Constructor(string, int)


Bitset::Bitset(char ** matrix, int size) : Bitset(matrix, size, size) {}


Bitset::Bitset(char ** matrix, int rows, int cols) : Bitset(rows*cols)
{
	if (rows <= 0 || cols <= 0)
	{
		throw new invalid_argument("Invalid size received.");
	} // end if

	int counter = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			char temp = matrix[i][j];

			if (temp)
			{
				setBit(counter, 1);
			} // end if
			else
			{
				setBit(counter, 0);
			} // end else

			counter++;
		} // end for j
	} // end for i
} // end Constructor(char**, int, int)                                 


Bitset::Bitset(Bitset &other)
{
	size = other.size;
	capacity = other.capacity;
	set = new char[capacity];
	
	for(int i = 0; i < capacity; i++)
	{
		set[i] = other.set[i];
	}
}


Bitset::~Bitset(void)
{
	delete[] set;
} // end Destructor


int Bitset::operator()(int i, int j)
{
	if (testBit(i, j))
	{
		return 1;
	} // end if

	return 0;
} // end operator()


int Bitset::operator[](int i)
{
	int byte = i / __BYTEOFFSET;
	int bit = i % __BYTEOFFSET;

	if (testBit(byte, bit))
	{
		return 1;
	} // end if

	return 0;
} // end operator[]


void Bitset::setBit(int i, int i_val)
{
	int byte = i / __BYTEOFFSET;
	int bit = i % __BYTEOFFSET;

	setBit(byte, bit, i_val);
} // end setBit(int,int)


void Bitset::setBit(int i, int j, int i_val)
{
	if (i >= capacity)
	{
		string error = "Not a valid byte. Received: " + i;
		error += " Expected: 0-" + size;
		throw new invalid_argument(error);
	} // end if

	char temp = set[i];
	char* bit = &set[i];

	if (i_val && !testBit(i,j)) // bit is to be set and bit is not set
	{
		switch (j)
		{
		case 0:
			*(bit) |= __SETBIT0MASK;
			break;
		case 1:
			*(bit) |= __SETBIT1MASK;
			break;
		case 2:
			*(bit) |= __SETBIT2MASK;
			break;
		case 3:
			*(bit) |= __SETBIT3MASK;
			break;
		case 4:
			*(bit) |= __SETBIT4MASK;
			break;
		case 5:
			*(bit) |= __SETBIT5MASK;
			break;
		case 6:
			*(bit) |= __SETBIT6MASK;
			break;
		case 7:
			*(bit) |= __SETBIT7MASK;
			break;
		default:
			string error = "Not a valid bit. Received: " + j;
			error += " Expected: 0-7";
			throw new invalid_argument(error);
		} // end switch
	} // end if
	else if(!i_val && testBit(i,j)) // bit is to be reset and bit is set
	{
		switch (j)
		{
		case 0:
			*(bit) &= __RESETBIT0MASK;
			break;
		case 1:
			*(bit) &= __RESETBIT1MASK;
			break;
		case 2:
			*(bit) &= __RESETBIT2MASK;
			break;
		case 3:
			*(bit) &= __RESETBIT3MASK;
			break;
		case 4:
			*(bit) &= __RESETBIT4MASK;
			break;
		case 5:
			*(bit) &= __RESETBIT5MASK;
			break;
		case 6:
			*(bit) &= __RESETBIT6MASK;
			break;
		case 7:
			*(bit) &= __RESETBIT7MASK;
			break;
		default:
			string error = "Not a valid bit. Received: " + j;
			error += " Expected: 0-7";
			throw new invalid_argument(error);
		} // end switch
	} // end else
} // end method setBit(int,int,int)


void Bitset::resize(int i_size)
{
	int i_bytes = bitsToBytes(i_size);

	if (i_bytes > capacity)
	{
		char * temp = new char[i_bytes];

		memset(temp, 0, sizeof(char)*i_bytes);

		for (int i = 0; i < capacity; i++)
		{
			temp[i] = set[i];
		} // end for

		delete[] set;

		set = temp;
		temp = NULL;

		capacity = i_bytes;
		size = i_size;
	} // end if
	else
	{
		size = i_size;
	} // end else
} // end method resize


void Bitset::writeSetToFile(string s_fileName)
{
	remove(s_fileName.c_str());
	ofstream file(s_fileName.c_str(), ios::app | ios::out);

	if (file.is_open() && !file.bad())
	{
		for (int i = 0; i < size; i++)
		{
			char temp = set[i];

			file << temp;
		} // end for 
	} // end if
} // end method writeSetToFile


bool Bitset::testBit(int i, int j)
{
	if (i >= capacity)
	{
		string error = "Not a valid byte. Received: " + i;
		error += " Expected: 0-" + (capacity-1);
		throw new invalid_argument(error);
	} // end if
	if ((i*__BYTEOFFSET + j > size))
	{
		string error = "Not a valid bit. Received: " + (i*__BYTEOFFSET + j);
		error += " Expected: 0-" + (size-1);
		throw new invalid_argument(error);
	} // end if

	char temp = set[i];

	switch (j)
	{
	case 0:
		return temp & __SETBIT0MASK;
	case 1:
		return temp & __SETBIT1MASK;
	case 2:
		return temp & __SETBIT2MASK;
	case 3:
		return temp & __SETBIT3MASK;
	case 4:
		return temp & __SETBIT4MASK;
	case 5:
		return temp & __SETBIT5MASK;
	case 6:
		return temp & __SETBIT6MASK;
	case 7:
		return temp & __SETBIT7MASK;
	default:
		string error = "Not a valid bit. Received: " + j;
		error += " Expected: 0-7";
		throw new invalid_argument(error);
	} // end switch;
} // end method testBit                                             


int Bitset::countCharsInFile(string s_fileName)
{
	ifstream file(s_fileName.c_str(), ios::in);

	int counter = 0;

	char c = 0;

	while (file >> c)
	{
		counter++;
	} // end while

	return counter;
} // end method countCharsInFile


int Bitset::bitsToBytes(int bits)
{
	int temp = bits % __BYTEOFFSET;
	int result = 0;

	if (temp) // size is not a multiple of 8
	{//   q+1  = (8q+r -   r  +       8     ) /       8 
		result = (bits - temp + __BYTEOFFSET) / __BYTEOFFSET;
	} // end if
	else // size is a multiple of 8
	{//    q   =  8q  /       8
		result = bits / __BYTEOFFSET;
	} // end else

	return result;
} // end method bitsToBytes                                    
