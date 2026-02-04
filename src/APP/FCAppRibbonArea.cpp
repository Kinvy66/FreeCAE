/**
 * @file FCAppRibbonArea.cpp
 * @brief App的Ribbon区域接口，负责ribbon层的管理和调度
 * @date 2025-10-28
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAppRibbonArea.h"
#include "AppMainWindow.h"
#include <QActionGroup>
// SARibbon
#include "SARibbonMainWindow.h"
#include "SARibbonBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"
#include "SARibbonContextCategory.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonMenu.h"
#include "SARibbonCtrlContainer.h"
#include "SARibbonApplicationButton.h"
#include "SARibbonLineWidgetContainer.h"
// Qt
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QStandardPaths>
#include <QFontComboBox>
#include <QComboBox>
#include <QInputDialog>
#include <QMenu>
// ui
#include "FCAppRibbonApplicationMenu.h"
// api
#include "FCAppUI.h"
#include "FCAppCommand.h"
#include "FCAppCore.h"
// #include "FCAppDockingArea.h"
#include "FCAppActions.h"
// #include "FCRecentFilesManager.h"
// Qt-Advanced-Docking-System
#include "DockManager.h"
#include "DockAreaWidget.h"

// 快速链接信号槽
#define FCAPPRIBBONAREA_ACTION_BIND(actionname, functionname)                                                          \
connect(actionname, &QAction::triggered, this, &FCAppRibbonArea::functionname)

#define FCAPPRIBBONAREA_COMMON_SETTING_CPP(MiddleName, ShapeEditPannelWidget, FontEditWidget)                          \
    QPen FCAppRibbonArea::get##MiddleName##Pen() const                                                                 \
{                                                                                                                  \
        return ShapeEditPannelWidget->getBorderPen();                                                                  \
}                                                                                                                  \
    QBrush FCAppRibbonArea::get##MiddleName##Brush() const                                                             \
{                                                                                                                  \
        return ShapeEditPannelWidget->getBackgroundBrush();                                                            \
}                                                                                                                  \
    QFont FCAppRibbonArea::get##MiddleName##Font() const                                                               \
{                                                                                                                  \
        return FontEditWidget->getCurrentFont();                                                                       \
}                                                                                                                  \
    QColor FCAppRibbonArea::get##MiddleName##FontColor() const                                                         \
{                                                                                                                  \
        return FontEditWidget->getCurrentFontColor();                                                                  \
}                                                                                                                  \
    void FCAppRibbonArea::set##MiddleName##Pen(const QPen& v)                                                          \
{                                                                                                                  \
        QSignalBlocker b(ShapeEditPannelWidget);                                                                       \
        ShapeEditPannelWidget->setBorderPen(v);                                                                        \
}                                                                                                                  \
    void FCAppRibbonArea::set##MiddleName##Brush(const QBrush& v)                                                      \
{                                                                                                                  \
        QSignalBlocker b(ShapeEditPannelWidget);                                                                       \
        ShapeEditPannelWidget->setBackgroundBrush(v);                                                                  \
}                                                                                                                  \
    void FCAppRibbonArea::set##MiddleName##Font(const QFont& v)                                                        \
{                                                                                                                  \
        QSignalBlocker b(FontEditWidget);                                                                              \
        FontEditWidget->setCurrentFont(v);                                                                             \
}                                                                                                                  \
    void FCAppRibbonArea::set##MiddleName##FontColor(const QColor& v)                                                  \
{                                                                                                                  \
        QSignalBlocker b(FontEditWidget);                                                                              \
        FontEditWidget->setCurrentFontColor(v);                                                                        \
}

//===================================================
// using FC namespace -- 禁止在头文件using！！
//===================================================

using namespace FC;

//===================================================
// DAAppRibbonArea
//===================================================

FCAppRibbonArea::FCAppRibbonArea(FCUIInterface* u)
    : FCRibbonAreaInterface(u)
{
    FCAppUI* appui = qobject_cast< FCAppUI* >(u);
    mApp          = qobject_cast< AppMainWindow* >(appui->mainWindow());
    mActions      = qobject_cast< FCAppActions* >(u->getActionInterface());
    mAppCmd       = qobject_cast< FCAppCommand* >(u->getCommandInterface());
    // ribbon的构建在setDockingArea进行，为了保证ribbon在dock之后构建
}

FCAppRibbonArea::~FCAppRibbonArea()
{
}


/**
 * @brief 构建所有的action
 */
