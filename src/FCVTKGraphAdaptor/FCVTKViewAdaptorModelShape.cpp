/**
 * @file FCVTKViewAdaptorModelShape.cpp
 * @brief 几何形状代理 -> VTK 图元：从 FCAbsGeoShapeAgent 得到 FCAbsGeoCommand，生成 FCVTKGraphObjectModelCmd
 */
#include "FCVTKViewAdaptorModelShape.h"
#include "FCVTKGraphObjectModelCmd.h"
#include <FCGeometryInterface/FCAbsGeoShapeAgent.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>

namespace FC {

bool FCVTKViewAdaptorModelShape::update()
{
    if (!_dataObj) return false;
    if (!m_needUpdate && m_outputData) return true;

    FCAbsGeoShapeAgent* shapeAgent = dynamic_cast<FCAbsGeoShapeAgent*>(_dataObj);
    if (!shapeAgent) return false;
    FCAbsGeoCommand* cmd = shapeAgent->getGeoCommand();
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
