// src/storage/indexing_engine.cpp

#include "include/storage/indexing_engine.hpp"
#include <stdexcept>

IndexingEngine::IndexingEngine(const std::string& dbPath, int btreeOrder)
    : dbPath(dbPath) {
    nodeIndexFile.open(dbPath + "node_index.db", std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    edgeIndexFile.open(dbPath + "edge_index.db", std::ios::in | std::ios::out | std::ios::binary | std::ios::app);

    if (!nodeIndexFile.is_open() || !edgeIndexFile.is_open()) {
        throw std::runtime_error("Failed to open index files");
    }

    nodeIndex = std::make_unique<BTree>(btreeOrder);
    edgeIndex = std::make_unique<BTree>(btreeOrder);

    loadIndexes();
}

IndexingEngine::~IndexingEngine() {
    flush();
    nodeIndexFile.close();
    edgeIndexFile.close();
}

void IndexingEngine::addNodeIndex(int nodeId, long diskOffset) {
    nodeIndex->insert(nodeId, diskOffset);
}

long IndexingEngine::getNodeDiskOffset(int nodeId) {
    return nodeIndex->search(nodeId);
}

void IndexingEngine::removeNodeIndex(int nodeId) {
    nodeIndex->remove(nodeId);
}

void IndexingEngine::addEdgeIndex(int edgeId, long diskOffset) {
    edgeIndex->insert(edgeId, diskOffset);
}

long IndexingEngine::getEdgeDiskOffset(int edgeId) {
    return edgeIndex->search(edgeId);
}

void IndexingEngine::removeEdgeIndex(int edgeId) {
    edgeIndex->remove(edgeId);
}

void IndexingEngine::flush() {
    saveIndexes();
}

void IndexingEngine::loadIndexes() {
    // Read serialized B-tree data from files and deserialize
    std::string nodeIndexData((std::istreambuf_iterator<char>(nodeIndexFile)),
                               std::istreambuf_iterator<char>());
    std::string edgeIndexData((std::istreambuf_iterator<char>(edgeIndexFile)),
                               std::istreambuf_iterator<char>());
    
    if (!nodeIndexData.empty()) {
        *nodeIndex = BTree::deserialize(nodeIndexData);
    }
    if (!edgeIndexData.empty()) {
        *edgeIndex = BTree::deserialize(edgeIndexData);
    }
}

void IndexingEngine::saveIndexes() {
    // Serialize B-trees and write to files
    std::string nodeIndexData = nodeIndex->serialize();
    std::string edgeIndexData = edgeIndex->serialize();

    nodeIndexFile.seekp(0);
    nodeIndexFile.write(nodeIndexData.c_str(), nodeIndexData.size());
    nodeIndexFile.flush();

    edgeIndexFile.seekp(0);
    edgeIndexFile.write(edgeIndexData.c_str(), edgeIndexData.size());
    edgeIndexFile.flush();
}