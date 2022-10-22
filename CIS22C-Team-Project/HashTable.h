// Specification file for the HashNode class
/*
 Written by: Zohreh Ashtarilarki and Han Qiang
 */



/*=====================================================================
  HashTable is used for the storing of Movies using the imdb code.
  HashTable utilizes HashNode and various functions such as insert, delete,
  etc. to make sure everything is stored correctly without problems.
 ======================================================================*/



#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "HashNode.h"

#include <iostream>
using namespace std;
//using std::string;

template<class ItemType>
class HashTable : public HashNode<ItemType>
{
private:
    //Array of pointers to entries
    HashNode<ItemType> ** hashAry;
    int hashSize ;
    int count;

public:
    // Han Qiang:modify the constructor
    HashTable()
    {
        count = 0;
        hashSize = 53;
        hashAry = new HashNode<ItemType>*[hashSize];
        for (int i = 0; i < hashSize; i++)
        {
            hashAry[i] = NULL;
        }
    }

    HashTable(int n)
    {
        count = 0;
        hashSize = n;
        hashAry = new HashNode<ItemType>*[hashSize];
        for (int i = 0; i < hashSize; i++)
        {
            hashAry[i] = NULL;
        }

    }



    ~HashTable(){ delete [] hashAry; }

    int getCount() const    { return count; }
    int getSize() const { return hashSize; }
    double getLoadFactor() const {return 100.0 * count / hashSize; }

    bool isEmpty() const    { return count == 0; }

    bool insertHash( ItemType &itemIn );
    bool deleteHash( ItemType &itemOut, string key);
    int searchHash( ItemType &target, string key);
    void displayStatistics();

    void rehash(int newSize);
    void writeToFile(stringstream getOutData(ItemType&), string fileName);


private:
    int _hash(string key) const;
    void _helperStatistics(int &, int &);
};


/*~*~*~*
  A simple hash function
 *~**/
template<class ItemType>
int HashTable<ItemType>::_hash(string key) const
{

    //cout << hashSize << endl;
    int sum = 0;
    for (int i = 0; key[i]; i++)
        sum += key[i];
    return sum % hashSize;
};

/*~*~*~*
  hash insert - LinkedList collision resolution
*~**/

template<class ItemType>
bool HashTable<ItemType>::insertHash(  ItemType &itemIn )
{
    //create entry to add
    int ncoll = 0;
    if (count == hashSize)
    {

        return false;
    }

    // determine bucket, i.e. hashed index
    int bucket = _hash(itemIn->getImdb());
    HashNode<ItemType>* entry = hashAry[bucket];

    // Add the entry to the chain at bucket
    if (entry == nullptr)
    {

        entry = new HashNode<ItemType>;
        entry->setItem(itemIn);
        entry->setNext(NULL);
        hashAry[bucket] = entry;


    }
    //collision
    else {
        ncoll++;
        while (entry->getNext() != NULL)
        {
            ncoll++;
            //cout <<  ncoll; (for testing purposes)
            entry = entry->getNext();
        }

        // creating the new node
        entry = new HashNode<ItemType>;
        entry->setItem(itemIn);
        entry->setNext(NULL);

        entry->setNext(hashAry[bucket]);
        hashAry[bucket] = entry;
        hashAry[bucket]->setNoCollisions(ncoll);
    }
    //update counter
    ++count;
    return true;

}

    //collision



/*~*~*~*
   hash delete - Linked List collision resolution
   - looks for key in hash table
   - if found:
       - copies its data to itemOut
       - replaces data in the hash node with an "empty" record
       - returns true
   - if not found - returns false
*~**/

template<class ItemType>
bool HashTable<ItemType>::deleteHash( ItemType &itemOut, string key)
{
    int bucket = _hash(key);

    //exit if no hashed address
    if (hashAry[bucket] == nullptr){
        return false;
    }
     //determine if the node w/o linked list is the one
    if (hashAry[bucket]->getNoCollisions() == 0 && hashAry[bucket]->getItem()->getImdb() == key){
        itemOut = hashAry[bucket]->getItem();

        //hashAry[bucket]->setNoCollisions(0);
        hashAry[bucket] = NULL;
        count--;
        return true;
    }
    //node with linked list
    else{
        HashNode<ItemType> *pWalk = hashAry[bucket];
        HashNode<ItemType> *pPre = pWalk;

        //Skip all nodes whose value member is not equal to key
        while(pWalk != nullptr && pWalk->getItem()->getImdb() != key){
            pPre = pWalk;
            pWalk = pWalk->getNext();
        }

        //if we are at the right node
        if (pWalk && pWalk->getItem()->getImdb() == key)
        {
            itemOut = pWalk->getItem();
            int ncoll =  hashAry[bucket]->getNoCollisions();
            ncoll--;

            if (pWalk->getNext() && pPre != pWalk){

                pPre->setNext(pWalk->getNext());
                delete pWalk;
            }
            else if (pWalk->getNext() && pPre == pWalk)
            {
                hashAry[bucket] = pWalk->getNext();
                delete pWalk;

            }
            else if ( !pWalk->getNext() && pPre != pWalk)
            {
                pPre->setNext(NULL);
                delete pWalk;
            }
            hashAry[bucket]->setNoCollisions(ncoll);
            --count;
            return true;
        }


    }

    return false;

}


