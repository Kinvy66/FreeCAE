/**
 * @file FCProjectTreeWidget.cpp
 * @brief 工程树形结构
 * @date 2026-02-20
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026. All rights reserved.
 */
#include "FCProjectTreeWidget.h"
#include <FCData/FCGlobalData.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeoNode.h>
#include <FCGeometryEntity/FCGeoOpType.h>
#include <algorithm>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QDebug>

namespace FC
{

static QString iconPathForGeoOpType(FCGeoOpType type)
{
    switch (type) {
    case FCGeoOpType::Block:       return QStringLiteral(":/icon/geometry/cube.png");
    case FCGeoOpType::Cylinder:   return QStringLiteral(":/icon/geometry/cylinder.png");
    case FCGeoOpType::Sphere:     return QStringLiteral(":/icon/geometry/sphere.png");
    case FCGeoOpType::Cone:     return QStringLiteral(":/icon/geometry/cone.png");
    case FCGeoOpType::Torus:     return QStringLiteral(":/icon/geometry/torus.png");
    case FCGeoOpType::Helix:     return QStringLiteral(":/icon/geometry/helix.png");        
    case FCGeoOpType::Union:
    case FCGeoOpType::Difference:
    case FCGeoOpType::Intersection: return QStringLiteral(":/icon/geometry/boolean_opt.png");
    case FCGeoOpType::Fillet:     return QStringLiteral(":/icon/geometry/fillet.png");
    case FCGeoOpType::Chamfer:   return QStringLiteral(":/icon/geometry/chamfer.png");
    case FCGeoOpType::Import:    return QStringLiteral(":/icon/geometry/import_geometry.png");
    case FCGeoOpType::Extrude:   return QStringLiteral(":/icon/geometry/extrude.png");
    case FCGeoOpType::Revolve:   return QStringLiteral(":/icon/geometry/revolve.png");
    case FCGeoOpType::Sweep:     return QStringLiteral(":/icon/geometry/sweep.png");
    default:                      return QStringLiteral(":/icon/undefined.png");
    }
}

FCProjectTreeWidget::FCProjectTreeWidget(QWidget* parent)
    : QTreeWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
    
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(onDoubleClicked(QTreeWidgetItem*, int)));
    
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onModelCustomContextMenu(QPoint)));

    connect(this, &QTreeWidget::itemSelectionChanged, this, &FCProjectTreeWidget::onSelectionChanged);

    setHeaderHidden(true);
    setColumnCount(1);
   
}

FCProjectTreeWidget::~FCProjectTreeWidget()
{
}

void FCProjectTreeWidget::updateTree()
{
    this->clear();

    mPostProcessingRoot = nullptr;

    if (mProjectRoot == nullptr) {
         mProjectRoot = new QTreeWidgetItem();
    }
    
     // mProjectRoot = new QTreeWidgetItem();
   
    mProjectRoot->setText(0, tr("Untitled"));
    // mProjectRoot->setData(0, Role_Id, QVariant());
    // mProjectRoot->setData(0, Role_Type, QVariant::fromValue(ProjectTreeEnum::ProjectTree_Root));
    
    mProjectRoot->setIcon(0, QIcon(":/icon/project.png"));
    this->addTopLevelItem(mProjectRoot);
    this->setCurrentItem(mProjectRoot);
    
    
    buildCategoryItems();

    // updateGlobalDefineItems();
    // updateComponentItems();
    // updateStudyItems();
    // updateResultItems();

    setItemsExpandable(true);
    this->expandToDepth(1);

    // QList<CompTreeItem*> compWidgets = this->findChildren<CompTreeItem*>();
    // for (auto widget : compWidgets)
    // {
    //     if (widget == nullptr)
    //         continue;
    //     connect(widget, SIGNAL(sigIconButtonClicked()), this, SLOT(soltIconButtonClicked()));
    // }
}

