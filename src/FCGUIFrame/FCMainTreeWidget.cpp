/**
 * @file FCMainTreeWidget.cpp
 * @brief 工程树窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCMainTreeWidget.h"
#include "FCProjectTreeWidget.h"
#include <QVBoxLayout>

namespace FC 
{
FCMainTreeWidget::FCMainTreeWidget(QWidget *parent)
    : QWidget(parent)
{
    // QLabel* lab = new QLabel(this);
    // lab->setText("Tree Widget");
    
    mTreeWidget = new FCProjectTreeWidget(this);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0，确保铺满
    layout->setSpacing(0);  // 设置间距为0
    layout->addWidget(mTreeWidget);  // 添加VTK窗口组件到布局
    init();
}

FCMainTreeWidget::~FCMainTreeWidget()
{
    
}

void FCMainTreeWidget::init()
{
    mTreeWidget->updateTree();
} 


} // namespace FC




