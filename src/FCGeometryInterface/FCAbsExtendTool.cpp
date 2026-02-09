/**
 * @file FCExtendTool.cpp
  * @brief 构造辅助几何工具实现
 */
#include "FCAbsExtendTool.h"

namespace FC {

void FCExtendTool::crossVector(const double* first, const double* second, double* result)
{
    result[0] = first[1] * second[2] - first[2] * second[1];
    result[1] = first[2] * second[0] - first[0] * second[2];
    result[2] = first[0] * second[1] - first[1] * second[0];
}

double FCExtendTool::dotVector(const double* first, const double* second)
{
    return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
}

bool FCExtendTool::isZeroVector(const double* vec)
{
    return qFuzzyIsNull(vec[0]) && qFuzzyIsNull(vec[1]) && qFuzzyIsNull(vec[2]);
}

double FCExtendTool::vectorNorm(const double* vec)
{
    return std::sqrt(std::pow(vec[0], 2) + std::pow(vec[1], 2) + std::pow(vec[2], 2));
}

bool FCExtendTool::isFlipDirection(const double* first, const double* second)
{
    if (isZeroVector(first) || isZeroVector(second)) return false;
    double cosVal = dotVector(first, second) / vectorNorm(first) / vectorNorm(second);
    return qFuzzyCompare(cosVal, -1.0);
}

bool FCExtendTool::isEdgeOnFaceTopo(const VirtualShape& edge, const VirtualShape& face)
{
    FCVirtualTopoMapper mapper;
    mapper.mapTopo(face.VirtualTopoId, FCGeoEnum::VSEdge);
    const int nbEdges = mapper.length();
    for (int i = 0; i < nbEdges; ++i) {
        FCAbsVirtualTopo* vtp = mapper.virtualTopo(i);
        if (vtp && vtp->getDataObjectID() == edge.VirtualTopoId) return true;
    }
    return false;
}

bool FCExtendTool::isConnectedFace(const VirtualShape& face1, const VirtualShape& face2)
{
    FCVirtualTopoMapper mapper1, mapper2;
    mapper1.mapTopo(face1.VirtualTopoId, FCGeoEnum::VSEdge);
    mapper2.mapTopo(face2.VirtualTopoId, FCGeoEnum::VSEdge);
    if (mapper1.length() == 0 || mapper2.length() == 0) return false;
    QList<FCAbsVirtualTopo*> edgeList1;
    for (int i = 0; i < mapper1.length(); ++i)
        edgeList1.append(mapper1.virtualTopo(i));
    for (int i = 0; i < mapper2.length(); ++i) {
        if (edgeList1.contains(mapper2.virtualTopo(i))) return true;
    }
    return false;
}

bool FCExtendTool::isConnectedFaces(QList<VirtualShape>& faces)
{
    if (faces.isEmpty()) return false;
    QVector<QVector<int>> adj(faces.size());
    for (int i = 0; i < faces.size(); ++i) {
        for (int j = i + 1; j < faces.size(); ++j) {
            if (isConnectedFace(faces[i], faces[j])) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    QVector<bool> visited(faces.size(), false);
    std::queue<int> q;
    q.push(0);
    visited[0] = true;
    int count = 1;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                ++count;
            }
        }
    }
    return count == faces.size();
}

} // namespace FC
