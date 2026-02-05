/**
 * @file FCElementFactory.h
 * @brief 单元工厂（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTFACTORY_H
#define FCELEMENTFACTORY_H

#include "FCModelInterfaceAPI.h"
#include "FCModelEnum.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementFactory
{
public:
    static FCAbstractElement* createElement(FCModelEnum::FITKEleType t);

private:
    FCElementFactory() = default;
    FCElementFactory(const FCElementFactory&) = delete;
    ~FCElementFactory() = default;
};

} // namespace FC

#endif // FCELEMENTFACTORY_H
