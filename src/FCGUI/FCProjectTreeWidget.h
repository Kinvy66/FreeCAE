/**
 * @file FCProjectTreeWidget.h
 * @brief 工程树窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCPROJECTTREEWIDGET_H
#define FCPROJECTTREEWIDGET_H

#include "FCGUIAPI.h"
#include <QWidget>

namespace FC 
{
class FCGUI_API FCProjectTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FCProjectTreeWidget(QWidget* parent = nullptr);
};
} // namespace FC


#endif // FCPROJECTTREEWIDGET_H
