// src/cache/cache_manager.cpp

#include "cache/cache_manager.hpp"

CacheManager::CacheManager(size_t capacity) : capacity(capacity) {}

void CacheManager::cacheNode(int nodeId, std::shared_ptr<Node> node) {
    // NOT_IMPLEMENTED
}

std::shared_ptr<Node> CacheManager::getNode(int nodeId) {
    // NOT_IMPLEMENTED
    return nullptr;
}

void CacheManager::removeNode(int nodeId) {
    // NOT_IMPLEMENTED
}

void CacheManager::cacheEdge(int edgeId, std::shared_ptr<Edge> edge) {
    // NOT_IMPLEMENTED
}

std::shared_ptr<Edge> CacheManager::getEdge(int edgeId) {
    // NOT_IMPLEMENTED
    return nullptr;
}

void CacheManager::removeEdge(int edgeId) {
    // NOT_IMPLEMENTED
}

void CacheManager::clear() {
    nodeCache.clear();
    edgeCache.clear();
    lruListNodes.clear();
    lruListEdges.clear();
}

size_t CacheManager::size() const {
    return nodeCache.size() + edgeCache.size();
}

bool CacheManager::isFull() const {
    return size() >= capacity;
}

void CacheManager::evict() {
    // NOT_IMPLEMENTED
}