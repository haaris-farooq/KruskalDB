// storage/btree.cpp
#include "storage/btree.hpp"

BTreeNode::BTreeNode(bool leaf) : isLeaf(leaf) {}

BTree::BTree(int t) : root(nullptr), t(t) {}

BTree::~BTree() {
    deleteTree(root);
}

void BTree::insert(int key, long value) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys.push_back({key, value});
    } else {
        if (root->keys.size() == 2*t - 1) {
            BTreeNode* newRoot = new BTreeNode(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0, root);
            insertNonFull(newRoot, key, value);
            root = newRoot;
        } else {
            insertNonFull(root, key, value);
        }
    }
    validateTree();
}

long BTree::search(int key) const {
    return searchInternal(root, key);
}

void BTree::remove(int key) {
    if (root == nullptr) {
        return;
    }
    removeInternal(root, key);
    if (root->keys.empty()) {
        BTreeNode* oldRoot = root;
        root = root->isLeaf ? nullptr : root->children[0];
        delete oldRoot;
    }
    validateTree();
}

void BTree::splitChild(BTreeNode* parent, int index, BTreeNode* child) {
    BTreeNode* newChild = new BTreeNode(child->isLeaf);
    parent->children.insert(parent->children.begin() + index + 1, newChild);
    parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);

    newChild->keys.assign(child->keys.begin() + t, child->keys.end());
    child->keys.erase(child->keys.begin() + t - 1, child->keys.end());

    if (!child->isLeaf) {
        newChild->children.assign(child->children.begin() + t, child->children.end());
        child->children.erase(child->children.begin() + t, child->children.end());
    }
}

void BTree::insertNonFull(BTreeNode* node, int key, long value) {
    int i = node->keys.size() - 1;

    if (node->isLeaf) {
        for (size_t j = 0; j < node->keys.size(); ++j) {
            if (node->keys[j].first == key) {
                node->keys[j].second = value;
                return;
            }
        }

        node->keys.push_back({});
        while (i >= 0 && key < node->keys[i].first) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = {key, value};
    } else {
        while (i >= 0 && key < node->keys[i].first) {
            i--;
        }
        i++;
        
        if (i > 0 && node->keys[i-1].first == key) {
            node->keys[i-1].second = value;
            return;
        }

        if (node->children[i]->keys.size() == 2*t - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i].first) {
                i++;
            }
        }
        insertNonFull(node->children[i], key, value);
    }
}

long BTree::searchInternal(BTreeNode* node, int key) const {
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i].first) {
        i++;
    }
    if (i < node->keys.size() && key == node->keys[i].first) {
        return node->keys[i].second;
    }
    if (node->isLeaf) {
        throw std::runtime_error("Key not found");
    }
    return searchInternal(node->children[i], key);
}

void BTree::removeInternal(BTreeNode* node, int key) {
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i].first) {
        i++;
    }
    if (node->isLeaf) {
        if (i < node->keys.size() && node->keys[i].first == key) {
            node->keys.erase(node->keys.begin() + i);
        } else {
            throw std::runtime_error("Key not found for removal");
        }
    } else {
        if (i < node->keys.size() && node->keys[i].first == key) {
            if (node->children[i]->keys.size() >= t) {
                BTreeNode* pred = node->children[i];
                while (!pred->isLeaf) {
                    pred = pred->children.back();
                }
                node->keys[i] = pred->keys.back();
                removeInternal(node->children[i], pred->keys.back().first);
            } else if (node->children[i + 1]->keys.size() >= t) {
                BTreeNode* succ = node->children[i + 1];
                while (!succ->isLeaf) {
                    succ = succ->children.front();
                }
                node->keys[i] = succ->keys.front();
                removeInternal(node->children[i + 1], succ->keys.front().first);
            } else {
                BTreeNode* left = node->children[i];
                BTreeNode* right = node->children[i + 1];
                left->keys.push_back(node->keys[i]);
                left->keys.insert(left->keys.end(), right->keys.begin(), right->keys.end());
                if (!left->isLeaf) {
                    left->children.insert(left->children.end(), right->children.begin(), right->children.end());
                }
                node->keys.erase(node->keys.begin() + i);
                node->children.erase(node->children.begin() + i + 1);
                delete right;
                removeInternal(left, key);
            }
        } else {
            if (node->children[i]->keys.size() == t - 1) {
                BTreeNode* child = node->children[i];
                if (i > 0 && node->children[i - 1]->keys.size() >= t) {
                    BTreeNode* leftSibling = node->children[i - 1];
                    child->keys.insert(child->keys.begin(), node->keys[i - 1]);
                    node->keys[i - 1] = leftSibling->keys.back();
                    leftSibling->keys.pop_back();
                    if (!child->isLeaf) {
                        child->children.insert(child->children.begin(), leftSibling->children.back());
                        leftSibling->children.pop_back();
                    }
                } else if (i < node->children.size() - 1 && node->children[i + 1]->keys.size() >= t) {
                    BTreeNode* rightSibling = node->children[i + 1];
                    child->keys.push_back(node->keys[i]);
                    node->keys[i] = rightSibling->keys.front();
                    rightSibling->keys.erase(rightSibling->keys.begin());
                    if (!child->isLeaf) {
                        child->children.push_back(rightSibling->children.front());
                        rightSibling->children.erase(rightSibling->children.begin());
                    }
                } else {
                    if (i < node->children.size() - 1) {
                        BTreeNode* rightSibling = node->children[i + 1];
                        child->keys.push_back(node->keys[i]);
                        child->keys.insert(child->keys.end(), rightSibling->keys.begin(), rightSibling->keys.end());
                        if (!child->isLeaf) {
                            child->children.insert(child->children.end(), rightSibling->children.begin(), rightSibling->children.end());
                        }
                        node->keys.erase(node->keys.begin() + i);
                        node->children.erase(node->children.begin() + i + 1);
                        delete rightSibling;
                    } else {
                        BTreeNode* leftSibling = node->children[i - 1];
                        leftSibling->keys.push_back(node->keys[i - 1]);
                        leftSibling->keys.insert(leftSibling->keys.end(), child->keys.begin(), child->keys.end());
                        if (!leftSibling->isLeaf) {
                            leftSibling->children.insert(leftSibling->children.end(), child->children.begin(), child->children.end());
                        }
                        node->keys.erase(node->keys.begin() + i - 1);
                        node->children.erase(node->children.begin() + i);
                        delete child;
                        i--;
                    }
                }
            }
            removeInternal(node->children[i], key);
        }
    }
}

