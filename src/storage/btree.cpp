// src/storage/btree.cpp

#include "include/storage/btree.hpp"
#include <stdexcept>

BTreeNode::BTreeNode(bool leaf) : isLeaf(leaf) {}

BTree::BTree(int t) : root(nullptr), t(t) {
    if (t < 2) {
        throw std::invalid_argument("BTree degree must be 2 or more");
    }
}

BTree::~BTree() {
    deleteTree(root);
}

void BTree::insert(int key, long value) {
    // NOT_IMPLEMENTED
}

long BTree::search(int key) {
    // NOT_IMPLEMENTED
    return -1;
}

void BTree::remove(int key) {
    // NOT_IMPLEMENTED
}

std::string BTree::serialize() const {
    // NOT_IMPLEMENTED
    return "";
}

BTree BTree::deserialize(const std::string& data) {
    // NOT_IMPLEMENTED
    return BTree(2);  // Placeholder
}

void BTree::splitChild(BTreeNode* parent, int index, BTreeNode* child) {
    // NOT_IMPLEMENTED
}

void BTree::insertNonFull(BTreeNode* node, int key, long value) {
    // NOT_IMPLEMENTED
}

long BTree::searchInternal(BTreeNode* node, int key) {
    // NOT_IMPLEMENTED
    return -1;
}

void BTree::removeInternal(BTreeNode* node, int key) {
    // NOT_IMPLEMENTED
}

void BTree::deleteTree(BTreeNode* node) {
    if (node != nullptr) {
        if (!node->isLeaf) {
            for (auto child : node->children) {
                deleteTree(child);
            }
        }
        delete node;
    }
}