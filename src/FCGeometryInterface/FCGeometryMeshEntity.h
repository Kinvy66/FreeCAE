/**
 * @file FCGeometryMeshEntity.h
 * @brief 几何显示网格实体（移植自 FITKInterfaceGeometry FITKGeometryMeshEntity）
 */
#ifndef FCGEOMETRYMESHENTITY_H
#define FCGEOMETRYMESHENTITY_H

#include "FCGeometryInterfaceAPI.h"
#include <QList>

namespace FC {

/** 三维点坐标 */
class FCGEOMETRYINTERFACE_API FCGeoMeshVSPt
{
public:
    FCGeoMeshVSPt() = default;
    FCGeoMeshVSPt(float x, float y, float z);
    FCGeoMeshVSPt(double* coor);
    void setXYZ(float x, float y, float z);
    float x() const { return m_coor[0]; }
    float y() const { return m_coor[1]; }
    float z() const { return m_coor[2]; }
private:
    float m_coor[3]{ 0.f, 0.f, 0.f };
};

typedef FCGeoMeshVSPt FCGeoMeshTriNor;

/** 三角形（三个节点索引） */
class FCGEOMETRYINTERFACE_API FCGeoMeshVSTri
{
public:
    FCGeoMeshVSTri(int p1, int p2, int p3);
    int getNode(int index) const;
    const int* getNode() const { return m_p; }
private:
    int m_p[3]{ -1, -1, -1 };
};

/** 几何边（点序列） */
class FCGEOMETRYINTERFACE_API FCGeoMeshVSEdgeEntity
{
public:
    FCGeoMeshVSEdgeEntity() = default;
    virtual ~FCGeoMeshVSEdgeEntity();
    void clear();
    void appendPoint(FCGeoMeshVSPt* pt);
    int getPointCount() const { return m_ptList.size(); }
    const QList<FCGeoMeshVSPt*>& getPoints() const { return m_ptList; }
private:
    QList<FCGeoMeshVSPt*> m_ptList;
};

/** 几何面（边 + 三角形列表） */
class FCGEOMETRYINTERFACE_API FCGeoMeshVSFaceEntity : public FCGeoMeshVSEdgeEntity
{
public:
    FCGeoMeshVSFaceEntity() = default;
    ~FCGeoMeshVSFaceEntity() override;
    void clear();
    void appendMeshTri(FCGeoMeshVSTri* tri);
    int getTriangleCount() const { return m_triList.size(); }
    const QList<FCGeoMeshVSTri*>& getTriangles() const { return m_triList; }
private:
    QList<FCGeoMeshVSTri*> m_triList;
};

} // namespace FC

#endif // FCGEOMETRYMESHENTITY_H
