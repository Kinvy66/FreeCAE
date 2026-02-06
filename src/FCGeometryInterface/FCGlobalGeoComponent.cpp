/**
 * @file FCGlobalGeoComponent.cpp
 * @brief 全局几何分组实现（FCGeoComponent 由 FCModelInterface 提供）
 */
#include "FCGlobalGeoComponent.h"

namespace FC {

FCGlobalGeoComponent::FCGlobalGeoComponent(FCModelEnum::FITKModelSetType type)
    : FCAbstractNamedDataObject()
    , m_geoType(type)
{}

} // namespace FC
