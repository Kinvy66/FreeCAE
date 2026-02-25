/**
 * @file FCPropertyWidget.h
 * @brief 属性窗口，用于显示当前选中项的属性（几何、参数、材料、物理场等）
 * @date 2026-02-04
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

    /** 设置内容窗口，替换当前显示的内容（几何体时由调用方组合「构建栏+属性」再传入） */
    void setContentWidget(QWidget* w);

signals:
};

} // namespace FC

#endif // FCPROPERTYWIDGET_H
