#include "FCActionCreateCubeOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCData/FCDataRepo.h>
#include <FCPropertyWidget.h>
#include <FCCubeInfoWidget.h>
#include <QDebug>
#include "FCOperatorRepo.h"
#include "FCProjectTreeEventOperator.h"

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
    
    // 更新工程树：几何序列（命令列表）在「几何」节点下显示，符合函数式几何设计（只保存序列，不保存具体实体）
    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    
    
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
    
    QString name = QString("Box_1");//.arg(boxCmd->getDataObjectID());
    boxCmd->setDataObjectName(geoList->checkName(name));

    // 函数式几何：仅将命令追加到序列（FCGeoCommandList），不保存具体几何实体；构建由 FCCubeInfoWidget「构建」触发并更新 FCGeometryEntityModel
    geoList->appendDataObj(boxCmd);
    _currentBoxCmd = boxCmd;
    return true;
}

} // namespace FC

