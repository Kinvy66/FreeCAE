#include "FCActionCreateCubeOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCData/FCDataRepo.h>
#include <FCGUI/FCPropertyWidget.h>
#include <FCGUI/FCGeometryProperty/FCCubeInfoWidget.h>
#include <QDebug>

namespace FC
{

FCActionCreateCubeOperator::FCActionCreateCubeOperator()
{
}

bool FCActionCreateCubeOperator::execGUI()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking || !_currentBoxCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;

    // 1. 创建 FCCubeInfoWidget 并放入 FCPropertyWidget（参数设置 Dock）内
    FCCubeInfoWidget* cubeWidget = new FCCubeInfoWidget(propWidget);
    propWidget->setContentWidget(cubeWidget);

    // 2. 根据 boxCmd 将默认参数设置到 UI 对应区域
    cubeWidget->setBoxCommand(_currentBoxCmd);

    docking->raiseDockingArea(FCDockingAreaInterface::DockingAreaSetting);
    return true;
}

bool FCActionCreateCubeOperator::execProfession()
{
    _currentBoxCmd = nullptr;

    FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
    if (!factory) {
        qWarning() << "FCActionCreateCubeOperator: FCGeoInterfaceFactory is null";
        return false;
    }

    FCGeoModelBox* boxCmd = factory->createCommandT<FCGeoModelBox>(FCGeoEnum::FGTBox);
    if (!boxCmd) {
        qWarning() << "FCActionCreateCubeOperator: Failed to create FCGeoModelBox";
        return false;
    }

    FCGeoCommandList* geoList = FCDATAREPO->getFirstDataByType<FCGeoCommandList>();
    if (!geoList) {
        qWarning() << "FCActionCreateCubeOperator: FCGeoCommandList not found (ensure createGeoData() creates it)";
        return false;
    }

    double point1[3] = { 0.0, 0.0, 0.0 };
    double length[3] = { 100.0, 100.0, 100.0 };
    boxCmd->setPoint1(point1);
    boxCmd->setLength(length);

    QString name = QString("Box_%1").arg(boxCmd->getDataObjectID());
    boxCmd->setDataObjectName(geoList->checkName(name));

    if (!boxCmd->update()) {
        qWarning() << "FCActionCreateCubeOperator: boxCmd->update() failed";
        return false;
    }

    geoList->appendDataObj(boxCmd);
    _currentBoxCmd = boxCmd;
    return true;
}

} // namespace FC

