// include/storage/btree.hpp
#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <queue>
#include <stdexcept>
#include <iostream>
#include <limits>

class BTreeNode {
public:
    bool isLeaf;
    std::vector<std::pair<int, long>> keys;
    std::vector<BTreeNode*> children;

    BTreeNode(bool leaf = true);
};

class BTree {
public:
    BTree(int t);
    ~BTree();

    void insert(int key, long value);
    long search(int key) const;
    void remove(int key);
    bool isEmpty() const { return root == nullptr; }
    std::string serialize() const;
    static BTree deserialize(const std::string& data);

    // New public methods for debugging and validation
    void validateTree() const;
    void printTree() const;

    // Setter for root
    void setRoot(BTreeNode* newRoot) { root = newRoot; }

private:
    BTreeNode* root;
    int t; // Minimum degree (defines the range for number of keys)

    void splitChild(BTreeNode* parent, int index, BTreeNode* child);
    void insertNonFull(BTreeNode* node, int key, long value);
    long searchInternal(BTreeNode* node, int key) const;
    void removeInternal(BTreeNode* node, int key);
    void deleteTree(BTreeNode* node);

    // New private methods for debugging and validation
    void validateNode(BTreeNode* node, BTreeNode* parent, int minKey, int maxKey) const;
    void printNode(BTreeNode* node, int depth) const;
};
