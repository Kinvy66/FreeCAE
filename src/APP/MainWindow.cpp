/******************************************************************************
 * File     : MainWindow.cpp
 * Brief    : Rrinbon 菜单区和主窗口创建
 * Author   : Kinvy
 * Email    : kinvy66@163.com
 * Date     : 2025-10-19
 * Version  : V1.0.0
 * Detail   :
 * Copyright: Copyright By Kinvy, All Rights Reserved
******************************************************************************/
#include "MainWindow.h"

#if !SARIBBON_USE_3RDPARTY_FRAMELESSHELPER
#include "SAFramelessHelper.h"
#endif

#include "SARibbonApplicationButton.h"
#include "SARibbonBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonCategory.h"
#include <QCheckBox>
#include "SARibbonColorToolButton.h"
#include <QComboBox>
#include "SARibbonCustomizeDialog.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonGallery.h"
#include "SARibbonMenu.h"

#include <QStatusBar>
#include <spdlog/spdlog.h>
 #include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : SARibbonMainWindow(parent)
{
    this->setWindowTitle(tr("FreeCAE"));
    this->setWindowIcon(QIcon(":/icon/res/icon/FreeCAE.png"));
    
    setStatusBar(new QStatusBar());
    
     SARibbonBar* ribbon = ribbonBar();
    
    m_app = new AppWidget(this);
    this->setCentralWidget(m_app);
    
    // 普通打印
    spdlog::info("Welcome to info spdlog!");
    
    createApplicationButton();
    
    // 添加主页标签   
    SARibbonCategory* categoryHome = ribbon->addCategoryPage(tr("主页"));
    //! cn: SARibbonBar的Category和Panel，以及对应的Action都应该设置ObjectName，因为如果要自定义action，这些ObjectName是必不可少的
    //! en: The category , panel and actions of SARibbonBar, should be set with Object Names, as these Object Names are essential for customizing actions
    categoryHome->setObjectName(("categoryMain"));
    createCategoryHome(categoryHome);
    spdlog::info("add home page!");
    
    
    // 添加定义标签
    SARibbonCategory* categoryDefine = new SARibbonCategory();
    categoryDefine->setCategoryName(tr("定义"));
    categoryDefine->setObjectName(("categoryDefine"));
    createCategoryDefine(categoryDefine);
    ribbon->addCategoryPage(categoryDefine);
    spdlog::info("add define page!");
    
    
    // 添加几何标签
    SARibbonCategory* categoryGeometry = new SARibbonCategory();
    categoryGeometry->setCategoryName(tr("几何"));
    categoryGeometry->setObjectName(("categoryDefine"));
    createCategoryGeometry(categoryGeometry);
    ribbon->addCategoryPage(categoryGeometry);
    spdlog::info("add geometry page!");
    
    // 添加材料标签
    SARibbonCategory* categoryMaterial = new SARibbonCategory();
    categoryMaterial->setCategoryName(tr("材料"));
    categoryMaterial->setObjectName(("categoryMaterial"));
    createCategoryMaterial(categoryMaterial);
    ribbon->addCategoryPage(categoryMaterial);
    spdlog::info("add material page!");
    
    
    // 添加物理场标签
    SARibbonCategory* categoryPhysic = new SARibbonCategory();
    categoryPhysic->setCategoryName(tr("材料"));
    categoryPhysic->setObjectName(("categoryPhysic"));
    createCategoryPhysic(categoryPhysic);
    ribbon->addCategoryPage(categoryPhysic);
    spdlog::info("add physic page!");
    
    
    // 添加网格标签
    SARibbonCategory* categoryGrid = new SARibbonCategory();
    categoryGrid->setCategoryName(tr("网格"));
    categoryGrid->setObjectName(("categoryGrid"));
    createCategoryGrid(categoryGrid);
    ribbon->addCategoryPage(categoryGrid);
    spdlog::info("add grid page!");
    
    // 添加研究标签
    SARibbonCategory* categoryStudy = new SARibbonCategory();
    categoryStudy->setCategoryName(tr("研究"));
    categoryStudy->setObjectName(("categoryStudy"));
    createCategoryStudy(categoryStudy);
    ribbon->addCategoryPage(categoryStudy);
    spdlog::info("add study page!");
    
    // 添加结果标签
    SARibbonCategory* categoryResult = new SARibbonCategory();
    categoryResult->setCategoryName(tr("研究"));
    categoryResult->setObjectName(("categoryResult"));
    createCategoryResult(categoryResult);
    ribbon->addCategoryPage(categoryResult);
    spdlog::info("add result page!");
    
    // 添加工具标签
    SARibbonCategory* categoryTool = new SARibbonCategory();
    categoryTool->setCategoryName(tr("工具"));
    categoryTool->setObjectName(("categoryTool"));
    createCategoryTool(categoryTool);
    ribbon->addCategoryPage(categoryTool);
    spdlog::info("add tool page!");
    
    this->resize(800, 600);   // 窗口大小 800x600
    
    
}

MainWindow::~MainWindow() {}

/**
 * @brief 创建ApplicationButton
 * ApplicationButton是Ribbon界面左上角的按钮，通常用于显示文件相关操作，如新建、打开、保存等
 */
