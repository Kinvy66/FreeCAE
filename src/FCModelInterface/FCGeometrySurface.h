/**
 * @file FCGeometrySurface.h
 * @brief 几何模型表面（移植自 FITKInterfaceModel）
 */
#ifndef FCGEOMETRYSURFACE_H
#define FCGEOMETRYSURFACE_H

#include "FCModelInterfaceAPI.h"
#include "FCModelSurface.h"

namespace FC {

class FCAbstractModel;
class FCModelSet;

class FCMODELINTERFACE_API FCGeometrySurface : public FCModelSurface
{
    FC_CLASS(FC, FCGeometrySurface);
    Q_OBJECT
public:
    explicit FCGeometrySurface() = default;
    virtual ~FCGeometrySurface() = default;

    FCModelSurface::ModelSurfaceType getModelSurfaceType() override;

    void addGeometrySet(int modelID, int setID, int surfIndex);
    void addGeometrySet(FCAbstractModel* model, FCModelSet* set, int surfIndex);
    void clearGeometrySet();

    int getSurfaceIndexAt(int index) const;
    void setSurfaceIndex(int index, int surfIndex);

    QList<SurfaceAbsSetInfo> getAbsoluteSetInfo() override;

protected:
    QList<int> _surfIndice;
};

} // namespace FC

#endif // FCGEOMETRYSURFACE_H
