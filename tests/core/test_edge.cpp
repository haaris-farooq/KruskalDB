// tests/core/test_edge.cpp
#include <gtest/gtest.h>
#include "core/edge.hpp"

class EdgeTest : public ::testing::Test {
protected:
    void SetUp() override {
        edge = std::make_unique<Edge>(1, 10, 20, "TEST_TYPE");
    }
    std::unique_ptr<Edge> edge;
};

TEST_F(EdgeTest, Construction) {
    EXPECT_EQ(edge->getId(), 1);
    EXPECT_EQ(edge->getSourceNodeId(), 10);
    EXPECT_EQ(edge->getTargetNodeId(), 20);
    EXPECT_EQ(edge->getType(), "TEST_TYPE");
}

TEST_F(EdgeTest, SetAndGetId) {
    edge->setId(5);
    EXPECT_EQ(edge->getId(), 5);
}

TEST_F(EdgeTest, Properties) {
    edge->setProperty("weight", 3.14);
    edge->setProperty("active", true);
    EXPECT_EQ(edge->getProperty<double>("weight"), 3.14);
    EXPECT_EQ(edge->getProperty<bool>("active"), true);
    EXPECT_THROW(edge->getProperty<std::string>("nonexistent"), std::out_of_range);
}

TEST_F(EdgeTest, HasAndRemoveProperty) {
    edge->setProperty("test", std::string("value"));
    EXPECT_TRUE(edge->hasProperty("test"));
    edge->removeProperty("test");
    EXPECT_FALSE(edge->hasProperty("test"));
}

TEST_F(EdgeTest, GetPropertyKeys) {
    edge->setProperty("key1", std::string("value1"));
    edge->setProperty("key2", std::string("value2"));
    auto keys = edge->getPropertyKeys();
    EXPECT_EQ(keys.size(), 2);
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "key1") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "key2") != keys.end());
}

TEST_F(EdgeTest, SerializeAndDeserialize) {
    edge->setProperty("weight", 3.14);
    edge->setProperty("active", true);
    std::string serialized = edge->serialize();
    Edge deserialized = Edge::deserialize(serialized);
    EXPECT_EQ(deserialized.getId(), edge->getId());
    EXPECT_EQ(deserialized.getSourceNodeId(), edge->getSourceNodeId());
    EXPECT_EQ(deserialized.getTargetNodeId(), edge->getTargetNodeId());
    EXPECT_EQ(deserialized.getType(), edge->getType());
    EXPECT_EQ(deserialized.getProperty<double>("weight"), 3.14);
    EXPECT_EQ(deserialized.getProperty<bool>("active"), true);
}

TEST_F(EdgeTest, PropertyTypes) {
    edge->setProperty("int_prop", 42);
    edge->setProperty("double_prop", 3.14);
    edge->setProperty("string_prop", std::string("test"));
    edge->setProperty("bool_prop", true);

    EXPECT_EQ(edge->getProperty<int>("int_prop"), 42);
    EXPECT_EQ(edge->getProperty<double>("double_prop"), 3.14);
    EXPECT_EQ(edge->getProperty<std::string>("string_prop"), "test");
    EXPECT_EQ(edge->getProperty<bool>("bool_prop"), true);

    // Test wrong type retrieval
    EXPECT_THROW(edge->getProperty<double>("int_prop"), std::bad_variant_access);
    EXPECT_THROW(edge->getProperty<std::string>("bool_prop"), std::bad_variant_access);
}

TEST_F(EdgeTest, SerializeDeserializeAllTypes) {
    edge->setProperty("int_prop", 42);
    edge->setProperty("double_prop", 3.14);
    edge->setProperty("string_prop", std::string("test"));
    edge->setProperty("bool_prop", true);

    std::string serialized = edge->serialize();
    Edge deserialized = Edge::deserialize(serialized);

    EXPECT_EQ(deserialized.getProperty<int>("int_prop"), 42);
    EXPECT_EQ(deserialized.getProperty<double>("double_prop"), 3.14);
    EXPECT_EQ(deserialized.getProperty<std::string>("string_prop"), "test");
    EXPECT_EQ(deserialized.getProperty<bool>("bool_prop"), true);
}