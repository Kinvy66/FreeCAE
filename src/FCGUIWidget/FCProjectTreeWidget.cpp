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
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <algorithm>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QDebug>

namespace FC
{

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
    // mProjectRoot->setData(1, 0, -1);
    // mProjectRoot->setData(2, 0, QVariant::fromValue(ProjectTreeEnum::ProjectTree_Root));
    
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
}

void FCProjectTreeWidget::onModelCustomContextMenu(QPoint point)
{
    QTreeWidgetItem* item = this->currentItem();
    if (item == nullptr)
        return;
    
    ProjectTreeEnum treeType = item->data(2, 0).value<ProjectTreeEnum>();
    QMenu menu(this);
    
    switch (treeType)
    {
    case ProjectTreeEnum::ProjectTree_GeometryEntity:
        addMenuActions(menu, "actionProjectTreeGeometryDelete", tr("删除"));
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
    // 与 TreeWidget 一致：可与 FCTreeEventOperator / FCOperatorRepo 联动，后续接入
}

void FCProjectTreeWidget::onDoubleClicked(QTreeWidgetItem* item, int column)
{
    if (item == nullptr)
        return;
    Q_UNUSED(column);
    // 与 TreeWidget 一致：可根据 data(1,0) / data(2,0) 触发编辑等，后续接入
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
    // acOper->setArgs("objID", item->data(1, 0).toInt());
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
    if (dagData && dagData->module() && dagData->module()->tree()) {
        QList<int> ids = dagData->module()->tree()->nodeIds();
        std::sort(ids.begin(), ids.end());
        for (int id : ids) {
            FC::FCGeoNode node = dagData->module()->tree()->node(id);
            QTreeWidgetItem* item = new QTreeWidgetItem(mComponentGeometry);
            item->setText(0, node.name.isEmpty() ? QStringLiteral("Node%1").arg(id) : node.name);
            item->setData(1, 0, id);
            item->setData(2, 0, QVariant::fromValue(ProjectTreeEnum::ProjectTree_GeometryEntity));
        }
        return;
    }
    FC::FCGeoCommandList* geoList = globalData->getData<FC::FCGeoCommandList>(FC::GDTGeom);
    if (!geoList) return;
    QList<FC::FCAbsGeoCommand*> rootCmds = geoList->getRootCommandList();
    for (FC::FCAbsGeoCommand* cmd : rootCmds) {
        if (!cmd) continue;
        QTreeWidgetItem* item = new QTreeWidgetItem(mComponentGeometry);
        item->setText(0, cmd->getDataObjectName());
        item->setData(1, 0, cmd->getDataObjectID());
        item->setData(2, 0, QVariant::fromValue(ProjectTreeEnum::ProjectTree_GeometryEntity));
    }
}

void FCProjectTreeWidget::expandGeometryAndSelectCommand(int cmdId)
{
    if (!mComponentGeometry || cmdId < 0) return;
    setExpanded(indexFromItem(mComponentGeometry), true);
    for (int i = 0; i < mComponentGeometry->childCount(); ++i) {
        QTreeWidgetItem* child = mComponentGeometry->child(i);
        if (child->data(1, 0).toInt() == cmdId) {
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

void FCProjectTreeWidget::addMenuActions(QMenu& menu, QString actions, QString objectName)
{
    QAction* act = menu.addAction(objectName);
    act->setObjectName(actions);
    connect(act, SIGNAL(triggered()), this, SLOT(acitonClicked()));
}

void FCProjectTreeWidget::deleteEntityItem()
{
    
}


} // namespace FC