void FCProjectTreeWidget::buildCategoryItems()
{
    mGlobalDef = new QTreeWidgetItem(mProjectRoot);
    mGlobalDef->setText(0, tr("全局定义"));
    mGlobalDef->setIcon(0, QIcon(":/icon/global_define.png"));
    
    mComponentRoot = new QTreeWidgetItem(mProjectRoot);
    mComponentRoot->setText(0, tr("组件"));
    mComponentRoot->setIcon(0, QIcon(":/icon/component.png"));
    
    mComponentDefine = new QTreeWidgetItem(mComponentRoot);
    mComponentDefine->setText(0, tr("定义"));
    mComponentDefine->setIcon(0, QIcon(":/icon/define.png"));
    
    mComponentGeometry = new QTreeWidgetItem(mComponentRoot);
    mComponentGeometry->setText(0, tr("几何"));
    mComponentGeometry->setIcon(0, QIcon(":/icon/geometry.png"));
    
    mComponentMaterial = new QTreeWidgetItem(mComponentRoot);
    mComponentMaterial->setText(0, tr("材料"));
    mComponentMaterial->setIcon(0, QIcon(":/icon/material.png"));
    
    mComponentPhysic = new QTreeWidgetItem(mComponentRoot);
    mComponentPhysic->setText(0, tr("物理场"));
    mComponentPhysic->setIcon(0, QIcon(":/icon/undefined.png"));
    
    mComponentMesh = new QTreeWidgetItem(mComponentRoot);
    mComponentMesh->setText(0, tr("网格"));
    mComponentMesh->setIcon(0, QIcon(":/icon/mesh.png"));
    

    mStudyRoot = new QTreeWidgetItem(mProjectRoot);
    mStudyRoot->setText(0, tr("研究"));
    mStudyRoot->setIcon(0, QIcon(":/icon/study1.png"));
    
    mPostProcessingRoot = new QTreeWidgetItem(mProjectRoot);
    mPostProcessingRoot->setText(0, tr("结果"));
    mPostProcessingRoot->setIcon(0, QIcon(":/icon/result.png"));
    // mPostProcessingRoot->setData();
}

void FCProjectTreeWidget::onModelCustomContextMenu(QPoint point)
{
    QTreeWidgetItem* item = this->currentItem();
    if (item == nullptr)
        return;
    
    ProjectTreeEnum treeType = item->data(0, Role_Type).value<ProjectTreeEnum>();
    QMenu menu(this);
    
    switch (treeType)
    {
    case ProjectTreeEnum::ProjectTree_GeometryEntity:
        addMenuActions(menu, "actionProjectTreeGeometryDelete", tr("删除"), ":/icon/delete.png");
        break;
    case ProjectTreeEnum::ProjectTree_MeshEntity:
        addMenuActions(menu, "actionProjectTreeMeshDelete", tr("删除"));
        break;
    case ProjectTreeEnum::ProjectTree_Result3DPlot:
        addMenuActions(menu, "actionProjectTreeResultDelete", tr("删除"));
        break;
    default:
        break;
    }
    
    if (menu.actions().size() == 0)
        return;
    menu.exec(this->mapToGlobal(point));
}

void FCProjectTreeWidget::onItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    syncPropertyPanelToCurrentItem();
}

void FCProjectTreeWidget::onSelectionChanged()
{
    syncPropertyPanelToCurrentItem();
}

void FCProjectTreeWidget::syncPropertyPanelToCurrentItem()
{
    QTreeWidgetItem* item = currentItem();
    if (!item) {
        emit noEntitySelected();
        return;
    }
    QVariant idVar = item->data(0, Role_Id);
    QVariant typeVar = item->data(0, Role_Type);
    if (!idVar.isValid() || idVar.toULongLong() == 0) {
        emit noEntitySelected();
        return;
    }
    if (!typeVar.isValid()) {
        emit noEntitySelected();
        return;
    }
    ProjectTreeEnum type = typeVar.value<ProjectTreeEnum>();
    if (type == ProjectTreeEnum::ProjectTree_GeometryEntity) {
        FCID nodeId = static_cast<FCID>(idVar.toULongLong());
        emit geometryNodeSelected(nodeId);
    } else {
        emit noEntitySelected();
    }
}

void FCProjectTreeWidget::onDoubleClicked(QTreeWidgetItem* item, int column)
{
    if (item == nullptr)
        return;
    Q_UNUSED(column);
    // 与 TreeWidget 一致：可根据 data(0, Role_Id) / data(0, Role_Type) 触发编辑等，后续接入
}

