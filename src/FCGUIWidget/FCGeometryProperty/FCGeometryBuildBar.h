/**
 * @file FCGeometryBuildBar.h
 * @brief 几何体共有操作栏：构建、全部构建等，仅在选中几何体节点时与属性一起显示在 FCPropertyWidget 上方
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYBUILDBAR_H
#define FCGEOMETRYBUILDBAR_H

#include "../FCGUIWidgetAPI.h"
#include <QWidget>

class QPushButton;

namespace FC {

/**
 * @brief 几何体操作栏：仅包含「构建」「全部构建」等按钮，发出信号由外部连接至 IGeometryBuildContent
 *
 * 不依赖 IGeometryBuildContent。由调用方在选中几何体节点时创建本栏与属性内容，
 * 组合为 [本栏 + 属性 Widget] 后通过 FCPropertyWidget::setContentWidget 显示。
 */
class FCGUIWIDGET_API FCGeometryBuildBar : public QWidget
{
    Q_OBJECT
public:
    explicit FCGeometryBuildBar(QWidget *parent = nullptr);

signals:
    /** 构建：当前选中节点之前的所有几何形成实体并显示 */
    void buildClicked();
    /** 全部构建：几何节点下所有几何构建并显示 */
    void buildAllClicked();

private slots:
    void onBuildClicked();
    void onBuildAllClicked();

private:
    QPushButton* m_btnBuild{ nullptr };
    QPushButton* m_btnBuildAll{ nullptr };
};

} // namespace FC

#endif // FCGEOMETRYBUILDBAR_H
