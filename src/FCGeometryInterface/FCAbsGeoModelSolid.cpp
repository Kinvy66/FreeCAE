/**
 * @file FCGeoModelSolid.cpp
 * @brief 实体几何命令实现（接口层）
 */
#include "FCAbsGeoModelSolid.h"

namespace FC {

FCGeoEnum::FCGeometryComType FCAbsGeoModelClosedSurfaceSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTClosedSurfaceSolid;
}
bool FCAbsGeoModelClosedSurfaceSolid::update() { return true; }

FCGeoEnum::FCGeometryComType FCAbsGeoModelExtrudeSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtrudeSolid;
}
bool FCAbsGeoModelExtrudeSolid::update() { return true; }

FCGeoEnum::FCGeometryComType FCAbsGeoModelRevolSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTRevolSolid;
}
bool FCAbsGeoModelRevolSolid::update() { return true; }

FCGeoEnum::FCGeometryComType FCAbsGeoModelSweepSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTSweepSolid;
}
bool FCAbsGeoModelSweepSolid::update() { return true; }

FCGeoEnum::FCGeometryComType FCAbsGeoModelMultiSectionSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTMultiSectionSolid;
}
bool FCAbsGeoModelMultiSectionSolid::update() { return true; }

VirtualShape FCAbsGeoModelMultiSectionSolid::section(int index) const
{
    if (index >= 0 && index < m_sections.size())
        return m_sections.at(index);
    return VirtualShape();
}

void FCAbsGeoModelMultiSectionSolid::setSection(int index, const VirtualShape& s)
{
    if (index >= 0 && index < m_sections.size())
        m_sections[index] = s;
}

} // namespace FC
