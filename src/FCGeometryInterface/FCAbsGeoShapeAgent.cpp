/**
 * @file FCAbsGeoShapeAgent.cpp
 * @brief 抽象几何形状代理实现（移植自 FITKInterfaceGeometry）
 */
#include "FCAbsGeoShapeAgent.h"
#include "FCAbsGeoCommand.h"
#include "FCVirtualTopoManager.h"
#include <QVariant>

namespace FC {

FCAbsGeoShapeAgent::FCAbsGeoShapeAgent(FCAbsGeoCommand* command)
    : _command(command)
{
    _vtmanager = new FCVirtualTopoManager;
}

FCAbsGeoShapeAgent::~FCAbsGeoShapeAgent()
{
    if (_vtmanager) { delete _vtmanager; _vtmanager = nullptr; }
    if (_meshVS) { delete _meshVS; _meshVS = nullptr; }
}

FCAbsGeoCommand* FCAbsGeoShapeAgent::getGeoCommand()
{
    return _command;
}

FCVirtualTopoManager* FCAbsGeoShapeAgent::getVirtualTopoManager()
{
    return _vtmanager;
}

void FCAbsGeoShapeAgent::updateVS()
{
    buildVirtualTopo(false);
    triangulation();
}

FCGeometryMeshVS* FCAbsGeoShapeAgent::getMeshVS()
{
    return _meshVS;
}

bool FCAbsGeoShapeAgent::writeSTLFile(QString filePath)
{
    Q_UNUSED(filePath);
    return false;
}

bool FCAbsGeoShapeAgent::isSameState(QVariant& stateVal1, QVariant& stateVal2)
{
    Q_UNUSED(stateVal1);
    Q_UNUSED(stateVal2);
    return false;
}

} // namespace FC
