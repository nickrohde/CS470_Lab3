SOURCE_BITSET=Bitset.cpp
SOURCE_BITSET_H=Bitset.h
SOURCE_TEST=test.cpp
OBJECTS=test.o Bitset.o
BITSET=Bitset
TEST=Test

CC=g++
STD=-std=c++11
PTHREAD=-pthread

all: $(TEST)

$(BITSET): $(OBJECTS)
	$(CC) -c $(SOURCE_BITSET) $(STD) -o $(BITSET)

$(TEST): $(BITSET)
	$(CC) $(SOURCE_TEST) -o $(TEST) $(PTHREAD) $(STD)

$(OBJECTS): $(SOURCE_BITSET_H)

clean: 
	rm -f $(TEST) $(BITSET) *.o

