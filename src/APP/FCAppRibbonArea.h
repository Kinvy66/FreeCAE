/**
 * @file FCAppRibbonArea.h
 * @brief App的Ribbon区域接口，负责ribbon层的管理和调度
 * @date 2025-10-28
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCAPPRIBBONAREA_H
#define FCAPPRIBBONAREA_H

#include <FCRibbonAreaInterface.h>
#include <QObject>
#include <QAction>
#include <QUndoStack>
#include <QSpinBox>
#include <QWidgetAction>
// #include "DADataManageWidget.h"
// #include "DAWorkFlowGraphicsScene.h"

#define FCAPPRIBBONAREA_COMMON_SETTING_H(MiddleName)                                                                   \
public:                                                                                                                \
    QPen get##MiddleName##Pen() const;                                                                                 \
    QBrush get##MiddleName##Brush() const;                                                                             \
    QFont get##MiddleName##Font() const;                                                                               \
    QColor get##MiddleName##FontColor() const;                                                                         \
    public slots:                                                                                                          \
    void set##MiddleName##Pen(const QPen& v);                                                                          \
    void set##MiddleName##Brush(const QBrush& v);                                                                      \
    void set##MiddleName##Font(const QFont& v);                                                                        \
    void set##MiddleName##FontColor(const QColor& v);

// Qt
class QComboBox;
class QToolBar;
class QMenuBar;
class QFontComboBox;
class QUndoStack;

// Qt-Advanced-Docking-System 前置申明
namespace ads
{
class CDockWidget;
}

// SA Ribbon
class SARibbonBar;
class SARibbonCategory;
class SARibbonPanel;
class SARibbonContextCategory;
class SARibbonLineWidgetContainer;
class SARibbonButtonGroupWidget;
class SARibbonCtrlContainer;

namespace FC 
{
class AppMainWindow;
class FCAppDockingArea;
class FCAppActions;
class FCAppCommand;
class FCPyDTypeComboBox;
class FCCommandInterface;
class FCAppDataManager;
class FCDataOperateOfDataFrameWidget;
class FCAppProject;
class FCAppRibbonApplicationMenu;
//
class FCDataOperatePageWidget;
// 窗口
class FCFontEditPannelWidget;
class FCShapeEditPannelWidget;
class FCColorPickerButton;
class FCFigureWidget;
class FCChartOperateWidget;
class FCChartWidget;
class FCDataOperateWidget;
class FCWorkFlowEditWidget;
class FCWorkFlowOperateWidget;



class FCAppRibbonArea : public FCRibbonAreaInterface
{
    friend class AppMainWindow;
    Q_OBJECT
public:
public:
    /**
	 * @brief 上下文类型
	 */
    enum ContextCategoryType
    {
        ContextCategoryData,      ///< Data相关的上下文
        ContextCategoryWorkflow,  ///< Workflow相关的上下文
        ContextCategoryChart,     ///< Chart相关的上下文
        AllContextCategory        ///< 这个代表所有的上下文
    };
    Q_ENUM(ContextCategoryType)
    
public:
    FCAppRibbonArea(FCUIInterface* u);
    ~FCAppRibbonArea();
    
    // 发生语言变更时会触发此函数
    virtual void retranslateUi() override;
    // 获取app
    AppMainWindow* app() const;
    // 获取ribbon
    SARibbonBar* ribbonBar() const;
    // 获取主标签
    SARibbonCategory* getRibbonCategoryMain() const;
    // 通过FCCommandInterface构建redo/undo的action
    void buildRedoUndo();
    // 更新ActionLockBackgroundPixmap的check statue
    // void updateActionLockBackgroundPixmapCheckStatue(bool c);
    // 显示上下文(会把其他上下文隐藏)
    void showContextCategory(ContextCategoryType type);
    // 隐藏上下文
    void hideContextCategory(ContextCategoryType type);
    
    
public:
    //===================================================
    // 更新操作
    //===================================================
    // 更新绘图相关的ribbon
    // void updateFigureAboutRibbon(DAFigureWidget* fig);
    // void updateChartAboutRibbon(DAChartWidget* chart);
    // void updateChartGridAboutRibbon(DAChartWidget* chart);
    // void updateChartZoomPanAboutRibbon(DAChartWidget* chart);
    // void updateChartPickerAboutRibbon(DAChartWidget* chart);
    // void updateChartLegendAboutRibbon(DAChartWidget* chart);
    
    // void updateWorkflowAboutRibbon(DAWorkFlowOperateWidget* wfo);
    // 重置文字
    void resetText();
    
private:
    // 构建所有的action
    void buildMenu();
    // 构建界面
    void buildRibbon();
    // 构建主页标签
    void buildRibbonMainCategory();
    // 构建几何标签
    void buildRibbonGeometryCategory();
    // 构建网格标签
    void buildRibbonMeshCategory();
    // 构建研究标签
    void buildRibbonStudyCategory();
    // 构建结果标签
    void buildRibbonResultCategory();
    // 构建工具标签
    void buildRibbonToolsCategory();
    // 构建快速响应栏
    void buildRibbonQuickAccessBar();
    // todo: 上下文标签页

    // 构建ApplicationMenu
    void buildApplicationMenu();
    // 构建右工具栏
    void buildRightButtonBar();
    
    FCAPPRIBBONAREA_COMMON_SETTING_H(Edit)
    FCAPPRIBBONAREA_COMMON_SETTING_H(WorkFlowEdit)
    
    /**
	 * @brief 设置dock区，有些pannel的action是依赖dock界面的，统一在这里设置
	 * @param dock
	 */
    void setDockingArea(FCAppDockingArea* dock);
    
