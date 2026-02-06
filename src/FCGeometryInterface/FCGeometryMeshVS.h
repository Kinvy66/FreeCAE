/**
 * @file FCGeometryMeshVS.h
 * @brief 几何可视化网格占位（移植自 FITKInterfaceGeometry，具体实现可后续接 OCC/VTK）
 */
#ifndef FCGEOMETRYMESHVS_H
#define FCGEOMETRYMESHVS_H

#include "FCGeometryInterfaceAPI.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCMacros.h>

namespace FC {

class FCGEOMETRYINTERFACE_API FCGeometryMeshVS : public FCAbstractDataObject
{
    FC_CLASS(FC, FCGeometryMeshVS);
public:
    explicit FCGeometryMeshVS();
    ~FCGeometryMeshVS() override;
    void clear();
};

} // namespace FC

#endif // FCGEOMETRYMESHVS_H
