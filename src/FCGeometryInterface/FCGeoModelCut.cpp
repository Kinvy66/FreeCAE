/**
 * @file FCGeoModelCut.cpp
 * @brief 实体切割命令实现（接口层）
 */
#include "FCGeoModelCut.h"

namespace FC {

FCGeoEnum::FITKGeometryComType FCGeoModelExtrudeCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTExtrudeCut;
}
bool FCGeoModelExtrudeCut::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelRevolCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTRevolCut;
}
bool FCGeoModelRevolCut::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelSweepCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTSweepCut;
}
bool FCGeoModelSweepCut::update() { return true; }

FCGeoEnum::FITKGeometryComType FCGeoModelMultiSectionCut::getGeometryCommandType()
{
    return FCGeoEnum::FGTMultiSectionCut;
}
bool FCGeoModelMultiSectionCut::update() { return true; }

VirtualShape FCGeoModelMultiSectionCut::section(int index) const
{
    if (index >= 0 && index < m_sections.size())
        return m_sections.at(index);
    return VirtualShape();
}

void FCGeoModelMultiSectionCut::setSection(int index, const VirtualShape& s)
{
    if (index >= 0 && index < m_sections.size())
        m_sections[index] = s;
}

} // namespace FC
