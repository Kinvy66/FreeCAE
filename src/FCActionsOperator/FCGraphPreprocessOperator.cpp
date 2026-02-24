/**
 * @file FCGraphPreprocessOperator.cpp
 * @brief 前处理可视化事件操作器
 * @date 2026-02-24
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGraphPreprocessOperator.h"
#include <FCDockingAreaInterface.h>
#include "FCRenderWidget.h"
#include "FCGraph3DWindowVTK.h"

namespace FC 
{

FCGraphPreprocessOperator::FCGraphPreprocessOperator()
{
    
}

FCGraphPreprocessOperator::~FCGraphPreprocessOperator()
{
    
}

void FCGraphPreprocessOperator::updateGraph(int dataObjId, bool forceUpdate)
{
    
}

void FCGraphPreprocessOperator::updateGraph()
{
    
    
    // todo: 更新vtk渲染窗口
    
}

FCVTKGraphObject3D *FCGraphPreprocessOperator::getModelGraphObjectByDataId(int dataObjId)
{
    return nullptr;
}

void FCGraphPreprocessOperator::setEnableModelTransparent(bool flag)
{
    
}

void FCGraphPreprocessOperator::setEnableMeshTransparent(bool flag)
{
    
}

void FCGraphPreprocessOperator::setModelVisible(int dataObjId, bool visibility)
{
    
}

void FCGraphPreprocessOperator::setMeshVisible(int dataObjId, bool visibility)
{
    
}

void FCGraphPreprocessOperator::highlight(int dataObjId, QVariant info, QColor color)
{
    
}

void FCGraphPreprocessOperator::advHighlight(int dataObjId, QVector<int> &indice, QColor color)
{
    
}

void FCGraphPreprocessOperator::clearHighlight()
{
    
}

void FCGraphPreprocessOperator::reRender(bool fitView)
{
    // 获取可视化窗口。
    FCGraph3DWindowVTK* graphWidget = getGraphWidget();
    if (!graphWidget)
    {
        return;
    }
    
    if (fitView)
    {
        graphWidget->fitView();
    }
    else
    {
        graphWidget->reRender();
    }
}

FCGraph3DWindowVTK *FCGraphPreprocessOperator::getGraphWidget()
{
    FCDockingAreaInterface* docking = dockingArea();
    FCGraph3DWindowVTK* graphWidget = docking->getGraphicOperateWidget()->getGraph3DWindow();

    return graphWidget;
}

} // namespace FC