void FCAppRibbonArea::buildMenu()
{

    mMenuTheme = new SARibbonMenu(mApp);
    // mMenuTheme->setObjectName(QStringLiteral("menuTheme"));
    // mMenuTheme->setIcon(QIcon(QStringLiteral(":/app/bright/Icon/theme.svg")));
    // mMenuTheme->addAction(m_actions->actionRibbonThemeOffice2013);
    // mMenuTheme->addAction(m_actions->actionRibbonThemeOffice2016Blue);
    // mMenuTheme->addAction(m_actions->actionRibbonThemeOffice2021Blue);
    // mMenuTheme->addAction(m_actions->actionRibbonThemeDark);

}

/**
 * @brief 发生语言变更时会触发此函数
 */
void FCAppRibbonArea::retranslateUi()
{
    // resetText();
}

/**
 * @brief 重置文字
 */
void FCAppRibbonArea::resetText()
{
    ribbonBar()->applicationButton()->setText(tr("文件"));  // 文件
    
    // mCategoryMain->setCategoryName(tr("主页"));              // cn:主页
    // mCategoryGeometry->setCategoryName(tr("几何"));       // cn:几何
    // mCategoryMesh->setCategoryName(tr("Mesh"));              // cn:网格
    // mCategoryStudy->setCategoryName(tr("Study"));            // cn: 研究
    // mCategoryResult->setCategoryName(tr("Reslut"));          // cn:结果
    // mCategoryTools->setCategoryName(tr("Tools"));            // cn:工具
    
    //todo:其他panel以及action的翻译
}

/**
 * @brief 构建ribbon
 */
void FCAppRibbonArea::buildRibbon()
{
    ribbonBar()->showMinimumModeButton();
    ribbonBar()->applicationButton()->setText(tr("文件"));
    buildRibbonMainCategory();
    buildRibbonGeometryCategory();
    buildRibbonMeshCategory();
    buildRibbonStudyCategory();
    buildRibbonResultCategory();
    buildRibbonToolsCategory();
    buildRibbonQuickAccessBar();
    // todo:上下文标签
    //
    buildApplicationMenu();
    //
    buildRightButtonBar();
}

/**
 * @brief 构建主页标签
 * 主页的category objname = fc-ribbon-category-main
 */
void FCAppRibbonArea::buildRibbonMainCategory()
{
    mCategoryMain = new SARibbonCategory(app());
    mCategoryMain->setObjectName(QStringLiteral("fc-ribbon-category-main"));
    mCategoryMain->setCategoryName(tr("主页"));
    //---------File Pannel -----------------------------------------------
    mPanelMainFileOpt = mCategoryMain->addPanel(tr("文件"));
    mPanelMainFileOpt->setObjectName(QStringLiteral("fc-ribbon-category-main.file"));
    mPanelMainFileOpt->addLargeAction(mActions->actionNew);
    mPanelMainFileOpt->addLargeAction(mActions->actionOpen);
    mPanelMainFileOpt->addLargeAction(mActions->actionSave);
    
    mPanelModelOpt = mCategoryMain->addPanel(tr("模型"));
    mPanelModelOpt->setObjectName(QStringLiteral("fc-ribbon-category-main.model"));
    mPanelModelOpt->addLargeAction(mActions->actionHomeImportGeometry);
    mPanelModelOpt->addLargeAction(mActions->actionHomeImportMesh);
    
    mPanelLayoutOpt = mCategoryMain->addPanel(tr("布局"));
    mPanelLayoutOpt->setObjectName(QStringLiteral("fc-ribbon-category-main.layout"));
    mPanelLayoutOpt->addLargeAction(mActions->actionWindowViews);
    mPanelLayoutOpt->addLargeAction(mActions->actionResetLayout);
    
    
    ribbonBar()->addCategoryPage(mCategoryMain);
}

/**
 * @brief 构建几何标签
 * 主页的category objname = fc-ribbon-category-geometry
 */