Q_SIGNALS:
    
private Q_SLOTS:
    
    
public:
    ///////////////////////////////////////////
    /// ribbon
    ///////////////////////////////////////////
    // pass
    
    FCAppActions* mActions;       ///< 所有的action管理
    FCAppDockingArea* mDockArea;  ///< 注意这个变量不能在构造函数中调用
    AppMainWindow* mApp;
    FCAppCommand* mAppCmd;        ///< cmd
    //----------------------------------------------------
    // Main
    //----------------------------------------------------
    SARibbonCategory* mCategoryMain;           ///< 主页标签
    SARibbonPanel* mPanelMainFileOpt;         ///< 文件操作
    SARibbonPanel* mPanelModelOpt;            ///< 模型操作
    SARibbonPanel* mPanelGeometryOpt;         ///< 几何操作
    SARibbonPanel* mPanelMeshOpt;             ///< 网格操作
    SARibbonPanel* mPanelStudyOpt;            ///< 研究操作
    SARibbonPanel* mPanelLayoutOpt;           ///< 布局操作
    
    //----------------------------------------------------
    // Geometry
    //----------------------------------------------------
    SARibbonCategory* mCategoryGeometry;           ///< 几何标签
    SARibbonPanel* mPanelGeometryBuildOpt;        ///< 构建操作
    SARibbonPanel* mPanelGeometryImpExpOpt;       ///< 导入/导出操作
    SARibbonPanel* mPanelGeometryPrimOpt;         ///< 体素操作
    SARibbonPanel* mPanelGeometryWrkPlaneOpt;     ///< 工作平面操作
    SARibbonPanel* mPanelGeometryOperatopsOpt;    ///< 操作
    SARibbonPanel* mPanelGeometryOtherOpt;        ///< 其他操作
    
    //----------------------------------------------------
    // Mesh
    //----------------------------------------------------
    SARibbonCategory* mCategoryMesh;           ///< 网格标签
    SARibbonPanel* mPanelMeshBuildOpt;        ///< 构建操作
    SARibbonPanel* mPanelMeshImpExpOpt;       ///< 导入/导出操作
    SARibbonPanel* mPanelMeshGenerateOpt;     ///< 生成操作
    SARibbonPanel* mPanelMeshAttrOpt;         ///< 属性操作
    SARibbonPanel* mPanelMeshOperationOpt;    ///< 编辑操作
    SARibbonPanel* mPanelMeshEvaluateOpt;     ///< 计算操作
    SARibbonPanel* mPanelMeshClearOpt;        ///< 清除操作
    SARibbonPanel* mPanelMeshOtherOpt;        ///< 其他操作
    
    //----------------------------------------------------
    // Study
    //----------------------------------------------------
    SARibbonCategory* mCategoryStudy;           ///< 研究标签
    // SARibbonPanel* mPanelStudyOpt;             ///< 计算操作
    SARibbonPanel* mPanelStudySolverOpt;       ///< 求解器操作
    SARibbonPanel* mPanelStudyStepOpt;         ///< 研究步骤
    SARibbonPanel* mPanelStudyCleanOpt;        ///< 清除操作
    
    //----------------------------------------------------
    // Result
    //----------------------------------------------------
    SARibbonCategory* mCategoryResult;      ///< 结果标签
    SARibbonPanel* mPanelPlotGroupRes;     ///< 绘图组操作
    SARibbonPanel* mPanelDataSetRes;       ///< 数据集
    SARibbonPanel* mPanelNumrcalRes;       ///< 数值结果
    SARibbonPanel* mPanelExportRes;        ///< 导出结果
    SARibbonPanel* mPanelClearRes;         ///< 清除操作
    
    //----------------------------------------------------
    // Tools
    //----------------------------------------------------
    SARibbonCategory* mCategoryTools;   ///< 工具标签
    SARibbonPanel* mPanelDevTools;     ///< 开发工具

   
    // ApplicationMenu
    FCAppRibbonApplicationMenu* mApplicationMenu;        ///< ribbon-app menu
    
    // 菜单相关
    // QMenu* mExportWorkflowSceneToImageMenu { nullptr };  ///< scene导出为图片菜单
    // QMenu* mMenuViewLineMarkers { nullptr };            ///< 视图标记线
    // QMenu* mMenuInsertRow { nullptr };                  ///< 针对insertrow的action menu
    // QMenu* mMenuInsertColumn { nullptr };               ///< 这对insertcol的action menu
    QMenu* mMenuTheme { nullptr };                      ///< 主题菜单    

    
};
} // namespace FC
#endif // FCAPPRIBBONAREA_H
