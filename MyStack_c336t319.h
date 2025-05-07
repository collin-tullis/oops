#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include <iostream>
#include <algorithm>

#include "MyVector_c336t319.h"

template <typename DataType>
class MyStack //: private MyVector<DataType>
{  
  public:
  MyVector<DataType> myVec;

    // default constructor
    explicit MyStack(size_t initSize = 0)
    {
        // code begins
        myVec = MyVector<DataType>(initSize);
        // code ends
    }

    // copy constructor
    MyStack(const MyStack & rhs)
    {
        // code begins
        myVec = rhs.myVec;
        // code ends
    }

    // move constructor
    MyStack(MyStack && rhs):
    myVec{rhs.myVec}
    {
        // code begins
        rhs.myVec = MyVector<DataType>();
        // code ends
    }

    // destructor
    ~MyStack()
    {
        ;    
    }

    // copy assignment
    MyStack & operator= (const MyStack & rhs)
    {
        // code begins
        if(this != &rhs)
        {
            MyVector<DataType> cop;
            cop = rhs.myVec;
            std::swap(myVec, cop);
        }
        return *this;
        // code ends
    }

    // move assignment
    MyStack & operator= (MyStack && rhs)
    {
        // code begins
        std::swap(myVec, rhs.myVec);
        rhs.myVec = MyVector<DataType>();
        // code ends
    }

    // insert x to the stack
    void push(const DataType & x)
    {
        // code begins
        if(size() == (capacity()))
        {
            myVec.reserve(capacity() *2);
        }
        myVec.push_back(x);
        // code ends
    }

    // insert x to the stack
    void push(DataType && x)
    {
        // code begins
        if(size() == (capacity()))
        {
            myVec.reserve(capacity() *2);
        }
        myVec.push_back(std::move(x));

        // code ends
    }

    // remove the last element from the stack
    void pop(void)
    {
        // code begins
        if(myVec.size() != 0)
        {
            myVec.resize(myVec.size() - 1);
        }
        // code ends
    }

    // access the last element of the stack
    const DataType & top(void) const
    {
        // code begins
        return myVec.back();
        // code ends
    }

    // check if the stack is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        // code begins
        return (myVec.size() == 0);
        // code ends
    }

    // access the size of the stack
    size_t size() const
    {
        // code begins
        return myVec.size();
        // code ends
    }

    // access the capacity of the stack
    size_t capacity(void) const 
    {
        // code begins
        return myVec.capacity();
        // code ends
    }

};


#endif // __MYSTACK_H__