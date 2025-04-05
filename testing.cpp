#include <iostream>
#include "bst.h"
#include "print_bst.h"

using namespace std;

int main() {
    // Create a BST of integers
    BinarySearchTree<int, string> bst;
    
    cout << "\n=== Testing Empty Tree ===" << endl;
    bst.print();  // This will use print_bst.h's functionality
    
    cout << "\n=== Testing Single Insert (root) ===" << endl;
    bst.insert(std::make_pair(5, "five"));
    bst.print();
    
    cout << "\n=== Testing Multiple Inserts ===" << endl;
    bst.insert(std::make_pair(3, "three"));
    bst.insert(std::make_pair(7, "seven"));
    bst.insert(std::make_pair(2, "two"));
    bst.insert(std::make_pair(4, "four"));
    bst.insert(std::make_pair(6, "six"));
    bst.insert(std::make_pair(8, "eight"));
    bst.print();
    
    cout << "\n=== Testing Value Update ===" << endl;
    cout << "Updating value of key 5 to 'FIVE'" << endl;
    bst.insert(std::make_pair(5, "FIVE"));
    bst.print();

    cout << "\n=== Testing Find ===" << endl;
    auto it = bst.find(5);
    if (it != bst.end()) {
        cout << "Found key 5 with value: " << it->second << endl;
    }

    cout << "\n=== Testing Remove ===" << endl;
    cout << "Removing key 3..." << endl;
    bst.remove(3);
    bst.print();

    cout << "\n=== Testing Clear ===" << endl;
    bst.clear();
    bst.print();

    return 0;
}
