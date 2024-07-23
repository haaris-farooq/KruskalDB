// cache_manager.hpp

#pragma once

#include <unordered_map>
#include <list>
#include <memory>
#include "core/node.hpp"
#include "core/edge.hpp"

class CacheManager {
public:
    explicit CacheManager(size_t capacity);

    void cacheNode(int nodeId, std::shared_ptr<Node> node);
    std::shared_ptr<Node> getNode(int nodeId);
    void removeNode(int nodeId);

    void cacheEdge(int edgeId, std::shared_ptr<Edge> edge);
    std::shared_ptr<Edge> getEdge(int edgeId);
    void removeEdge(int edgeId);

    void clear();
    size_t size() const;
    bool isFull() const;

private:
    size_t capacity;
    std::unordered_map<int, std::pair<std::shared_ptr<Node>, std::list<int>::iterator>> nodeCache;
    std::unordered_map<int, std::pair<std::shared_ptr<Edge>, std::list<int>::iterator>> edgeCache;
    std::list<int> lruListNodes;
    std::list<int> lruListEdges;

    void evict();
};