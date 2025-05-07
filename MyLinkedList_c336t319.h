#ifndef _MY_LINKEDLIST_H_
#define _MY_LINKEDLIST_H_

#include <algorithm>
#include <iostream>

template <typename DataType>
class MyLinkedList
{
  private:
    
    struct Node
    {
        DataType  data;
        Node   *prev;
        Node   *next;
        
        Node(const DataType &d = DataType{ }, Node *p = nullptr, Node *n = nullptr) : 
        data{d}, 
        prev{p}, 
        next{n} 
        { }

        Node(DataType&& d, Node* p = nullptr, Node* n = nullptr) : 
        data{std::move(d)}, 
        prev{p}, 
        next{n} 
        { }
    };

    int theSize;        // the number of elements that the linked list is currently holding
    Node *head;         // pointer to the head node; does not hold real data
    Node *tail;         // pointer to the tail note; does not hold real data

    void init( )
    { 
        // code begins
        //init for node
        theSize = 0;
        head = new Node();
        tail = new Node();

        head -> next = tail;
        tail -> prev = head;
        // code ends

        return;
    }

  public:

    // define the const_iterator class
    class const_iterator
    { 
      protected:
        Node *current;
        // returns a mutable object
        // defined as proected as we don't expect it to be called directly by other classess
        DataType& retrieve() const
        { 
            return current->data; 
        }
        // constructor to be hidden from other classes; 
        // we don't expect direct construction of iterator form pointer
        const_iterator(Node *p) : 
        current{p} 
        {}
        
        friend class MyLinkedList<DataType>;

      public:
        const_iterator() : 
        current{nullptr} 
        {}

        // add const to the return type and make the return value by retrieve() non-mutable
        const DataType& operator* () const
        {
            // code begins
            return current -> data;
            // code ends
        }
        
        // move to the next data element
        // the prefix increment (e.g. ++ p) with 0 parameter
        const_iterator& operator++ ()
        {
            // code begins
            current = current -> next;
            return *this;
            // code ends
        }

        // move to the next data element
        // the suffix increment (e.g. p ++) with 1 int parameter
        const_iterator operator++ ( int )
        {
            // code begins
            const_iterator old_it = *this;
            current = current -> next;
            return old_it;
            // code ends
        }

        // move to the previous data element
        // the prefix decrement (e.g. -- p) with 0 parameter
        const_iterator& operator-- ()
        {
            // code begins
            current = current -> prev;
            return *this;
            // code ends
        }

        // move to the previous data element
        // the suffix decrement (e.g. p --) with 1 int parameter
        const_iterator operator-- (int)
        {
            // code begins
            const_iterator old_it = *this;
            current = current -> prev;
            return old_it;
            // code ends
        }

        // check whether two iterators point to the same data element
        bool operator== (const const_iterator& rhs) const
        {
            // code begins
            return current == rhs.current;
            // code ends 
        }
        
        // check whether two iterators point to the same data element
        bool operator!= (const const_iterator & rhs) const
        {
            // code begins
            return current != rhs.current;
            // code ends
        }
      
    };

    // define the iterator class; inherit from class const_iterator
    class iterator : public const_iterator
    {
      protected:
        iterator(Node *p) : 
        const_iterator{p}
        {}
        
        friend class MyLinkedList<DataType>;
      
      public:
        // default constructor
        iterator()
        {}

        // the return type is mutable
        DataType& operator* ()
        { 
            // code begins
            return this -> current -> data;
            // code ends 
        }
        
        // keep the return type as non-mutable
        const DataType& operator* () const
        { 
            // code begins
            return const_iterator::operator*();
            // code ends
        }

        iterator & operator++ ()
        {
            // code begins
            this -> current = this -> current -> next;
            return *this;
            // code ends
        }

        iterator operator++ (int)
        {
            // code begins
            iterator old_it = *this;
            this -> current = this -> current -> next;
            return old_it;
            // code ends
        }

        iterator operator-- ()
        {
            // code begins
            this -> current = this -> current -> prev;
            return *this;
            // code ends
        }
        
        iterator operator-- (int)
        {
            // code begins
            iterator old_it = *this;
            this -> current = this -> current -> prev;
            return old_it;
            // code ends
        }

        // don't need to overload operator= and operator!= as they should behave the same as in const_iterator
    };

  // defining the MyLinkedList class methods
  public:

    // default constructor
    MyLinkedList()
    { 
        init(); 
    }
  
    // copy constructor
    MyLinkedList(const MyLinkedList& rhs)
    { 
        // code begins
        init();
        Node* ptr = rhs.head -> next;
        for(int i = 0; i < rhs.theSize; ++ i)
        {
            push_back(ptr->data);
            ptr = ptr -> next;
        }
        // code ends
    }

    // move constructor
    MyLinkedList (MyLinkedList&& rhs) :
    theSize(rhs.theSize),
    head(rhs.head),
    tail(rhs.tail)
    { 
        // code begins
        rhs.theSize = 0;
        rhs.head = nullptr;
        rhs.tail = nullptr;
        // code ends
    }

    // destructor
    ~MyLinkedList()
    {
        // code begins
        Node* current = head;
        while (current != nullptr)
        {
            Node* nextNode = current -> next;
            delete current;
            current = nextNode;
        }
        // code ends
    }

    // copy assignment
    MyLinkedList & operator= (const MyLinkedList& rhs)
    { 
        // code begins
        if(this != &rhs)
        {
            MyLinkedList tmp(rhs);
            std::swap(this -> theSize, tmp.theSize);
            std::swap(this -> head, tmp.head);
            std::swap(this -> tail, tmp.tail);
        }
        return *this;
        // code ends
    }
  
