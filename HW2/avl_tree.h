// Tolga Toker 32639
// CS300 Homework 2
// AVL Tree Implementation header file

#ifndef avl_h
#define avl_h
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct DocumentItem {
    string documentName;
    int count;
    DocumentItem(string docName, int c) : documentName(docName), count(c) {}
};
struct WordItem {
    string word;
    // List of DocumentItem’s. In order to keep the documents
    //you can again use the BST that you are going to
};



template <class Key, class Value>
class AVLSearchTree;

template <class Key, class Value>
class AVLNode
{
    Key word;
    Value value;
    AVLNode *left;
    AVLNode *right;
    int height;
    AVLNode( const Key & theElement, const Value & theVal, AVLNode *lt, AVLNode *rt, int h = 0)
            : word( theElement ), value( theVal ), left( lt ), right( rt ), height(h ) { }

    friend class AVLSearchTree<Key, Value>;
};

template<class Key,class Value>
class AVLSearchTree{
private:

    AVLNode<Key,Value> * root;
    const Value ITEM_NOT_FOUND;
    const Value & elementAt( AVLNode<Key,Value> *t ) const;

    AVLNode<Key,Value>* insert( const Key& x,const Value&v, AVLNode<Key,Value>* & t );
    AVLNode<Key,Value>* remove(const Key& x,AVLNode<Key,Value>* & t );

    AVLNode<Key,Value> * findMin( AVLNode<Key,Value> *t ) const;
    AVLNode<Key,Value>* findMax(AVLNode<Key,Value> *t ) const;
    AVLNode<Key,Value>* find( const Key & x, AVLNode<Key,Value>*t );
    void makeEmpty( AVLNode<Key,Value> * & t ) const;
    void printTree(AVLNode<Key,Value> *t) const;
    AVLNode<Key,Value> * clone( AVLNode<Key,Value>*t) const;

    //helper functions that are need for AVL implementation
    int height(AVLNode<Key,Value>*t);
    int max(int lhs, int rhs) const;
    AVLNode<Key,Value>* rightRotate(AVLNode<Key,Value>*t);
    AVLNode<Key,Value>* leftRotate(AVLNode<Key,Value>*t);
public:
    // CONSTRUCTORS
    explicit AVLSearchTree( const Value & notFound );
    AVLSearchTree(const AVLNode<Key,Value> & rhs);

    // DESTRUCTOR
    ~AVLSearchTree();

    // PUBLIC FUNCTIONS
    const Key & findMin( ) const;
    const Key & findMax( ) const;
    const Value & find( const Key & x );
    bool isEmpty( ) const;
    void printTree( ) const;

    void makeEmpty( );
    void insert(const Key & x ,const Value&v);
    void remove(const Key & x );



    //functions that are need for program implementation
    bool contains(const Key &x);
    int getBalance(AVLNode<Key,Value>*t);
    Value& getValue();

};

//CONSTRUCTOR
template<class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree(const Value & notFound)
    : ITEM_NOT_FOUND(notFound), root(nullptr) {}

//DESTRUCTOR
template<class Key,class Value>
AVLSearchTree<Key,Value>::~AVLSearchTree(){
    makeEmpty();
}
//COPY CONSTRUCTOR
template<class Key,class Value>
AVLSearchTree<Key,Value>::AVLSearchTree(const AVLNode<Key,Value> & rhs):root( nullptr ), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND )
{
    *this = rhs;
}
/**
 If t is NULL (i.e., the node doesn't exist),
 it returns the constant ITEM_NOT_FOUND. Otherwise,
 it returns the value stored in the node.
*/
template<class Key,class Value>
const Value & AVLSearchTree<Key,Value>::elementAt(AVLNode<Key, Value>*t) const{
    return t == nullptr ? ITEM_NOT_FOUND: t->value;
}
/**
    Find item x in the tree.
    Return the matching item or ITEM_NOT_FOUND if not found.
*/
//Public method: find
template<class Key, class Value>
const Value & AVLSearchTree<Key,Value>::find(const Key& x){

    return elementAt(find(x, root));
}

