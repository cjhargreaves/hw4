#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

  if (this->root_ == nullptr) {
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    return;
  }

  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);

  AVLNode<Key, Value>* parent = nullptr;

  while (curr != nullptr) {
    if (new_item.first == curr->getKey()) {
      curr->setValue(new_item.second);
      return;
    }

    parent = curr;

    if (new_item.first < curr->getKey()) {
      curr = curr->getLeft();
    } else if (new_item.first > curr->getKey()) {
      curr = curr->getRight();
    }
  }

  AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);

  if (new_item.first < parent->getKey()) {
    parent->setLeft(newNode);
  } else if (new_item.first > parent->getKey()) {
    parent->setRight(newNode);
  }

  insertFix(newNode, parent);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {

  AVLNode<Key, Value>* right = node->getRight();

  right->setParent(node->getParent());

  if (node->getParent() != nullptr) {
    if (node->getParent()->getLeft() == node) {
      node->getParent()->setLeft(right);
    } else if (node->getParent()->getRight() == node) {
      node->getParent()->setRight(right);
    }
  } else {
    this->root_ = right;
  }

  node->setRight(right->getLeft());

  if (right->getLeft() != nullptr) {
    right->getLeft()->setParent(node);
  }

  right->setLeft(node);
  node->setParent(right);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {

  AVLNode<Key, Value>* left = node->getLeft();

  left->setParent(node->getParent());

  if (node->getParent() != nullptr) {
    if (node->getParent()->getLeft() == node) {
      node->getParent()->setLeft(left);
    } else if (node->getParent()->getRight() == node) {
      node->getParent()->setRight(left);
    }
  } else {
    this->root_ = left;
  }

  node->setLeft(left->getRight());

  if (left->getRight() != nullptr) {
    left->getRight()->setParent(node);
  }

  left->setLeft(node);
  node->setParent(left);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent) {

  if (parent == nullptr || parent->getParent() == nullptr) {
    return;
  }

  AVLNode<Key, Value>* grandparent = parent->getParent();

  if (parent == grandparent->getLeft()) {
    grandparent->updateBalance(-1);

    if (grandparent->getBalance() == 0) {
      return;
    }

    else if (grandparent->getBalance() == -1) {
      insertFix(parent, grandparent);
    }

    else if (grandparent->getBalance() == -2) {

      if (node == parent->getLeft()) {
        
        rotateRight(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
      }
      
      else {
        rotateLeft(parent);
        rotateRight(grandparent);

        if (node->getBalance() == -1) {

          parent->setBalance(0);
          grandparent->setBalance(1);
          node->setBalance(0);
        }
        else if (node->getBalance() == 0) {
          parent->setBalance(0);
          grandparent->setBalance(0);
          node->setBalance(0);
        }
        else if (node->getBalance() == 1) {
          parent->setBalance(-1);
          grandparent->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  } 

  else {
    
    grandparent->updateBalance(1);

    if (grandparent->getBalance() == 0) {
      return;
    }

    else if (grandparent->getBalance() == 1) {
      insertFix(parent, grandparent);
    }

    else if (grandparent->getBalance() == 2) {

      if (node == parent->getRight()) {
        rotateLeft(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
      }

      else {
        rotateRight(parent);
        rotateLeft(grandparent);

        if (node->getBalance() == 1) {
          parent->setBalance(0);
          grandparent->setBalance(-1);
          node->setBalance(0);
        }

        else if (node->getBalance() == 0) {
          parent->setBalance(0);
          grandparent->setBalance(0);
          node->setBalance(0);
        }

        else if (node->getBalance() == -1) {
          parent->setBalance(1);
          grandparent->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
  //

  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);

  while (curr != nullptr) {

    if (key == curr->getKey()) {
      break;
    }

    else if (key < curr->getKey()) {
      curr = curr->getLeft();
    }

    else {
      curr = curr->getRight();
    }
  }

  if ( curr == nullptr ) { return; }

  // two children, swap with predecessor
  //
  if (curr->getLeft() != nullptr && curr->getRight() != nullptr) {
    AVLNode<Key, Value>* predecessor = static_cast<AVLNode<Key, Value>*>(this->predecessor(curr));
    nodeSwap(curr, predecessor);
  }

  // curr has at most one child now
  //
  AVLNode<Key, Value>* parent = curr->getParent();

  bool isLeft = (parent != nullptr && parent->getLeft() == curr);

  // get child to replace current if exist
  //
  AVLNode<Key, Value>* child = curr->getLeft();
  if (child == nullptr) {
    child = curr->getRight();
  }

  if (parent == nullptr) {
    this->root_ = child;
  }

  else if (isLeft) {
    parent->setLeft(child);
  }

  else {
    parent->setRight(child);
  }

  if (child != nullptr) {
    child->setParent(parent);
  }

  if (parent != nullptr) {
    int8_t diff;

    if (isLeft) {
      diff = 1;
    } else {
      diff = -1;
    }

    removeFix(parent, diff);
  }


  delete curr;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff) {


  if (node == nullptr) { return; }

  AVLNode<Key, Value>* parent = node->getParent();
  int8_t ndiff = 0;

  if (parent != nullptr) {
    if (parent->getLeft() == node) {
      ndiff = 1;
    } else {
      ndiff = -1;
    }
  }

  if (diff == -1) {

    if (node->getBalance() + diff == -2) {
      AVLNode<Key, Value>* child = node->getLeft();

      if (child->getBalance() == -1) {
        rotateRight(node);
        node->setBalance(0);
        child->setBalance(0);
        removeFix(parent, ndiff);
      }

      else if (child->getBalance() == 0) {
        rotateRight(node);
        node->setBalance(-1);
        child->setBalance(1);

      }

      else if (child->getBalance() == 1) {
        AVLNode<Key, Value>* grandchild = child->getRight();
        rotateLeft(child);
        rotateRight(node);

        if (grandchild->getBalance() == 1) {

          node->setBalance(0);
          child->setBalance(-1);
          grandchild->setBalance(0);
        }

        else if (grandchild->getBalance() == 0) {
          node->setBalance(0);
          child->setBalance(0);
          grandchild->setBalance(0);
        }

        else if (grandchild->getBalance() == -1) {
          node->setBalance(1);
          child->setBalance(0);
          grandchild->setBalance(0);
        }

        removeFix(parent, ndiff);
      }
    }

    else if (node->getBalance() + diff == -1) {
      node->setBalance(-1);

    }

    else if (node->getBalance() +diff == 0) {
      node->setBalance(0);
      removeFix(parent, ndiff);
    }
  }

  else if (diff == 1) {
    if (node->getBalance() + diff == 2) {
      AVLNode<Key, Value>* child = node->getRight();

      if (child->getBalance() == 1) {
        rotateLeft(node);
        node->setBalance(0);
        child->setBalance(0);
        removeFix(parent, ndiff);
      }

      else if (child->getBalance() == 0) {
        rotateLeft(node);
        node->setBalance(1);
        child->setBalance(-1);

      }

      else if (child->getBalance() == -1) {
        AVLNode<Key, Value>* grandchild = child->getLeft();
        rotateRight(child);
        rotateLeft(node);

        if (grandchild->getBalance() == -1) {
          node->setBalance(0);
          child->setBalance(1);
          grandchild->setBalance(0);

        }

        else if (grandchild->getBalance() == 0) {
          node->setBalance(0);
          child->setBalance(0);
          grandchild->setBalance(0);
        }

        else if (grandchild->getBalance() == 1) {
          node->setBalance(-1);
          child->setBalance(0);
          grandchild->setBalance(0);
        }

        removeFix(parent, ndiff);
      }
    }

    else if (node->getBalance() + diff == 1) {
      node->setBalance(1);
    }

    else if (node->getBalance() + diff == 0) {
      node->setBalance(0);
      removeFix(parent, ndiff);

    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
