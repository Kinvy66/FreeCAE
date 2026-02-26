#include "FCActionCreateCubeOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeoNode.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCDataRepo.h>
#include <FCData/FCAbstractDataObject.h>
#include <FCPropertyWidget.h>
#include <FCGeometryBuildBar.h>
#include <FCCubeInfoWidget.h>
#include <QVBoxLayout>
#include <QDebug>
#include "FCOperatorRepo.h"
#include "FCProjectTreeEventOperator.h"
#include "FCGraphPreprocessOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"

namespace FC
{

FCActionCreateCubeOperator::FCActionCreateCubeOperator()
{
}

bool FCActionCreateCubeOperator::execGUI()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return true;
    if (mCurrentNodeId < 0 && !mCurrentBoxCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;
    
    // 更新工程树：几何序列（命令列表）在「几何」节点下显示，符合函数式几何设计（只保存序列，不保存具体实体）
    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    // 展开几何节点并选中新建的节点（DAG 节点 ID 或旧版命令 ID）
    int selectId = mCurrentNodeId >= 0 ? mCurrentNodeId : (mCurrentBoxCmd ? mCurrentBoxCmd->getDataObjectID() : -1);
    if (selectId >= 0 && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(selectId);
    }

    // 1. 选中几何体节点时：组合「构建栏 + 几何属性」再放入属性窗口（仅几何体显示构建栏）
    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    FCCubeInfoWidget* cubeWidget = new FCCubeInfoWidget(propWidget);
    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(cubeWidget, 1);
    propWidget->setContentWidget(container);

    // 2. 绑定 DAG 节点（及用于 VTK 显示的 Box 命令）或旧版 box 命令到 UI
    if (FC::FCGlobalData* g = FC::FCGlobalData::getGlobalData()) {
        if (FC::FCGeometryDAGData* dagData = g->getGeometryData<FC::FCGeometryDAGData>()) {
            cubeWidget->setDAGNode(dagData, mCurrentNodeId, mCurrentBoxCmd);
        }
    }
    if (mCurrentNodeId < 0 && mCurrentBoxCmd)
        cubeWidget->setBoxCommand(mCurrentBoxCmd);

    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setUIInterface(uiInterface());
        connect(cubeWidget, &FCCubeInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        connect(cubeWidget, &FCCubeInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
    }
    FCBuildGeometryOperator* buildOper = FCOPERATORREPO->getOperatorT<FCBuildGeometryOperator>("BuildGeometry");
    FCBuildAllGeometryOperator* buildAllOper = FCOPERATORREPO->getOperatorT<FCBuildAllGeometryOperator>("BuildAllGeometry");
    if (buildOper) {
        buildOper->setUIInterface(uiInterface());
        connect(buildBar, &FCGeometryBuildBar::buildClicked, this, [buildOper]() { buildOper->actionTriggered(); });
    }
    if (buildAllOper) {
        buildAllOper->setUIInterface(uiInterface());
        connect(buildBar, &FCGeometryBuildBar::buildAllClicked, this, [buildAllOper]() { buildAllOper->actionTriggered(); });
    }

    docking->raiseDockingArea(FCDockingAreaInterface::DockingAreaSetting);
    return true;
}

bool FCActionCreateCubeOperator::execProfession()
{
    mCurrentBoxCmd = nullptr;
    mCurrentNodeId = -1;

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

    // 参数键与 FCGeometryModule::applyParamsToCommand(Block) 一致
    FC::FCGeoParamSet params;
    params[QStringLiteral("point1_x")] = 0.0;
    params[QStringLiteral("point1_y")] = 0.0;
    params[QStringLiteral("point1_z")] = 0.0;
    params[QStringLiteral("length_x")] = 100.0;
    params[QStringLiteral("length_y")] = 100.0;
    params[QStringLiteral("length_z")] = 100.0;
    QString name = QStringLiteral("Box_1");
    mCurrentNodeId = dagData->module()->addBlock(params, name);
    dagData->setDirty(true);

    // 节点 ID = 命令 ID：直接从 Repo 取该 Box 命令用于 VTK 显示，无需再创建一份
    if (mCurrentNodeId >= 0) {
        mCurrentBoxCmd = FC::FCDATAREPO->getDataAs<FC::FCGeoModelBox>(mCurrentNodeId);
        if (mCurrentBoxCmd && !mCurrentBoxCmd->update())
            mCurrentBoxCmd = nullptr;
    }
    return true;
}

} // namespace FC