void FCProjectTreeWidget::acitonClicked()
{
    QTreeWidgetItem* item = this->currentItem();
    if (item == nullptr)
        return;
    
    QObject* senderObject = sender();
    if (senderObject == nullptr)
        return;
    
    // 与 TreeWidget 一致：通过 FCOperatorRepo 执行动作，后续接入
    // auto acOper = Core::FCOperatorRepo::getInstance()->getOperatorT<Core::FCActionOperator>(senderObject->objectName());
    // if (acOper == nullptr) return;
    // acOper->setEmitter(senderObject);
    // acOper->setArgs("objID", item->data(0, Role_Id).toULongLong());
    // acOper->actionTriggered();
    Q_UNUSED(senderObject);
}

void FCProjectTreeWidget::soltIconButtonClicked()
{
    // CompTreeItem* senderWidget = dynamic_cast<CompTreeItem*>(this->sender());
    // if (senderWidget == nullptr)
    //     return;
    // QTreeWidgetItem* item = senderWidget->getTreeItem();
    // if (item == nullptr)
    //     return;
    // Q_UNUSED(item);
    // 与 TreeWidget 一致：可根据节点类型做显示/隐藏等，后续接入
}



void FCProjectTreeWidget::updateGlobalDefineItems()
{
    
}

void FCProjectTreeWidget::updateComponentItems()
{
    
}

void FCProjectTreeWidget::updateGeometryItems()
{
    if (!mComponentGeometry) return;
    while (mComponentGeometry->childCount() > 0) {
        QTreeWidgetItem* child = mComponentGeometry->takeChild(0);
        delete child;
    }
    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) return;
    FC::FCGeometryDAGData* dagData = globalData->getData<FC::FCGeometryDAGData>(FC::GDTGeom);
    if (!dagData || !dagData->module() || !dagData->module()->tree())
        return;
    QList<FCID> ids = dagData->module()->tree()->nodeIds();
    std::sort(ids.begin(), ids.end());
    for (FCID id : ids) {
        FC::FCGeoNode node = dagData->module()->tree()->node(id);
        QTreeWidgetItem* item = new QTreeWidgetItem(mComponentGeometry);
        item->setText(0, node.name.isEmpty() ? QStringLiteral("Node%1").arg(static_cast<qulonglong>(id)) : node.name);
        item->setIcon(0, QIcon(iconPathForGeoOpType(node.type)));
        item->setData(0, TreeRole::Role_Id, static_cast<qulonglong>(id));
        item->setData(0, Role_Type, QVariant::fromValue(ProjectTreeEnum::ProjectTree_GeometryEntity));
    }
}

void FCProjectTreeWidget::expandGeometryAndSelectCommand(FCID cmdId)
{
    if (!mComponentGeometry || cmdId == FCID_INVALID) return;
    setExpanded(indexFromItem(mComponentGeometry), true);
    const quint64 key = static_cast<quint64>(cmdId);
    for (int i = 0; i < mComponentGeometry->childCount(); ++i) {
        QTreeWidgetItem* child = mComponentGeometry->child(i);
        if (child->data(0, Role_Id).toULongLong() == key) {
            setCurrentItem(child);
            scrollToItem(child);
            break;
        }
    }
}

void FCProjectTreeWidget::updateMaterialItems()
{
    
}

void FCProjectTreeWidget::updatePhysicItems(int index)
{
    
}

void FCProjectTreeWidget::updateStudyItems()
{
    
}

void FCProjectTreeWidget::updateStudyStepItmes(int index)
{
    
}

void FCProjectTreeWidget::updateSolverItems(int index)
{
    
}

void FCProjectTreeWidget::updateResultItems()
{
    
}

void FCProjectTreeWidget::update3DPlot()
{
    
}

void FCProjectTreeWidget::update2DPlot()
{
    
}

void FCProjectTreeWidget::update1DPlot()
{
    
}

void FCProjectTreeWidget::addMenuActions(QMenu& menu, QString actions, QString objectName, const QString& iconPath)
{
    QAction* act = nullptr;
    if (iconPath.isEmpty()) {
        act = menu.addAction(objectName);
    } else {
        act = menu.addAction(QIcon(iconPath), objectName);
    }
    act->setObjectName(actions);
    connect(act, SIGNAL(triggered()), this, SLOT(acitonClicked()));
}

void FCProjectTreeWidget::deleteEntityItem()
{
    
}


} // namespace FC
