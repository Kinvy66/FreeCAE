/**
 * @file FCGeoModelManager.h
 * @brief 几何模型管理器（移植自 FITKInterfaceGeometry）
 */
#ifndef FCGEOMODELMANAGER_H
#define FCGEOMODELMANAGER_H

#include "FCGeometryInterfaceAPI.h"
#include <FCModelInterface/FCAbstractGeoModel.h>
#include <FCData/FCAbstractDataManager.hpp>

namespace FC {

class FCGEOMETRYINTERFACE_API FCGeoModelManager
    : public FCAbstractDataManager<FCAbstractGeoModel>
{
public:
    explicit FCGeoModelManager();
    ~FCGeoModelManager() override;

    template<class T>
    T* getDataByIndexT(int index)
    {
        return dynamic_cast<T*>(getDataByIndex(index));
    }
    void getBoundaryBox(double* minPt, double* maxPt);
};

} // namespace FC

#endif // FCGEOMODELMANAGER_H
