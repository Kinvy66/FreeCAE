/**
 * @file FCAppEventBus.h
 * @brief 应用事件总线实现：操作器发事件、多窗口通过订阅联动
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCAPPEVENTBUS_H
#define FCAPPEVENTBUS_H

#include <FCIEventBus.h>

namespace FC {

/**
 * @brief 事件总线具体实现
 * 操作器通过 getEventBus()->emitEvent(type, data) 发送；各连接器/窗口订阅 eventReceived 并过滤处理。
 */
class FCAppEventBus : public FCIEventBus
{
    Q_OBJECT
public:
    explicit FCAppEventBus(QObject* parent = nullptr);
    void emitEvent(int eventType, const QVariantMap& data) override;
};

} // namespace FC

#endif // FCAPPEVENTBUS_H
