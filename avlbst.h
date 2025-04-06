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

  // rotation functions
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);

    
  // rebalance functions
    void insertFix(AVLNode<Key, Value>* node, int8_t diff);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{

  // if tree is empty -> make a new root
  // find where to put the new node like a regular bst
  // add the node and update balance factors
  // if tree gets unbalanced fix it with rotations
  //

  if (this->root_ == nullptr) {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      return;
  }

  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value>* parent = nullptr;
  bool check = false;

  while (curr != nullptr) {

      parent = curr;

      if (new_item.first < curr->getKey()) {
          curr = curr->getLeft();
          check = true;

      } 

      else if (curr->getKey() < new_item.first) {
          curr = curr->getRight();
          check = false;

      } 

      else {
          curr->setValue(new_item.second);
          return;

      }
  }


  AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
  
  if (check) {
      parent->setLeft(newNode);
      if (parent->getBalance() == 0) {
          parent->setBalance(1);
          insertFix(parent, 1);

      }
      else {
          parent->setBalance(0);
      }
  } 
  
  else {
      parent->setRight(newNode);
      if (parent->getBalance() == 0) {
          parent->setBalance(-1);
          reBalanceInsert(parent, -1);
      }
      else {
          parent->setBalance(0);
      }
  }
}


// left rotation around the given node
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {

    AVLNode<Key, Value>* right = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();
    
    node->setRight(right->getLeft());

    if (right->getLeft() != nullptr) {

        right->getLeft()->setParent(node);

    }
    
    right->setLeft(node);
    node->setParent(right);
    right->setParent(parent);
    

    if (parent == nullptr) {
        this->root_ = right;
    } 

    else if (parent->getLeft() == node) {
        parent->setLeft(right);

    } 

    else {
        parent->setRight(right);

    }
}

// right rotation around the given node
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {

  AVLNode<Key, Value>* left = node->getLeft();
  AVLNode<Key, Value>* parent = node->getParent();
  
  node->setLeft(left->getRight());


  if (left->getRight() != nullptr) {
      left->getRight()->setParent(node);
  }
  
  left->setRight(node);
  node->setParent(left);
  left->setParent(parent);
  
  if (parent == nullptr) {
      this->root_ = left;

  } 

  else if (parent->getLeft() == node) {
      parent->setLeft(left);

  } 

  else {

      parent->setRight(left);

  }

}

