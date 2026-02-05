/**
 * @file FCAbstractPart.h
 * @brief 抽象部件模型（移植自 FITKInterfaceModel）
 */
#ifndef FCABSTRACTPART_H
#define FCABSTRACTPART_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModel.h"
#include <FCData/FCAbstractNamedDataObject.h>

namespace FC {

class FCMODELINTERFACE_API FCAbstractPart : public FCAbstractModel
{
public:
    explicit FCAbstractPart() = default;
    virtual ~FCAbstractPart() = default;

    virtual FCAbstractNamedDataObject* getAbstractMesh() = 0;

    template<class T>
    T* getMeshAs() { return dynamic_cast<T*>(getAbstractMesh()); }

    virtual FCAbstractNamedDataObject* getAbstractGeom() = 0;

    template<class T>
    T* getGeomAs() { return dynamic_cast<T*>(getAbstractGeom()); }
};

} // namespace FC

#endif // FCABSTRACTPART_H
