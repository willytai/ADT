/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() : _isSorted(0) {
      _head = new DListNode<T>(T(""));
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {
        if (_node->_data == T("")) return *this;
        _node = _node->_next;
        return *this;
      }
      iterator operator ++ (int) {
        if (_node->_data == T("")) return *this;
        iterator ret = *this;
        this->_node = this->_node->_next;
        return ret;
      }
      iterator& operator -- () {
        if (_node->_prev->_data == T("")) return *this;
        _node = _node->_prev;
        return *this;
      }
      iterator operator -- (int) {
        if (_node->_prev->_data == T("")) return *this;
        iterator ret = *this;
        this->_node = this->_node->_prev;
        return ret;
      }

      iterator& operator = (const iterator& i) {
        if (this != &i) {
          _node = i._node;
        }
        return *this;
      }

      bool operator != (const iterator& i) const {
        return _node != i._node;
      }
      bool operator == (const iterator& i) const {
        return _node == i._node;
      }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return _head; }
   iterator end() const { return _head->_prev; }
   bool empty() const {
     if (_head->_next == _head)
       return true;
     return false;
   }
   size_t size() const {
     DListNode<T>* tmp = _head;
     size_t count = 0;
     while (tmp->_next != _head){
        tmp = tmp -> _next;
        ++count;
     }
     return count;
   }

   void push_back(const T& x) {
     _isSorted = false;
     if (_head->_prev == _head) {
       DListNode<T>* tmp = new DListNode<T>(x, _head, _head);
       _head->_prev = _head->_next = tmp;
       _head = tmp;
     }
     else {
       DListNode<T>* tmp = new DListNode<T>(x, _head->_prev->_prev, _head->_prev);
       _head->_prev->_prev->_next = tmp;
       _head->_prev->_prev = tmp;
     }
   }
   void pop_front() {
     if (empty()) return;
     _head->_prev->_next = _head->_next;
     _head->_next->_prev = _head->_prev;
     DListNode<T>* tmp = _head;
     delete _head;
     _head = tmp->_next;
   }
   void pop_back() {
     if (empty()) return;
     DListNode<T>* bye = _head->_prev->_prev;
     bye->_prev->_next = bye->_next;
     bye->_next->_prev = bye->_prev;
     delete bye;
   }

   // return false if nothing to erase
   bool erase(iterator pos) {
     if (pos == begin()) {
       pop_front();
       return true;
     }
     if (pos == end()) return false;

     DListNode<T>* bye = pos._node;
     bye->_prev->_next = bye->_next;
     bye->_next->_prev = bye->_prev;

     delete bye;
     _isSorted = false;
     return true;
   }
   bool erase(const T& x) {
     iterator li = begin();
     while (li != end()) {
       if (*li == x) {
         _isSorted = false;
         return erase(li);
       }
       ++li;
     }
     return false;
   }

   void clear() { // delete all nodes except for the dummy node
     while (_head->_next != _head)
        pop_front();
   }

   void sort() { // too slow for some unknown fucking reason
     if (empty() || _isSorted) return;
     iterator li = begin();
     for (; li != _head->_prev->_prev; ++li) {
       for (iterator lp = li; lp != end(); ++lp)
         if (*li > *lp) ::swap(*li, *lp);
     }
     _isSorted = true;
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
