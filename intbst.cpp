// intbst.cpp
// Implements class IntBST
// Noah Ruderman, 4/21/26

#include "intbst.h"

#include <iostream>
using std::cout;
using namespace std; //adding this because I am used to it

// constructor sets up empty tree
IntBST::IntBST() { 
    root = NULL; //since we are already doing IntBST:: it's already accessing the class which contains the root pointer, since it is defined as a pointer
    // doing root instead of *root gives us the pointer itself, not the key in the root.
}

// destructor deletes all nodes, using the recursive clear helper function
IntBST::~IntBST() {
    clear(root);
}

// recursive helper for destructor
void IntBST::clear(Node *n) {
    if (n) { // checks if the pointer to the node exists, then go in and clear the left, clear the right, and delete the node itself
        // this way also if n is null (i.e. we go into the left child but it doesn't exist) then nothing happens and it doesn't break
        clear(n->left);
        clear(n->right);
        delete n;
    }
}

// insert value in tree; return false if duplicate
bool IntBST::insert(int value) {
    return insert(value, root); // calls the recursive helper function, starting with the root node
}

// recursive helper for insert (assumes n is never 0)
bool IntBST::insert(int value, Node*& n) { //must be passed by reference otherwise we will never modify the original BST, claude told me
    // base case, we are at a nullptr so insert
    if (!n) {
        n = new Node(value); //make a heap allocated Node, otherwise it wouldn't exist after function call
        return true;
    }

    if (n) { //only insert if there is a parent to go into, this is lowk redundant but i started with it so i'll keep it
        if (n->info == value) { // value is a duplicate
            return false;
        } else if (n->info > value) {
            return insert(value, n->left); //recurse on the left as we are smaller
        } else {
            return insert(value, n->right); //recurse on the right as we are larger
        }
    }
}

// print tree data pre-order
void IntBST::printPreOrder() const {
    printPreOrder(root); //it knows what root is because it is the public function
}

// recursive helper for printPreOrder()
void IntBST::printPreOrder(Node *n) const {
    // preorder is the "insertion" method, so we go parent, left, right, recursing in that order

    //base case
    if (!n) {
        return; //if there is no node, then we are done and we can return
    }

    cout << n->info << " "; // print the parent first, then go into the left and right
    printPreOrder(n->left);
    printPreOrder(n->right);
}

// print tree data in-order, with helper
void IntBST::printInOrder() const {
    printInOrder(root); //calls the recursive helper function, starting with the root node
}
void IntBST::printInOrder(Node *n) const {
    //base case
    if (!n) {
        return; //if there is no node, then we are done and we can return
    }

    printInOrder(n->left); //recurse on the left
    cout << n->info << " "; // print the parent
    printInOrder(n->right); //recurse on the right
}

// prints tree data post-order, with helper
void IntBST::printPostOrder() const {
    printPostOrder(root); //calls the recursive helper function, starting with the root node
}

void IntBST::printPostOrder(Node *n) const {
    // this one is the "deletion" method where we kill left, right, and then the root

    //base case
    if (!n) {
        return; //if there is no node, then we are done and we can return
    }

    printPostOrder(n->left); //recurse on the left
    printPostOrder(n->right); //recurse on the right
    cout << n->info << " "; // print the parent last, after the left and right
}

// return sum of values in tree
int IntBST::sum() const {
    return sum(root); 
}

// recursive helper for sum
int IntBST::sum(Node *n) const {
    // base case
    if (!n) {
        return 0; //if there is no node, then the sum is 0 so we can still sum with the rest and not mess anything up
    }

    return n->info + sum(n->left) + sum(n->right); // sum is the parent plus the left and right sums
}

// return count of values
int IntBST::count() const {
    return count(root);
}

// recursive helper for count
int IntBST::count(Node *n) const {
    // base case
    if (!n) {
        return 0; //sum with the rest, if there isn't a node then we have 0!
    }

    return 1 + count(n->left) + count(n->right);
}

