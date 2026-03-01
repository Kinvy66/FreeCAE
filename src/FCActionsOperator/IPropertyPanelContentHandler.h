/**
 * @file IPropertyPanelContentHandler.h
 * @brief 属性面板内容处理器接口：按选中类型（几何/网格/物理场等）提供对应内容，COMSOL 式
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef IPROPERTYPANELCONTENTHANDLER_H
#define IPROPERTYPANELCONTENTHANDLER_H

#include "FCActionsOperatorAPI.h"
#include <FCData/FCType.h>

namespace FC
{
class FCDockingAreaInterface;  // 前向声明，避免依赖 FCInterface 头文件

/**
 * @brief 属性面板内容处理器接口
 * 每种工程树对象类型（几何、网格、物理场、研究、结果等）对应一个 Handler 实现，
 * 由 FCPropertyPanelOperator 根据 EventTreeNodeSelected 的 entityType 调用。
 */
class FCACTIONSOPERATOR_API IPropertyPanelContentHandler
{
public:
    virtual ~IPropertyPanelContentHandler() = default;

    /**
     * @brief 在属性面板中显示指定节点的内容
     * @param docking Docking 区域（取属性窗口等）
     * @param nodeId 当前选中的树节点 ID
     */
    virtual void showContent(FCDockingAreaInterface* docking, FCID nodeId) = 0;
};

} // namespace FC

#endif // IPROPERTYPANELCONTENTHANDLER_H
