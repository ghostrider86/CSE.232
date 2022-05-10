/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:
   //
   // Construct
   //
   //
   // Construct
   //

   Node() : 
       pNext(nullptr), pPrev(nullptr), data() { } 

   Node(const T& data) : 
       pNext(nullptr), pPrev(nullptr), data(data) {}

   Node(T&& data) :
       pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
    if (pSource == nullptr)
        return nullptr;

    Node <T>* pDestination = new Node<T>(pSource->data);
    const Node <T>* pSrc = pSource;
    Node <T>* pDes = pDestination;

    for (pSrc = pSrc->pNext; pSrc; pSrc = pSrc->pNext)
        pDes = insert(pDes, pSrc->data, true);
   return pDestination;
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
    const Node <T>* pSrc;
    Node <T>* pDes = pDestination;
    Node <T>* pDesPrevious = nullptr;

    //thought process here was to use the assignment operator to replace slots that are filled
    for (pSrc = pSource; pSrc != nullptr && pDes != nullptr; pSrc = pSrc->pNext)
    {
        pDes->data = pSrc->data;
        pDesPrevious = pDes;
        pDes = pDes->pNext;
    }

    //if there are excess items, insert at the end
    if (pSrc != nullptr)
    {
        pDes = pDesPrevious;
        for (; pSrc != nullptr; pSrc = pSrc->pNext)
        {
            //new node
            pDes = insert(pDes, pSrc->data, true);
            // fille destination if empty
            if (nullptr == pDestination)
                pDestination = pDes;
        }
    }
    //if destination has excess unneeded items then remove them
    else if (pDes != nullptr)
    {
        bool setToNull = false;
        if (pDes->pPrev)
            pDes->pPrev->pNext = nullptr;
        else
            setToNull = true;
        //delete list
        clear(pDes);
        if (setToNull)
            pDestination = nullptr;
    }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   
   return new Node<T>;
}


/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
    Node <T>* pNew = new Node <T>(t);

    if (pCurrent != NULL && !after)
    {
        pNew->pPrev = pCurrent->pPrev;
        pNew->pNext = pCurrent;

        pCurrent->pPrev = pNew;
        if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;
    }
    else if (pCurrent != NULL && after)
    {
        pNew->pNext = pCurrent->pNext;
        pNew->pPrev = pCurrent;

        pCurrent->pNext = pNew;
        if (pNew->pNext)
            pNew->pNext->pPrev = pNew;
    }
   return pNew;
}

/******************************************************
 * FIND
 * Find a given node in an unsorted linked list.  Return
 * a pointer to the node if it is found, NULL otherwise.
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : a pointer to the node if it is found
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   return 99;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   return out;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   
}


