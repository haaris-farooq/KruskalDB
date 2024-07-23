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
    
    // Read the serialized data length
    int dataLength;
    nodesFile.read(reinterpret_cast<char*>(&dataLength), sizeof(int));
    
    // Read the serialized node data
    std::string serializedData(dataLength, '\0');
    nodesFile.read(&serializedData[0], dataLength);
    
    // Deserialize node data
    Node deserializedNode = Node::deserialize(serializedData);
    
    // Create and return a shared pointer to the deserialized node
    return std::make_shared<Node>(std::move(deserializedNode));
}

void StorageEngine::saveNodeToDisk(const Node& node) {
    nodesFile.seekp(0, std::ios::end);
    long offset = nodesFile.tellp();
    
    // Serialize node data
    std::string serializedData = node.serialize();
    
    // Write the length of the serialized data
    int dataLength = serializedData.length();
    nodesFile.write(reinterpret_cast<const char*>(&dataLength), sizeof(int));
    
    // Write the serialized node data to disk
    nodesFile.write(serializedData.c_str(), serializedData.length());
    
    // Update the index
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
    
    // Read the serialized data length
    int dataLength;
    edgesFile.read(reinterpret_cast<char*>(&dataLength), sizeof(int));
    
    // Read the serialized edge data
    std::string serializedData(dataLength, '\0');
    edgesFile.read(&serializedData[0], dataLength);
    
    // Deserialize edge data
    Edge deserializedEdge = Edge::deserialize(serializedData);
    
    // Create and return a shared pointer to the deserialized edge
    auto edge = std::make_shared<Edge>(std::move(deserializedEdge));
    edge->setDirty(false);  // The edge just loaded from disk is not dirty
    return edge;
}

void StorageEngine::saveEdgeToDisk(const Edge& edge) {
    edgesFile.seekp(0, std::ios::end);
    long offset = edgesFile.tellp();
    
    // Serialize edge data
    std::string serializedData = edge.serialize();
    
    // Write the length of the serialized data
    int dataLength = serializedData.length();
    edgesFile.write(reinterpret_cast<const char*>(&dataLength), sizeof(int));
    
    // Write the serialized edge data to disk
    edgesFile.write(serializedData.c_str(), serializedData.length());
    
    // Update the index
    indexingEngine->addEdgeIndex(edge.getId(), offset);
    
    // After saving, the edge is no longer dirty
    const_cast<Edge&>(edge).setDirty(false);
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