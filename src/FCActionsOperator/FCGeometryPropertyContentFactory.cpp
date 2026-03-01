/**
 * @file FCGeometryPropertyContentFactory.cpp
 * @brief 几何属性面板内容工厂实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryPropertyContentFactory.h"
#include "FCGraphPreprocessOperator.h"
#include "FCOperatorRepo.h"
#include <FCGeometryBuildBar.h>
#include <FCCubeInfoWidget.h>
#include <FCSphereInfoWidget.h>
#include <FCCylinderInfoWidget.h>
#include <FCConeInfoWidget.h>
#include <FCTorusInfoWidget.h>
#include <FCHelixInfoWidget.h>
#include <FCDockingAreaInterface.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeoOpType.h>
#include <QVBoxLayout>

namespace FC
{

namespace {

template<typename T>
T* findWidgetByType(QWidget* w)
{
    T* c = qobject_cast<T*>(w);
    if (c) return c;
    for (QObject* child : w->children()) {
        QWidget* cw = qobject_cast<QWidget*>(child);
        if (cw) {
            T* found = findWidgetByType<T>(cw);
            if (found) return found;
        }
    }
    return nullptr;
}

void wireContentWidgetToGraphPreprocess(QWidget* contentWidget,
    FCGeometryDAGData* dagData, FCID nodeId, FCDockingAreaInterface* docking)
{
    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (!graphOper) return;
    graphOper->setDockingArea(docking);

    if (FCCubeInfoWidget* w = findWidgetByType<FCCubeInfoWidget>(contentWidget)) {
        QObject::connect(w, &FCCubeInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        QObject::connect(w, &FCCubeInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
        w->setDAGNode(dagData, nodeId, nullptr);
    } else if (FCSphereInfoWidget* w = findWidgetByType<FCSphereInfoWidget>(contentWidget)) {
        QObject::connect(w, &FCSphereInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        QObject::connect(w, &FCSphereInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
        w->setDAGNode(dagData, nodeId, nullptr);
    } else if (FCCylinderInfoWidget* w = findWidgetByType<FCCylinderInfoWidget>(contentWidget)) {
        QObject::connect(w, &FCCylinderInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        QObject::connect(w, &FCCylinderInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
        w->setDAGNode(dagData, nodeId, nullptr);
    } else if (FCConeInfoWidget* w = findWidgetByType<FCConeInfoWidget>(contentWidget)) {
        QObject::connect(w, &FCConeInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        QObject::connect(w, &FCConeInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
        w->setDAGNode(dagData, nodeId, nullptr);
    } else if (FCTorusInfoWidget* w = findWidgetByType<FCTorusInfoWidget>(contentWidget)) {
        QObject::connect(w, &FCTorusInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        QObject::connect(w, &FCTorusInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
        w->setDAGNode(dagData, nodeId, nullptr);
    } else if (FCHelixInfoWidget* w = findWidgetByType<FCHelixInfoWidget>(contentWidget)) {
        QObject::connect(w, &FCHelixInfoWidget::geometryBuilt, graphOper, &FCGraphPreprocessOperator::onGeometryBuilt);
        QObject::connect(w, &FCHelixInfoWidget::geometrySequenceBuilt, graphOper, &FCGraphPreprocessOperator::onGeometrySequenceBuilt);
        w->setDAGNode(dagData, nodeId, nullptr);
    }
}

QWidget* createInfoWidgetByType(QWidget* parent, FCGeoOpType nodeType)
{
    switch (nodeType) {
    case FCGeoOpType::Block:   return new FCCubeInfoWidget(parent);
    case FCGeoOpType::Sphere:  return new FCSphereInfoWidget(parent);
    case FCGeoOpType::Cylinder: return new FCCylinderInfoWidget(parent);
    case FCGeoOpType::Cone:   return new FCConeInfoWidget(parent);
    case FCGeoOpType::Torus:  return new FCTorusInfoWidget(parent);
    case FCGeoOpType::Helix:  return new FCHelixInfoWidget(parent);
    default:                   return new FCCubeInfoWidget(parent);
    }
}

} // namespace

GeometryPropertyContent FCGeometryPropertyContentFactory::createContent(
    QWidget* parent,
    FCGeoOpType nodeType,
    FCGeometryDAGData* dagData,
    FCID nodeId,
    FCDockingAreaInterface* docking,
    const QList<QAction*>& buildBarGlobalActions) const
{
    GeometryPropertyContent result;
    FCGeometryBuildBar* buildBar = new FCGeometryBuildBar(parent);
    if (!buildBarGlobalActions.isEmpty())
        buildBar->addGlobalActions(buildBarGlobalActions);

    QWidget* contentWidget = createInfoWidgetByType(parent, nodeType);
    wireContentWidgetToGraphPreprocess(contentWidget, dagData, nodeId, docking);

    QWidget* container = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(buildBar);
    layout->addWidget(contentWidget, 1);

    result.container = container;
    result.buildBar = buildBar;
    return result;
}

void FCGeometryPropertyContentFactory::updateDAGNode(
    QWidget* container,
    FCGeometryDAGData* dagData,
    FCID nodeId,
    FCGeoOpType nodeType) const
{
    if (!container || !dagData) return;
    QLayout* lay = container->layout();
    if (!lay || lay->count() < 2) return;
    QWidget* contentWidget = lay->itemAt(1)->widget();
    if (!contentWidget) return;

    switch (nodeType) {
    case FCGeoOpType::Block:
        if (FCCubeInfoWidget* w = findWidgetByType<FCCubeInfoWidget>(contentWidget))
            w->setDAGNode(dagData, nodeId, nullptr);
        break;
    case FCGeoOpType::Sphere:
        if (FCSphereInfoWidget* w = findWidgetByType<FCSphereInfoWidget>(contentWidget))
            w->setDAGNode(dagData, nodeId, nullptr);
        break;
    case FCGeoOpType::Cylinder:
        if (FCCylinderInfoWidget* w = findWidgetByType<FCCylinderInfoWidget>(contentWidget))
            w->setDAGNode(dagData, nodeId, nullptr);
        break;
    case FCGeoOpType::Cone:
        if (FCConeInfoWidget* w = findWidgetByType<FCConeInfoWidget>(contentWidget))
            w->setDAGNode(dagData, nodeId, nullptr);
        break;
    case FCGeoOpType::Torus:
        if (FCTorusInfoWidget* w = findWidgetByType<FCTorusInfoWidget>(contentWidget))
            w->setDAGNode(dagData, nodeId, nullptr);
        break;
    case FCGeoOpType::Helix:
        if (FCHelixInfoWidget* w = findWidgetByType<FCHelixInfoWidget>(contentWidget))
            w->setDAGNode(dagData, nodeId, nullptr);
        break;
    default:
        if (FCCubeInfoWidget* w = findWidgetByType<FCCubeInfoWidget>(contentWidget))
            w->setDAGNode(dagData, nodeId, nullptr);
        break;
    }
}

} // namespace FC
