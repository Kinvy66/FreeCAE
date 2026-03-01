/**
 * @file FCGeometryBuildBar.cpp
 * @brief 几何体共有操作栏实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryBuildBar.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>

namespace FC {

FCGeometryBuildBar::FCGeometryBuildBar(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(2, 2, 2, 2);
    m_btnBuild = new QPushButton(tr("构建"), this);
    m_btnBuildAll = new QPushButton(tr("全部构建"), this);
    m_layout->addWidget(m_btnBuild);
    m_layout->addWidget(m_btnBuildAll);
    m_layout->addStretch(1);
    connect(m_btnBuild, &QPushButton::clicked, this, &FCGeometryBuildBar::onBuildClicked);
    connect(m_btnBuildAll, &QPushButton::clicked, this, &FCGeometryBuildBar::onBuildAllClicked);
}

void FCGeometryBuildBar::addGlobalAction(QAction* action)
{
    if (!action || !m_layout) return;
    QToolButton* btn = new QToolButton(this);
    btn->setDefaultAction(action);
    m_layout->insertWidget(m_layout->count() - 1, btn);
}

void FCGeometryBuildBar::addGlobalActions(const QList<QAction*>& actions)
{
    for (QAction* a : actions)
        addGlobalAction(a);
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
