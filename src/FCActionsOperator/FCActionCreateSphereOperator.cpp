#include "FCActionCreateSphereOperator.h"
#include <FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryInterface/FCAbsGeoModelSphere.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCDataRepo.h>
#include <FCPropertyWidget.h>
#include <FCGeometryBuildBar.h>
#include <FCSphereInfoWidget.h>
#include <QVBoxLayout>
#include <QDebug>
#include "FCOperatorRepo.h"
#include "FCProjectTreeEventOperator.h"
#include "FCGraphPreprocessOperator.h"
#include "FCBuildGeometryOperator.h"
#include "FCBuildAllGeometryOperator.h"

namespace FC
{

FCActionCreateSphereOperator::FCActionCreateSphereOperator()
{
}

bool FCActionCreateSphereOperator::execGUI()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return true;
    if (mCurrentNodeId == FCID_INVALID && !mCurrentSphereCmd) return true;

    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return true;

    auto treeOper = FCOPERATORREPO->getOperatorT<FCProjectTreeEventOperator>("ProjectTreeEvent");
    if (treeOper) {
        treeOper->setUIInterface(uiInterface());
        treeOper->updateTree();
    }
    FCID selectId = (mCurrentNodeId != FCID_INVALID) ? mCurrentNodeId : (mCurrentSphereCmd ? mCurrentSphereCmd->getDataObjectID() : FCID_INVALID);
    if (selectId != FCID_INVALID && docking) {
        FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
        FCProjectTreeWidget* treeWidget = modelWidget ? modelWidget->getTreeWidget() : nullptr;
        if (treeWidget)
            treeWidget->expandGeometryAndSelectCommand(selectId);
    }

    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(propWidget);
    FCSphereInfoWidget* sphereWidget = new FCSphereInfoWidget(propWidget);
    QWidget* container = new QWidget(propWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(buildBar);
    containerLayout->addWidget(sphereWidget, 1);
    propWidget->setContentWidget(container);

    if (FC::FCGlobalData* g = FC::FCGlobalData::getGlobalData()) {
        if (FC::FCGeometryDAGData* dagData = g->getGeometryData<FC::FCGeometryDAGData>()) {
            sphereWidget->setDAGNode(dagData, mCurrentNodeId, mCurrentSphereCmd);
        }
    }
    if (mCurrentNodeId == FCID_INVALID && mCurrentSphereCmd)
        sphereWidget->setSphereCommand(mCurrentSphereCmd);

    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setUIInterface(uiInterface());
        connect(sphereWidget, &FCSphereInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        connect(sphereWidget, &FCSphereInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
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

bool FCActionCreateSphereOperator::execProfession()
{
    mCurrentSphereCmd = nullptr;
    mCurrentNodeId = FCID_INVALID;

    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) {
        qWarning() << "FCActionCreateSphereOperator: FCGlobalData not found";
        return false;
    }
    FC::FCGeometryDAGData* dagData = globalData->getGeometryData<FC::FCGeometryDAGData>();
    if (!dagData) {
        qWarning() << "FCActionCreateSphereOperator: FCGeometryDAGData not found";
        return false;
    }

    FC::FCGeoParamSet params;
    params[QStringLiteral("location_x")] = 0.0;
    params[QStringLiteral("location_y")] = 0.0;
    params[QStringLiteral("location_z")] = 0.0;
    params[QStringLiteral("radius")] = 50.0;
    QString name = QStringLiteral("Sphere_1");
    mCurrentNodeId = dagData->module()->addSphere(params, name);
    dagData->setDirty(true);

    if (mCurrentNodeId != FCID_INVALID)
        mCurrentSphereCmd = FCDATAREPO->getDataAs<FC::FCGeoModelSphere>(mCurrentNodeId);
    return true;
}

} // namespace FC