void MainWindow::createApplicationButton()
{
    SARibbonBar* ribbon = ribbonBar();
    if (!ribbon) {
        return;
    }
    
    QAbstractButton* btn = ribbon->applicationButton();
    
    if (!btn) {
        //! cn: SARibbonBar默认就会创建一个SARibbonApplicationButton，因此，在正常情况下，这个位置不会进入
        //! en: SARibbonBar creates a SARibbonApplicationButton by default. Therefore, under normal circumstances, this location will not enter
        btn = new SARibbonApplicationButton(this);
        // btn = new QAction(this);
        ribbon->setApplicationButton(btn);
    }
    btn->setText(tr("文件"));
    
    //! cn: SARibbonMenu和QMenu的操作是一样的
    //! en: The operations of SARibbonMenu and QMenu are the same
    if (!m_MenuApplicationBtn) {
        m_MenuApplicationBtn = new SARibbonMenu(this);
        m_MenuApplicationBtn->addAction(createAction(tr("新建"), ":/icon/res/icon/icon_file_open.png",
                                                    QKeySequence(Qt::CTRL + Qt::Key_N)));
        m_MenuApplicationBtn->addAction(createAction(tr("运行"), ":/icon/res/icon/icon_file_open.png"));
        m_MenuApplicationBtn->addSeparator();
        m_MenuApplicationBtn->addAction(createAction(tr("打开"), ":/icon/icon/action.svg",
                                                    QKeySequence(Qt::CTRL + Qt::Key_O)));
        m_MenuApplicationBtn->addAction(createAction(tr("打开自"), ":/icon/icon/action.svg",
                                                    QKeySequence("Ctrl+Shift+O")));
        
        // mMenuApplicationBtn->addAction(createAction(tr("最近打开的文件"), ":/icon/icon/action.svg"));
        
        SARibbonMenu* recentMenu = new SARibbonMenu(tr("最近打开的文件"), this);
        // TODO: 最近打开动态获取
        recentMenu->addAction(createAction(tr("project1.cae"), ":/icon/icon/file.svg"));
        recentMenu->addAction(createAction(tr("project2.cae"), ":/icon/icon/file.svg"));
        recentMenu->addAction(createAction(tr("project3.cae"), ":/icon/icon/file.svg"));
        m_MenuApplicationBtn->addMenu(recentMenu);
        
        m_MenuApplicationBtn->addAction(createAction(tr("案例库"), ":/icon/icon/action.svg"));
        m_MenuApplicationBtn->addSeparator();
        m_MenuApplicationBtn->addAction(createAction(tr("保存"), ":/icon/icon/action.svg"));
        m_MenuApplicationBtn->addAction(createAction(tr("另存为"), ":/icon/icon/action3.svg"));
        m_MenuApplicationBtn->addSeparator();
        
        SARibbonMenu* helpMenu = new SARibbonMenu(tr("帮助"), this);
        helpMenu->addAction(createAction(tr("帮助"),":/icon/res/icon/icon_app_help.png"));
        helpMenu->addAction(createAction(tr("关于"), ":/icon/res/icon/icon_app_help.png"));
        m_MenuApplicationBtn->addMenu(helpMenu);
        
        m_MenuApplicationBtn->addAction(createAction(tr("首选项"), ":/icon/icon/action3.svg"));
        m_MenuApplicationBtn->addSeparator();
        m_MenuApplicationBtn->addAction(createAction(tr("退出"), ":/icon/icon/action3.svg"));
     
    }
    SARibbonApplicationButton* appBtn = qobject_cast< SARibbonApplicationButton* >(btn);
    if (!appBtn) {
        return;
    }
    appBtn->setMenu(m_MenuApplicationBtn);
}

/**
 * @brief 创建主页分类页面内容
 * @param page 主分类页面指针
 */
void MainWindow::createCategoryHome(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryDefine(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryGeometry(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryMaterial(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryPhysic(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryGrid(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryStudy(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryResult(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryTool(SARibbonCategory *page)
{
    
}


/**
 * @brief 创建动作
 * @param text 动作文本
 * @param iconurl 图标路径
 * @param objName 对象名称
 * @return 创建的动作指针
 *
 * cn:创建带有文本、图标和对象名称的动作
 * en:Create an action with text, icon, and object name
 */
QAction* MainWindow::createAction(const QString& text, const QString& iconurl, const QString& objName)
{
    QAction* act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(objName);
    return act;
}

/**
 * @brief 创建动作（重载版本）
 * @param text 动作文本
 * @param iconurl 图标路径
 * @return 创建的动作指针
 *
 * cn:创建带有文本和图标的动作，对象名称使用文本内容
 * en:Create an action with text and icon, using the text content as the object name
 */
QAction* MainWindow::createAction(const QString& text, const QString& iconurl)
{
    QAction* act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(text);
    return act;
}

QAction *MainWindow::createAction(const QString &text, const QString &iconurl, QKeySequence shortcut)
{
    QAction* act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(text);
    act->setShortcut(shortcut);
    return act;
}