// need to fix balance issues after insertion
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* node, int8_t diff)
{

  // checks if we need to fix anything balance is 2 or -2
  //
  // what rotation do we need
  // single left/right rotation
  // double rotation
  // update all balance factors after rotation
  //
  // 
  if (node == nullptr || node->getBalance() == 0) {
      return;
  }

  AVLNode<Key, Value>* parent = node->getParent();
  
  int8_t odiff = 0;

  if (parent) {
      if (parent->getLeft() == node) {
          odiff = 1;
      } else {
          odiff = -1;
      }
  }



  if (node->getBalance() == 2) {

      AVLNode<Key, Value>* leftChild = node->getLeft();
      if (leftChild == nullptr) return;
      
      if (leftChild->getBalance() >= 0) {
          rotateRight(node);
          node->setBalance(0);
          node->getParent()->setBalance(0);
      } 
      else {

          AVLNode<Key, Value>* other = leftChild->getRight();
          if (other == nullptr) return;
          
          rotateLeft(leftChild);
          rotateRight(node);
          
          if (other->getBalance() == 1) {
              node->setBalance(-1);
              leftChild->setBalance(0);

          } 
          
          else if (other->getBalance() == -1) {
              node->setBalance(0);
              leftChild->setBalance(1);

          } 
          else {
              node->setBalance(0);
              leftChild->setBalance(0);

          }

          other->setBalance(0);
      }

      return; 
  }


  else if (node->getBalance() == -2) {
      AVLNode<Key, Value>* right = node->getRight();
      if (right == nullptr) { return; } 
      
      if (right->getBalance() <= 0) {
          rotateLeft(node);
          node->setBalance(0);
          node->getParent()->setBalance(0);
      } 

      else {
      AVLNode<Key, Value>* lgChild = right->getLeft();
          if (lgChild == nullptr) { return;  }
          
          rotateRight(right);
          rotateLeft(node);
          
          if (lgChild->getBalance() == -1) {
              node->setBalance(1);
              right->setBalance(0);
          } 

          else if (lgChild->getBalance() == 1) {
              node->setBalance(0);
              right->setBalance(-1);
          } 

          else {
              node->setBalance(0);
              right->setBalance(0);
          }

          lgChild->setBalance(0);
      }

      return; 

  }
  
  if (parent) {
      parent->updateBalance(odiff);

      if (parent->getBalance() != 0) {
          insertFix(parent, odiff);

      }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

// delete a node
//
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  // find node we want to delete can use internal find
  // if it has 2 children, swap with predecessor
  // actually remove the node
  // fix balance issues
  // clean

  AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  
  // basic check
  if (node == nullptr) {
      return;
  }
  
  // check if no children
  if (node->getLeft() != nullptr && node->getRight() != nullptr) {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
      nodeSwap(node, pred);
  }

  AVLNode<Key, Value>* parent = node->getParent();

  AVLNode<Key, Value>* child = nullptr;
  
  // check case for one child
  if (node->getLeft() != nullptr) {
    child = node->getLeft();
  } else {
    child = node->getRight();
  }

  int diff = 0;

  if (parent != nullptr) {

      if (parent->getLeft() == node) {
          diff = -1; 
        } 
      else {
          diff = 1;


      }
  }

  if (child != nullptr) {
      child->setParent(parent);
  }

  if (parent == nullptr) {
      this->root_ = child;
  } else if (parent->getLeft() == node) {
      parent->setLeft(child);
  } else {
      parent->setRight(child);
  }

  delete node;

  if (parent != nullptr) {
      removeFix(parent, diff);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{

  // will work similar to insert fix
  //
  // deals with different balance cases
  // needs to deal with deleted side being short
  // might need to go up the tree to fix more nodes
  //

  if (node == nullptr) return;

  AVLNode<Key, Value>* parent = node->getParent();
  int8_t odiff = 0;
  
  if (parent != nullptr) {
      if (parent->getLeft() == node) {
          odiff = -1;
      } else {
          odiff = 1;
      }
  }

  node->updateBalance(diff);
  
  if (node->getBalance() == 2) {
      AVLNode<Key, Value>* left = node->getLeft();
      
      if (left->getBalance() >= 0) {
          rotateRight(node);
          
          if (left->getBalance() == 0) {
              node->setBalance(1);
              left->setBalance(-1);
              return;

          } 
          
          else {
              node->setBalance(0);
              left->setBalance(0);
              if (parent) removeFix(parent, odiff);

          }
      } 

      else {
          AVLNode<Key, Value>* rgChild = left->getRight();
          rotateLeft(left);
          rotateRight(node);
          
          if (rgChild->getBalance() == 1) {
              node->setBalance(-1);
              left->setBalance(0);

          } 
          
          else if (rgChild->getBalance() == -1) {
              node->setBalance(0);
              left->setBalance(1);
          } 
          
          else {
              node->setBalance(0);
              left->setBalance(0);
          }
          rgChild->setBalance(0);
          
          if (parent) {
            removeFix(parent, odiff);
          }
      }
  }

  else if (node->getBalance() == -2) {
      AVLNode<Key, Value>* right = node->getRight();
      
      if (right->getBalance() <= 0) {
          rotateLeft(node);
          
          if (right->getBalance() == 0) {
              node->setBalance(-1);
              right->setBalance(1);
              return;

          } 

          else {
              node->setBalance(0);
              right->setBalance(0);

              if (parent) {
                removeFix(parent, odiff);
              }
          }

      } 

      else {
          AVLNode<Key, Value>* lgChild = right->getLeft();
          rotateRight(right);
          rotateLeft(node);
          
          if (lgChild->getBalance() == -1) {
              node->setBalance(1);
              right->setBalance(0);
          } 

          else if (lgChild->getBalance() == 1) {
              node->setBalance(0);
              right->setBalance(-1);
          } 

          else {
              node->setBalance(0);
              right->setBalance(0);
          }
          lgChild->setBalance(0);
          
          if (parent) {
            removeFix(parent, odiff);
          }
      }
  } 

  else if (node->getBalance() == 1 || node->getBalance() == -1) {
      return;
  } 

  else {
      if (parent && diff != 0) {
          removeFix(parent, odiff);
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
