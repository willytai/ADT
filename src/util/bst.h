/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>
#include <iomanip>
#include <dlist.h>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   BSTreeNode(const T& x, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0, BSTreeNode<T>* p = 0) :
              _data(x), _left(l), _right(r), _parent(p) {}

   BSTreeNode<T>* insert(const T& x, BSTreeNode<T>* node, BSTreeNode<T>* p) const {
     if (!node)
       return new BSTreeNode<T>(x, 0, 0, p);
     if (x > node->_data)
       node->_right = insert(x, node->_right, node);
     else
       node->_left = insert(x, node->_left, node);
   }

   T                _data;
   BSTreeNode<T>*   _left;
   BSTreeNode<T>*   _right;
   BSTreeNode<T>*   _parent;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!

 public:
   BSTree() : _size(0), _isSorted(true) { makeroot(); }
   ~BSTree(){ clear(); delete _root; }

   class iterator {

        friend class BSTree;

     public:
        iterator(BSTreeNode<T>* n= 0): _node(n) {}
        iterator(const iterator& i) : _node(i._node) {}
        ~iterator() {} // Should NOT delete _node

        const T& operator * () const { return _node->_data; }
        T& operator * () { return _node->_data; }

        iterator& operator ++ () {
          if (_node->_parent == _node) return *this;
          if (_node->_right)
            _node = getleft(_node->_right);
          else {
            BSTreeNode<T>* tmp = _node;
            while (tmp == tmp->_parent->_right)
              tmp = tmp->_parent;
            _node = tmp->_parent;
          }
          return *this;
        }
        iterator operator ++ (int) {
          if (_node->_parent == _node) return *this;
          iterator ret = *this;
          if (_node->_right)
            _node = getleft(_node->_right);
          else {
            BSTreeNode<T>* tmp = _node;
            while (tmp == tmp->_parent->_right)
              tmp = tmp->_parent;
            _node = tmp->_parent;
          }
          return ret;
        }

        iterator& operator -- () {
          if (_node->_parent == _node) {
            _node = getright(_node->_right);
            return *this;
          }
          if (_node->_left)
            _node = getright(_node->_left);
          else {
            BSTreeNode<T>* tmp = _node;
            while (tmp == tmp->_parent->_left)
              tmp = tmp->_parent;
            if (tmp->_parent == tmp) return *this;
            _node = tmp->_parent;
          }
          return *this;
        }
        iterator operator -- (int) {
          iterator ret(*this);
          if (_node->_parent == _node) {
            _node = getright(_node->_right);
            return ret;
          }
          if (_node->_left)
            _node = getright(_node->_left);
          else {
            BSTreeNode<T>* tmp = _node;
            while (tmp == tmp->_parent->_left)
              tmp = tmp->_parent;
            if (tmp->_parent == tmp) return *this;
            _node = tmp->_parent;
          }
          return ret;
        }

        iterator& operator = (const iterator& i) {
          if (this == &i) return *this;
          _node = i._node;
          return *this;
        }

        bool operator != (const iterator& i) const { return _node != i._node; }
        bool operator == (const iterator& i) const { return _node == i._node; }

     private:
       BSTreeNode<T>* _node;

       BSTreeNode<T>* getleft(BSTreeNode<T>* t) {
         if (t->_left == NULL)
           return t;
         getleft(t->_left);
       }

       BSTreeNode<T>* getright(BSTreeNode<T>* t) {
         if (t->_right == NULL)
           return t;
         getright(t->_right);
       }
   };

    iterator begin() const { return getbegin(_root); }
    iterator end() const { return _end;}

    bool empty() const { return !_root; }
    size_t size() const { return _size; }
    void insert(const T& x) {
      if (!_size) {
        _root->_data = x;
        ++_size;
        _end = new BSTreeNode<T>(T(""), _root, _root, 0);
        _end->_parent = _end;
        _root->_parent = _end;
        return;
      }
      _root = _root->insert(x, _root, 0);
      ++_size;
    }
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }
    bool erase(iterator pos) {
      if (empty() || pos == end()) return false;

      BSTreeNode<T>* bye;
      BSTreeNode<T>* orphan = 0;

      if ((pos._node->_right) && (pos._node->_left)) {
        bye = successor(pos._node);
        orphan = bye->_right;
      }
      else {
        bye = pos._node;
        orphan = (bye->_right ? bye->_right : bye->_left);
      }

      if (orphan != NULL)
        orphan->_parent = bye->_parent;

      if (bye == _root) {
        _root = orphan;
        _end->_right = _end->_left = orphan;
      }
      else if (determinchild(bye))
        bye->_parent->_right = orphan;
      else
        bye->_parent->_left = orphan;

      if (bye != pos._node)
        *pos = bye->_data;

      delete bye;
      --_size;
      if (!_size) makeroot();
      return true;
    }
    bool erase(const T& x) {
      iterator li = begin();
      for (; li != end(); ++li) {
        if (x == *li)
          return erase(li);
      }
      return false;
    }
    void clear() {
      if (!_root) return;
      release(_root);
      makeroot();
      _size = 0;
    }
    void print() const {
   	  debug ("" , _root);
    }
    void sort() {}

  private:
    BSTreeNode<T>* _root;
    BSTreeNode<T>* _end;
    size_t _size;
    mutable bool _isSorted;

    BSTreeNode<T>* getbegin (BSTreeNode<T>* node) const { // smallest element
      if (!node->_left)
        return node;
      getbegin(node->_left);
    }
    BSTreeNode<T>* getlast(BSTreeNode<T>* node) const { // largest element
      if (!node->_right)
        return node;
      getlast(node->_right);
    }
    BSTreeNode<T>* successor(BSTreeNode<T>* t) { return getbegin(t->_right); }
    bool determinchild(BSTreeNode<T>* n) { // true if n is right child
      if (n->_parent->_right == n) return true;
      return false;
    }
    void makeroot() {
      _root = new BSTreeNode<T>(T(""), 0, 0, 0);
      _end = _root;
    }
    void release(BSTreeNode<T>*& t) {
      if (t->_left)
        release(t->_left);
      if (t->_right)
        release(t->_right);
      delete t;
      t = 0;
    }
    void debug (string t, BSTreeNode<T>* n) const {
    	if (n != _end) {
     		cerr << t << n->_data << endl;
     		if (n->_left != 0 && n->_left != _end) debug(t + "  ", n->_left);
     		else cerr << (t + "  ") << "[0]" << endl;
     		if (n->_right != 0 && n->_right != _end) debug(t + "  ", n->_right);
     		else cerr << (t + "  ") << "[0]" <<endl;
   		}
   		else cerr << t << "[0]" << endl;
   	}
};

#endif // BST_H
