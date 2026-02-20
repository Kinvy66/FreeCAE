/**
 * @file FCMainTreeWidget.h
 * @brief 工程树窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCPMAINTREEWIDGET_H
#define FCPMAINTREEWIDGET_H

#include "FCGUIFrameAPI.h"
#include <QWidget>

namespace FC 
{
class FCProjectTreeWidget;

class FCGUIFRAME_API FCMainTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FCMainTreeWidget(QWidget* parent = nullptr);
    ~FCMainTreeWidget();
    
    void init();
    
private:
    FCProjectTreeWidget* mTreeWidget = nullptr;  
};
} // namespace FC


#endif // FCPMAINTREEWIDGET_H
