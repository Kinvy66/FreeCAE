/**
 * @file FCIEventBus.h
 * @brief 事件总线接口：统一发出与订阅应用内事件，实现操作器与多窗口联动
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCIEVENTBUS_H
#define FCIEVENTBUS_H

#include "FCInterfaceAPI.h"
#include <QObject>
#include <QVariantMap>

namespace FC {

/**
 * @brief 事件总线抽象接口
 * 实现类在 APP 层（FCAppEventBus）。操作器通过 uiInterface()->getEventBus() 获取并 emit；
 * 需要联动的窗口（如属性面板连接器）通过 setEventBus 订阅并响应。
 */
class FCINTERFACE_API FCIEventBus : public QObject
{
    Q_OBJECT
public:
    explicit FCIEventBus(QObject* parent = nullptr) : QObject(parent) {}
    ~FCIEventBus() override = default;

    /**
     * @brief 发送事件，由总线分发给所有订阅者
     * @param eventType 事件类型（见 FCEventTypes.h）
     * @param data 载荷（如 nodeId, geometryIds 等）
     */
    virtual void emitEvent(int eventType, const QVariantMap& data) = 0;

signals:
    /** 事件发出时触发，订阅者连接此信号并根据 eventType 过滤处理 */
    void eventReceived(int eventType, QVariantMap data);
};

} // namespace FC

#endif // FCIEVENTBUS_H
