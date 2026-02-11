/**
 * @file FCGeoToMeshMapper.h
 * @brief 几何到网格的映射（移植自 FITKInterfaceMesh FITKGeoToMeshMapper）
 */
#ifndef FCGEOTOMESHMAPPER_H
#define FCGEOTOMESHMAPPER_H

#include "FCMeshInterfaceAPI.h"
#include <QObject>
#include <QHash>

namespace FC {

/**
 * @brief 几何到网格的映射
 */
class FCMESHINTERFACE_API FCGeoToMeshMapper : public QObject
{
public:
    explicit FCGeoToMeshMapper();
    virtual ~FCGeoToMeshMapper();

    void clear();

    void insertMeshVertexOfGeoNode(int geoNodeID, int meshVertexID);
    void removeMeshVertexOfGeoNode(int geoNodeID);
    QHash<int, int> getMeshVertexOfGeoNode();

    void insertMeshEleOfGeoFace(int faceID, int eleID, int faceIndex);
    void removeMeshEleOfGeoFace(int faceID);
    QHash<int, QHash<int, int>> getMeshEleOfGeoFace();

    void insertMeshEleOfGeoEdge(int edgeID, int eleID, int edgeIndex);
    void removeMeshEleOfGeoEdge(int edgeID);
    QHash<int, QHash<int, int>> getMeshEleOfGeoEdge();

    void insertMapOfGeoSolid(int solidID, int meshEleID);
    void removeMapOfGeoSolid(int solidID);
    QHash<int, QList<int>> getMeshEleOfGeoSolid();

protected:
    QHash<int, int> _meshVerOfGeoNode;
    QHash<int, QHash<int, int>> _meshEleOfGeoEdge;
    QHash<int, QHash<int, int>> _meshEleOfGeoFace;
    QHash<int, QList<int>> _meshEleOfGeoSolid;
};

} // namespace FC

#endif // FCGEOTOMESHMAPPER_H
