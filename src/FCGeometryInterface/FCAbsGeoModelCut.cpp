/**
 * @file FCAbsGeoModelCut.cpp
 * @brief å®?ä½?å??å?²å?½ä»¤å®?ç?°ï¼?æ?¥å?£å±?ï¼?
 */
#include "FCAbsGeoModelCut.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCAbsGeoModelExtrudeCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtrudeCut;
}
bool FCAbsGeoModelExtrudeCut::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelRevolCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTRevolCut;
}
bool FCAbsGeoModelRevolCut::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelSweepCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTSweepCut;
}
bool FCAbsGeoModelSweepCut::update() { return true; }

FCGeoEnum::FITKGeometryComType FCAbsGeoModelMultiSectionCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTMultiSectionCut;
}
bool FCAbsGeoModelMultiSectionCut::update() { return true; }

VirtualShape FCAbsGeoModelMultiSectionCut::section(int index) const
{
    if (index >= 0 && index < m_sections.size())
        return m_sections.at(index);
    return VirtualShape();
}

void FCAbsGeoModelMultiSectionCut::setSection(int index, const VirtualShape& s)
{
    if (index >= 0 && index < m_sections.size())
        m_sections[index] = s;
}

} // namespace FC
