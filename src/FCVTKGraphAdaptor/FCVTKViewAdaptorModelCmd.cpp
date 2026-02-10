/**
 * @file FCVTKViewAdaptorModelCmd.cpp
 * @brief 几何命令 -> VTK 图元适配器实现
 */
#include "FCVTKViewAdaptorModelCmd.h"
#include "FCVTKGraphObjectModelCmd.h"
#include <FCGeometryInterface/FCAbsGeoCommand.h>

namespace FC {

bool FCVTKViewAdaptorModelCmd::update()
{
    if (!_dataObj) return false;
    if (!m_needUpdate && m_outputData) return true;

    FCAbsGeoCommand* cmd = dynamic_cast<FCAbsGeoCommand*>(_dataObj);
    if (!cmd) return false;

    if (m_outputData) {
        delete m_outputData;
        m_outputData = nullptr;
    }

    FCVTKGraphObjectModelCmd* outputData = new FCVTKGraphObjectModelCmd(cmd);
    m_needUpdate = false;
    if (outputData->getActorCount() == 0) {
        delete outputData;
        return false;
    }
    m_outputData = outputData;
    return true;
}

} // namespace FC
