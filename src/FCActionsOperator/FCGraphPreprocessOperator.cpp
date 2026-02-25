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
#include <FCRenderWidget.h>
#include <FCGraph3DWindowVTK.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCData/FCAbstractDataObject.h>
#include <FCVTKGraphAdaptor/FCVTKViewAdaptorModelCmd.h>
#include <FCVTKGraphAdaptor/FCVTKGraphObject3D.h>
#include <FCRenderWindowVTK/FCGraphRender.h>
#include <FCRenderWindowVTK/FCGraphObjManager.h>
#include <FCRenderWindowVTK/FCGraphObjectVTK.h>

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
    FCGraph3DWindowVTK* graphWidget = getGraphWidget();
    if (!graphWidget) return;
    if (fitView)
    {
        graphWidget->fitView();
    }
    else
    {
        graphWidget->reRender();
    }
}

FCGraph3DWindowVTK* FCGraphPreprocessOperator::getGraphWidget()
{
    FCDockingAreaInterface* docking = m_docking ? m_docking : dockingArea();
    if (!docking) return nullptr;
    FCRenderWidget* rw = docking->getGraphicOperateWidget();
    return rw ? rw->getGraph3DWindow() : nullptr;
}

void FCGraphPreprocessOperator::onGeometryBuilt(FCAbsGeoCommand* cmd)
{
    if (!cmd) return;
    FCGraph3DWindowVTK* graphWin = getGraphWidget();
    if (!graphWin) return;

    // 同一几何多次「构建」会重复 addObject 导致内存增长，先清空渲染器 0 再添加
    FCGraphRender* render0 = graphWin->getRenderer(0);
    if (render0) {
        FCGraphObjManager* mgr = render0->getGraphObjManager();
        if (mgr) {
            int n = mgr->getGraphObjCount();
            for (int i = n - 1; i >= 0; --i) {
                FCGraphObjectVTK* obj = mgr->getGraphObjAt(i);
                if (obj) {
                    render0->removeObject(obj);
                    delete obj;
                }
            }
        }
    }

    FCVTKViewAdaptorModelCmd adaptor;
    adaptor.setDataObject(static_cast<FCAbstractDataObject*>(cmd));
    if (!adaptor.update()) return;
    FCVTKGraphObject3D* graphObj = adaptor.getOutputData();
    if (!graphObj || graphObj->getActorCount() == 0) return;
    graphWin->addObject(0, graphObj, true);
    graphWin->reRender();
}

void FCGraphPreprocessOperator::onGeometrySequenceBuilt(const QList<FCAbsGeoCommand*>& cmds)
{
    FCGraph3DWindowVTK* graphWin = getGraphWidget();
    if (!graphWin) return;

    FCGraphRender* render0 = graphWin->getRenderer(0);
    if (render0) {
        FCGraphObjManager* mgr = render0->getGraphObjManager();
        if (mgr) {
            int n = mgr->getGraphObjCount();
            for (int i = n - 1; i >= 0; --i) {
                FCGraphObjectVTK* obj = mgr->getGraphObjAt(i);
                if (obj) {
                    render0->removeObject(obj);
                    delete obj;
                }
            }
        }
    }
    for (FCAbsGeoCommand* c : cmds) {
        if (!c) continue;
        FCVTKViewAdaptorModelCmd adaptor;
        adaptor.setDataObject(static_cast<FCAbstractDataObject*>(c));
        if (!adaptor.update()) continue;
        FCVTKGraphObject3D* graphObj = adaptor.getOutputData();
        if (graphObj && graphObj->getActorCount() > 0)
            graphWin->addObject(0, graphObj, false);
    }
    if (!cmds.isEmpty())
        graphWin->fitView();
    graphWin->reRender();
}

} // namespace FC
