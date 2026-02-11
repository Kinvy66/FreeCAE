/**
 * @file FCGeoToMeshMapper.cpp
 * @brief 几何到网格的映射实现（移植自 FITKInterfaceMesh FITKGeoToMeshMapper）
 */
#include "FCGeoToMeshMapper.h"

namespace FC {

FCGeoToMeshMapper::FCGeoToMeshMapper() {}

FCGeoToMeshMapper::~FCGeoToMeshMapper() {}

void FCGeoToMeshMapper::clear()
{
    _meshVerOfGeoNode.clear();
    _meshEleOfGeoEdge.clear();
    _meshEleOfGeoFace.clear();
    _meshEleOfGeoSolid.clear();
}

void FCGeoToMeshMapper::insertMeshVertexOfGeoNode(int geoNodeID, int meshVertexID)
{
    _meshVerOfGeoNode.insert(geoNodeID, meshVertexID);
}

void FCGeoToMeshMapper::removeMeshVertexOfGeoNode(int geoNodeID)
{
    _meshVerOfGeoNode.remove(geoNodeID);
}

QHash<int, int> FCGeoToMeshMapper::getMeshVertexOfGeoNode()
{
    return _meshVerOfGeoNode;
}

void FCGeoToMeshMapper::insertMeshEleOfGeoFace(int faceID, int eleID, int faceIndex)
{
    _meshEleOfGeoFace[faceID].insert(eleID, faceIndex);
}

void FCGeoToMeshMapper::removeMeshEleOfGeoFace(int faceID)
{
    _meshEleOfGeoFace.remove(faceID);
}

QHash<int, QHash<int, int>> FCGeoToMeshMapper::getMeshEleOfGeoFace()
{
    return _meshEleOfGeoFace;
}

void FCGeoToMeshMapper::insertMeshEleOfGeoEdge(int edgeID, int eleID, int edgeIndex)
{
    _meshEleOfGeoEdge[edgeID].insert(eleID, edgeIndex);
}

void FCGeoToMeshMapper::removeMeshEleOfGeoEdge(int edgeID)
{
    _meshEleOfGeoEdge.remove(edgeID);
}

QHash<int, QHash<int, int>> FCGeoToMeshMapper::getMeshEleOfGeoEdge()
{
    return _meshEleOfGeoEdge;
}

void FCGeoToMeshMapper::insertMapOfGeoSolid(int solidID, int meshEleID)
{
    _meshEleOfGeoSolid[solidID].append(meshEleID);
}

void FCGeoToMeshMapper::removeMapOfGeoSolid(int solidID)
{
    _meshEleOfGeoSolid.remove(solidID);
}

QHash<int, QList<int>> FCGeoToMeshMapper::getMeshEleOfGeoSolid()
{
    return _meshEleOfGeoSolid;
}

} // namespace FC
