//Written by: Han Qiang


/*~~~~~~~~~~~~~~~~~~~~~~~~
Binary Node is used to store the item
(using non unique key, in this case Movie Title).
The node will have a left and right child depending
on the non unique key of other nodes.
~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef BinaryNode_h
#define BinaryNode_h

template<class ItemType>
class BinaryNode
{
private:
    ItemType              item;         // Data portion
    BinaryNode<ItemType>* leftPtr;        // Pointer to left child
    BinaryNode<ItemType>* rightPtr;        // Pointer to right child

public:
    // constructors
    BinaryNode(const ItemType & anItem)
    {
        item = anItem;
        leftPtr = 0;
        rightPtr = 0;
    }
    BinaryNode(const ItemType & anItem,
               BinaryNode<ItemType>* left,
               BinaryNode<ItemType>* right)
    {
        item = anItem;
        leftPtr = left;
        rightPtr = right;
    }
    // setters
    void setItem(const ItemType & anItem)
    {
        item = anItem;
    }
    void setLeftPtr(BinaryNode<ItemType>* left)
    {
        leftPtr = left;
    }
    void setRightPtr(BinaryNode<ItemType>* right)
    {
        rightPtr = right;
    }

    // getters
    ItemType getItem() const
    {
        return item;
    }
    BinaryNode<ItemType>* getLeftPtr() const
    {
        return leftPtr;
    }
    BinaryNode<ItemType>* getRightPtr() const
    {
        return rightPtr;
    }

    // other functions
    bool isLeaf() const
    {
        return (leftPtr == 0 && rightPtr == 0);
    }

};


#endif /* BinaryNode_h */
