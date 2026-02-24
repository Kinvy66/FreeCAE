/**
 * @file FCProjectTreeEventOperator.cpp
 * @brief 工程树操作器
 * @date 2026-02-24
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCProjectTreeEventOperator.h"
#include <FCInterface/FCDockingAreaInterface.h>
#include <FCGUIFrame/FCMainTreeWidget.h>
#include <FCGUIWidget/FCProjectTreeWidget.h>
#include <QDebug>

namespace FC 
{

FCProjectTreeEventOperator::FCProjectTreeEventOperator()
{
    
}

FCProjectTreeEventOperator::~FCProjectTreeEventOperator()
{
    
}

void FCProjectTreeEventOperator::updateTree()
{
    FCDockingAreaInterface* docking = dockingArea();
    if (!docking) return;
    FCMainTreeWidget* modelWidget = docking->getModelBuilderWidget();
    if (!modelWidget) return;
    FCProjectTreeWidget* treeWidget = modelWidget->getTreeWidget();
    if (!treeWidget) return;
    treeWidget->updateGeometryItems();
}

} // namespace FC
