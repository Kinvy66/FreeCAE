/**
 * @file FCVirtualTopoMapper.h
 * @brief 虚拓扑映射关系查询（移植自 FITKInterfaceGeometry FITKAbsVirtualTopoMapper）
 */
#ifndef FCVIRTUALTOPOMAPPER_H
#define FCVIRTUALTOPOMAPPER_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoEnum.h"
#include "FCAbsVirtualTopo.h"
#include <QList>

namespace FC {

/**
 * @brief 拓扑关系查询：按形状类型向下/向上迭代子拓扑或父拓扑
 */
class FCGEOMETRYINTERFACE_API FCVirtualTopoMapper
{
public:
    explicit FCVirtualTopoMapper() = default;
    virtual ~FCVirtualTopoMapper() = default;

    void mapTopo(FCAbsVirtualTopo* vtp, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly = true);
    void mapTopo(int vtpID, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly = true);
    int length() const;
    FCAbsVirtualTopo* virtualTopo(int index) const;

    /** 注入按 ID 解析虚拓扑的回调（如从 FCDataRepo 查询），未设置时 mapTopo(int) 仅清空结果 */
    using VirtualTopoResolver = FCAbsVirtualTopo* (*)(int id);
    static void setVirtualTopoResolver(VirtualTopoResolver resolver);
    static VirtualTopoResolver getVirtualTopoResolver();

private:
    void iteratorLower(FCAbsVirtualTopo* topo, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly);
    void iteratorUpper(FCAbsVirtualTopo* topo, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly);

    QList<FCAbsVirtualTopo*> m_mappedTopo;
    static VirtualTopoResolver s_resolver;
};

} // namespace FC

#endif // FCVIRTUALTOPOMAPPER_H
