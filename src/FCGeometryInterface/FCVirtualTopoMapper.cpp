/**
 * @file FCVirtualTopoMapper.cpp
 * @brief 虚拓扑关系查询实现
 */
#include "FCVirtualTopoMapper.h"

namespace FC {

FCVirtualTopoMapper::VirtualTopoResolver FCVirtualTopoMapper::s_resolver = nullptr;

void FCVirtualTopoMapper::setVirtualTopoResolver(VirtualTopoResolver resolver)
{
    s_resolver = resolver;
}

FCVirtualTopoMapper::VirtualTopoResolver FCVirtualTopoMapper::getVirtualTopoResolver()
{
    return s_resolver;
}

void FCVirtualTopoMapper::mapTopo(FCAbsVirtualTopo* vtp, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
{
    m_mappedTopo.clear();
    if (!vtp) return;
    FCGeoEnum::VTopoShapeType st = vtp->getShapeType();
    if (static_cast<int>(shapeType) <= static_cast<int>(st))
        iteratorLower(vtp, shapeType, basicTopoOnly);
    else
        iteratorUpper(vtp, shapeType, basicTopoOnly);
}

void FCVirtualTopoMapper::mapTopo(int vtpID, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
{
    m_mappedTopo.clear();
    if (s_resolver) {
        FCAbsVirtualTopo* vtp = s_resolver(vtpID);
        if (vtp) mapTopo(vtp, shapeType, basicTopoOnly);
    }
}

int FCVirtualTopoMapper::length() const
{
    return m_mappedTopo.size();
}

FCAbsVirtualTopo* FCVirtualTopoMapper::virtualTopo(int index) const
{
    if (index < 0 || index >= m_mappedTopo.size()) return nullptr;
    return m_mappedTopo.at(index);
}

void FCVirtualTopoMapper::iteratorLower(FCAbsVirtualTopo* topo, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
{
    if (!topo) return;
    if (basicTopoOnly && topo->getCombinedTopoCount() != 0) return;
    if (!basicTopoOnly && topo->getCombinedToTopo()) return;
    FCGeoEnum::VTopoShapeType t = topo->getShapeType();
    if (static_cast<int>(t) < static_cast<int>(shapeType)) return;
    if (static_cast<int>(t) == static_cast<int>(shapeType) && !m_mappedTopo.contains(topo))
        m_mappedTopo.append(topo);
    const int n = topo->getSubTopoCount();
    for (int i = 0; i < n; ++i)
        iteratorLower(topo->getSubTopo(i), shapeType, basicTopoOnly);
}

void FCVirtualTopoMapper::iteratorUpper(FCAbsVirtualTopo* topo, FCGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
{
    if (!topo) return;
    if (basicTopoOnly && topo->getCombinedTopoCount() != 0) return;
    if (!basicTopoOnly && topo->getCombinedToTopo()) return;
    FCGeoEnum::VTopoShapeType t = topo->getShapeType();
    if (static_cast<int>(t) > static_cast<int>(shapeType)) return;
    if (static_cast<int>(t) == static_cast<int>(shapeType) && !m_mappedTopo.contains(topo))
        m_mappedTopo.append(topo);
    const int n = topo->getParentTopoCount();
    for (int i = 0; i < n; ++i)
        iteratorUpper(topo->getParentTopo(i), shapeType, basicTopoOnly);
}

} // namespace FC
