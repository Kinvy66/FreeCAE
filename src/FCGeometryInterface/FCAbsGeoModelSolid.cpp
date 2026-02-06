/**
 * @file FCGeoModelSolid.cpp
 * @brief å®?ä½?å? ä½?å?½ä»¤å®?ç?°ï¼?æ?¥å?£å±?ï¼?
 */
#include "FCAbsGeoModelSolid.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCAbsGeoModelClosedSurfaceSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTClosedSurfaceSolid;
}
bool FCAbsGeoModelClosedSurfaceSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelExtrudeSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtrudeSolid;
}
bool FCAbsGeoModelExtrudeSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelRevolSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTRevolSolid;
}
bool FCAbsGeoModelRevolSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelSweepSolid::getGeometryCommandType()
{
    return FCGeoEnum::FGTSweepSolid;
}
bool FCAbsGeoModelSweepSolid::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelMultiSectionSolid::getGeometryCommandType()
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
