/**
 * @file FCGeometryMeshVS.h
 * @brief 几何可视化网格（移植自 FITKInterfaceGeometry FITKGeometryMeshVS）
 */
#ifndef FCGEOMETRYMESHVS_H
#define FCGEOMETRYMESHVS_H

#include "FCGeometryInterfaceAPI.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCMacros.h>
#include <QHash>

namespace FC {

class FCGeoMeshVSPt;
class FCGeoMeshVSEdgeEntity;
class FCGeoMeshVSFaceEntity;

class FCGEOMETRYINTERFACE_API FCGeometryMeshVS : public FCAbstractDataObject
{
    FC_CLASS(FC, FCGeometryMeshVS);
public:
    explicit FCGeometryMeshVS();
    ~FCGeometryMeshVS() override;

    void insertPoint(int id, FCGeoMeshVSPt* pt);
    void insertEdge(int id, FCGeoMeshVSEdgeEntity* edge);
    void insertFace(int id, FCGeoMeshVSFaceEntity* face);

    const QHash<int, FCGeoMeshVSPt*>& getPointItems() const;
    const QHash<int, FCGeoMeshVSEdgeEntity*>& getEdgeItems() const;
    const QHash<int, FCGeoMeshVSFaceEntity*>& getFaceItems() const;

    void clear();
    bool isValid() const;

private:
    QHash<int, FCGeoMeshVSPt*> _points;
    QHash<int, FCGeoMeshVSEdgeEntity*> _edges;
    QHash<int, FCGeoMeshVSFaceEntity*> _faces;
};

} // namespace FC

#endif // FCGEOMETRYMESHVS_H
