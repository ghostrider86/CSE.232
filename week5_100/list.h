/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Jacob Elzinga, Michael Jackson
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
 
class TestList;        // forward declaration for unit tests
class TestHash;

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list() 
   {
      clear();
   }

   // 
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);

   //
   // Iterator
   //

   class  iterator;
   iterator begin()  { return iterator(pHead); }
   iterator rbegin() { return iterator(); }
   iterator end()    { return iterator(nullptr); }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(T&& data);
   void push_back (const T&  data);
   void push_back (T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //

   bool empty()  const { return size() == 0; }
   size_t size() const { return numElements;   }


private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Constructers
   // 
   Node() : pNext(nullptr), pPrev(nullptr), data() {} //default
   Node(const T &  data) : pNext(nullptr), pPrev(nullptr), data(data) {}//copy
   Node(T && data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)){}//move

   //
   // Data
   //

   T data;                 // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator() 
   {
       p = nullptr;
   }
   iterator(Node * pRHS) 
   {
      p = pRHS;
   }
   iterator(const iterator  & rhs) 
   {
      p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
       this->p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return (rhs.p == p ? true : false); }
   bool operator != (const iterator & rhs) const { return (rhs.p != p ? true: false); }

   // dereference operator, fetch a node
   T & operator * ()
   {
      return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
       if (p->pNext) {
           p = p->pNext;
           return p;
      }
       p = nullptr;
           return p;
   }

   // prefix increment
   iterator & operator ++ ()
   {
       iterator foo = p;
       if (p->pNext) {
           p = p->pNext;
           return foo;
       }
       p = nullptr;
       return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
       if (p->pPrev) {
           p = p->pPrev;
           return p;
       }
       p = nullptr;
       return p;
   }

   // prefix decrement
   iterator & operator -- ()
   {
       if (p > 0) {
           iterator foo = p;
           if (p->pPrev) {
               p = p->pPrev;
               return foo;
           }
       }
       p == nullptr;
       return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it, T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) 
{
    if (num) {
        Node* pPrevious = pHead = pTail = new Node(T(t));
        pHead->pPrev = nullptr;
        for (int i = 1; i < num; i++) {
            Node* pNew = new Node(T(t));
            pNew->pPrev = pPrevious;
            pNew->pPrev->pNext = pNew;
            pPrevious = pNew;
        }
        pPrevious->pNext = nullptr;
        pTail = pPrevious;
    }
   numElements = num;
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last)
{
    pHead = pTail = nullptr;
   numElements = 0;
   auto foo = first;
   while (foo != last)
   {
       push_back(*foo);
       foo++;
   }
   
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il)
{
   numElements = 99;
   pHead = pTail = new list <T> ::Node();
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num)
{
   numElements = 99;
   pHead = pTail = new list <T> ::Node();
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() 
{
   numElements = 0;
   pHead = pTail = nullptr;
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) 
{
   pHead = pTail = nullptr;
   numElements = 0;
   *this = rhs;
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)
{
   numElements = 99;
   pHead = pTail = new list <T> ::Node();
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{

}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{

}

template <typename T>
void list <T> ::push_back(T && data)
{

}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{

}

template <typename T>
void list <T> ::push_front(T && data)
{

}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{

}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{

}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
   return *(new T);
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
   return *(new T);
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
   return end();
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data) 
{
   if (empty()) {
       Node *pNew = new list<T>::Node();
       pHead = pTail = pNew;
       numElements = 1;
       pNew->data = data;
       return end();
      }
   if (it == end()){
       Node *pNew = new list<T>::Node();
       pTail->pNext = pNew;
       pNew->pPrev = pTail;
       pTail = pNew;       numElements++;
       return iterator(pNew);
   }
   else{
      Node *pNew = new list<T>::Node();
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;
      if (pNew->pPrev != nullptr){
          pNew->pPrev->pNext = pNew;
      }
      return iterator(pNew);
   }
}

template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
   return end();
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
    lhs.phead = rhs.pHead;
    lhs.pTail = rhs.pTail;
    lhs.numElements = rhs.numElements;

    rhs.pHead = nullptr;
    rhs.pTail = nullptr;
    rhs.numElements = 0;
}


//#endif
}; // namespace custom
