/**
 * @file FCPropertyWidget.h
 * @brief 属性窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCPROPERTYWIDGET_H
#define FCPROPERTYWIDGET_H
#include "FCGUIFrameAPI.h"
#include <QWidget>

namespace FC 
{
class FCGUIFRAME_API FCPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FCPropertyWidget(QWidget *parent = nullptr);

    /** 设置参数区内容窗口（如 FCCubeInfoWidget），替换当前显示的内容 */
    void setContentWidget(QWidget* w);

signals:
};

} // namespace FC


#endif // FCPROPERTYWIDGET_H
