#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int pathDepth(Node* root);


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

