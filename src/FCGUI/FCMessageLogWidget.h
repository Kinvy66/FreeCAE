/**
 * @file FCMessageLogWidget.h
 * @brief 消息日志窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCMESSAGELOGWIDGET_H
#define FCMESSAGELOGWIDGET_H

#include "FCGUIAPI.h"
#include <QWidget>

namespace FC 
{
class FCGUI_API FCMessageLogWidget :public QWidget
{
    Q_OBJECT
public:
    explicit FCMessageLogWidget(QWidget* parent = nullptr);
};
} // namespace FC


#endif // FCMESSAGELOGWIDGET_H
