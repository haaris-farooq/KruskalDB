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
    edge->setProperty("weight", Property(3.14));
    edge->setProperty("active", Property(true));

    EXPECT_EQ(edge->getProperty("weight").asDouble(), 3.14);
    EXPECT_EQ(edge->getProperty("active").asBool(), true);
    EXPECT_THROW(edge->getProperty("nonexistent"), std::out_of_range);
}

TEST_F(EdgeTest, HasAndRemoveProperty) {
    edge->setProperty("test", Property("value"));
    EXPECT_TRUE(edge->hasProperty("test"));
    edge->removeProperty("test");
    EXPECT_FALSE(edge->hasProperty("test"));
}

TEST_F(EdgeTest, GetPropertyKeys) {
    edge->setProperty("key1", Property("value1"));
    edge->setProperty("key2", Property("value2"));
    auto keys = edge->getPropertyKeys();
    EXPECT_EQ(keys.size(), 2);
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "key1") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "key2") != keys.end());
}

TEST_F(EdgeTest, SerializeAndDeserialize) {
    edge->setProperty("weight", Property(3.14));
    edge->setProperty("active", Property(true));

    std::string serialized = edge->serialize();
    Edge deserialized = Edge::deserialize(serialized);

    EXPECT_EQ(deserialized.getId(), edge->getId());
    EXPECT_EQ(deserialized.getSourceNodeId(), edge->getSourceNodeId());
    EXPECT_EQ(deserialized.getTargetNodeId(), edge->getTargetNodeId());
    EXPECT_EQ(deserialized.getType(), edge->getType());
    EXPECT_EQ(deserialized.getProperty("weight").asDouble(), 3.14);
    EXPECT_EQ(deserialized.getProperty("active").asBool(), true);
}