void BTree::deleteTree(BTreeNode* node) {
    if (node != nullptr) {
        if (!node->isLeaf) {
            for (BTreeNode* child : node->children) {
                deleteTree(child);
            }
        }
        delete node;
    }
}

std::string BTree::serialize() const {
    if (root == nullptr) {
        return "";
    }
    
    std::ostringstream oss;
    oss << t << "|";

    std::queue<BTreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        BTreeNode* node = q.front();
        q.pop();
        
        oss << (node->isLeaf ? "1" : "0") << "|";
        oss << node->keys.size() << "|";
        for (const auto& kv : node->keys) {
            oss << kv.first << ":" << kv.second << "|";
        }

        if (!node->isLeaf) {
            oss << node->children.size() << "|";
            for (BTreeNode* child : node->children) {
                q.push(child);
            }
        } else {
            oss << "0|";
        }
    }
    
    return oss.str();
}

BTree BTree::deserialize(const std::string& data) {
    if (data.empty()) {
        throw std::runtime_error("Cannot deserialize from empty data.");
    }

    std::istringstream iss(data);
    std::string token;

    std::getline(iss, token, '|');
    int t = std::stoi(token);

    BTree tree(t);

    std::getline(iss, token, '|');
    bool isLeaf = (token == "1");

    std::getline(iss, token, '|');
    int keyCount = std::stoi(token);
    
    BTreeNode* root = new BTreeNode(isLeaf);

    for (int i = 0; i < keyCount; ++i) {
        std::getline(iss, token, '|');
        size_t colonPos = token.find(':');
        int key = std::stoi(token.substr(0, colonPos));
        long value = std::stol(token.substr(colonPos + 1));
        root->keys.push_back({key, value});
    }

    tree.setRoot(root);

    std::queue<BTreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        BTreeNode* parent = q.front();
        q.pop();

        std::getline(iss, token, '|');
        int childCount = std::stoi(token);

        for (int i = 0; i < childCount; ++i) {
            std::getline(iss, token, '|');
            bool childIsLeaf = (token == "1");
            std::getline(iss, token, '|');
            int childKeyCount = std::stoi(token);

            BTreeNode* child = new BTreeNode(childIsLeaf);

            for (int j = 0; j < childKeyCount; ++j) {
                std::getline(iss, token, '|');
                size_t colonPos = token.find(':');
                int key = std::stoi(token.substr(0, colonPos));
                long value = std::stol(token.substr(colonPos + 1));
                child->keys.push_back({key, value});
            }

            parent->children.push_back(child);

            if (!childIsLeaf) {
                q.push(child);
            }
        }
    }

    tree.validateTree();
    return tree;
}

void BTree::validateTree() const {
    if (root == nullptr) {
        return;
    }
    validateNode(root, nullptr, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}

void BTree::validateNode(BTreeNode* node, BTreeNode* parent, int minKey, int maxKey) const {
    if (node == nullptr) return;

    if (node != root && (node->keys.size() < t - 1 || node->keys.size() > 2 * t - 1)) {
        throw std::runtime_error("Invalid number of keys in node");
    }

    for (size_t i = 0; i < node->keys.size(); ++i) {
        if (node->keys[i].first < minKey || node->keys[i].first > maxKey) {
            throw std::runtime_error("Key out of range");
        }
        if (i > 0 && node->keys[i].first <= node->keys[i-1].first) {
            throw std::runtime_error("Keys not in ascending order");
        }
    }

    if (!node->isLeaf) {
        if (node->children.size() != node->keys.size() + 1) {
            throw std::runtime_error("Invalid number of children");
        }
        for (size_t i = 0; i <= node->keys.size(); ++i) {
            int childMinKey = (i == 0) ? minKey : node->keys[i-1].first;
            int childMaxKey = (i == node->keys.size()) ? maxKey : node->keys[i].first;
            validateNode(node->children[i], node, childMinKey, childMaxKey);
        }
    }
}

void BTree::printTree() const {
    if (root == nullptr) {
        return;
    }
    printNode(root, 0);
}

void BTree::printNode(BTreeNode* node, int depth) const {
    if (node == nullptr) return;

    std::string indent(depth * 4, ' ');
    std::cout << indent << "Node (depth " << depth << "): ";
    for (const auto& kv : node->keys) {
        std::cout << kv.first << ":" << kv.second << " ";
    }
    std::cout << std::endl;

    if (!node->isLeaf) {
        for (BTreeNode* child : node->children) {
            printNode(child, depth + 1);
        }
    }
}

void BTree::setRoot(BTreeNode* newRoot) {
    if (root != nullptr) {
        deleteTree(root);
    }
    root = newRoot;
}

bool BTree::isEmpty() const {
    return root == nullptr;
}