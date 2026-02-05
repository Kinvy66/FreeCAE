/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraph3DClipDialog.h
 * @brief 三维渲染窗口裁切对话框（占位，仅 VTK+Qt）
 */
#ifndef FCGRAPH3DCLIPDIALOG_H
#define FCGRAPH3DCLIPDIALOG_H

#include "FCRenderWindowVTKAPI.h"
#include <QDialog>

namespace FC
{
class FCGraph3DWindowVTK;

class FCRENDERWINDOWVTK_API FCGraph3DClipDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FCGraph3DClipDialog(FCGraph3DWindowVTK* graph3DWindow, bool clipCreateNewModel, QWidget* parent = nullptr);
    ~FCGraph3DClipDialog() override;
    void init();

protected:
    void showEvent(QShowEvent* event) override;

private:
    FCGraph3DWindowVTK* m_graph3DWindow = nullptr;
    bool m_clipCreateNewModel = true;
};
} // namespace FC
#endif
