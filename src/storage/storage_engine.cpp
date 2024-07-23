// src/storage/storage_engine.cpp

#include "include/storage/storage_engine.hpp"
#include <stdexcept>

StorageEngine::StorageEngine(const std::string& dbPath, size_t cacheCapacity, int btreeOrder) {
    nodesFile.open(dbPath + "nodes.db", std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    edgesFile.open(dbPath + "edges.db", std::ios::in | std::ios::out | std::ios::binary | std::ios::app);

    if (!nodesFile.is_open() || !edgesFile.is_open()) {
        throw std::runtime_error("Failed to open database files");
    }

    cacheManager = std::make_unique<CacheManager>(cacheCapacity);
    indexingEngine = std::make_unique<IndexingEngine>(dbPath, btreeOrder);
}

StorageEngine::~StorageEngine() {
    flush();
    nodesFile.close();
    edgesFile.close();
}

std::shared_ptr<Node> StorageEngine::getNode(int nodeId) {
    auto cachedNode = cacheManager->getNode(nodeId);
    if (cachedNode) {
        return cachedNode;
    }
    return loadNodeFromDisk(nodeId);
}

void StorageEngine::updateNode(int nodeId, const std::function<void(Node&)>& updateFunc) {
    auto node = getNode(nodeId);
    updateFunc(*node);
    cacheManager->cacheNode(nodeId, node);
    // Write to disk during flush
}

void StorageEngine::addNode(const Node& node) {
    int nodeId = getNextNodeId();
    auto newNode = std::make_shared<Node>(node);
    newNode->setId(nodeId);
    saveNodeToDisk(*newNode);
    cacheManager->cacheNode(nodeId, newNode);
}

void StorageEngine::deleteNode(int nodeId) {
    // NOT_IMPLEMENTED
}

std::shared_ptr<Node> StorageEngine::loadNodeFromDisk(int nodeId) {
    long offset = indexingEngine->getNodeDiskOffset(nodeId);
    if (offset == -1) {
        throw std::runtime_error("Node not found");
    }
    
    nodesFile.seekg(offset);
    // Read node data from disk
    // Deserialize node data
    // Return std::shared_ptr<Node>
    
    // Placeholder implementation
    return std::make_shared<Node>(nodeId);
}

void StorageEngine::saveNodeToDisk(const Node& node) {
    nodesFile.seekp(0, std::ios::end);
    long offset = nodesFile.tellp();
    
    // Serialize node data
    // Write node data to disk
    
    indexingEngine->addNodeIndex(node.getId(), offset);
}

std::shared_ptr<Edge> StorageEngine::getEdge(int edgeId) {
    auto cachedEdge = cacheManager->getEdge(edgeId);
    if (cachedEdge) {
        return cachedEdge;
    }
    return loadEdgeFromDisk(edgeId);
}

void StorageEngine::updateEdge(int edgeId, const std::function<void(Edge&)>& updateFunc) {
    auto edge = getEdge(edgeId);
    updateFunc(*edge);
    cacheManager->cacheEdge(edgeId, edge);
    // Write to disk during flush
}

void StorageEngine::addEdge(const Edge& edge) {
    int edgeId = getNextEdgeId();
    auto newEdge = std::make_shared<Edge>(edge);
    newEdge->setId(edgeId);
    saveEdgeToDisk(*newEdge);
    cacheManager->cacheEdge(edgeId, newEdge);
}

void StorageEngine::deleteEdge(int edgeId) {
    // NOT_IMPLEMENTED
}

std::shared_ptr<Edge> StorageEngine::loadEdgeFromDisk(int edgeId) {
    long offset = indexingEngine->getEdgeDiskOffset(edgeId);
    if (offset == -1) {
        throw std::runtime_error("Edge not found");
    }
    
    edgesFile.seekg(offset);
    // Read edge data from disk
    // Deserialize edge data
    // Return std::shared_ptr<Edge>
    
    // Placeholder implementation
    return std::make_shared<Edge>(edgeId, 0, 0, ""); // Assuming Edge constructor takes id, source, target, and type
}

void StorageEngine::saveEdgeToDisk(const Edge& edge) {
    edgesFile.seekp(0, std::ios::end);
    long offset = edgesFile.tellp();
    
    // Serialize edge data
    // Write edge data to disk
    
    indexingEngine->addEdgeIndex(edge.getId(), offset);
}

void StorageEngine::flush() {
    // Flush cached nodes and edges to disk
    // Update indexes
    indexingEngine->flush();
}

int StorageEngine::getNextNodeId() {
    // NOT_IMPLEMENTED
    return 0;
}

int StorageEngine::getNextEdgeId() {
    // NOT_IMPLEMENTED
    return 0;
}