/**
    Internal method to find an item in a subtree.
    x is item to search for.
    t is the node that roots the tree.
    Return node containing the matched item.
*/
template<class Key, class Value>
AVLNode<Key,Value>* AVLSearchTree<Key,Value>::find(const Key& x, AVLNode<Key, Value>*t ){
    //if the tree is empty
    if(t==nullptr){
        return nullptr;
    }
    else if(x < t->word){ //if x is alphabetically before the root word
        return find(x,t->left); // go to left subtree
    }
    else if(t->word < x){ //if x is alphabetically after the root word
        return find(x,t->right); //go yo right subtree
    }
    return t; //match
 }
  /*
  ACCEPTABLE RANGE {-1,0,1}
 */
 template <class Key, class Value>
int AVLSearchTree<Key,Value>::getBalance(AVLNode<Key, Value>*t){ // t is representing the root
    if(t == nullptr){ // if the tree is empty
        return -1;
    }
    /*
    if not empty return the value of
    heights subtraction
    */
    return(height(t->left)-height(t->right));
}
// to fix a left-left imbalance
template<class Key, class Value>
AVLNode<Key,Value>* AVLSearchTree<Key,Value>::rightRotate(AVLNode<Key,Value>* t){
    AVLNode<Key,Value>* tLeft = t->left;
    AVLNode<Key,Value>* tLeftRight = tLeft->right;

    // Perform rotation
    tLeft->right = t;
    t->left = tLeftRight;

    // Finding the longest subtree and updating the heights

    t->height = max(height(t->left),height(t->right)) + 1;
    tLeft->height = max(height(tLeft->left),height(tLeft->right))+1;

    return tLeft; // New root
}


// to fix right-right imbalance
template<class Key, class Value>
AVLNode<Key,Value>* AVLSearchTree<Key,Value>::leftRotate(AVLNode<Key, Value>* t){
    AVLNode<Key,Value>* tRight = t->right;
    AVLNode<Key,Value>* tRightLeft = tRight->left;

    // Perform rotation
    tRight->left = t;
    t->right = tRightLeft;

    // Finding the longest subtree and updating the heights
    t->height = max(height(t->left), height(t->right)) + 1;
    tRight->height = max(height(tRight->left), height(tRight->right)) + 1;

    return tRight; // New root
}


template<class Key, class Value>
int AVLSearchTree<Key,Value>::height(AVLNode<Key,Value>* t) {
    if(t==nullptr){
        return -1;
    }
    return t->height;
}

 /*
    Return maximum of lhs and rhs.
 */
