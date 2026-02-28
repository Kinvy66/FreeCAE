#include "FCActionCreateConeOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryInterface/FCAbsGeoModelCone.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCDataRepo.h>
#include <FCPropertyWidget.h>
#include <FCGeometryBuildBar.h>
#include <FCConeInfoWidget.h>
#include <QVBoxLayout>
#include "FCOperatorRepo.h"
#include "FCProjectTreeEventOperator.h"
#include "FCGraphPreprocessOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"

namespace FC
{

FCActionCreateConeOperator::FCActionCreateConeOperator()
{
}

bool FCActionCreateConeOperator::execGUI()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return true;
    if (mCurrentNodeId == FCID_INVALID && !mCurrentConeCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;

    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    FCID selectId = (mCurrentNodeId != FCID_INVALID) ? mCurrentNodeId : (mCurrentConeCmd ? mCurrentConeCmd->getDataObjectID() : FCID_INVALID);
    if (selectId != FCID_INVALID && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(selectId);
    }

    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    FCConeInfoWidget* coneWidget = new FCConeInfoWidget(propWidget);
    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(coneWidget, 1);
    propWidget->setContentWidget(container);

    if (FC::FCGlobalData* g = FC::FCGlobalData::getGlobalData()) {
        if (FC::FCGeometryDAGData* dagData = g->getGeometryData<FC::FCGeometryDAGData>()) {
            coneWidget->setDAGNode(dagData, mCurrentNodeId, mCurrentConeCmd);
        }
    }
    if (mCurrentNodeId == FCID_INVALID && mCurrentConeCmd)
        coneWidget->setConeCommand(mCurrentConeCmd);

    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setUIInterface(uiInterface());
        connect(coneWidget, &FCConeInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        connect(coneWidget, &FCConeInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
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

bool FCActionCreateConeOperator::execProfession()
{
    mCurrentConeCmd = nullptr;
    mCurrentNodeId = FCID_INVALID;

    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) {
        qWarning() << "FCActionCreateConeOperator: FCGlobalData not found";
        return false;
    }
    FC::FCGeometryDAGData* dagData = globalData->getGeometryData<FC::FCGeometryDAGData>();
    if (!dagData) {
        qWarning() << "FCActionCreateConeOperator: FCGeometryDAGData not found";
        return false;
    }

    FC::FCGeoParamSet params;
    params[QStringLiteral("location_x")] = 0.0;
    params[QStringLiteral("location_y")] = 0.0;
    params[QStringLiteral("location_z")] = 0.0;
    params[QStringLiteral("axis_x")] = 0.0;
    params[QStringLiteral("axis_y")] = 0.0;
    params[QStringLiteral("axis_z")] = 1.0;
    params[QStringLiteral("height")] = 100.0;
    params[QStringLiteral("bottom_radius")] = 50.0;
    params[QStringLiteral("top_radius")] = 0.0;
    QString name = QStringLiteral("Cone_1");
    mCurrentNodeId = dagData->module()->addCone(params, name);
    dagData->setDirty(true);

    if (mCurrentNodeId != FCID_INVALID)
        mCurrentConeCmd = FCDATAREPO->getDataAs<FC::FCGeoModelCone>(mCurrentNodeId);
    return true;
}

} // namespace FC