/*~*~*~*
   hash search
   if found:
      - copy data to itemOut
      - returns the number of collisions for this key
   if not found, returns -1
*~**/
template<class ItemType>
int HashTable<ItemType>::searchHash( ItemType &itemOut, string key)
{
    // determine initial bucket
    int bucket = _hash(key);

    //exit if no hashed address
    if (hashAry[bucket] == nullptr)
    {
        return 0;
    }

    else
    {

        HashNode<ItemType> *pWalk = hashAry[bucket];

        while(pWalk != nullptr){
           // cout << key;
            //cout << pWalk->getItem()->getImdb();
            if (pWalk->getItem()->getImdb() == key){
                itemOut = pWalk->getItem();
               // int noCol = pWalk->getNoCollisions(); // I just change it to make it                                                  easier to test.
                return 1;
            }
            //move to nextPtr;
            pWalk = pWalk->getNext();


        }

    }
    return 0;

}

template<class ItemType>
void HashTable<ItemType>::_helperStatistics(int &totalCollisions, int &maxLinkedList){
    for (int i = 0; i < hashSize; i++){
        // if hash address is empty - skip
        if (!hashAry[i]){
            continue;
        }
        else {
            if (hashAry[i] == NULL){
                continue;
            }
            else {
                totalCollisions += hashAry[i]->getNoCollisions();
                if (maxLinkedList < hashAry[i]->getNoCollisions()){
                    maxLinkedList = hashAry[i]->getNoCollisions();
            }

            }
        }
    }
}
template<class ItemType>
void HashTable<ItemType>::displayStatistics(){

    int totalCollisions = 0;
    int maxLinkedList = 0;
    _helperStatistics(totalCollisions, maxLinkedList);

    cout << "Load factor: " << 100.0 * count / hashSize << endl;
    cout << "Number of collisions: " << totalCollisions << endl;
    cout << "Length of the longest linked list:" << maxLinkedList << endl;

}


// Han Qiang: rehash and writeToFile


// part of the hashtable class
// traverse the hash table
// hash each items to a large table

template<class ItemType>
void HashTable<ItemType>::rehash(int newSize)
{
    HashNode<ItemType>** oldArr = hashAry;
    int size = hashSize;
    hashSize = newSize;
    hashAry = new HashNode<ItemType>*[newSize];
    count = 0;
    for (int i = 0; i < hashSize; i++){

        hashAry[i] = NULL;
    }


    for (int bucketList = 0; bucketList < size; bucketList++)
    {
        if (oldArr[bucketList] != NULL)
        {
            HashNode<ItemType> *entry = oldArr[bucketList];
            while (entry != NULL)
            {
                ItemType item = entry->getItem();
                insertHash(item);
                entry = entry->getNext();
            }
            delete entry;
        }

    }
    delete [] oldArr;
}

// part of the hashtable class
// traverse the hash table
// put the data to a stringstream
// write it to a new file
template<class ItemType>
void HashTable<ItemType>::writeToFile(stringstream getOutData(ItemType&), string fileName)
{

    ItemType itemOut;
    ofstream outFile(fileName);
    if (!outFile)
    {
        cout << "failed to open" << endl;
        return;
    }

    for (int i = 0; i < hashSize; i++)
    {
        if (hashAry[i] != NULL)
        {
            HashNode<ItemType> *entry = hashAry[i];
            while (entry != NULL)
            {
                itemOut = entry->getItem();

                stringstream myStream = getOutData(itemOut);
                outFile << myStream.str();
                entry = entry->getNext();
            }
        }

    }
    outFile.close();
}

#endif // HASHTABLE_H_
