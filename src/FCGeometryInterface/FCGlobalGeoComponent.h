/**
 * @file FCGlobalGeoComponent.h
 * @brief 全局几何分组（移植自 FITKInterfaceGeometry FITKGlobalGeoComponent）
 * 使用 FCModelInterface 中的 FCGeoComponent，避免类型重定义。
 */
#ifndef FCGLOBALGEOCOMPONENT_H
#define FCGLOBALGEOCOMPONENT_H

#include "FCGeometryInterfaceAPI.h"
#include <FCModelInterface/FCAbstractGeoModel.h>
#include <FCData/FCAbstractDataManager.hpp>
#include <FCData/FCAbstractNamedDataObject.h>
#include <FCData/FCMacros.h>

namespace FC {

/**
 * @brief 全局几何分组（可存储不同模型内同种类型形状）
 * 管理的元素类型为 FCModelInterface::FCGeoComponent
 */
class FCGEOMETRYINTERFACE_API FCGlobalGeoComponent : public FCAbstractDataManager<FCGeoComponent>,
    public FCAbstractNamedDataObject
{
    FC_CLASS(FC, FCGlobalGeoComponent);
public:
    explicit FCGlobalGeoComponent(FCModelEnum::FITKModelSetType type);
    ~FCGlobalGeoComponent() override = default;
    FCModelEnum::FITKModelSetType getGeoType() const { return m_geoType; }
protected:
    FCModelEnum::FITKModelSetType m_geoType{ FCModelEnum::FMSNone };
};

/**
 * @brief 全局几何分组管理器
 */
class FCGEOMETRYINTERFACE_API FCGlobalGeoComponentManager : public FCAbstractDataManager<FCGlobalGeoComponent>
{
public:
    FCGlobalGeoComponentManager() = default;
    ~FCGlobalGeoComponentManager() override = default;
};

} // namespace FC

#endif // FCGLOBALGEOCOMPONENT_H
