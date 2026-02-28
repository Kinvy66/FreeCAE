#include "FCActionCreateHelixOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryInterface/FCAbsGeoModelHelix.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCDataRepo.h>
#include <FCPropertyWidget.h>
#include <FCGeometryBuildBar.h>
#include <FCHelixInfoWidget.h>
#include <QVBoxLayout>
#include "FCOperatorRepo.h"
#include "FCProjectTreeEventOperator.h"
#include "FCGraphPreprocessOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"

namespace FC
{

FCActionCreateHelixOperator::FCActionCreateHelixOperator()
{
}

bool FCActionCreateHelixOperator::execGUI()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return true;
    if (mCurrentNodeId == FCID_INVALID && !mCurrentHelixCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;

    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    FCID selectId = (mCurrentNodeId != FCID_INVALID) ? mCurrentNodeId : (mCurrentHelixCmd ? mCurrentHelixCmd->getDataObjectID() : FCID_INVALID);
    if (selectId != FCID_INVALID && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(selectId);
    }

    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    FCHelixInfoWidget* helixWidget = new FCHelixInfoWidget(propWidget);
    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(helixWidget, 1);
    propWidget->setContentWidget(container);

    if (FC::FCGlobalData* g = FC::FCGlobalData::getGlobalData()) {
        if (FC::FCGeometryDAGData* dagData = g->getGeometryData<FC::FCGeometryDAGData>()) {
            helixWidget->setDAGNode(dagData, mCurrentNodeId, mCurrentHelixCmd);
        }
    }
    if (mCurrentNodeId == FCID_INVALID && mCurrentHelixCmd)
        helixWidget->setHelixCommand(mCurrentHelixCmd);

    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setUIInterface(uiInterface());
        connect(helixWidget, &FCHelixInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        connect(helixWidget, &FCHelixInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
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

bool FCActionCreateHelixOperator::execProfession()
{
    mCurrentHelixCmd = nullptr;
    mCurrentNodeId = FCID_INVALID;

    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) {
        qWarning() << "FCActionCreateHelixOperator: FCGlobalData not found";
        return false;
    }
    FC::FCGeometryDAGData* dagData = globalData->getGeometryData<FC::FCGeometryDAGData>();
    if (!dagData) {
        qWarning() << "FCActionCreateHelixOperator: FCGeometryDAGData not found";
        return false;
    }

    FC::FCGeoParamSet params;
    params[QStringLiteral("location_x")] = 0.0;
    params[QStringLiteral("location_y")] = 0.0;
    params[QStringLiteral("location_z")] = 0.0;
    params[QStringLiteral("number_of_turns")] = 5.0;
    params[QStringLiteral("major_radius")] = 30.0;
    params[QStringLiteral("minor_radius")] = 5.0;
    params[QStringLiteral("axial_pitch")] = 20.0;
    params[QStringLiteral("chirality")] = 1.0;
    QString name = QStringLiteral("Helix_1");
    mCurrentNodeId = dagData->module()->addHelix(params, name);
    dagData->setDirty(true);

    if (mCurrentNodeId != FCID_INVALID)
        mCurrentHelixCmd = FCDATAREPO->getDataAs<FC::FCGeoModelHelix>(mCurrentNodeId);
    return true;
}

} // namespace FC
