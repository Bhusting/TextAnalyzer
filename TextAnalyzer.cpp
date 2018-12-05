#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "TextAnalyzer.h"

using namespace std;

TextAnalyzer::TextAnalyzer() {
	tableSize = 27;										//sets the table size to 27
	HashItem** HashT = new HashItem*[tableSize];		//creates a new HashTable that is the size of the table
	HashTable = HashT;									//sets the pointer HashTable to the array

														//uses stl bind to bind the default Hash function to the funcP pointer.
	funcP = bind(&TextAnalyzer::Hash, this, placeholders::_1);

	//loops through the hash entries array and sets the value = 0
	//Then creates a new HashItem for each section in the HashTable Array
	for (int i = 0; i < tableSize; i++) {
		hashEntries[i] = 0;
		HashTable[i] = new HashItem;
		HashTable[i]->value = "";
		HashTable[i]->next = NULL;
	}

}

TextAnalyzer::TextAnalyzer(unsigned int size, std::function<unsigned int(std::string)> func) {
	tableSize = size;									//sets the size of the tabe equal to its passed in value
	HashItem** HashT = new HashItem*[tableSize];;		//creates a new HashTable that is the size of the table
	HashTable = HashT;									//sets the pointer HashTable to the array

														//sets the pointer to the passed in function
	funcP = func;

	//loops through the hash entries array and sets the value = 0
	//Then creates a new HashItem for each section in the HashTable Array
	for (int i = 0; i < tableSize; i++) {
		hashEntries[i] = 0;
		HashTable[i] = new HashItem;
		HashTable[i]->value = "";
		HashTable[i]->next = NULL;
	}
}

TextAnalyzer::~TextAnalyzer() {
	//deletes the Table and Entries array
	delete[] HashTable, hashEntries;
}

unsigned int TextAnalyzer::Hash(const std::string key) {
	unsigned int hash = 0;			//This is the result of the hash 

									//sets the value of hash bawsed upon what the first character is in the passed in string
	if ((int)key[0] > 64 && (int)key[0] < 91)
		hash = ((int)key[0] - 64);
	else if ((int)key[0] > 96 && (int)key[0] < 123)
		hash = ((int)key[0] - 96);

	//takes the mod of the hash variable to find the position with which to place the string
	hash = hash % tableSize;

	return hash;
}

unsigned int TextAnalyzer::GetCount(const std::string value) const {
	int index = funcP(value);		//takes the hash of the passed in string to find the place it is stored in

									//makes sure the hash is a valid number within the bounds of the array
	if (index < tableSize && index >= 0) {
		HashItem * ptr = HashTable[index];		//creates a pointer at the index with which the string should be in

												//loops until either the ptr value equals the passed in string or until it reaches the end of the linked list
		while (ptr != NULL) {
			if (ptr->value == value) {
				return ptr->num;
			}
			ptr = ptr->next;
		}
		cout << "Could not find the passed in string in the Hash Array" << endl;
	}
	else {
		cout << "Hash function passed in a value larger than the array" << endl;
	}
	return 0;
}

void TextAnalyzer::PrintHashEntries() const {
	//prints all of the entries and items within the HashTable array as well as how many times they appear	
	for (int i = 0; i < tableSize; i++) {
		HashItem * ptr = HashTable[i];
		while (ptr != NULL && ptr->value != "") {
			cout << ptr->value << " = " << GetCount(ptr->value) << endl;
			ptr = ptr->next;
		}
	}


}

void TextAnalyzer::PrintHashStats() const {

	cout << "Hash table entries per slot: ";

	//prints out how many entries are inside each hash slot
	for (int i = 0; i < tableSize; i++) {

		cout << hashEntries[i] << " ";

	}

	cout << endl;

	cout << slots << " hash table slots used out of " << tableSize << " total" << endl;

	cout << max << " max entries in a table slot. " << ((float)unique / (float)tableSize) << " avg entries in a table slot." << endl;

	cout << unique << " unique words found." << endl;

	cout << total << " total words counted." << endl << endl;
}

void TextAnalyzer::Insert(const std::string value) {
	unsigned int index = funcP(value);		//takes the hash of the passed in string to find the place it is stored in

											//if there is no items inside the array set the first value equal to the passed in string and increment num, slots, total, hashEntries, and unique
	if (index < tableSize && index >= 0) {
		if (HashTable[index]->value == "") {
			HashTable[index]->value = value;
			HashTable[index]->num++;
			slots++, total++, unique++;
			hashEntries[index]++;
		}
		else {
			//creates a new pointer to the first item in the slot as well as a new Hash Item
			HashItem* ptr = HashTable[index];
			HashItem* n = new HashItem;
			n->value = value;
			n->next = NULL;
			n->num++;

			//loops through each item in the slot to see if the word has been used or if it has reaches the end of the array
			while (ptr->next != NULL && ptr->value != value) {
				if (ptr->value != value) {
					ptr = ptr->next;
				}
				else {
					break;
				}
			}

			//if the ptr has not found the string inside the slot then create a new entry and increment hashEntries and check to see if the hashEntry is larger than max
			if (ptr->value != value) {
				ptr->next = n;
				unique++;
				hashEntries[index]++;
				if (hashEntries[index] > max) {
					max = hashEntries[index];
				}
			}
			else {
				ptr->num++;
			}
			total++;
		}
	}
	else {
		cout << "Hash function passed in a value larger or smmaller than the size of the array." << endl;
	}

}