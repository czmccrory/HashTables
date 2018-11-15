#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE

#include "HashNode.h"
#include <vector>
#include <list>

using namespace std;

enum HashTableError { OUT_OF_MEMORY, KEY_NOT_FOUND, DUPLICATE_KEY, OUT_OF_BOUNDS }; // extend if necessary

typedef unsigned long ulint;

class HashTable {
  typedef vector <list<HashNode>> Table;
  Table *table; // size of table is stored in the Table data structure
  size_t num;   // number of entries in the HashTable

public:
  HashTable();       // constructor, initializes table of size 1
  HashTable(size_t); // constructor, requires size of table as arg
  ~HashTable();      // deconstructor
  void initializeTable(); //initializes table

  size_t size(); // returns size of the hash table (number of buckets)
  size_t hash_function(ulint);  // the table's hash function
  ulint getValue(ulint);    // find and return data associated with key
  bool existingKey(ulint);

  void insert(ulint,ulint); // insert data associated with key into table
  void erase(ulint);        // remove key and associated data from table

  void rehash(size_t); // sets a new size for the hash table, rehashes the hash table 
};

// Constructor
// Initializes table of size 11
inline HashTable::HashTable() {
  initializeTable();
  table->resize(11);
}

// Constructor
// Initializes table of size tableSize
// Parameters:
// tableSize - size of table
inline HashTable::HashTable(size_t tableSize) {
  initializeTable();
  table->resize(tableSize);
}

// Deconstructor
inline HashTable::~HashTable() {
  delete this->table;
}

void inline HashTable::initializeTable() {
  table = new Table;

  if(table == NULL) {
    throw OUT_OF_MEMORY;
  }

  num = 0;
}

// returns size of the hash table (number of buckets)
size_t inline HashTable::size() {
  return this->table->size(); 
}

// the table's hash function
size_t inline HashTable::hash_function(ulint aKey) {
  size_t index = aKey % size();

  if(index > (size() -1)) {
    throw OUT_OF_BOUNDS;
  }

  return index;
}

// find and return data associated with key
ulint inline HashTable::getValue(ulint key) {
  size_t index = hash_function(key);
  list<HashNode> aList = table->at(index);

  // iterate through list
  for(list<HashNode>::iterator it = aList.begin(); it != aList.end(); ++it) {
    // checks if key you are at is the key you are looking for
    if(it->getKey() == key) {
      // returns value at key
      return it->getValue();
    }
  }
  // throws exception - key you are looking for cannot be found
  throw KEY_NOT_FOUND;
}

// find and return key
bool inline HashTable::existingKey(ulint keyToFind) {
  size_t index = hash_function(keyToFind);
  list<HashNode> aList = table->at(index);

  // iterate through list
  for(list<HashNode>::iterator it = aList.begin(); it != aList.end(); ++it) {
    // checks if key you are at is the key you are looking for
    if(it->getKey() == keyToFind) {
      // returns true if key found
      return true;
    }
  }
  
  return false;
}

// insert data associated with key into table
void inline HashTable::insert(ulint keyToInsert, ulint dataToInsert) {
  size_t index = hash_function(keyToInsert);
  HashNode *newNode = new HashNode;
  list<HashNode> aList = this->table->at(index);

  // iterate through list
  for(list<HashNode>::iterator it = aList.begin(); it != aList.end(); ++it) {
    // checks if key you are at is the key you are looking for
    if(it->getKey() == keyToInsert) {
      throw DUPLICATE_KEY;
    }
  }
  newNode->assign(keyToInsert, dataToInsert);
  table->at(index).push_back(*newNode);
  num++;

  size_t loadFactor = num/size();
  if(loadFactor >= 0.9) {
    rehash((2*size())+1);
  }
}

// remove key and associated data from table
void inline HashTable::erase(ulint keyToRemove) {
  size_t index = hash_function(keyToRemove);
  list<HashNode> *aList ;
  aList = &(table->at(index));

  list<HashNode>::iterator it;

  for(it = aList->begin(); it != aList->end(); ++it) {
    // checks if key you are at is the key you are looking for
    if(it->getKey() == keyToRemove) {
      aList->erase(it);
      num--;
      return;
    }
  }
  throw KEY_NOT_FOUND;
}

// sets a new size for the hash table, rehashes the hash table
void inline HashTable::rehash(size_t newSize) {
  Table newTable = *table; // set newTable to be same as current table
  table->clear(); // clear current table
  table->resize(newSize); // resizes table to new size

  // for every list in the table
  for(list<HashNode> &list : newTable) {
    // for every node in the list
    for(HashNode &node : list) {
      insert(node.getKey(), node.getValue());
    }
  }
}

#endif