/**
 * @file FCRenderWidget.h
 * @brief VTK渲染窗口容器，内嵌 FCGraph3DWindowVTK
 * @date 2026-02-04
 * @version V0.0.1
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCRENDERWIDGET_H
#define FCRENDERWIDGET_H

#include "FCGUIFrameAPI.h"
#include <QWidget>

namespace FC
{
class FCGraph3DWindowVTK;
class FCGraph3DWindowInitializer;
}

namespace FC
{
class FCGUIFRAME_API FCRenderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FCRenderWidget(QWidget* parent = nullptr);
    ~FCRenderWidget() override;

    /** 获取内嵌的 VTK 三维渲染窗口，用于添加对象、设置视角等 */
    FC::FCGraph3DWindowVTK* getGraph3DWindow() const { return m_graph3DWindow; }

private:
    FC::FCGraph3DWindowInitializer* m_initializer = nullptr;
    FC::FCGraph3DWindowVTK* m_graph3DWindow = nullptr;
};
} // namespace FC

#endif // FCRENDERWIDGET_H