    // move assignment
    MyLinkedList & operator= (MyLinkedList && rhs)
    { 
        // code begins
        if(this == &rhs)
        {
            return *this;
        }
        std::swap(this -> theSize, rhs.theSize);
        std::swap(this -> head, rhs.head);
        std::swap(this -> tail, rhs.tail);
        rhs.theSize = 0;
        rhs.head = nullptr;
        rhs.tail = nullptr;
        return *this;
        // code ends
    }

    // iterator interface
    iterator begin()
    { 
        // code begins
        if(empty())
        {
            return iterator(tail);
        }
        return iterator(head -> next);
        // code ends 
    }

    const_iterator begin() const
    { 
        // code begins
        if(empty())
        {
            return const_iterator(tail);
        }
        return const_iterator(head -> next);
        // code ends 
    }
  
    iterator end()
    { 
        // code begins
        return iterator(tail);
        // code ends 
    }

    const_iterator end() const
    { 
        // code begins
        return const_iterator(tail);
        // code ends 
    }
  
    // gets the size of the linked list
    int size() const
    {
        // code begins
        return theSize;
        // code ends
    }

    // checks if the linked list is empty; return True if empty, false otherwise
    bool empty( ) const
    { 
        // code begins
        return (head -> next == tail);
        // code ends 
    }

    // deletes all nodes excepts the head and tail
    void clear( )
    {
        // code begins
        while(!empty())
        {
            pop_front();
        }
        // code ends
    }

    // return the first data element as mutable
    DataType& front()
    { 
        // code begins
        return *begin();
        // code ends 
    }

    // return the first data element as non-mutable
    const DataType& front() const
    { 
        // code begins
        return *begin();
        // code ends
    }

    // return the last data element as mutable
    DataType& back()
    { 
        // code begins
        return *--end();
        // code ends 
    }

    // return the last data element as non-mutable
    const DataType& back() const
    { 
        // code begins
        return *--end();
        // code ends 
    }

    // insert x before itr; return iterator pointing to the newly inserted data element
    iterator insert(iterator itr, const DataType& x)
    {
        // code begins
        Node* ptr = itr.current;
        theSize++;
        return(ptr->prev = ptr -> prev -> next = new Node(x, ptr -> prev, ptr));
        // code ends
    }

    // insert x before itr; return iterator pointing to the newly inserted data element
    iterator insert(iterator itr, DataType&& x)
    {
        // code begins
        Node* ptr = itr.current;
        theSize++;
        return (ptr -> prev = ptr -> prev -> next = new Node(std::move(x), ptr -> prev, ptr));
        // code ends 
    }

    // delete the data element pointed by itr; return the iterator pointing to the data element next to the one being deleted
    iterator erase(iterator itr)
    {
        // code begins
        Node* ptr = itr.current;
        iterator retVal(ptr -> next);
        ptr -> prev -> next = ptr -> next;
        ptr -> next -> prev = ptr -> prev;
        delete ptr;
        theSize--;
        return retVal;
        // code ends
    }

    // delete the data elements from iterator "from" (inclusive) to iterator "to" (exclusive)
    // return the iterator "to"
    iterator erase( iterator from, iterator to )
    { 
        // code begins
        for(iterator itr = from; itr != to; )
        {
            itr = erase(itr);
        }
        return to;
        // code ends
    }

    // insert data element x to the very begining of the linked list
    void push_front(const DataType& x)
    { 
        // code begins
        insert(begin(), x);
        // code ends
    }
  
    void push_front(DataType&& x)
    { 
        // code begins
        insert(begin(), std::move(x));
        // code ends 
    }
    
    // insert data element x to the very end of the linked list
    void push_back(const DataType& x)
    {
        // code begins
        insert(end(), x);
        // code ends
    }
    
    void push_back(DataType&& x)
    { 
        // code begins
        insert(end(), std::move(x));
        // code ends
    }
    
    // delete the very first data element
    void pop_front()
    {
        // code begins
        erase(begin());
        // code ends
    }
    
    // delete the very last data element
    void pop_back( )
    { 
        // code begins
        erase(--end());
        // code ends
    }
    
    // reverse the entire linked list
    void reverseList()
    {
        // code begins
        MyLinkedList newList;
        Node* current = tail->prev;
        while(current != head)
        {  
            newList.push_back(current->data);
            current = current->prev;
        }
        std::swap(*this, newList);
        // code ends
    }
 
    // append a linked list to the end of the current one
    MyLinkedList<DataType>& appendList(MyLinkedList<DataType>&& rlist) 
    {
        // code begins
        /*
        if(!rlist.head)
        {
            return *this;
        }
        if(!head)
        {
            head = rlist.head;
            tail = rlist.tail;
        }
        else
        {
            tail -> next = rlist.head;
            tail = rlist.tail;
        }
        rlist.head = nullptr;
        rlist.tail = nullptr;
        return *this;
        */
       if(rlist.empty())
       {
            return *this;
       }
       for(auto it = rlist.begin(); it != rlist.end(); ++it)
       {
        push_back(*it);
       }
       rlist.clear();
       return *this;
        // code ends
    }

    // swap the data element pointed by itr with its next data element
    // return false if the next data element does not exist; true otherwise
    bool swapAdjElements(iterator& itr)
    {
        // code begins
        if(itr.current == nullptr || itr.current->next == nullptr || itr.current->next == tail)
        {
            return false;
        }   
        std::swap(itr.current -> data, itr.current -> next -> data);
        return true;
        // code ends
    }

    bool remove(const DataType& x)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it == x)
            {
                erase(it);
                return true;
            }
        }
        return false;
    }

    bool remove(DataType&& x)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it == x)
            {
                erase(it);
                return true;
            }
        }
        return false;
    }

};

#endif // __MYLIST_H__