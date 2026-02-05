/**
 * @file FCAssembly.cpp
 * @brief 装配体实现（移植自 FITKInterfaceModel）
 */
#include "FCAssembly.h"

namespace FC {

FCAssembly::FCAssembly()
{}

FCAssembly::~FCAssembly()
{}

FCModelEnum::AbsModelType FCAssembly::getAbsModelType()
{
    return FCModelEnum::Assembly;
}

} // namespace FC
