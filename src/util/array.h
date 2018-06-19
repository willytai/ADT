/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0), _isSorted(0){}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*_node); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () {
        _node = _node + 1;
        return *this;
      }
      iterator operator ++ (int) {
        iterator ret = *this;
        ++(*this);
        return ret;
      }
      iterator& operator -- () {
        _node -= 1;
        return (*this);
      }
      iterator operator -- (int) {
        iterator ret = *this;
        --(*this);
        return ret;
      }

      iterator operator + (int i) const { return _node + i; }
      iterator& operator += (int i) {
        _node = _node + i;
        return (*this);
      }

      iterator& operator = (const iterator& i) {
        if (this != &i) _node = i._node;
        return (*this);
      }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return _data; }
   iterator end() const { return begin() + _size; }
   bool empty() const { return (!_size); }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) {
     _isSorted = false;
     if (_size < _capacity) {
       _data[_size] = x;
       _size++;
     } else {
       _capacity = (!_capacity ? 1 : 2*_capacity);
       T* tmp = new T[_capacity];
       for (size_t i = 0; i < _size; i++) {
         tmp[i] = _data[i];
       }
       if (_data) delete [] _data;
       tmp[_size++] = x;
       _data = tmp;
     }
   }

   void pop_front() {
     _data[0] = _data[_size - 1];
     pop_back();
   }
   void pop_back() { if (_size > 0) _size--; }

   bool erase(iterator pos) {
     _isSorted = false;
     if (pos == end()) return false;
     if (_size > 0) {
       *pos = _data[--_size];
       return true;
     } return false;
   }
   bool erase(const T& x) {
     _isSorted = false;
     for (size_t i = 0; i < _size; ++i) {
       if (_data[i] == x) {
         _data[i] = _data[_size - 1];
         --_size;
         return true;
       }
     } return false;
   }

   void clear() {
     delete [] _data;
     _data = 0;
     _size = _capacity = 0;
     _isSorted = false;
   }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { if (!empty() && !_isSorted) ::sort(_data, _data+_size); _isSorted = true;}

   // Nice to have, but not required in this homework...
   void reserve(size_t n) {
     if (n < _size) _capacity = _size;
     else {
       _capacity = (_capacity / n + 1) * n;
       T* tmp = new T[n];
       for (size_t i = 0; i < _size; i++)
         tmp[i] = _data[i];
     }
   }
   void resize(size_t n) {
     if (n <= _size)
       while(_size > n)
         --_size;
     else {
       if (n > _capacity) {
         T* tmp = new T[n];
         for (size_t i = 0; i < _size; i++)
           tmp[i] = _data[i];
         for (size_t i = _size; i < n; i++) {
           tmp[i] = T("");
           ++_size;
         }
        delete [] _data;
        _data = tmp;
         _size = _capacity = n;
       } else {
         for (size_t i = _size; i < n; i++) {
           _data[i] = T("");
           ++_size;
         }
       }
     }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
