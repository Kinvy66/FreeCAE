/**
 * @file FCProgressWidget.h
 * @brief 进度窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCPROGRESSWIDGET_H
#define FCPROGRESSWIDGET_H

#include "FCGUIAPI.h"
#include <QWidget>

namespace FC 
{
class FCGUI_API FCProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FCProgressWidget(QWidget* parent = nullptr);
};
} // namespace FC


#endif // FCPROGRESSWIDGET_H
