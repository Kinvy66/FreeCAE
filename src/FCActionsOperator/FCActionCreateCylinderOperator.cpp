#include "FCActionCreateCylinderOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryInterface/FCAbsGeoModelCylinder.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCDataRepo.h>
#include <FCPropertyWidget.h>
#include <FCGeometryBuildBar.h>
#include <FCCylinderInfoWidget.h>
#include <QVBoxLayout>
#include <QDebug>
#include "FCOperatorRepo.h"
#include "FCProjectTreeEventOperator.h"
#include "FCGraphPreprocessOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"

namespace FC
{

FCActionCreateCylinderOperator::FCActionCreateCylinderOperator()
{
}

bool FCActionCreateCylinderOperator::execGUI()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return true;
    if (mCurrentNodeId == FCID_INVALID && !mCurrentCylinderCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;

    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    FCID selectId = (mCurrentNodeId != FCID_INVALID) ? mCurrentNodeId : (mCurrentCylinderCmd ? mCurrentCylinderCmd->getDataObjectID() : FCID_INVALID);
    if (selectId != FCID_INVALID && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(selectId);
    }

    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    FCCylinderInfoWidget* cylinderWidget = new FCCylinderInfoWidget(propWidget);
    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(cylinderWidget, 1);
    propWidget->setContentWidget(container);

    if (FC::FCGlobalData* g = FC::FCGlobalData::getGlobalData()) {
        if (FC::FCGeometryDAGData* dagData = g->getGeometryData<FC::FCGeometryDAGData>()) {
            cylinderWidget->setDAGNode(dagData, mCurrentNodeId, mCurrentCylinderCmd);
        }
    }
    if (mCurrentNodeId == FCID_INVALID && mCurrentCylinderCmd)
        cylinderWidget->setCylinderCommand(mCurrentCylinderCmd);

    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setUIInterface(uiInterface());
        connect(cylinderWidget, &FCCylinderInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        connect(cylinderWidget, &FCCylinderInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
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

bool FCActionCreateCylinderOperator::execProfession()
{
    mCurrentCylinderCmd = nullptr;
    mCurrentNodeId = FCID_INVALID;

    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) {
        qWarning() << "FCActionCreateCylinderOperator: FCGlobalData not found";
        return false;
    }
    FC::FCGeometryDAGData* dagData = globalData->getGeometryData<FC::FCGeometryDAGData>();
    if (!dagData) {
        qWarning() << "FCActionCreateCylinderOperator: FCGeometryDAGData not found";
        return false;
    }

    FC::FCGeoParamSet params;
    params[QStringLiteral("location_x")] = 0.0;
    params[QStringLiteral("location_y")] = 0.0;
    params[QStringLiteral("location_z")] = 0.0;
    params[QStringLiteral("direction_x")] = 0.0;
    params[QStringLiteral("direction_y")] = 0.0;
    params[QStringLiteral("direction_z")] = 1.0;
    params[QStringLiteral("radius")] = 50.0;
    params[QStringLiteral("length")] = 100.0;
    QString name = QStringLiteral("Cylinder_1");
    mCurrentNodeId = dagData->module()->addCylinder(params, name);
    dagData->setDirty(true);

    if (mCurrentNodeId != FCID_INVALID)
        mCurrentCylinderCmd = FCDATAREPO->getDataAs<FC::FCGeoModelCylinder>(mCurrentNodeId);
    return true;
}

} // namespace FC