template<class Key, class Value>
int AVLSearchTree<Key,Value>:: max( int lhs, int rhs ) const{ // the inputs will be heights
    return (lhs > rhs)? lhs : rhs;
}
// checks if the tree is empty or not
template<class Key,class Value>
bool AVLSearchTree<Key,Value>::isEmpty() const{
    if(root == nullptr){ //if the tree is empty
        return true;
    }
    return false;
}
//external findMin function
template<class Key,class Value>
const Key & AVLSearchTree<Key,Value>::findMin() const{
    return elementAt(findMin(root));
}
//internal findMin function
template<class Key, class Value>
AVLNode<Key,Value>* AVLSearchTree<Key, Value>::findMin(AVLNode<Key,Value>* t) const {
    // keep traversing left until you reach the leftmost leaf
    while (t != nullptr && t->left != nullptr){
        t = t->left;
    }
    return t;
}
//external findMax function
template<class Key,class Value>
const Key& AVLSearchTree<Key,Value>::findMax() const{
    return elementAt(findMax(root));
}
//intern findMax function
template<class Key,class Value>
AVLNode<Key,Value>* AVLSearchTree<Key,Value>::findMax(AVLNode<Key,Value>*t) const{
    // keep traversing left until you reach the rightmost lead
    while(t!= nullptr && t ->right != nullptr){
        t = t->right;
    }
    return t;
}
//external makeEmpty
template<class Key,class Value>
void AVLSearchTree<Key,Value>::makeEmpty(){
    makeEmpty(root);
}
//internal makeEmpty
template<class Key, class Value>
void AVLSearchTree<Key,Value>::makeEmpty(AVLNode<Key,Value>* & t ) const{
    if(t != nullptr){
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = nullptr;
}
/*
    external printTree function
    FOR DEBUG PURPOSES
*/
template<class Key, class Value>
void AVLSearchTree<Key,Value>::printTree() const{
    if( isEmpty()){
        cout << "Empty tree" << endl;
    }
    else{
         printTree(root);
    }
}
//internal printTree functions
template<class Key,class Value>
void AVLSearchTree<Key,Value>::  printTree(AVLNode<Key,Value>*t) const{
    if (t != nullptr) {
        // printing the left subtree
        printTree(t->left);
        //printing the current node
        cout << t->word << " (Height: " << t->height << ") ";
        //printing the right subtree
        printTree(t->right);
    }
}
template<class Key,class Value>
bool AVLSearchTree<Key,Value>:: contains(const Key &x){
    return find(x, root) != nullptr;
}
template<class Key, class Value>
AVLNode<Key,Value>* AVLSearchTree<Key,Value>::clone(AVLNode<Key,Value>* t) const {
    if (t == nullptr) {
        return nullptr;
    }

    // recursively clone left and right subtrees
    AVLNode<Key,Value>* cloneLeft = clone(t->left);
    AVLNode<Key,Value>* cloneRight = clone(t->right);

    // Create a new WordItem for the current node with the cloned left and right subtrees
    AVLNode<Key,Value>* clone = new AVLNode<Key,Value>(t->word);
    clone->left = cloneLeft;
    clone->right = cloneRight;
    clone->value = t->value;

    return clone;
}
/**
    * Insert x into the tree; duplicates are ignored.
*/
template<class Key, class Value>
void AVLSearchTree<Key,Value>:: insert(const Key & x,const Value&v) {
    insert(x,v, root); //insert it into the list
}
template<class Key, class Value>
AVLNode<Key,Value>* AVLSearchTree<Key, Value>::insert(const Key& x, const Value& v, AVLNode<Key, Value>* & t) {
    if (t == nullptr) { //if the tree is empty
        t = new AVLNode<Key, Value>(x, v, nullptr, nullptr);

    } else if (x < t->word) { // If x is alphabetically before the root word
        // Go to the left subtree
        if(t->left != nullptr){
            insert(x, v, t->left);
        }
        else{
            t->left = new  AVLNode<Key, Value>(x, v, nullptr, nullptr);
        }
                     // bu insertion islemini yapiyo
    } else if (t->word < x) { // If x is alphabetically after the root word
         // Go to the right subtree
        if(t->right != nullptr){
            insert(x, v, t->right);
        }
        else{
            t->right = new  AVLNode<Key, Value>(x, v, nullptr, nullptr);
        }
    }

    //updating the height of the current node
    t->height = max(height(t->left), height(t->right)) + 1;
    //calculating the balance factor of the current node
    int balanceFactor = getBalance(t);
    //for left-left case
    if (balanceFactor > 1 && x < t->left->word) {
        t = rightRotate(t);
    }
    //for right-right case
    else if (balanceFactor < -1 && x > t->right->word) {
        t = leftRotate(t);
    }
    //for the left-right case
    else if (balanceFactor > 1 && x > t->left->word) {
        t->left = leftRotate(t->left);
        t = rightRotate(t);
    }
    //for the right-left case
    else if (balanceFactor < -1 && x < t->right->word) {
        t->right = rightRotate(t->right);
        t = leftRotate(t);
    }
    return t;
}
//external remove function
template<class Key,class Value>
void AVLSearchTree<Key,Value>::remove(const Key&x){
    root = remove(x, root);
}
template<class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::remove(const Key& x, AVLNode<Key, Value>* & t) {
    if (t == nullptr) {
        return t;
    }

    if (x < t->word) {
        t->left = remove(x, t->left);
    } else if (t->word < x) {
        t->right = remove(x, t->right);
    } else {
        if (t->left == nullptr || t->right == nullptr) {
            AVLNode<Key, Value>* temp = t->left ? t->left : t->right;
            if (temp == nullptr) {
                temp = t;
                t = nullptr;
            } else {
                *t = *temp;
            }
            delete temp;
        } else {
            AVLNode<Key, Value>* successor = findMin(t->right);
            t->word = successor->word;
            t->value = successor->value;
            t->right = remove(successor->word, t->right);
        }
    }

    if (t == nullptr) {
        return t;
    }

    t->height = max(height(t->left), height(t->right)) + 1;
    int balanceFactor = getBalance(t);

    if (balanceFactor > 1 && getBalance(t->left) >= 0) {
        return rightRotate(t);
    }
    if (balanceFactor > 1 && getBalance(t->left) < 0) {
        t->left = leftRotate(t->left);
        return rightRotate(t);
    }
    if (balanceFactor < -1 && getBalance(t->right) <= 0) {
        return leftRotate(t);
    }
    if (balanceFactor < -1 && getBalance(t->right) > 0) {
        t->right = rightRotate(t->right);
        return leftRotate(t);
    }

    return t;
}



#endif // avl_h