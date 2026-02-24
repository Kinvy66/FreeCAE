#include "FCActionCreateCubeOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeoNode.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCAbstractDataObject.h>
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
    if (!docking) return true;
    if (_currentNodeId < 0 && !_currentBoxCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;
    
    // 更新工程树：几何序列（命令列表）在「几何」节点下显示，符合函数式几何设计（只保存序列，不保存具体实体）
    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    // 展开几何节点并选中新建的节点（DAG 节点 ID 或旧版命令 ID）
    int selectId = _currentNodeId >= 0 ? _currentNodeId : (_currentBoxCmd ? _currentBoxCmd->getDataObjectID() : -1);
    if (selectId >= 0 && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(selectId);
    }

    // 1. 创建 FCCubeInfoWidget 并放入 FCPropertyWidget（参数设置 Dock）内
    FCCubeInfoWidget* cubeWidget = new FCCubeInfoWidget(propWidget);
    propWidget->setContentWidget(cubeWidget);

    // 2. 绑定 DAG 节点（及用于 VTK 显示的 Box 命令）或旧版 box 命令到 UI
    if (FC::FCGlobalData* g = FC::FCGlobalData::getGlobalData()) {
        if (FC::FCGeometryDAGData* dagData = g->getGeometryData<FC::FCGeometryDAGData>()) {
            cubeWidget->setDAGNode(dagData, _currentNodeId, _currentBoxCmd);
        }
    }
    if (_currentNodeId < 0 && _currentBoxCmd)
        cubeWidget->setBoxCommand(_currentBoxCmd);

    connect(cubeWidget, &FCCubeInfoWidget::geometryBuilt, this, &FCActionCreateCubeOperator::onGeometryBuilt);
    connect(cubeWidget, &FCCubeInfoWidget::geometrySequenceBuilt, this, &FCActionCreateCubeOperator::onGeometrySequenceBuilt);

    docking->raiseDockingArea(FCDockingAreaInterface::DockingAreaSetting);
    return true;
}

void FCActionCreateCubeOperator::onGeometryBuilt(FC::FCAbsGeoCommand* cmd)
{
    if (!cmd) return;  // DAG 路径不发射 cmd，由 ensureBuild + 实体模型刷新处理
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return;
    FCRenderWidget* rw = docking->getGraphicOperateWidget();
    if (!rw) return;
    FCGraph3DWindowVTK* graphWin = rw->getGraph3DWindow();
    if (!graphWin) return;

    FCVTKViewAdaptorModelCmd adaptor;
    adaptor.setDataObject(static_cast<FC::FCAbstractDataObject*>(cmd));
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
        adaptor.setDataObject(static_cast<FC::FCAbstractDataObject*>(cmd));
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
    _currentNodeId = -1;

    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) {
        qWarning() << "FCActionCreateCubeOperator: FCGlobalData not found";
        return false;
    }
    FC::FCGeometryDAGData* dagData = globalData->getGeometryData<FC::FCGeometryDAGData>();
    if (!dagData) {
        qWarning() << "FCActionCreateCubeOperator: FCGeometryDAGData not found (ensure createGeoData() creates it)";
        return false;
    }

    FC::FCGeoParamSet params;
    params[QStringLiteral("length")] = 100.0;
    params[QStringLiteral("width")]  = 100.0;
    params[QStringLiteral("height")] = 100.0;
    QString name = QStringLiteral("Box_1");
    _currentNodeId = dagData->module()->addBlock(params, name);
    dagData->ensureBuild();

    // 创建用于 VTK 显示的 Box 命令（不加入命令列表），点击「构建」时用其驱动 3D 显示
    FC::FCGeoInterfaceFactory* factory = FC::FCGeoInterfaceFactory::instance();
    if (factory) {
        FC::FCGeoModelBox* boxCmd = factory->createCommandT<FC::FCGeoModelBox>(FC::FCGeoEnum::FGTBox);
        if (boxCmd) {
            double point1[3] = { 0.0, 0.0, 0.0 };
            double length[3] = { 100.0, 100.0, 100.0 };
            boxCmd->setPoint1(point1);
            boxCmd->setLength(length);
            if (boxCmd->update())
                _currentBoxCmd = boxCmd;
            else
                delete boxCmd;
        }
    }
    return true;
}

} // namespace FC