void FCAppRibbonArea::buildRibbonGeometryCategory()
{
    mCategoryGeometry = new SARibbonCategory(app());
    mCategoryGeometry->setObjectName(QStringLiteral("fc-ribbon-category-geometry"));
    mCategoryGeometry->setCategoryName(tr("几何"));
    
    mPanelGeometryBuildOpt = mCategoryGeometry->addPanel(tr("构建"));
    mPanelGeometryBuildOpt->setObjectName(QStringLiteral("fc-ribbon-category-geometry.build"));
    mPanelGeometryBuildOpt->addLargeAction(mActions->actionGeometryModelBuildAll);
    
    
    // 1. import/export panel
    
    mPanelGeometryImpExpOpt = mCategoryGeometry->addPanel(tr("导入/导出"));       ///< 导入/导出操作
    mPanelGeometryImpExpOpt->setObjectName(QStringLiteral("fc-ribbon-category-geometry.importexport"));
    mPanelGeometryImpExpOpt->addSmallAction(mActions->actionImportGeometryModel);
    mPanelGeometryImpExpOpt->addSmallAction(mActions->actionExportGeometryModel);
    mPanelGeometryImpExpOpt->addSmallAction(mActions->actionInsertSequenen);
    
    // 2. create geometry panel
    mPanelGeometryPrimOpt = mCategoryGeometry->addPanel(tr("模型创建"));         ///< 体素操作
    mPanelGeometryPrimOpt->setObjectName(QStringLiteral("fc-ribbon-category-geometry.createmodel"));
    mPanelGeometryPrimOpt->addSmallAction(mActions->actionCreateCube);
    mPanelGeometryPrimOpt->addSmallAction(mActions->actionCreateCone);
    mPanelGeometryPrimOpt->addSmallAction(mActions->actionCreateCylinder);
    mPanelGeometryPrimOpt->addSmallAction(mActions->actionCreateSphere);
    mPanelGeometryPrimOpt->addSmallAction(mActions->actionCreateTorus);
    // mPanelGeometryPrimOpt->addSmallAction(m_actions->actionCreateHelix);
    
    SARibbonMenu* menuCreateMoreGemetryModel = new SARibbonMenu(mApp);
    menuCreateMoreGemetryModel->addAction(mActions->actionCreatorBezierCurve);
    menuCreateMoreGemetryModel->addAction(mActions->actionCreatorTetrahedron);
    mActions->actionMenuCreateMoreGemetryModel->setMenu(menuCreateMoreGemetryModel);
    mPanelGeometryPrimOpt->addLargeAction(mActions->actionMenuCreateMoreGemetryModel);
    
    
    // 3. work plane panel
    mPanelGeometryWrkPlaneOpt = mCategoryGeometry->addPanel(tr("工作平面")) ;     ///< 工作平面操作
    mPanelGeometryWrkPlaneOpt->setObjectName(QStringLiteral("fc-ribbon-category-geometry.workplane"));
    mPanelGeometryWrkPlaneOpt->addLargeAction(mActions->actionMenuSelectWorkPlane);
    mPanelGeometryWrkPlaneOpt->addLargeAction(mActions->actionWorkPlane);
    mPanelGeometryWrkPlaneOpt->addSeparator();
    mPanelGeometryWrkPlaneOpt->addLargeAction(mActions->actionExtrude);
    mPanelGeometryWrkPlaneOpt->addSmallAction(mActions->actionRevolve);
    mPanelGeometryWrkPlaneOpt->addSmallAction(mActions->actionSweep);
    mPanelGeometryWrkPlaneOpt->addSmallAction(mActions->actionLoft);
    
    
    // 4. geometry operator panel
    mPanelGeometryOperatopsOpt = mCategoryGeometry->addPanel(tr("操作"));    ///< 操作
    mPanelGeometryOperatopsOpt->setObjectName(QStringLiteral("fc-ribbon-category-geometry.operate"));    
    SARibbonMenu* menuGeometryBooleanOpt = new SARibbonMenu(mApp);
    menuGeometryBooleanOpt->addAction(mActions->actionBooleanUnion);
    menuGeometryBooleanOpt->addAction(mActions->actionBooleanIntersection);
    menuGeometryBooleanOpt->addAction(mActions->actionBooleanDifference);
    menuGeometryBooleanOpt->addAction(mActions->actionBooleanCompose);
    mActions->actionMenuGeometryBooleanOpt->setMenu(menuGeometryBooleanOpt);
    mPanelGeometryOperatopsOpt->addLargeAction(mActions->actionMenuGeometryBooleanOpt);
    SARibbonMenu* menuGeometryTransformOpt = new SARibbonMenu(mApp);
    menuGeometryTransformOpt->addAction(mActions->actionTransformRigid);
    menuGeometryTransformOpt->addAction(mActions->actionTransformCopy);
    menuGeometryTransformOpt->addAction(mActions->actionTransformScale);
    menuGeometryTransformOpt->addAction(mActions->actionTransformMove);
    menuGeometryTransformOpt->addAction(mActions->actionTransformMirror);
    menuGeometryTransformOpt->addAction(mActions->actionTransformArray);
    mActions->actionMenuGeometryTransformOpt->setMenu(menuGeometryTransformOpt);
    mPanelGeometryOperatopsOpt->addAction(mActions->actionMenuGeometryTransformOpt);
    mPanelGeometryOperatopsOpt->addSmallAction(mActions->actionChamferOpt);
    mPanelGeometryOperatopsOpt->addSmallAction(mActions->actionFilletOpt);
    mPanelGeometryOperatopsOpt->addSmallAction(mActions->actionGeometryDelete);
    
    
    // 5. other panel
    mPanelGeometryOtherOpt = mCategoryGeometry->addPanel(tr("其他"));        ///< 其他操作
    mPanelGeometryOtherOpt->setObjectName(QStringLiteral("fc-ribbon-category-geometry.other"));    
    mPanelGeometryOtherOpt->addLargeAction(mActions->actionGeometryMeasure);
    mPanelGeometryOtherOpt->addLargeAction(mActions->actionGeometryDeletSequence);
    

    ribbonBar()->addCategoryPage(mCategoryGeometry);
}

