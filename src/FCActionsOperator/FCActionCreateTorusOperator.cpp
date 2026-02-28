#include "FCActionCreateTorusOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryInterface/FCAbsGeoModelTorus.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCDataRepo.h>
#include <FCPropertyWidget.h>
#include <FCGeometryBuildBar.h>
#include <FCTorusInfoWidget.h>
#include <QVBoxLayout>
#include "FCOperatorRepo.h"
#include "FCProjectTreeEventOperator.h"
#include "FCGraphPreprocessOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"

namespace FC
{

FCActionCreateTorusOperator::FCActionCreateTorusOperator()
{
}

bool FCActionCreateTorusOperator::execGUI()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return true;
    if (mCurrentNodeId == FCID_INVALID && !mCurrentTorusCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;

    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    FCID selectId = (mCurrentNodeId != FCID_INVALID) ? mCurrentNodeId : (mCurrentTorusCmd ? mCurrentTorusCmd->getDataObjectID() : FCID_INVALID);
    if (selectId != FCID_INVALID && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(selectId);
    }

    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    FCTorusInfoWidget* torusWidget = new FCTorusInfoWidget(propWidget);
    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(torusWidget, 1);
    propWidget->setContentWidget(container);

    if (FC::FCGlobalData* g = FC::FCGlobalData::getGlobalData()) {
        if (FC::FCGeometryDAGData* dagData = g->getGeometryData<FC::FCGeometryDAGData>()) {
            torusWidget->setDAGNode(dagData, mCurrentNodeId, mCurrentTorusCmd);
        }
    }
    if (mCurrentNodeId == FCID_INVALID && mCurrentTorusCmd)
        torusWidget->setTorusCommand(mCurrentTorusCmd);

    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setUIInterface(uiInterface());
        connect(torusWidget, &FCTorusInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        connect(torusWidget, &FCTorusInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
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

bool FCActionCreateTorusOperator::execProfession()
{
    mCurrentTorusCmd = nullptr;
    mCurrentNodeId = FCID_INVALID;

    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) {
        qWarning() << "FCActionCreateTorusOperator: FCGlobalData not found";
        return false;
    }
    FC::FCGeometryDAGData* dagData = globalData->getGeometryData<FC::FCGeometryDAGData>();
    if (!dagData) {
        qWarning() << "FCActionCreateTorusOperator: FCGeometryDAGData not found";
        return false;
    }

    FC::FCGeoParamSet params;
    params[QStringLiteral("location_x")] = 0.0;
    params[QStringLiteral("location_y")] = 0.0;
    params[QStringLiteral("location_z")] = 0.0;
    params[QStringLiteral("axis_x")] = 0.0;
    params[QStringLiteral("axis_y")] = 0.0;
    params[QStringLiteral("axis_z")] = 1.0;
    params[QStringLiteral("major_radius")] = 50.0;
    params[QStringLiteral("minor_radius")] = 15.0;
    params[QStringLiteral("angle")] = 360.0;
    QString name = QStringLiteral("Torus_1");
    mCurrentNodeId = dagData->module()->addTorus(params, name);
    dagData->setDirty(true);

    if (mCurrentNodeId != FCID_INVALID)
        mCurrentTorusCmd = FCDATAREPO->getDataAs<FC::FCGeoModelTorus>(mCurrentNodeId);
    return true;
}

} // namespace FC
