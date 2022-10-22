// Written by: Han Qiang


/*=====================================================================
  BinaryTree is used for the creation, deletion, and printing of a tree.
 ======================================================================*/


#ifndef BinaryTree_h
#define BinaryTree_h

#include "BinaryNode.h"

template<class ItemType>
class BinaryTree
{
protected:
    BinaryNode<ItemType>* rootPtr;        // ptr to root node
    int count;                            // number of nodes in tree

public:
    // "admin" functions
    BinaryTree()
    {
        rootPtr = 0;
        count = 0;
    }
    BinaryTree(const BinaryTree<ItemType> & tree) { }
    virtual ~BinaryTree()
    {
        destroyTree(rootPtr);
    }

    // common functions for all binary trees
    bool isEmpty() const
    {
        return count == 0;
    }
    int getCount() const
    {
        return count;
    }
    void clear()
    {
        destroyTree(rootPtr);
        rootPtr = 0;
        count = 0;
    }

    void inOrder(void visit(ItemType &)) const
    {
        _inorder(visit, rootPtr);
    }

    void printTree(void visit(ItemType &, int)) const
    {
        _printTree(visit, rootPtr, 1);
    }

private:
    // delete all nodes from the tree
    void destroyTree(BinaryNode<ItemType>* nodePtr);

    // internal traverse
    void _inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
    void _printTree(void visit(ItemType &, int), BinaryNode<ItemType>* nodePtr, int level) const;

};

//Destroy the entire tree
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
    if(nodePtr) // != NULL
    {
        destroyTree(nodePtr->getLeftPtr());
        destroyTree(nodePtr->getRightPtr());
        delete nodePtr;
    }
}


//Inorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr) // != NULL
    {
        ItemType item = nodePtr->getItem();
        _inorder(visit, nodePtr->getLeftPtr());
        visit(item);
        _inorder(visit, nodePtr->getRightPtr());
    }
}



//Prints tree as an indented list
template<class ItemType>
void BinaryTree<ItemType>::_printTree(void visit(ItemType &, int), BinaryNode<ItemType>* nodePtr, int level) const
{
    if(nodePtr)
    {
        ItemType item = nodePtr->getItem();
        visit(item, level);
        level++;

        _printTree(visit,nodePtr->getLeftPtr(),level);
        _printTree(visit,nodePtr->getRightPtr(),level);

    }

}

#endif /* BinaryTree_h */

