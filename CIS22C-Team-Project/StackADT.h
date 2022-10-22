// Written by: Sabrina Diaz-Erazo

/*~*~*~*~*~*~*~*~*~*~*~*~
 Stack template: The purpose of this file is to aide with the undo delete function
 by implementing a stack. When a movie is deleted, it is pushed into the undo
 stack. If the user selects the undo delete option in the menu, the movie is
 popped out of the stack and inserted back into the database.
~*~*~*~*~*~*~*~*~*~*~*~*/

#ifndef STACK_ADT
#define STACK_ADT

template <class T>
class Stack
{
private:
    // Structure for the stack nodes
    struct StackNode
    {
        T value;          // Value in the node
        StackNode *next;  // Pointer to next node
    };

    StackNode *top;     // Pointer to the stack top
    int length;

public:
    // Constructor
    Stack()
    {
        top = NULL;
        length = 0;
    }

    // Destructor
    ~Stack();

    // Stack operations:
    bool push(T);
    T pop();
    T peek();
    bool isEmpty();
    int getLength();
};

/*~*~*
  Member function push inserts the argument onto
  the stack.
*~*~*/
template <class T> bool Stack<T>::push(T item)
{
    StackNode *newNode; // Pointer to a new node

    // Allocate a new node and store num there.
    newNode = new StackNode;
    if (!newNode)
        return false;
    newNode->value = item;

    // Update links and counter
    newNode->next = top;
    top = newNode;
    length++;

    return true;
}

/*~*~*
  Member function pop deletes the value at the top
  of the stack and returns it.
  Assume stack is not empty.
*~*~*/
template <class T> T Stack<T>::pop()
{
    // Delete value at top of stack
    T number = top->value;
    top = top->next;
    // Counter
    --length;
    // Return deleted value
    return number;
}

/*~*~*
  Member function peek returns the value at the top of
  the stack without removing it; assumes stack is not
  empty.
*~*~*/
template <class T> T Stack<T>::peek()
{
    return top->value;
}

/*~*~*
  Member function isEmpty returns true if the stack is
  empty, false otherwise.
*~*~*/
template <class T> bool Stack<T>::isEmpty()
{
    if (length == 0)      // If the stack is empty
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*~*~*
  Member function getLength returns the number of
  elements in the stack.
*~*~*/
template <class T> int Stack<T>::getLength()
{
    return length;
}

/*~*~*
  Destructor
*~*~*/
template <class T> Stack<T>::~Stack()
{
    StackNode *currNode;

    // Position nodePtr at the top of the stack.
    currNode = top;

    // Traverse the list deleting each node.
    while (currNode)
    {
        StackNode *temp = currNode;
        currNode = currNode->next;
        delete temp;
        temp = NULL;
    }
}


#endif
