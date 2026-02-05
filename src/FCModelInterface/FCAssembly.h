/**
 * @file FCAssembly.h
 * @brief 装配体（移植自 FITKInterfaceModel）
 */
#ifndef FCASSEMBLY_H
#define FCASSEMBLY_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModel.h"
#include "FCAbstractAssInstance.h"
#include "FCModelEnum.h"
#include <FCData/FCAbstractDataManager.hpp>
#include <FCData/FCMacros.h>

namespace FC {

class FCMODELINTERFACE_API FCAssembly : public FCAbstractModel,
    public FCAbstractDataManager<FCAbstractAssInstance>
{
    FC_CLASS(FC, FCAssembly);
public:
    explicit FCAssembly();
    virtual ~FCAssembly();

    virtual FCModelEnum::AbsModelType getAbsModelType() override;

    template<class T>
    T* getInstanceAs(int index) { return dynamic_cast<T*>(getDataByIndex(index)); }
};

} // namespace FC

#endif // FCASSEMBLY_H
