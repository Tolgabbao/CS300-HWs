#ifndef bst_tree_h
#define bst_tree_h

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

template <typename T, typename U>
class BSTNode {
    public:
        T key;
        U value;
        BSTNode<T, U>* left;
        BSTNode<T, U>* right;
        BSTNode(T key, U value) {
            this->key = key;
            this->value = value;
            left = nullptr;
            right = nullptr;
        }
};

template <typename T, typename U>
class AVLSearchTree {
    private:
        BSTNode<T, U>* root;
        void insert(BSTNode<T, U>*& node, T key, U value);
        void printInOrder(BSTNode<T, U>* node);
        void clear(BSTNode<T, U>* node);
        bool contains(BSTNode<T, U>* node, T key);
        U& getValue(BSTNode<T, U>* node, T key);
    public:
        AVLSearchTree();
        ~AVLSearchTree();
        void insert(T key, U value);
        void printInOrder();
        void clear();
        bool contains(T key);
        U& getValue();
};

template <typename T, typename U>
AVLSearchTree<T, U>::AVLSearchTree() {
    root = nullptr;
}

template <typename T, typename U>
AVLSearchTree<T, U>::~AVLSearchTree() {
    clear();
}

template <typename T, typename U>
void AVLSearchTree<T, U>::insert(T key, U value) {
    insert(root, key, value);
}

template <typename T, typename U>
void AVLSearchTree<T, U>::insert(BSTNode<T, U>*& node, T key, U value) {
    if (node == nullptr) {
        node = new BSTNode<T, U>(key, value);
        return;
    }
    if (key < node->key) {
        insert(node->left, key, value);
    } else {
        insert(node->right, key, value);
    }
}

template <typename T, typename U>
void AVLSearchTree<T, U>::printInOrder() {
    printInOrder(root);
    cout << endl;
}

template <typename T, typename U>
void AVLSearchTree<T, U>::printInOrder(BSTNode<T, U>* node) {
    if (node == nullptr) {
        return;
    }
    printInOrder(node->left);
    cout << node->key << " ";
    printInOrder(node->right);
}

template <typename T, typename U>
void AVLSearchTree<T, U>::clear() {
    clear(root);
    root = nullptr;
}

template <typename T, typename U>
void AVLSearchTree<T, U>::clear(BSTNode<T, U>* node) {
    if (node == nullptr) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T, typename U>
bool AVLSearchTree<T, U>::contains(T key) {
    return contains(root, key);
}

template <typename T, typename U>
bool AVLSearchTree<T, U>::contains(BSTNode<T, U>* node, T key) {
    if (node == nullptr) {
        return false;
    }
    if (node->key == key) {
        return true;
    }
    if (key < node->key) {
        return contains(node->left, key);
    } else {
        return contains(node->right, key);
    }
}

template <typename T, typename U>
U& AVLSearchTree<T, U>::getValue() {
    return root->value;
}

template <typename T, typename U>
U& AVLSearchTree<T, U>::getValue(BSTNode<T, U>* node, T key) {
    if (node->key == key) {
        return node->value;
    }
    if (key < node->key) {
        return getValue(node->left, key);
    } else {
        return getValue(node->right, key);
    }
}


#endif // bst_tree_h