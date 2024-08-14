#include <gtest/gtest.h>
#include "storage/btree.hpp"
#include <random>
#include <algorithm>
#include <iostream>

class BTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        btree = new BTree(3); // Minimum degree 3
    }

    void TearDown() override {
        delete btree;
    }

    BTree* btree;
};

TEST_F(BTreeTest, InsertionAndSearch) {
    btree->insert(10, 100);
    btree->insert(20, 200);
    btree->insert(5, 50);

    EXPECT_EQ(btree->search(10), 100);
    EXPECT_EQ(btree->search(20), 200);
    EXPECT_EQ(btree->search(5), 50);

    EXPECT_THROW(btree->search(15), std::runtime_error);
}

TEST_F(BTreeTest, Removal) {
    btree->insert(10, 100);
    btree->insert(20, 200);
    btree->insert(5, 50);

    btree->remove(20);
    EXPECT_THROW(btree->search(20), std::runtime_error);

    EXPECT_EQ(btree->search(10), 100);
    EXPECT_EQ(btree->search(5), 50);
}

TEST_F(BTreeTest, LargeInsertionAndRemoval) {
    const int N = 1000;
    std::vector<int> keys(N);
    for (int i = 0; i < N; ++i) {
        keys[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(keys.begin(), keys.end(), g);

    // Insertion
    for (int i = 0; i < N; ++i) {
        int key = keys[i];
        std::cout << "Inserting key: " << key << std::endl;
        btree->insert(key, key * 10);
        
        try {
            btree->validateTree();
        } catch (const std::exception& e) {
            FAIL() << "Tree validation failed after inserting key " << key << ": " << e.what();
        }

        if (i % 100 == 0) {
            std::cout << "Tree structure after " << i+1 << " insertions:" << std::endl;
            btree->printTree();
        }
    }

    // Search
    for (int i = 0; i < N; ++i) {
        int key = keys[i];
        try {
            long value = btree->search(key);
            EXPECT_EQ(value, key * 10) << "Mismatch for key " << key;
        } catch (const std::exception& e) {
            FAIL() << "Exception during search: " << e.what() << " for key " << key;
        }
    }

    // Removal
    std::shuffle(keys.begin(), keys.end(), g);
    for (int i = 0; i < N; ++i) {
        int key = keys[i];
        std::cout << "Removing key: " << key << std::endl;
        btree->remove(key);
        
        try {
            btree->validateTree();
        } catch (const std::exception& e) {
            FAIL() << "Tree validation failed after removing key " << key << ": " << e.what();
        }

        if (i % 100 == 0) {
            std::cout << "Tree structure after " << i+1 << " removals:" << std::endl;
            btree->printTree();
        }
    }

    // Verify the tree is empty
    EXPECT_TRUE(btree->isEmpty());
}

TEST_F(BTreeTest, SerializationDeserialization) {
    // Create a small tree with a known structure
    btree->insert(3, 30);
    btree->insert(1, 10);
    btree->insert(5, 50);
    btree->insert(7, 70);
    btree->insert(2, 20);

    std::string serialized = btree->serialize();

    BTree deserialized = BTree::deserialize(serialized);

    // Verify the structure and content of the deserialized tree
    EXPECT_EQ(deserialized.search(1), 10);
    EXPECT_EQ(deserialized.search(2), 20);
    EXPECT_EQ(deserialized.search(3), 30);
    EXPECT_EQ(deserialized.search(5), 50);
    EXPECT_EQ(deserialized.search(7), 70);

    EXPECT_THROW(deserialized.search(4), std::runtime_error);
    EXPECT_THROW(deserialized.search(6), std::runtime_error);
}

TEST_F(BTreeTest, EmptyTreeSerialization) {
    std::string serialized = btree->serialize();
    EXPECT_TRUE(serialized.empty());

    EXPECT_THROW({
        BTree deserialized = BTree::deserialize(serialized);
    }, std::runtime_error);
}

TEST_F(BTreeTest, DuplicateKeyInsertion) {
    btree->insert(10, 100);
    btree->insert(10, 200);

    EXPECT_EQ(btree->search(10), 200);
}

TEST_F(BTreeTest, BoundaryConditions) {
    // Test with minimum degree 3, so nodes can have 2-5 keys
    BTree* smallTree = new BTree(3);

    // Insert 6 keys to force a split
    for (int i = 1; i <= 6; ++i) {
        smallTree->insert(i, i * 10);
    }

    for (int i = 1; i <= 6; ++i) {
        EXPECT_EQ(smallTree->search(i), i * 10);
    }

    // Remove keys to test merging
    for (int i = 1; i <= 3; ++i) {
        smallTree->remove(i);
    }

    for (int i = 4; i <= 6; ++i) {
        EXPECT_EQ(smallTree->search(i), i * 10);
    }

    delete smallTree;
}