/**
 * @brief 构建网格标签
 * 主页的category objname = fc-ribbon-category-mesh
 */
void FCAppRibbonArea::buildRibbonMeshCategory()
{
    mCategoryMesh = new SARibbonCategory(app());
    mCategoryMesh->setObjectName(QStringLiteral("fc-ribbon-category-mesh"));
    mCategoryMesh->setCategoryName(tr("网格"));
    
    // // 1.  mesh panel
    mPanelMeshBuildOpt = mCategoryMesh->addPanel(tr("构建"));
    mPanelMeshBuildOpt->setObjectName(QStringLiteral("fc-ribbon-category-mesh.build"));
    mPanelMeshBuildOpt->addLargeAction(mActions->actionMeshBuild);
    mPanelMeshBuildOpt->addLargeAction(mActions->actionMenuSelectMesh);
    mPanelMeshBuildOpt->addLargeAction(mActions->actionAddMesh);
    
    // // 2. import/export panel
    mPanelMeshImpExpOpt = mCategoryMesh->addPanel(tr("导入/导出"));
    mPanelMeshImpExpOpt->setObjectName(QStringLiteral("fc-ribbon-category-mesh.importexport"));
    mPanelMeshImpExpOpt->addLargeAction(mActions->actionMeshImportMesh);
    mPanelMeshImpExpOpt->addLargeAction(mActions->actionMeshExportMesh);
    
    
    // // 4. evaluate panel
    mPanelMeshEvaluateOpt = mCategoryMesh->addPanel(tr("计算"));
    mPanelMeshEvaluateOpt->setObjectName(QStringLiteral("fc-ribbon-category-mesh.evaluate"));
    mPanelMeshEvaluateOpt->addLargeAction(mActions->actionMeshMeasure);
    mPanelMeshEvaluateOpt->addLargeAction(mActions->actionMeshStatictic);
    mPanelMeshEvaluateOpt->addLargeAction(mActions->actionMeshPlot);
    
    // // 3. clear panel
    mPanelMeshClearOpt = mCategoryMesh->addPanel(tr("导入/导出"));
    mPanelMeshClearOpt->setObjectName(QStringLiteral("fc-ribbon-category-mesh.clear"));
    mPanelMeshClearOpt->addLargeAction(mActions->actionClearMesh);
    mPanelMeshClearOpt->addLargeAction(mActions->actionClearAllMesh);
    mPanelMeshClearOpt->addLargeAction(mActions->actionDeletMeshSequenen);
    
    ribbonBar()->addCategoryPage(mCategoryMesh);
}

/**
 * @brief 构建研究标签
 * 主页的category objname = fc-ribbon-category-study
 */
