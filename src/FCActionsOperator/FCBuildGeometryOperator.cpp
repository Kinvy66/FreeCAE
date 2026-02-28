/**
 * @file FCBuildGeometryOperator.cpp
 * @brief 几何体构建操作器：从属性窗口取当前几何内容，在 execProfession 中构建几何并写入 VTK 对象管理，在 execGUI 中通过前处理操作器刷新显示
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCBuildGeometryOperator.h"
#include "FCGraphPreprocessOperator.h"
#include "FCOperatorRepo.h"
#include <FCDockingAreaInterface.h>
#include <FCPropertyWidget.h>
#include <FCRenderWidget.h>
#include <FCGraph3DWindowVTK.h>
#include <FCGUIWidget/FCGeometryProperty/IGeometryBuildContent.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCData/FCAbstractDataObject.h>
#include <FCVTKGraphAdaptor/FCVTKViewAdaptorModelCmd.h>
#include <FCVTKGraphAdaptor/FCVTKGraphObject3D.h>
#include <FCRenderWindowVTK/FCGraphRender.h>
#include <FCRenderWindowVTK/FCGraphObjManager.h>
#include <FCRenderWindowVTK/FCGraphObjectVTK.h>
#include <QWidget>
#include <QLayout>
#include <QDebug>

namespace FC
{

FCBuildGeometryOperator::FCBuildGeometryOperator()
{
}

FCDockingAreaInterface* FCBuildGeometryOperator::getDockingArea() const
{
    return m_docking ? m_docking : dockingArea();
}

static FCGraph3DWindowVTK* getGraphWidget(FCDockingAreaInterface* docking)
{
    if (!docking) return nullptr;
    FCRenderWidget* rw = docking->getGraphicOperateWidget();
    return rw ? rw->getGraph3DWindow() : nullptr;
}

bool FCBuildGeometryOperator::execGUI()
{
    // 使用前处理操作器更新 VTK 显示（重绘）
    FCGraphPreprocessOperator* graphOper = FCOPERATORREPO->getOperatorT<FCGraphPreprocessOperator>("GraphPreprocessEvent");
    if (graphOper) {
        graphOper->setUIInterface(uiInterface());
        graphOper->reRender(false);
    }
    return true;
}

/**
 * @brief FCBuildGeometryOperator::execProfession
 * @return 
 * @todo 修改构建逻辑
 * @details 只从点击事件中获取到当前选中几何节点的id
 * 在DAG中从头到一直遍历，直到当前选中的几何节点id
 * 遍历的过程中会逐个的构建几何体，并且每一个几何体(操作)的构建的数据是前一个节点构建的输出
 * 这个步骤才是符合COMSOL的函数式几何构建
 */
bool FCBuildGeometryOperator::execProfession()
{
    FCDockingAreaInterface* docking = getDockingArea();
    if (!docking) return false;
    FCPropertyWidget* propWidget = docking->getSettingParametersWidget();
    if (!propWidget) return false;
    QLayout* lay = propWidget->layout();
    if (!lay || lay->count() == 0) return false;
    QWidget* content = lay->itemAt(0)->widget();
    if (!content) return false;
    IGeometryBuildContent* buildContent = findGeometryBuildContent(content);
    if (!buildContent) return false;

    buildContent->syncToModel();
    FCAbsGeoCommand* cmd = buildContent->getCurrentBuildCommand();
    if (!cmd) return false;
    if (!cmd->update()) return false;

    FCGraph3DWindowVTK* graphWin = getGraphWidget(docking);
    if (!graphWin) return false;

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
    if (!adaptor.update()) return false;
    FCVTKGraphObject3D* graphObj = adaptor.getOutputData();
    if (!graphObj || graphObj->getActorCount() == 0) return false;
    graphWin->addObject(0, graphObj, true);
    qInfo().noquote() << QStringLiteral("Geometry built: id=%1, name=%2")
                          .arg(static_cast<qulonglong>(cmd->getDataObjectID()))
                          .arg(cmd->getDataObjectName());
    return true;
}

IGeometryBuildContent* FCBuildGeometryOperator::findGeometryBuildContent(QWidget* w)
{
    IGeometryBuildContent* c = dynamic_cast<IGeometryBuildContent*>(w);
    if (c) return c;
    for (QObject* child : w->children()) {
        QWidget* cw = qobject_cast<QWidget*>(child);
        if (cw && (c = findGeometryBuildContent(cw))) return c;
    }
    return nullptr;
}

} // namespace FC
