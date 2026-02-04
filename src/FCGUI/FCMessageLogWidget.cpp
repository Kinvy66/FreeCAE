/**
 * @file FCMessageLogWidget.cpp
 * @brief 消息日志窗口
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCMessageLogWidget.h"
#include <QVBoxLayout>
#include "FCMessageLogView.h"

namespace FC 
{

FCMessageLogWidget::FCMessageLogWidget(QWidget *parent)
{
    // 创建布局
    mLayout = new QVBoxLayout(this);
    mLayout->setContentsMargins(0, 0 , 0, 0);
    mLayout->setSpacing(3);
    mLogView = new FCMessageLogView(this);
    mLayout->addWidget(mLogView);
}

FCMessageLogWidget::~FCMessageLogWidget()
{
    
}

void FCMessageLogWidget::init()
{
    
}

FCMessageLogView *FCMessageLogWidget::getWidget() const
{
     return mLogView;
}

} // namespace FC
