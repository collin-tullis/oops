#ifndef __MYHASHTABLE_H__
#define __MYHASHTABLE_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>

#include "MyVector_c336t319.h"
#include "MyLinkedList_c336t319.h"

static const long long uh_param_a = 53;       // universal hash function parameter a
static const long long uh_param_b = 97;       // universal hash function parameter b
static const long long prime_digits = 19;     // parameter used for finding a Mersenne prime
static const long long mersenne_prime = (1 << prime_digits) - 1;  // the Mersenne prime for universal hashing

// fast calculation of (n modulo mersenne_prime)
long long fastMersenneModulo(const long long n)
{
    // code begins
    long long result = (n & mersenne_prime) + (n >> prime_digits);
    if(result >= mersenne_prime)
    {
        result -= mersenne_prime;
    }
    return result;
    // code ends
}

// definition of the template hash function class
template <typename KeyType>
class HashFunc
{
  public:
    long long univHash(const KeyType key, const long long table_size) const;
};

// the hash function class that supports the hashing of the "long long" data type
template <>
class HashFunc<long long>
{
  public:
    long long univHash(const long long key, const long long table_size) const
    {
        long long hv = fastMersenneModulo(static_cast<long long>(uh_param_a * key + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// the has function class that supports the hashing of the "std::string" data type
template <>
class HashFunc<std::string>
{
  private:
    const int param_base = 37;      // the base used for inflating each character
  public:    
    long long univHash(const std::string& key, const long long table_size) const
    {
        long long hv = 0;
        for(size_t i = 0; i < key.length(); ++ i)
        {
            hv = param_base * hv + static_cast<long long>(key[i]);
        }
        hv = fastMersenneModulo(static_cast<long long>(uh_param_a * hv + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// definition of the template hashed object class
template <typename KeyType, typename ValueType>
class HashedObj
{
  public:

    KeyType key;
    ValueType value;

    HashedObj()
    {
        return;
    }

    HashedObj(const KeyType& k, const ValueType& v) :
        key(k),
        value(v)
    {
        return;
    }
  
    HashedObj(KeyType && k, ValueType && v) :
        key(std::move(k)),
        value(std::move(v))
    {
        return;
    }

    bool operator==(const HashedObj<KeyType, ValueType>& rhs)
    {
        return (key == rhs.key);
    }    

    bool operator!=(const HashedObj<KeyType, ValueType>& rhs)
    {
        return !(*this == rhs);
    }   

};

template <typename KeyType, typename ValueType>
class MyHashTable
{ 
  private:
    size_t theSize; // the number of data elements stored in the hash table
    MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > hash_table;    // the hash table implementing the separate chaining approach
    MyVector<size_t> primes;    // a set of precomputed and sorted prime numbers

    // pre-calculate a set of primes using the sieve of Eratosthenes algorithm
    // will be called if table doubling requires a larger prime number for table size
    // expected to update the private member "primes"
    void preCalPrimes(const size_t n)
    {
        // code begins
        std::vector<bool> prime(n+1, true);
        for(size_t p = 2; p*p <= n; p++)
        {
            if(prime[p] == true)
            {
                for(size_t i = p*p; i <= n; i+=p)
                {
                    prime[i] = false;
                }
            }
        }
        for(size_t p = 2; p <= n; p++)
        {
            if(prime[p])
            {
                primes.push_back(p);
            }
        }
        // code ends
    }

    // finding the smallest prime that is larger than or equal to n
    // should perform binary search against the private member "primes"
    size_t nextPrime(const size_t n)
    {
        // code begins
        if (primes.empty()) 
        {
            throw std::runtime_error("Error: primes list is empty.");
        }
    
        auto it = std::lower_bound(primes.begin(), primes.end(), n);
        if (it != primes.end()) 
        {
            return *it;
        }
        return primes.back();
        // code ends
    }

    // finds the MyLinkedList itertor that corresponds to the hashed object that has the specified key
    // returns the end() iterator if not found
    typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator find(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> hash_func;
        long long bucket = hash_func.univHash(key, static_cast<long long>(hash_table.size()));
        if (!hash_table[bucket]) return hash_table[bucket]->end();
        for(auto itr = hash_table[bucket] -> begin(); itr != hash_table[bucket] -> end(); ++ itr)
        {
            if((*itr).key == key)
            {
                return itr;
            }
        }
        return hash_table[bucket] -> end();
        // code ends
    }

    // rehashes all data elements in the hash table into a new hash table with new_size
    // note that the new_size can be either smaller or larger than the existing size
    void rehash(const size_t new_size)
    {
        // code begins
        size_t oldSize = theSize;
        MyVector<MyLinkedList<HashedObj<KeyType, ValueType> > * > tmp_hash_table(new_size);
        for(size_t i = 0; i < tmp_hash_table.size(); ++ i)
        {
            tmp_hash_table[i] = new MyLinkedList<HashedObj<KeyType, ValueType> >;
        }
    
        for(size_t i = 0; i < hash_table.size(); ++i)
        {
            for(auto itr = hash_table[i]->begin(); itr != hash_table[i]->end(); ++itr)
            {
                HashFunc<KeyType> hash_func;
                long long new_bucket = hash_func.univHash((*itr).key, new_size);
                tmp_hash_table[new_bucket]->push_back(std::move(*itr));
            }
        }

        for(size_t i = 0; i < hash_table.size(); ++i)
        {
            delete hash_table[i];
        }
    
        std::swap(hash_table, tmp_hash_table);
        theSize = oldSize;
        // code ends 
    }

    // doubles the size of the table and perform rehashing
    // the new table size should be the smallest prime that is larger than the expected new table size (double of the old size)
    void doubleTable()
    {
        size_t new_size = nextPrime(2 * hash_table.size());
        this->rehash(new_size);
        return;
    }

    // halves the size of the table and perform rehahsing
    // the new table size should be the smallest prime that is larger than the expected new table size (half of the old size)
    void halveTable()
    {
        size_t new_size = nextPrime(ceil(hash_table.size() / 2));
        this->rehash(new_size);
        return;
    }

  public:

    // the default constructor; allocate memory if necessary
    explicit MyHashTable(const size_t init_size = 3)
    {
        // code begins
        preCalPrimes(10000);
        hash_table.resize(init_size);
        for(size_t i = 0; i < init_size; ++ i)
        {
            hash_table[i] = new MyLinkedList<HashedObj<KeyType, ValueType> >;
        }
        theSize = 0;
        return;
        // code ends
    }

    // the default destructor; collect memory if necessary
    ~MyHashTable()
    {
        // code begins
        for(size_t i = 0; i < hash_table.size(); ++i)
        {
            delete hash_table[i];
        }
        return;
        // code ends
    }

    // checks if the hash tabel contains the given key
    bool contains(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> hash_func;
        long long bucket = hash_func.univHash(key, static_cast<long long>(hash_table.size()));
        for(auto itr = hash_table[bucket] -> begin(); itr != hash_table[bucket] -> end(); ++ itr)
        {
            if((*itr).key == key)
            {
                return true;
            }
        }
        return false;
        // code ends
    }

    // retrieves the data element that has the specified key
    // returns true if the key is contained in the hash table
    // return false otherwise
    bool retrieve(const KeyType& key, HashedObj<KeyType, ValueType>& data) const
    {
        // code begins
        HashFunc<KeyType> hash_func;
        long long bucket = hash_func.univHash(key, static_cast<long long>(hash_table.size()));
        for(auto itr = hash_table[bucket] -> begin(); itr != hash_table[bucket] -> end(); ++ itr)
        {
            if((*itr).key == key)
            {
                data = *itr;
                return true;
            }
        }
        return false;
        
        // code ends
    }

    // inserts the given data element into the hash table (copy)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(const HashedObj<KeyType, ValueType>& x)
    {
        // code begins
        if(contains(x.key))
        {
            return false;
        }
        HashFunc<KeyType> hash_func;
        long long bucket = hash_func.univHash(x.key, static_cast<long long>(hash_table.size()));
        hash_table[bucket] -> push_back(x);
        ++ theSize;
        if(2 * theSize >= hash_table.size())
        {
            doubleTable();
        }
        return true;
        // code ends
    }

    // inserts the given data element into the hash table (move)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(HashedObj<KeyType, ValueType> && x)
    {
        // code begins
        if(contains(x.key))
        {
            return false;
        }
        HashFunc<KeyType> hash_func;
        long long bucket = hash_func.univHash(x.key, static_cast<long long>(hash_table.size()));
        hash_table[bucket] -> push_back(std::move(x));
        ++ theSize;
        if(2 * theSize >= hash_table.size())
        {
            doubleTable();
        }
        return true;
        // code ends
    }    

    // removes the data element that has the key from the hash table
    // returns true if the key is contained in the hash table
    // returns false otherwise
    bool remove(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> hash_func;
        auto bucket = hash_func.univHash(key,
                        static_cast<long long>(hash_table.size()));
        bool removedAny = false;
    
        // walk the entire list in this bucket
        auto &lst = *hash_table[bucket];
        for (auto itr = lst.begin(); itr != lst.end(); /* no ++itr here */) {
            if ((*itr).key == key) {
                // erase returns the next iterator
                itr = lst.erase(itr);
                --theSize;
                removedAny = true;
            }
            else {
                ++itr;
            }
        }
    
        // now that all duplicates are gone, maybe shrink
        if (removedAny && 8 * theSize < hash_table.size()) {
            halveTable();
        }
    
        return removedAny;
        // code ends
    }

    // returns the number of data elements stored in the hash table
    size_t size()
    {
        // code begins
        return theSize;
        // code ends
    }

    // returns the capacity of the hash table
    size_t capacity()
    {
        // code begins
        return hash_table.size();
        // code ends
    }

    bool get(const KeyType& key, ValueType& out_value) const
    {
        HashedObj<KeyType, ValueType> holder;
        if(retrieve(key, holder))
        {
            out_value = std::move(holder.value);
            return true;
        }
        return false;
    }
    
    void keys(MyVector<KeyType>& out_keys) const
    {
        out_keys.resize(0);
        for(size_t i = 0; i < hash_table.size(); ++i)
        {
            auto* bucket = hash_table[i];
            for(auto itr = bucket->begin(); itr != bucket->end(); ++itr)
            {
                std::cerr << "Debug keys: bucket = " << i<< " key= " << (*itr).key << std::endl;
                out_keys.push_back((*itr).key);
            }
        }
    }

};


#endif // __MYHASHTABLE_H__
