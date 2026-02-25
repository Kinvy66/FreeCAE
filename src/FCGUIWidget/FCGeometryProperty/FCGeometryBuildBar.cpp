/**
 * @file FCGeometryBuildBar.cpp
 * @brief 几何体共有操作栏实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryBuildBar.h"
#include <QHBoxLayout>
#include <QPushButton>

namespace FC {

FCGeometryBuildBar::FCGeometryBuildBar(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    m_btnBuild = new QPushButton(tr("构建"), this);
    m_btnBuildAll = new QPushButton(tr("全部构建"), this);
    layout->addWidget(m_btnBuild);
    layout->addWidget(m_btnBuildAll);
    layout->addStretch(1);
    connect(m_btnBuild, &QPushButton::clicked, this, &FCGeometryBuildBar::onBuildClicked);
    connect(m_btnBuildAll, &QPushButton::clicked, this, &FCGeometryBuildBar::onBuildAllClicked);
}

void FCGeometryBuildBar::onBuildClicked()
{
    emit buildClicked();
}

void FCGeometryBuildBar::onBuildAllClicked()
{
    emit buildAllClicked();
}

} // namespace FC