void FCAppRibbonArea::buildRibbonStudyCategory()
{
    mCategoryStudy = new SARibbonCategory(app());
    mCategoryStudy->setObjectName(QStringLiteral("fc-ribbon-category-study"));
    mCategoryStudy->setCategoryName(tr("研究"));
    
    mPanelStudySolverOpt = mCategoryStudy->addPanel(tr("求解"));
    mPanelStudySolverOpt->setObjectName(QStringLiteral("fc-ribbon-category-study.solver"));
    mPanelStudySolverOpt->addLargeAction(mActions->actionSolverManager);
    mPanelStudySolverOpt->addLargeAction(mActions->actionCompute);   
    
    ribbonBar()->addCategoryPage(mCategoryStudy);
    
}

/**
 * @brief 构建结果标签
 * 主页的category objname = fc-ribbon-category-result
 */
void FCAppRibbonArea::buildRibbonResultCategory()
{
    mCategoryResult = new SARibbonCategory(app());
    mCategoryResult->setObjectName(QStringLiteral("fc-ribbon-category-result"));
    mCategoryResult->setCategoryName(tr("结果"));
    
    // //1. plot panel
    mPanelPlotGroupRes = mCategoryResult->addPanel(tr("绘图"));
    mPanelPlotGroupRes->setObjectName(QStringLiteral("fc-ribbon-category-result.plot"));
    mPanelPlotGroupRes->addLargeAction(mActions->action3DPlot);
    mPanelPlotGroupRes->addLargeAction(mActions->action2DPlot);
    mPanelPlotGroupRes->addLargeAction(mActions->action1DPlot);
    
    //  //2. export panel
    mPanelExportRes = mCategoryResult->addPanel(tr("数值"));
    mPanelExportRes->setObjectName(QStringLiteral("fc-ribbon-category-result.export"));
    SARibbonMenu* menuResultDataExport = new SARibbonMenu(mApp);
    menuResultDataExport->addAction(mActions->actionNumericalDataExport);
    menuResultDataExport->addAction(mActions->actionPlotDataExport);
    menuResultDataExport->addAction(mActions->actionMeshDataExport);
    menuResultDataExport->addAction(mActions->actionTableDataExport);
    mActions->actionMenuDataExport->setMenu(menuResultDataExport);
    mPanelExportRes->addLargeAction(mActions->actionMenuDataExport);
    mPanelExportRes->addLargeAction(mActions->actionImageExport);
    mPanelExportRes->addLargeAction(mActions->actionAnimationExport);
    
    // //3. Clear panel
    mPanelClearRes = mCategoryResult->addPanel(tr("清除"));
    mPanelClearRes->setObjectName(QStringLiteral("fc-ribbon-category-result.clear"));
    mPanelClearRes->addLargeAction(mActions->actionClearPlotData);

    
    ribbonBar()->addCategoryPage(mCategoryResult);
    
}

/**
 * @brief 构建工具标签
 * 主页的category objname = fc-ribbon-category-tools
 */
void FCAppRibbonArea::buildRibbonToolsCategory()
{
    mCategoryTools = new SARibbonCategory(app());
    mCategoryTools->setObjectName(QStringLiteral("fc-ribbon-category-tools"));
    mCategoryTools->setCategoryName(tr("工具"));
    
    mPanelDevTools = mCategoryTools->addPanel(tr("工具"));
    mPanelDevTools->setObjectName(QStringLiteral("fc-ribbon-category-tools.tool"));
    mPanelDevTools->addLargeAction(mActions->actionPlugin);
    mPanelDevTools->addLargeAction(mActions->actionAbout);
        
    ribbonBar()->addCategoryPage(mCategoryTools);
    
}

/**
 * @brief 构建ribbon的QuickAccessBar
 * 主页的category objname = da-ribbon-category-main
 */
