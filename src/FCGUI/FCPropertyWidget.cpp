/**
 * @file FCPropertyWidget.cpp
 * @brief 属性窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCPropertyWidget.h"
#include <QVBoxLayout>

namespace FC 
{

FCPropertyWidget::FCPropertyWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
}

void FCPropertyWidget::setContentWidget(QWidget* w)
{
    if (!w) return;
    QLayout* layout = this->layout();
    if (!layout) return;
    // 移除原有 content（保留 layout 中除占位外的第一个子 widget 的约定：此处简化为清空后添加）
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* old = item->widget())
            old->setParent(nullptr);
        delete item;
    }
    layout->addWidget(w);
}

} // namespace FC
