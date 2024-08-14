// tests/core/test_node.cpp
#include <gtest/gtest.h>
#include "core/node.hpp"

class NodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        node = std::make_unique<Node>(1);
    }

    std::unique_ptr<Node> node;
};

TEST_F(NodeTest, Construction) {
    EXPECT_EQ(node->getId(), 1);
}

TEST_F(NodeTest, SetAndGetId) {
    node->setId(5);
    EXPECT_EQ(node->getId(), 5);
}

TEST_F(NodeTest, Properties) {
    node->setProperty("name", std::string("Test Node"));
    node->setProperty("value", 42);
    node->setProperty("active", true);

    EXPECT_EQ(node->getProperty<std::string>("name"), "Test Node");
    EXPECT_EQ(node->getProperty<int>("value"), 42);
    EXPECT_EQ(node->getProperty<bool>("active"), true);
    EXPECT_THROW(node->getProperty<std::string>("nonexistent"), std::out_of_range);
}

TEST_F(NodeTest, HasAndRemoveProperty) {
    node->setProperty("test", std::string("value"));
    EXPECT_TRUE(node->hasProperty("test"));
    node->removeProperty("test");
    EXPECT_FALSE(node->hasProperty("test"));
}

TEST_F(NodeTest, GetPropertyKeys) {
    node->setProperty("key1", std::string("value1"));
    node->setProperty("key2", std::string("value2"));
    auto keys = node->getPropertyKeys();
    EXPECT_EQ(keys.size(), 2);
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "key1") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "key2") != keys.end());
}

TEST_F(NodeTest, Edges) {
    node->addEdge(1, true);  // outgoing edge
    node->addEdge(2, false);  // incoming edge
    
    auto outgoing = node->getOutgoingEdges();
    auto incoming = node->getIncomingEdges();
    
    EXPECT_EQ(outgoing.size(), 1);
    EXPECT_EQ(incoming.size(), 1);
    EXPECT_EQ(outgoing[0], 1);
    EXPECT_EQ(incoming[0], 2);

    node->removeEdge(1, true);
    outgoing = node->getOutgoingEdges();
    EXPECT_EQ(outgoing.size(), 0);
}

TEST_F(NodeTest, SerializeAndDeserialize) {
    node->setProperty("name", std::string("Test Node"));
    node->setProperty("value", 42);
    node->addEdge(1, true);
    node->addEdge(2, false);

    std::string serialized = node->serialize();
    std::cout << "Serialized Node: " << serialized << std::endl;

    Node deserialized;
    try {
        deserialized = Node::deserialize(serialized);
    } catch (const std::exception& e) {
        std::cout << "Deserialization failed: " << e.what() << std::endl;
        FAIL() << "Deserialization failed";
    }
    std::string deserializedSerialized = deserialized.serialize();
    std::cout << "Deserialized Serialized Node: " << deserializedSerialized << std::endl;

    // Check ID
    EXPECT_EQ(deserialized.getId(), node->getId()) << "ID mismatch: Expected " << node->getId() << " but got " << deserialized.getId();

    // Check Properties
    try {
        EXPECT_EQ(deserialized.getProperty<std::string>("name"), "Test Node") << "Property 'name' mismatch: Expected 'Test Node' but got " << deserialized.getProperty<std::string>("name");
    } catch (const std::exception& e) {
        std::cout << "Error while getting property 'name': " << e.what() << std::endl;
        throw;
    }

    try {
        EXPECT_EQ(deserialized.getProperty<int>("value"), 42) << "Property 'value' mismatch: Expected 42 but got " << deserialized.getProperty<int>("value");
    } catch (const std::exception& e) {
        std::cout << "Error while getting property 'value': " << e.what() << std::endl;
        throw;
    }

    // Check non-existent property
    try {
        deserialized.getProperty<std::string>("nonexistent");
        FAIL() << "Expected std::out_of_range for nonexistent property";
    } catch (const std::out_of_range&) {
        SUCCEED();
    } catch (const std::exception& e) {
        std::cout << "Unexpected exception for nonexistent property: " << e.what() << std::endl;
        throw;
    }

    // Check Edges
    auto originalOutgoingEdges = node->getOutgoingEdges();
    auto deserializedOutgoingEdges = deserialized.getOutgoingEdges();
    EXPECT_EQ(deserializedOutgoingEdges, originalOutgoingEdges) << "Outgoing edges mismatch";

    auto originalIncomingEdges = node->getIncomingEdges();
    auto deserializedIncomingEdges = deserialized.getIncomingEdges();
    EXPECT_EQ(deserializedIncomingEdges, originalIncomingEdges) << "Incoming edges mismatch";

    // Check for properties presence
    EXPECT_TRUE(deserialized.hasProperty("name")) << "Expected property 'name' to be present";
    EXPECT_TRUE(deserialized.hasProperty("value")) << "Expected property 'value' to be present";
    EXPECT_FALSE(deserialized.hasProperty("nonexistent")) << "Expected property 'nonexistent' to be absent";

    // Check for edge counts
    EXPECT_EQ(deserialized.getOutgoingEdges().size(), 1) << "Outgoing edges size mismatch: Expected 1 but got " << deserialized.getOutgoingEdges().size();
    EXPECT_EQ(deserialized.getIncomingEdges().size(), 1) << "Incoming edges size mismatch: Expected 1 but got " << deserialized.getIncomingEdges().size();
}