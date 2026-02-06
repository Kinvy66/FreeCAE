/**
 * @file FCGeometryMeshEntity.cpp
 * @brief 几何显示网格实体实现
 */
#include "FCGeometryMeshEntity.h"

namespace FC {

FCGeoMeshVSPt::FCGeoMeshVSPt(float x, float y, float z)
{
    setXYZ(x, y, z);
}

FCGeoMeshVSPt::FCGeoMeshVSPt(double* coor)
{
    if (coor) {
        m_coor[0] = static_cast<float>(coor[0]);
        m_coor[1] = static_cast<float>(coor[1]);
        m_coor[2] = static_cast<float>(coor[2]);
    }
}

void FCGeoMeshVSPt::setXYZ(float x, float y, float z)
{
    m_coor[0] = x;
    m_coor[1] = y;
    m_coor[2] = z;
}

FCGeoMeshVSTri::FCGeoMeshVSTri(int p1, int p2, int p3)
{
    m_p[0] = p1;
    m_p[1] = p2;
    m_p[2] = p3;
}

int FCGeoMeshVSTri::getNode(int index) const
{
    if (index < 0 || index > 2) return -1;
    return m_p[index];
}

FCGeoMeshVSEdgeEntity::~FCGeoMeshVSEdgeEntity()
{
    clear();
}

void FCGeoMeshVSEdgeEntity::clear()
{
    for (FCGeoMeshVSPt* p : m_ptList) delete p;
    m_ptList.clear();
}

void FCGeoMeshVSEdgeEntity::appendPoint(FCGeoMeshVSPt* pt)
{
    if (pt) m_ptList.append(pt);
}

FCGeoMeshVSFaceEntity::~FCGeoMeshVSFaceEntity()
{
    clear();
}

void FCGeoMeshVSFaceEntity::clear()
{
    for (FCGeoMeshVSTri* t : m_triList) delete t;
    m_triList.clear();
}

void FCGeoMeshVSFaceEntity::appendMeshTri(FCGeoMeshVSTri* tri)
{
    if (tri) m_triList.append(tri);
}

} // namespace FC
