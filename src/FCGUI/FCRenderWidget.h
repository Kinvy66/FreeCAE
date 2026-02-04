/**
 * @file FCRenderWidget.h
 * @brief VTK渲染窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCRENDERWIDGET_H
#define FCRENDERWIDGET_H

#include "FCGUIAPI.h"
#include <QWidget>

namespace FC 
{
class FCGUI_API FCRenderWidget : public  QWidget
{
    Q_OBJECT
public:
    explicit FCRenderWidget(QWidget* parent = nullptr);
};
} // namespace FC



#endif // FCRENDERWIDGET_H