// IMPLEMENT THIS FIRST: returns the node for a given value or NULL if none exists
// Parameters:
// int value: the value to be found
// Node* n: the node to start with (for a recursive call)
// Whenever you call this method from somewhere else, pass it
// the root node as "n"
IntBST::Node* IntBST::getNodeFor(int value, Node* n) const{
    // base case
    //1st base case, we didn't find
    if (!n) {
        return NULL; //if there isn't anything there, we didn't find the value
    }
    //second base case, we got it
    if (n->info == value) {
        return n;
    }
    // recursive cases
    if (n->info > value) {
        return getNodeFor(value, n->left); //recurse on the left as we are smaller
    } else {
        return getNodeFor(value, n->right); //recurse on the right as we are larger
    }
}

// returns true if value is in the tree; false if not
bool IntBST::contains(int value) const {
    return getNodeFor(value, root) != NULL; //if we get a node back, then it is in the tree, otherwise it is not.
    // != Null is saying doesn't not exist or it that it DOES exist, so in that case we return true, and false otherwise (Null != NUll is false)
}


// returns the Node containing the predecessor of the given value
IntBST::Node* IntBST::getPredecessorNode(int value) const{
    Node* n = getNodeFor(value, root); //get the node for the value
    if (!n) {
        return NULL; //if there is no node for the value, then there is no predecessor
    }

    // case 1: left subtree exists, predecessor is the rightmost node of the left subtree
    if (n->left) {
        n = n->left; //start at the left subtree
        while (n->right) { //stop one step early so n is still valid
            n = n->right;
        }
        return n; //rightmost node of left subtree = predecessor
    }

    // case 2: no left subtree, go up until we find a predecessor in their left tree
    Node* parent = n->parent;
    while (parent && n == parent->left) { //if we are always a left child, there is no predecessor
        n = parent;
        parent = parent->parent;
    }
    return parent; // returns null if we are the leftmost node, otherwise the parent WHICH IS PREDECCESOR if n is a right child, as this is after loop breaks
}

// returns the predecessor value of the given value or 0 if there is none
int IntBST::getPredecessor(int value) const{
    Node* predecessorNode = getPredecessorNode(value); //get the predecessor node
    if (!predecessorNode) {
        return 0; //if there is no predecessor node, then there is no predecessor value
    }
    return predecessorNode->info; //otherwise return the info of the predecessor node
}

// returns the Node containing the successor of the given value
IntBST::Node* IntBST::getSuccessorNode(int value) const{
    //copied from get predecessor node and changed left to right, since successor is the right child

    Node* n = getNodeFor(value, root); 
    if (!n) {
        return NULL; 
    }
    return n->right;
}

// returns the successor value of the given value or 0 if there is none
IntBST::Node* IntBST::getSuccessorNode(int value) const{
    //basically same as get predecessor node but with right and left switched, since successor is the next LARGEST val

    Node* n = getNodeFor(value, root);
    if (!n) {
        return NULL; 
    }

    // right subtree is case 1 where the successor is the leftmost value in it
    if (n->right) {
        n = n->right; 
        while (n->left) { 
            n = n->left;
        }
        return n; 
    }

    // case 2 going up until our value is a left child of one of the parents
    Node* parent = n->parent;
    while (parent && n == parent->right) { 
        n = parent;
        parent = parent->parent;
    }
    return parent;
}

// deletes the Node containing the given value from the tree
// returns true if the node exist and was deleted or false if the node does not exist
bool IntBST::remove(int value) {
    // find the node to delete
    Node* target = getNodeFor(value, root);
    
    if (!target) {
        return false; // case 1, target doesn't exist
    }

    // if node has 2 children
    if (target->left != NULL && target->right != NULL) {
        Node* successor = getSuccessorNode(target->info);
        
        target->info = successor->info;
        
        // now we can delete the successor as it won't have a left child
        target = successor; 
    }

    // node has one or no children, we can delete it directly
    Node* child = NULL;

    if (target->left != NULL) {
        child = target->left;
    } else {
        child = target->right;
    }

    // update child's parent pointer if child exists
    if (child != NULL) {
        child->parent = target->parent;
    }

    //update where the parent points to 
    if (target->parent == NULL) {
        // if our target has no parent it is the root
        root = child;
    } else if (target == target->parent->left) { //if traget was a left child...
        target->parent->left = child;
    } else { //target is a right child
        target->parent->right = child;
    }

    //delete the isolated node now that it won't mess up any connections in the tree
    delete target;
    return true;
}