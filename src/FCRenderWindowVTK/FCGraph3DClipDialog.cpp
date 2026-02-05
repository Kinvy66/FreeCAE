/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraph3DClipDialog.h"
#include "FCGraph3DWindowVTK.h"
#include <QShowEvent>

namespace FC
{
FCGraph3DClipDialog::FCGraph3DClipDialog(FCGraph3DWindowVTK* graph3DWindow, bool clipCreateNewModel, QWidget* parent)
    : QDialog(parent)
    , m_graph3DWindow(graph3DWindow)
    , m_clipCreateNewModel(clipCreateNewModel)
{
}

FCGraph3DClipDialog::~FCGraph3DClipDialog() = default;

void FCGraph3DClipDialog::init()
{
}

void FCGraph3DClipDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
}
} // namespace FC
