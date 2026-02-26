/**
 * @file IGeometryBuildContent.h
 * @brief 几何体属性内容“构建/全部构建”能力接口，供 FCPropertyWidget 顶部公共操作栏调用
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef IGEOMETRYBUILDCONTENT_H
#define IGEOMETRYBUILDCONTENT_H

#include "../FCGUIWidgetAPI.h"

namespace FC {

class FCAbsGeoCommand;

/**
 * @brief 几何体属性内容接口：支持「构建」「全部构建」的 widget 实现此接口
 *
 * FCPropertyWidget 上部为共有操作（构建、全部构建等），下部为当前选中项的属性。
 * 当内容 widget 实现本接口时，点击公共栏的「构建」「全部构建」会调用对应方法。
 */
class FCGUIWIDGET_API IGeometryBuildContent
{
public:
    virtual ~IGeometryBuildContent() = default;

    /** 将当前 UI 参数写回模型（DAG/命令），不触发构建与显示 */
    virtual void syncToModel() = 0;

    /** 获取当前要构建的几何命令（syncToModel 之后调用），无则返回 nullptr */
    virtual FCAbsGeoCommand* getCurrentBuildCommand() = 0;

    /** 构建：将当前参数写回并构建当前选中节点之前的所有几何，形成几何实体并在 VTK 中显示 */
    virtual void executeBuild() = 0;

    /** 全部构建：构建几何节点下所有几何，并在 VTK 中显示 */
    virtual void executeBuildAll() = 0;
};

} // namespace FC

#endif // IGEOMETRYBUILDCONTENT_H
