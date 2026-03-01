/**
 * @file FCAppEventBus.cpp
 * @brief 应用事件总线实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCAppEventBus.h"

namespace FC {

FCAppEventBus::FCAppEventBus(QObject* parent)
    : FCIEventBus(parent)
{
}

void FCAppEventBus::emitEvent(int eventType, const QVariantMap& data)
{
    QVariantMap copy = data;
    emit eventReceived(eventType, copy);
}

} // namespace FC
