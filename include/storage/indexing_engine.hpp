// include/storage/indexing_engine.hpp

#pragma once

#include <string>
#include <memory>
#include <fstream>
#include "storage/btree.hpp"

class IndexingEngine {
public:
    IndexingEngine(const std::string& dbPath, int btreeOrder);
    ~IndexingEngine();

    void addNodeIndex(int nodeId, long diskOffset);
    long getNodeDiskOffset(int nodeId);
    void removeNodeIndex(int nodeId);

    void addEdgeIndex(int edgeId, long diskOffset);
    long getEdgeDiskOffset(int edgeId);
    void removeEdgeIndex(int edgeId);

    void flush();

private:
    std::unique_ptr<BTree> nodeIndex;
    std::unique_ptr<BTree> edgeIndex;
    std::fstream nodeIndexFile;
    std::fstream edgeIndexFile;
    std::string dbPath;

    void loadIndexes();
    void saveIndexes();
};