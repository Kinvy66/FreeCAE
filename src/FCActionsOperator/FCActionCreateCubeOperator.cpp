#include "FCActionCreateCubeOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCData/FCDataRepo.h>
#include <FCPropertyWidget.h>
#include <FCCubeInfoWidget.h>
#include <FCRenderWidget.h>
#include <FCGraph3DWindowVTK.h>
#include <FCVTKGraphAdaptor/FCVTKViewAdaptorModelCmd.h>
#include <FCVTKGraphAdaptor/FCVTKGraphObject3D.h>
#include <FCRenderWindowVTK/FCGraphRender.h>
#include <FCRenderWindowVTK/FCGraphObjManager.h>
#include <FCRenderWindowVTK/FCGraphObjectVTK.h>
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
    // 展开几何节点并选中新建的几何体节点
    if (_currentBoxCmd && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(_currentBoxCmd->getDataObjectID());
    }

    // 1. 创建 FCCubeInfoWidget 并放入 FCPropertyWidget（参数设置 Dock）内
    FCCubeInfoWidget* cubeWidget = new FCCubeInfoWidget(propWidget);
    propWidget->setContentWidget(cubeWidget);

    // 2. 根据 boxCmd 将默认参数设置到 UI 对应区域
    cubeWidget->setBoxCommand(_currentBoxCmd);

    connect(cubeWidget, &FCCubeInfoWidget::geometryBuilt, this, &FCActionCreateCubeOperator::onGeometryBuilt);
    connect(cubeWidget, &FCCubeInfoWidget::geometrySequenceBuilt, this, &FCActionCreateCubeOperator::onGeometrySequenceBuilt);

    docking->raiseDockingArea(FCDockingAreaInterface::DockingAreaSetting);
    return true;
}

void FCActionCreateCubeOperator::onGeometryBuilt(FC::FCAbsGeoCommand* cmd)
{
    if (!cmd) return;
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return;
    FCRenderWidget* rw = docking->getGraphicOperateWidget();
    if (!rw) return;
    FCGraph3DWindowVTK* graphWin = rw->getGraph3DWindow();
    if (!graphWin) return;

    FCVTKViewAdaptorModelCmd adaptor;
    adaptor.setDataObject(cmd);
    if (!adaptor.update()) return;
    FCVTKGraphObject3D* graphObj = adaptor.getOutputData();
    if (!graphObj || graphObj->getActorCount() == 0) return;
    graphWin->addObject(0, graphObj, true);
    graphWin->reRender();
}

void FCActionCreateCubeOperator::onGeometrySequenceBuilt(const QList<FC::FCAbsGeoCommand*>& cmds)
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return;
    FCRenderWidget* rw = docking->getGraphicOperateWidget();
    if (!rw) return;
    FCGraph3DWindowVTK* graphWin = rw->getGraph3DWindow();
    if (!graphWin) return;

    FCGraphRender* render0 = graphWin->getRenderer(0);
    if (render0) {
        FCGraphObjManager* mgr = render0->getGraphObjManager();
        if (mgr) {
            int n = mgr->getGraphObjCount();
            for (int i = n - 1; i >= 0; --i) {
                FCGraphObjectVTK* obj = mgr->getGraphObjAt(i);
                render0->removeObject(obj);
            }
        }
    }
    for (FC::FCAbsGeoCommand* cmd : cmds) {
        if (!cmd) continue;
        FCVTKViewAdaptorModelCmd adaptor;
        adaptor.setDataObject(cmd);
        if (!adaptor.update()) continue;
        FCVTKGraphObject3D* graphObj = adaptor.getOutputData();
        if (graphObj && graphObj->getActorCount() > 0)
            graphWin->addObject(0, graphObj, false);
    }
    if (!cmds.isEmpty())
        graphWin->fitView();
    graphWin->reRender();
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

    geoList->appendDataObj(boxCmd);
    _currentBoxCmd = boxCmd;
    return true;
}

} // namespace FC

