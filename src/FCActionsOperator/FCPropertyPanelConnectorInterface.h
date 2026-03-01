/**
 * @file FCPropertyPanelConnectorInterface.h
 * @brief 属性面板连接器接口：统一“选择变化 → 更新属性面板内容”的入口，便于扩展网格/结果等其它子窗口联动
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCPROPERTYPANELCONNECTORINTERFACE_H
#define FCPROPERTYPANELCONNECTORINTERFACE_H

#include "FCActionsOperatorAPI.h"
#include <QObject>

namespace FC
{
class FCDockingAreaInterface;

/**
 * @brief 属性面板连接器接口
 * 子类负责：监听某一类选择（如几何节点、网格节点），在属性窗口显示对应内容并完成与 Operator 的接线。
 * 后续可扩展 FCMeshPropertyConnector、FCResultPropertyConnector 等，由 APP 层统一 setDockingArea 即可。
 */
class FCACTIONSOPERATOR_API FCPropertyPanelConnectorInterface : public QObject
{
    Q_OBJECT
public:
    explicit FCPropertyPanelConnectorInterface(QObject* parent = nullptr)
        : QObject(parent) {}
    ~FCPropertyPanelConnectorInterface() override = default;

    /** 设置 Docking 区域，并建立本连接器所需的信号连接（如树选中 → 更新属性面板） */
    virtual void setDockingArea(FCDockingAreaInterface* docking) = 0;
};

} // namespace FC

#endif // FCPROPERTYPANELCONNECTORINTERFACE_H
