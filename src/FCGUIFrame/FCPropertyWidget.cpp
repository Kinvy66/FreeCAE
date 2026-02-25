/**
 * @file FCPropertyWidget.cpp
 * @brief 属性窗口：显示几何/参数/材料/物理场等当前选中项属性
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
    while (QLayoutItem* item = layout->takeAt(0)) {
        QWidget* old = item->widget();
        delete item;
        if (old)
            delete old;  // 删除旧内容，避免 setParent(nullptr) 后变成悬浮窗
    }
    layout->addWidget(w);
}

} // namespace FC
