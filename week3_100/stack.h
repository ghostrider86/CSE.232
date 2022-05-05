/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    Jacob Elzinga
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
//#include "vector.h"
#include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   // 
   // Construct
   // 

   stack()                            { container.resize(0); }
   stack(const stack <T> &  rhs)      { *this = rhs; }
   stack(stack <T> && rhs)            { *this = std::move(rhs); }
   stack(const std::vector<T> &  rhs) { container = rhs.container; }
   stack(std::vector<T> && rhs)       { container = std::move(rhs.container); }
   ~stack()                           { for (int i = 0; i < size(); i++) pop(); }

   //
   // Assign
   //

   stack <T> & operator = (const stack <T> & rhs)
   {
      container = std::move(rhs.container);
      return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
      container = std::move(rhs.container);
      return *this;
   }

   void swap(stack <T>& rhs)
   {
       std::vector <T> tempdata = std::move(rhs.container);
       rhs.container = std::move(container);
       container = std::move(tempdata);
   }

   // 
   // Access
   //

         T& top()       {
             //check if the array is empty
             assert(!empty());
             return container[size() - 1];
         }
   const T& top() const { return container[size() - 1]; }

   // 
   // Insert
   // 

   void push(const T&  t) { container.push_back(t); }
   void push(T&& t) { container.push_back(std::move(t)); }

   //
   // Remove
   //

   void pop() 
   { 
    if (!empty())
        container.erase(container.begin() + (size() - 1));
   }

   //
   // Status
   //
   size_t  size () const { return container.size(); }
   bool empty   () const { return size() > 0 ? false : true; }
   
private:
   
  std::vector<T> container;  // underlying container
};



} // custom namespace


