// include/storage/btree.hpp

#pragma once

#include <vector>
#include <utility>

class BTreeNode {
public:
    bool isLeaf;
    std::vector<std::pair<int, long>> keys;
    std::vector<BTreeNode*> children;

    BTreeNode(bool leaf);
};

class BTree {
public:
    BTree(int t);
    ~BTree();

    void insert(int key, long value);
    long search(int key);
    void remove(int key);

    std::string serialize() const;
    static BTree deserialize(const std::string& data);

private:
    BTreeNode* root;
    int t;  // Minimum degree (defines the range for number of keys)

    void splitChild(BTreeNode* parent, int index, BTreeNode* child);
    void insertNonFull(BTreeNode* node, int key, long value);
    long searchInternal(BTreeNode* node, int key);
    void removeInternal(BTreeNode* node, int key);
    void deleteTree(BTreeNode* node);
};