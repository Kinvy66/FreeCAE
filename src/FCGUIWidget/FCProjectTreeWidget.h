/**
 * @file FCProjectTreeWidget.h
 * @brief 工程树形结构
 * @date 2026-02-20
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026. All rights reserved.
 */
#ifndef FCPROJECTTREEWIDGET_H
#define FCPROJECTTREEWIDGET_H
#include <QTreeWidget>
#include "FCGUIWidgetAPI.h"

namespace FC
{

enum class ProjectTreeEnum
{
    ProjectTree_None = 0,
    ProjectTree_Root,            // 工程根
    
    ProjectTree_GlobalDefine,   // 全局定义
    ProjectTree_Parameter,      // 参数
    ProjectTree_Variable,       // 变量
    ProjectTree_Function,       // 函数
    
    ProjectTree_Component,     // 组件
    ProjectTree_LocalDefine,
    ProjectTree_Geometry,
    ProjectTree_GeometryEntity,
    ProjectTree_Material,
    ProjectTree_MaterialEntity,
    ProjectTree_Physic,
    ProjectTree_Mesh,
    ProjectTree_MeshEntity,
    
    ProjectTree_Study,         // 研究
    ProjectTree_StudyStep,
    ProjectTree_Solver,
    
    ProjectTree_Result,         // 结果
    ProjectTree_Result3DPlotGroup,
    ProjectTree_Result3DPlot,
    ProjectTree_Result2DPlotGroup,
    ProjectTree_Result2DPlot,
    ProjectTree_Result1DPlotGroup,
    ProjectTree_Result1DPlot,
};

class FCGUIWIDGET_API FCProjectTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new FC Project Tree Widget object
     * @param[i]  parent         My Param doc
     */
    FCProjectTreeWidget(QWidget* parent);
    /**
     * @brief Destroy the FC Project Tree Widget object
     */
    ~FCProjectTreeWidget();
    /**
     * @brief 更新树
     */
    void updateTree();
    
    /**
     * @brief 更新几何树节点（按 id/name 添加或更新）
     */
    // void updateGeometryTree(quint64 id, const QString& name);
    /**
     * @brief 更新网格树节点
     */
    // void updateMeshTree(quint64 id, const QString& name);
    /**
     * @brief 更新后处理树节点
     */
    // void updatePostProcessingTree(quint64 id, const QString& name);
    /**
     * @brief 删除当前选中的实体节点
     */
    void deleteEntityItem();

signals:
    /** 当前选中项为几何实体节点时发射，nodeId 为 DAG 节点 ID（data(1,0)） */
    void geometryNodeSelected(int nodeId);

private slots:
    /**
     * @brief 鼠标右键点击事件
     * @param[i]  point          点击位置
     */
    void onModelCustomContextMenu(QPoint point);
    /**
     * @brief 鼠标点击事件
     * @param[i]  item           点击对象
     * @param[i]  column         点击列数
     */
    void onItemClicked(QTreeWidgetItem* item, int column);
    /**
     * @brief 鼠标双击事件
     * @param[i]  item           点击对象
     * @param[i]  column         点击列数
     */
    void onDoubleClicked(QTreeWidgetItem* item, int column);
    /**
     * @brief 右键菜单动作点击
     */
    void acitonClicked();
    /**
     * @brief 树节点图标按钮点击事件
     */
    void soltIconButtonClicked();
    
    /**
     * @brief 选择变化时检查是否为几何实体节点并发射 geometryNodeSelected
     * @todo 修改工程树选中项变化的逻辑
     */
    void onSelectionChanged();

public:
    /**
     * @brief 更新全局定义子节点（与 FCProjectTree 一致：参数1、变量1、函数1）
     */
    void updateGlobalDefineItems();
    
    /**
     * @brief 更新组件子节点（定义、几何、物理场、网格）
     */
    void updateComponentItems();
    
    void updateGeometryItems();
    /** 展开几何节点并选中指定命令 ID 对应的子项 */
    void expandGeometryAndSelectCommand(int cmdId);
    
    void updateMaterialItems();
    
    void updatePhysicItems(int index = 0);
    
    
    /**
     * @brief 更新研究子节点（步骤1、步骤2）
     */
    void updateStudyItems();
    
    void updateStudyStepItmes(int index = 0);
    void updateSolverItems(int index = 0);
    
    /**
     * @brief 更新结果子节点（3维绘图组、2维绘图组、1维绘图组）
     */
    void updateResultItems();
    
    void update3DPlot();
    void update2DPlot();
    void update1DPlot();
    
private:
    
    /**
     * @brief 构建一级类别目录
     */
    void buildCategoryItems();
    
  
    
    /**
     * @brief 添加右键菜单动作（与 TreeWidget::addMenuActions 一致）
     * @param[i]  menu          菜单
     * @param[i]  actions       动作 objectName
     * @param[i]  objectName    显示名称
     */
    void addMenuActions(QMenu& menu, QString actions, QString objectName);
    
private:
    QTreeWidgetItem* mProjectRoot = nullptr;
    QTreeWidgetItem* mGlobalDef = nullptr;
    
    QTreeWidgetItem* mComponentRoot = nullptr;
    QTreeWidgetItem* mComponentDefine = nullptr;
    QTreeWidgetItem* mComponentGeometry = nullptr;
    QTreeWidgetItem* mComponentMaterial = nullptr;
    QTreeWidgetItem* mComponentMesh = nullptr;
    QTreeWidgetItem* mComponentPhysic = nullptr;
    
    QTreeWidgetItem* mStudyRoot = nullptr;
    
    QTreeWidgetItem* mPostProcessingRoot = nullptr;
};
} // namespace FC



#endif // FCPROJECTTREEWIDGET_H
