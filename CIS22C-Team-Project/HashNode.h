// Specification file for the HashNode class
/*
 Written by: Zohreh Ashtarilarki
 */


/*=====================================================================
  HashNode is used for storing the item and utilizes pointers for sorting
  what HashNode would come after another. It also keeps track of the
  number of collisions a HashNode has while inserting in order to prevent
  a load factor beyond 75%.
 ======================================================================*/



#ifndef _HASH_NODE
#define _HASH_NODE


//using std::string;
template<class ItemType>
class HashNode
{
private:
    ItemType item;
    HashNode* nextPtr;
    int noCollisions;

public:
    HashNode() {noCollisions = 0; nextPtr = nullptr;}
    //overloaded constructors
    HashNode(ItemType &anItem){item = anItem; noCollisions = 0; nextPtr = nullptr;}

    // setters
    void setNext(HashNode* nextEntryPtr) {nextPtr = nextEntryPtr;} //for LL collision resolution

    void setItem(const ItemType & anItem) {item = anItem;}
    void setNoCollisions(int nCol) {noCollisions = nCol;}

    // getters
    HashNode * getNext() const {return nextPtr; } //for LL collision resolution

    ItemType getItem() const {return item;}
    int getNoCollisions() const {return noCollisions;}
};

#endif

