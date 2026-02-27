/**
 * @file FCGeometryMeshVS.h
 * @brief 几何可视化网格（移植自 FITKInterfaceGeometry FITKGeometryMeshVS）
 */
#ifndef FCGEOMETRYMESHVS_H
#define FCGEOMETRYMESHVS_H

#include "FCGeometryInterfaceAPI.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCIdHash.h>
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

    void insertPoint(FCID id, FCGeoMeshVSPt* pt);
    void insertEdge(FCID id, FCGeoMeshVSEdgeEntity* edge);
    void insertFace(FCID id, FCGeoMeshVSFaceEntity* face);

    const QHash<FCID, FCGeoMeshVSPt*>& getPointItems() const;
    const QHash<FCID, FCGeoMeshVSEdgeEntity*>& getEdgeItems() const;
    const QHash<FCID, FCGeoMeshVSFaceEntity*>& getFaceItems() const;

    void clear();
    bool isValid() const;

private:
    QHash<FCID, FCGeoMeshVSPt*> _points;
    QHash<FCID, FCGeoMeshVSEdgeEntity*> _edges;
    QHash<FCID, FCGeoMeshVSFaceEntity*> _faces;
};

} // namespace FC

#endif // FCGEOMETRYMESHVS_H
