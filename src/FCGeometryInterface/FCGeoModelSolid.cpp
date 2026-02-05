/**
 * @file FCGeoModelSolid.cpp
 * @brief 实体几何命令实现（接口层）
 */
#include "FCGeoModelSolid.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelClosedSurfaceSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTClosedSurfaceSolid;
}
bool FCGeoModelClosedSurfaceSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelExtrudeSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtrudeSolid;
}
bool FCGeoModelExtrudeSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelRevolSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTRevolSolid;
}
bool FCGeoModelRevolSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelSweepSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTSweepSolid;
}
bool FCGeoModelSweepSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelMultiSectionSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTMultiSectionSolid;
}
bool FCGeoModelMultiSectionSolid::update() { return true; }

VirtualShape FCGeoModelMultiSectionSolid::section(int index) const
{
    if (index >= 0 && index < m_sections.size())
        return m_sections.at(index);
    return VirtualShape();
}

void FCGeoModelMultiSectionSolid::setSection(int index, const VirtualShape& s)
{
    if (index >= 0 && index < m_sections.size())
        m_sections[index] = s;
}

} // namespace FC
