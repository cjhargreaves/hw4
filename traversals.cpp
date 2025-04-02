#include <iostream>
#include "equal-paths.h"

using namespace std;

void preOrder(Node* root);
void postOrder(Node* root);
void inOrder(Node* root);

bool equalPaths(Node* root);
int pathDepth(Node* root);

int main() {
  
  Node* root = new Node(3);

  // first layer
  root->left = new Node(5);
  root->right = new Node (4);

  // second layer
  root->left->left = new Node(6);
  root->left->right = new Node(7);

  root->right->left = new Node(8);
  root->right->right = new Node(9);

  // adding extra depth
  root->right->right->right = new Node(10);
  root->right->right->right->right = new Node(11);
  
  cout << "pre order" << endl;
  preOrder(root);
  cout << endl << endl << "post order" << endl;
  postOrder(root);
  cout << endl << endl << "in order" << endl;
  inOrder(root);

  cout << endl << endl;
  cout << "equal?" << endl;
  cout << equalPaths(root) << endl << endl;

}

void preOrder(Node* root) {

  if (root == nullptr) return;

  cout << root->key << endl;
  preOrder(root->left);
  preOrder(root->right);
}

void postOrder(Node* root) {

  if (root == nullptr) return;

  postOrder(root->left);
  postOrder(root->right);
  cout << root->key << endl;

}

void inOrder(Node* root) {
  
  if (root == nullptr) return;

  inOrder(root->left);
  cout << root->key << endl;
  inOrder(root->right);
}


bool equalPaths(Node * root)
{
  
  if (root == nullptr) return true;

  if (root->right == nullptr && root->left == nullptr) return true;
  
  if (root->left == nullptr) {
    return equalPaths(root->right);
  }

  if (root->right == nullptr) {
    return equalPaths(root->left);
  }

  return equalPaths(root->right) && 
         equalPaths(root->left) && 
         (pathDepth(root->right) == pathDepth(root->left));

  
}

int pathDepth(Node* root) {

  // if root is null then we just return 0
  if (root == nullptr) return 0;

  // if leaf node
  if (root->left == nullptr && root->right == nullptr) return 1;
  
  if (root->left == nullptr) {
    return 1 + pathDepth(root->right);
  }

  if (root->right == nullptr) {
    return 1 + pathDepth(root->left);
  }

  return 1 + max(pathDepth(root->left), pathDepth(root->right));


}

