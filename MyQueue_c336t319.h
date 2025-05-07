#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <iostream>
#include <algorithm>

#include "MyVector_c336t319.h"

template <typename DataType>
class MyQueue //: private MyVector<DataType>
{  
  private:
    size_t dataStart, dataEnd;
    MyVector<DataType> myVec;

    // changes the size of the array to newSize
    void resize(size_t newSize)
    {
        // code begins
        myVec.resize(newSize);
        // code ends
    }

    // requests for newCapacity amount of space
    void reserve(size_t newCapacity)
    {
        // code begins
        myVec.reserve(newCapacity);
        // code ends
    }

  public:

    // default constructor
    explicit MyQueue(size_t initSize = 0)
    {
        // code begins
        myVec = MyVector<DataType>(initSize);
        dataStart = 0;
        dataEnd = 0;
        // code ends
    }

    // copy constructor
    MyQueue(const MyQueue & rhs)
    {
        // code begins
        myVec = rhs.myVec;
        dataStart = rhs.dataStart;
        dataEnd = rhs.dataEnd;
        // code ends
    }

    // move constructor
    MyQueue(MyQueue && rhs):
    myVec{rhs.myVec},
    dataStart{rhs.dataStart},
    dataEnd{rhs.dataEnd}
    {
        // code begins
        rhs.myVec = MyVector<DataType>();
        rhs.dataStart = 0;
        rhs.dataEnd = 0;
        // code ends
    }

    // destructor
    ~MyQueue()
    {
        // code begins
        //with how I am doing this I dont think anything
        //needs to be in here
        // code ends
    }

    // copy assignment
    MyQueue & operator= (const MyQueue & rhs)
    {
        // code begins
        if(this != &rhs)
        {
            MyVector<DataType> cop;
            cop = rhs.myVec;
            std::swap(myVec, cop);
            dataStart = rhs.dataStart;
            dataEnd = rhs.dataEnd;
        }
        return *this;
        // code ends
    }

    // move assignment
    MyQueue & operator= (MyQueue && rhs)
    {
        // code begins
        if(this != &rhs)
        {
            std::swap(myVec, rhs.myVec);
            std::swap(dataStart, rhs.dataStart);
            std::swap(dataEnd, rhs.dataEnd);
            rhs.myVec = MyVector<DataType>();
            rhs.dataStart = 0;
            rhs.dataEnd = 0;
        }
        return *this;
        // code ends
    }

    // insert x into the queue
    void enqueue(const DataType & x)
    {
        // code begins
        if((size() == myVec.capacity()-1))
        {
            reserve(myVec.capacity()*2);
        }
        myVec[dataEnd] = x;
        if(myVec.capacity() == 0)
        {
            reserve(1);
        }
        if(size() == myVec.capacity()-1)
        {
            reserve(myVec.capacity()*2);
        }
        dataEnd = (dataEnd + 1) % myVec.capacity();
        // code ends
    }

    // insert x into the queue
    void enqueue(DataType && x)
    {
        // code begins
        if((dataEnd + 1) % myVec.capacity() == dataStart)
        {
            reserve(myVec.capacity()*2);
        }
        myVec[dataEnd] = std::move(x);
        dataEnd = (dataEnd + 1) % myVec.capacity();
        
        // code ends
    }

    // remove the first element from the queue
    void dequeue(void)
    {
        // code begins
        if(dataStart == dataEnd)
        {
            return;
        }
        dataStart = (dataStart + 1) % myVec.capacity();
        // code ends
    }

    // access the first element of the queue
    const DataType & front(void) const
    {
        // code begins
        return myVec[dataStart];
        // code ends
    }

    // check if the queue is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        // code begins
        return (dataStart == dataEnd);
        // code ends
    }

    // access the size of the queue
    size_t size() const
    {
        // code begins
        if(dataEnd >= dataStart)
        {
            return dataEnd - dataStart;
        }
        return myVec.capacity() - dataStart + dataEnd;
        // code ends
    }

    // access the capacity of the queue
    size_t capacity(void) const 
    {
        // code begins
        return myVec.capacity();
        // code ends
    }

};


#endif // __MYQUEUE_H__