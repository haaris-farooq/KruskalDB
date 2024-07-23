// include/storage/storage_engine.hpp

#pragma once

#include <fstream>
#include <string>
#include <memory>
#include <functional>
#include "core/node.hpp"
#include "core/edge.hpp"
#include "cache/cache_manager.hpp"
#include "storage/indexing_engine.hpp"

class StorageEngine {
public:
    StorageEngine(const std::string& dbPath, size_t cacheCapacity, int btreeOrder);
    ~StorageEngine();

    // Node operations
    std::shared_ptr<Node> getNode(int nodeId);
    void updateNode(int nodeId, const std::function<void(Node&)>& updateFunc);
    void addNode(const Node& node);
    void deleteNode(int nodeId);

    // Edge operations
    std::shared_ptr<Edge> getEdge(int edgeId);
    void updateEdge(int edgeId, const std::function<void(Edge&)>& updateFunc);
    void addEdge(const Edge& edge);
    void deleteEdge(int edgeId);

    // General operations
    void flush();

private:
    std::fstream nodesFile;
    std::fstream edgesFile;
    std::unique_ptr<CacheManager> cacheManager;
    std::unique_ptr<IndexingEngine> indexingEngine;

    // Node helper methods
    std::shared_ptr<Node> loadNodeFromDisk(int nodeId);
    void saveNodeToDisk(const Node& node);
    int getNextNodeId();

    // Edge helper methods
    std::shared_ptr<Edge> loadEdgeFromDisk(int edgeId);
    void saveEdgeToDisk(const Edge& edge);
    int getNextEdgeId();
};