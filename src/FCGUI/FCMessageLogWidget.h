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

class QVBoxLayout;

namespace FC 
{

class FCMessageLogView;

class FCGUI_API FCMessageLogWidget :public QWidget
{
    Q_OBJECT
public:
    explicit FCMessageLogWidget(QWidget* parent = nullptr);
    ~FCMessageLogWidget();
    void init();
    
    FCMessageLogView* getWidget() const ;
private:
    FCMessageLogView* mLogView;
    QVBoxLayout *mLayout;
    
};
} // namespace FC


#endif // FCMESSAGELOGWIDGET_H
