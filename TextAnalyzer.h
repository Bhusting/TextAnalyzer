#pragma once
#include <string>
#include <functional>

class TextAnalyzer {
public:

	TextAnalyzer();																			// default constructor
	TextAnalyzer(unsigned int size, std::function<unsigned int(std::string)> func);			//constructor for passed in hash function
	~TextAnalyzer();																		//destructor

	unsigned int Hash(const std::string);					//default Hash function
	unsigned int GetCount(const std::string) const;				//returns the count for how many times the number is used in the text file
	void PrintHashEntries() const;						//Prints every hash entry with the amount of times it was used
	void PrintHashStats() const;							//prints the stats of the entire table
	void Insert(const std::string);						//inserts a new hash entry

private:
	//tableSize holds the size of the hash array, slots holds the amount of used slots, unique holds the amount of unique words
	//total holds the total words, max holds the maximum amount of entries in a slot
	int tableSize = 0, slots = 0, unique = 0, total = 0, max = 0;
	int * hashEntries = new int[tableSize];		//this array holds the sizes of each entry in the hash array

												//this is the Hash Item. It holds the value which holds the string passed in and holds the number of times each word is used.
												//Next holds the pointer to the next hash item
	struct HashItem {
		std::string value;
		int num = 0;
		HashItem* next;
	};

	//This is a pointer to the hash array
	HashItem ** HashTable;

	//This uses stl function to make a pointer to whichever Hash function passed in
	std::function<unsigned int(std::string)> funcP;
};