void FCAppRibbonArea::buildRibbonQuickAccessBar()
{
    SARibbonQuickAccessBar* quickAccessBar = ribbonBar()->quickAccessBar();
    quickAccessBar->addSeparator();
    quickAccessBar->addAction(mActions->actionNew);
    quickAccessBar->addAction(mActions->actionOpen);
    quickAccessBar->addAction(mActions->actionSave);
    quickAccessBar->addAction(mActions->actionSaveAs);
    quickAccessBar->addAction(mActions->actionGlobalUndo);
    quickAccessBar->addAction(mActions->actionGlobalRedo);
    quickAccessBar->addAction(mActions->actionGlobalCopy);
    quickAccessBar->addAction(mActions->actionGlobalPaste);
    quickAccessBar->addAction(mActions->actionGlobalDuplicate);
    quickAccessBar->addAction(mActions->actionGlobalDelete);
    quickAccessBar->addAction(mActions->actionGlobalSelectAll);
    quickAccessBar->addAction(mActions->actionGlobalClearSelect);
    quickAccessBar->addAction(mActions->actionGlobaFind);
    
    quickAccessBar->addSeparator();
}

/**
 * @brief 构建ApplicationMenu
 */
void FCAppRibbonArea::buildApplicationMenu()
{
    mApplicationMenu = new FCAppRibbonApplicationMenu(app());
    mApplicationMenu->addAction(mActions->actionNew);
    mApplicationMenu->addAction(mActions->actionOpen);
    mApplicationMenu->addAction(mActions->actionSave);
    mApplicationMenu->addAction(mActions->actionSaveAs);
    mApplicationMenu->addSeparator();
    mApplicationMenu->addAction(mActions->actionPreferences);
    mApplicationMenu->addSeparator();
    mApplicationMenu->addAction(mActions->actionExit);
    
    // m_actions->recentFilesManager->attachToMenu(mApplicationMenu, tr("Recent Files"));  // cn:最近打开的文件
    SARibbonApplicationButton* appBtn = qobject_cast< SARibbonApplicationButton* >(ribbonBar()->applicationButton());
    if (nullptr == appBtn) {
        return;
    }
    mApplicationMenu->update();
    appBtn->setMenu(mApplicationMenu);
}

/**
 * @brief 构建右工具栏
 */
void FCAppRibbonArea::buildRightButtonBar()
{
    // ribbonBar()->activeRightButtonGroup();
    // SARibbonButtonGroupWidget* rbar = ribbonBar()->rightButtonGroup();
    // rbar->addMenuAction(mMenuTheme);
}

void FCAppRibbonArea::setDockingArea(FCAppDockingArea *dock)
{
    mDockArea = dock;
    buildMenu();
    buildRibbon();
    buildRedoUndo();
    resetText();
}


AppMainWindow* FCAppRibbonArea::app() const
{
    return (mApp);
}

SARibbonBar* FCAppRibbonArea::ribbonBar() const
{
    return (app()->ribbonBar());
}

/**
 * @brief mian标签
 * @return
 */
SARibbonCategory* FCAppRibbonArea::getRibbonCategoryMain() const
{
    return (mCategoryMain);
}

/**
 * @brief 通过DACommandInterface构建redo/undo的action
 * @param cmd
 */
void FCAppRibbonArea::buildRedoUndo()
{
    QUndoGroup& undoGroup = mAppCmd->undoGroup();
    // 设置redo,undo的action
    
    // m_actions->actionRedo = undoGroup.createRedoAction(this);
    // m_actions->actionRedo->setObjectName("actionRedo");
    // m_actions->actionRedo->setIcon(QIcon(":/app/bright/Icon/redo.svg"));
    // m_actions->actionRedo->setShortcut(QKeySequence::Redo);
    // m_actions->actionUndo = undoGroup.createUndoAction(this);
    // m_actions->actionUndo->setObjectName("actionUndo");
    // m_actions->actionUndo->setIcon(QIcon(":/app/bright/Icon/undo.svg"));
    // m_actions->actionUndo->setShortcut(QKeySequence::Undo);
    // SARibbonQuickAccessBar* bar = ribbonBar()->quickAccessBar();
    // if (!bar) {
    //     return;
    // }
    // bar->addAction(m_actions->actionUndo);
    // bar->addAction(m_actions->actionRedo);
}



/**
 * @brief 显示上下文(会把其他上下文隐藏)
 * @param type
 */
void FCAppRibbonArea::showContextCategory(FCAppRibbonArea::ContextCategoryType type)
{
    SARibbonBar* ribbon = ribbonBar();
    //todo
}

/**
 * @brief 隐藏上下文
 * @param type
 */
void FCAppRibbonArea::hideContextCategory(FCAppRibbonArea::ContextCategoryType type)
{
    SARibbonBar* ribbon = ribbonBar();
   //todo
}




