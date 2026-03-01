/**
 * @file FCGeometryPropertyContentHandler.h
 * @brief 几何属性面板内容处理器：仅负责“几何节点选中时”在属性面板显示几何内容
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 *
 * 由 FCPropertyPanelOperator 在 entityType==PropertyPanelEntityGeometry 时调用，
 * 与后续的 Mesh/Physics 等 Handler 并列。
 */
#ifndef FCGEOMETRYPROPERTYCONTENTHANDLER_H
#define FCGEOMETRYPROPERTYCONTENTHANDLER_H

#include "FCGeometryPropertyContentFactory.h"
#include "IPropertyPanelContentHandler.h"
#include "FCActionsOperatorAPI.h"
#include <FCData/FCType.h>
#include <QList>

class QAction;

namespace FC
{
class FCDockingAreaInterface;

/**
 * @brief 几何属性内容处理器
 * 实现 IPropertyPanelContentHandler，仅处理几何节点选中时的属性面板内容。
 */
class FCACTIONSOPERATOR_API FCGeometryPropertyContentHandler : public IPropertyPanelContentHandler
{
public:
    FCGeometryPropertyContentHandler() = default;
    ~FCGeometryPropertyContentHandler() override = default;

    void showContent(FCDockingAreaInterface* docking, FCID nodeId) override;

    /** 设置构建栏要展示的全局 actions，由 APP 层注入 */
    void setBuildBarGlobalActions(const QList<QAction*>& actions);

private:
    QList<QAction*> m_buildBarGlobalActions;
    FCGeometryPropertyContentFactory m_contentFactory;
};

} // namespace FC

#endif // FCGEOMETRYPROPERTYCONTENTHANDLER_H
