// Written by: Han Qiang



/*=====================================================================
  BinarySearchTree is used for insertions, searches, deletions, of nodes
  in the BinaryTree.
 ======================================================================*/


#ifndef BinarySearchTree_h
#define BinarySearchTree_h
#include "BinaryTree.h"

template<class ItemType>
class BinarySearchTree : public BinaryTree<ItemType>
{
public:
    // insert a node at the correct location
    bool insert(int compare(ItemType&, ItemType&),const ItemType &item);
    // find a target node
    bool search(int compare(ItemType&, ItemType&), ItemType &target, ItemType &returnedItem) const;
    // remove node if found
    bool remove(int compare(ItemType&, ItemType&), const ItemType &item);


private:
    // internal insert node: insert newNode in nodePtr subtree
    BinaryNode<ItemType>* _insert(int compare(ItemType&, ItemType&), BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode);

    // search for target node
    BinaryNode<ItemType>* _search(int compare(ItemType&,ItemType&), BinaryNode<ItemType>* treePtr, ItemType &target) const;


    // internal remove node: locate and delete target node under nodePtr subtree
    BinaryNode<ItemType>* _remove(int compare(ItemType&, ItemType&),BinaryNode<ItemType>* nodePtr,  ItemType target, bool &success);

    // delete target node from tree, called by internal remove node
    BinaryNode<ItemType>* _removeNode(int compare(ItemType&, ItemType&), BinaryNode<ItemType>* targetNodePtr);

    // remove the leftmost node in the left subtree of nodePtr
    BinaryNode<ItemType>* _removeLeftmostNode(int compare(ItemType&, ItemType&),BinaryNode<ItemType>* nodePtr, ItemType &successor);

};


///////////////////////// public function definitions ///////////////////////////
//Inserting items within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::insert(int compare(ItemType&, ItemType&), const ItemType & newEntry)
{
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
    this->rootPtr = _insert(compare, this->rootPtr, newNodePtr);
    return true;
}

//Wrapper for _search
// - it calls the private _search function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType>
bool BinarySearchTree<ItemType>::search(int compare(ItemType&, ItemType&), ItemType& target, ItemType & returnedItem) const
{
    BinaryNode<ItemType>* temp = nullptr;
    BinaryNode<ItemType>* nodePtr = this->rootPtr;
    temp = _search(compare,nodePtr, target);
    if (temp)
    {
        returnedItem = temp->getItem();
        return true;
    }
    return false;
}
//Wrapper for _remove
// - it calls the prive _remove function which delte the target item
// - if removed, it returns true
template<class ItemType>
bool BinarySearchTree<ItemType>:: remove(int compare(ItemType&, ItemType&), const ItemType &item)
{
    bool success;
    this->rootPtr = _remove(compare, this->rootPtr, item, success);
    return success;
}




//////////////////////////// private functions ////////////////////////////////////////////

//Implementation of the insert operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_insert(int compare(ItemType&,ItemType&),BinaryNode<ItemType>* nodePtr,
        BinaryNode<ItemType>* newNodePtr)
{

    if( !nodePtr) // == NULL
    {
        nodePtr = newNodePtr;
        return nodePtr;
    }
    else
    {
        ItemType item1 = nodePtr->getItem();
        ItemType item2 = newNodePtr->getItem();
        if (compare(item1, item2) > 0)
        {
            nodePtr->setRightPtr(_insert(compare, nodePtr->getRightPtr(), newNodePtr));
        }
        else
        {
            nodePtr->setLeftPtr(_insert(compare,nodePtr->getLeftPtr(), newNodePtr));
        }

    }
    return nodePtr;
}


//Implementation for the search operation
// - return NULL if target not found, otherwise
// - returns a pointer to the node that matched the target
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::
_search(int compare(ItemType&,ItemType&), BinaryNode<ItemType>* nodePtr,
        ItemType &target) const
{
    BinaryNode<ItemType>* found = nullptr;
    if (!nodePtr)
    {
        return nodePtr;
    }
    else
    {
        ItemType temp = nodePtr->getItem();
        if (compare(target,temp) == 1)
        {
            return _search(compare,nodePtr->getLeftPtr(), target);
        }
        else if (compare(target,temp) == -1)
        {
            return _search(compare, nodePtr->getRightPtr(), target);
        }
        else if (compare(target,temp) == 0)
        {
            return nodePtr;
        }
    }
    return found;
}


//Implementation for the remove operation
// - searches for the target
// - returns NULL if root is null
// - returns a pointer to a node that is removed

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::
_remove(int compare(ItemType&, ItemType&), BinaryNode<ItemType>* nodePtr,  ItemType target, bool &succes)
{
    if (nodePtr == NULL)
    {
        succes = false;
        return NULL;
    }
    ItemType temp = nodePtr->getItem();

    if (compare(target, temp) == 1)
    {
        nodePtr->setLeftPtr(_remove(compare,nodePtr->getLeftPtr(), target, succes));
    }
    else if (compare(target, temp) == -1)
    {
        nodePtr->setRightPtr(_remove(compare,nodePtr->getRightPtr(), target, succes));
    }
    else if (compare(target, temp) == 0)
    {
        nodePtr = _removeNode(compare, nodePtr);
        succes = true;
    }
    return nodePtr;
}

// Implementation for the remove operation
// - called by _remove
// - deletes the node if the node is a leaf and return the pointer
// - returns the lef pointer if the node has a left child
// - returns the right pointer the node it has a right child
// - calls the _removeLeftmostNode if the node has both left and right child

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::
_removeNode(int compare(ItemType&, ItemType&), BinaryNode<ItemType>* targetNodePtr)
{
    if (targetNodePtr->isLeaf())
    {
        delete targetNodePtr;
        targetNodePtr = 0;

        return targetNodePtr;
    }
    else if (targetNodePtr->getRightPtr() == 0)
    {
        BinaryNode<ItemType>* leftPtr = targetNodePtr->getLeftPtr();
        delete targetNodePtr;
        targetNodePtr = 0;

        return leftPtr;

    }
    else if (targetNodePtr->getLeftPtr() == 0)
    {
        BinaryNode<ItemType>* rightPtr = targetNodePtr->getRightPtr();
        delete targetNodePtr;
        targetNodePtr = 0;

        return rightPtr;
    }
    else
    {
        ItemType temp;
        targetNodePtr->setRightPtr(_removeLeftmostNode(compare, targetNodePtr->getRightPtr(), temp));
        targetNodePtr->setItem(temp);

        return targetNodePtr;
    }
}

// Implementation for the remove operation
// - called by _removeNode
// - deletes the node if the node has both left and right child
// - calls and returns the _removeNode if the node point has no left child
// - recursively calls itself if there is a left child

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::
_removeLeftmostNode(int compare(ItemType&, ItemType&),BinaryNode<ItemType>* nodePtr, ItemType &successor)
{
    if (nodePtr->getLeftPtr() == 0)
    {
        successor = nodePtr->getItem();
        return _removeNode(compare, nodePtr);

    }
    else
    {
        nodePtr->setLeftPtr(_removeLeftmostNode(compare,nodePtr->getLeftPtr(), successor));
        return nodePtr;
    }

}

#endif /* BinarySearchTree_h */
