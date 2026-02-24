/**
 * @file FCGraphEventOperator.cpp
 * @brief 三维可视对象数据操作器接口类
 * @date 2026-02-24
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGraphEventOperator.h"

namespace FC 
{

FCGraphEventOperator::~FCGraphEventOperator()
{
    
}

void FCGraphEventOperator::updateGraph(int dataObjId, bool forceUpdate)
{
    Q_UNUSED(dataObjId);
    Q_UNUSED(forceUpdate);
}

void FCGraphEventOperator::updateGraph()
{
    
}

FCVTKGraphObject3D *FCGraphEventOperator::getModelGraphObjectByDataId(int dataObjId)
{
    Q_UNUSED(dataObjId);
    return nullptr;
}

void FCGraphEventOperator::setEnableModelTransparent(bool flag)
{
    
}

void FCGraphEventOperator::setEnableMeshTransparent(bool flag)
{
    
}

void FCGraphEventOperator::setModelVisible(int dataObjId, bool visibility)
{
    
}

void FCGraphEventOperator::setMeshVisible(int dataObjId, bool visibility)
{
    
}

void FCGraphEventOperator::highlight(int dataObjId, QVariant info, QColor color)
{
    
}

void FCGraphEventOperator::advHighlight(int dataObjId, QVector<int> &indice, QColor color)
{
    
}

void FCGraphEventOperator::clearHighlight()
{
    
}

void FCGraphEventOperator::reRender(bool fitView)
{
    
}

} // namespace FC
