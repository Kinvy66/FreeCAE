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
#include <QWidgetAction>
#include <QActionGroup>

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
    categoryPhysic->setCategoryName(tr("物理场"));
    categoryPhysic->setObjectName(("categoryPhysic"));
    createCategoryPhysic(categoryPhysic);
    ribbon->addCategoryPage(categoryPhysic);
    spdlog::info("add physic page!");
    
    
    // 添加网格标签
    SARibbonCategory* categoryMesh = new SARibbonCategory();
    categoryMesh->setCategoryName(tr("网格"));
    categoryMesh->setObjectName(("categoryMesh"));
    createCategoryMesh(categoryMesh);
    ribbon->addCategoryPage(categoryMesh);
    spdlog::info("add mesh page!");
    
    // 添加研究标签
    SARibbonCategory* categoryStudy = new SARibbonCategory();
    categoryStudy->setCategoryName(tr("研究"));
    categoryStudy->setObjectName(("categoryStudy"));
    createCategoryStudy(categoryStudy);
    ribbon->addCategoryPage(categoryStudy);
    categoryStudy->setEnabled(false);

    spdlog::info("add study page!");
    
    // 添加结果标签
    SARibbonCategory* categoryResult = new SARibbonCategory();
    categoryResult->setCategoryName(tr("结果"));
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
    
    this->resize(1500, 600);   // 窗口大小 800x600
    
    
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
                                                    QKeySequence("Ctrl+N")));
        m_MenuApplicationBtn->addAction(createAction(tr("运行"), ":/icon/res/icon/icon_file_open.png"));
        m_MenuApplicationBtn->addSeparator();
        m_MenuApplicationBtn->addAction(createAction(tr("打开"), ":/icon/icon/action.svg",
                                                    QKeySequence("Ctrl+O")));
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
    //  1. App panel
    SARibbonPanel* panelApp = page->addPanel(tr("App"));

    QAction* actionApp = createAction(tr("App\n开发器"), ":/icon/res/icon/icon_app_help.png");

    connect(actionApp, &QAction::triggered, this, [=](){
        this->ribbonBar()->repaint();
    });
    addAction(actionApp);
    panelApp->addLargeAction(actionApp);

    // 2. 模型panel
    SARibbonPanel* panelModel = page->addPanel(tr("模型"));

    SARibbonMenu* menuComponent = new SARibbonMenu(this);
    // TODO： add component items
    QAction* actionComponent = createAction(tr("选择组件"), ":/icon/res/icon/icon_app_help.png");
    actionComponent->setMenu(menuComponent);
    actionComponent->setEnabled(false);
    panelModel->addAction(actionComponent);

    SARibbonMenu* menuAddComponent = new SARibbonMenu(this);
    QAction* actionAddComponent = createAction(tr("添加组件"), ":/icon/res/icon/icon_app_help.png");
    QAction* action3D = createAction(tr("三维"), ":/icon/res/icon/icon_app_help.png");
    QAction* action2D = createAction(tr("二维"), ":/icon/res/icon/icon_app_help.png");
    QAction* action1D = createAction(tr("一维"), ":/icon/res/icon/icon_app_help.png");
    QAction* action0D = createAction(tr("零维"), ":/icon/res/icon/icon_app_help.png");
    menuAddComponent->addAction(action3D);
    menuAddComponent->addAction(action2D);
    menuAddComponent->addAction(action1D);
    menuAddComponent->addAction(action0D);
    actionAddComponent->setMenu(menuAddComponent);
    panelModel->addAction(actionAddComponent);


    // 3. 定义panel
    SARibbonPanel* panelDefine = page->addPanel(tr("定义"));

    QAction* actionParameter = createAction(tr("参数"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuParameter = new SARibbonMenu(this);


    QLabel* labelSelect = new QLabel(tr("选择"), menuParameter);
    labelSelect->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelSelect->setStyleSheet(
        "QLabel {"
        "   background-color: #CCE4FF;"     // 浅蓝色背景
        "   color: black;"
        "   font-weight: bold;"
        "   padding: 4px 10px;"
        "}"
        );
    QWidgetAction* actionSelectTitle = new QWidgetAction(menuParameter);
    actionSelectTitle->setDefaultWidget(labelSelect);


    QLabel* labelAdd = new QLabel(tr("添加"), menuParameter);
    labelAdd->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelAdd->setStyleSheet(
        "QLabel {"
        "   background-color: #CCE4FF;"
        "   color: black;"
        "   font-weight: bold;"
        "   padding: 2px 10px;"
        "}"
        );
    QWidgetAction* actionAddTitle = new QWidgetAction(menuParameter);
    actionAddTitle->setDefaultWidget(labelAdd);


    // QAction* groupSelect = new QAction(QIcon(), tr("选择"), menuParameter);
    // groupSelect->setChecked(true);
    // groupSelect->setEnabled(false);
    // QAction* groupAdd = new QAction(QIcon(), tr("添加"), menuParameter);
    // groupAdd->setEnabled(false);
    // groupAdd->setChecked(true);

    QAction* ActionParameter1 = createAction(tr("参数1"), ":/icon/res/icon/icon_app_help.png");
    QAction* ActionAddParameter1 = createAction(tr("参数"), ":/icon/res/icon/icon_app_help.png");

    menuParameter->addAction(actionSelectTitle);
    menuParameter->addAction(ActionParameter1);
    menuParameter->addAction(actionAddTitle);
    menuParameter->addAction(ActionAddParameter1);

    actionParameter->setMenu(menuParameter);
    panelDefine->addAction(actionParameter);

    QAction* actionVar = createAction(tr("变量"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuVar = new SARibbonMenu(this);
    QAction* actionGlobalVar = createAction(tr("全局变量"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionLocalVar = createAction(tr("局部变量"), ":/icon/res/icon/icon_app_help.png");
    menuVar->addAction(actionGlobalVar);
    menuVar->addAction(actionLocalVar);
    actionVar->setMenu(menuVar);
    panelDefine->addSmallAction(actionVar,  QToolButton::InstantPopup);


    QAction* actionFunction = createAction(tr("函数"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuFunction = new SARibbonMenu(this);

    // ===== 创建分组标题 QLabel =====
    QLabel* labelSelectFunction = new QLabel(tr("全局"), menuFunction);
    labelSelectFunction->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelSelectFunction->setStyleSheet(
        "QLabel {"
        "   background-color: #CCE4FF;"  // 标题背景色（选中风格）
        "   color: black;"
        "   font-weight: bold;"
        "   padding: 4px 10px;"
        "}"
        );
    QWidgetAction* actionSelectFunction = new QWidgetAction(menuFunction);
    actionSelectFunction->setDefaultWidget(labelSelectFunction);

    // ===== 创建网格区域 QWidget =====
    QWidget* gridWidget = new QWidget(menuFunction);
    QGridLayout* gridLayout = new QGridLayout(gridWidget);
    gridLayout->setContentsMargins(8, 8, 8, 8);
    gridLayout->setSpacing(6);

    // ===== 创建按钮辅助函数 =====
    auto createGridButton = [&](const QString& text, const QString& iconPath) {
        QPushButton* btn = new QPushButton(QIcon(iconPath), text);
        btn->setIconSize(QSize(24, 24));
        btn->setFlat(true);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        btn->setStyleSheet(
            "QPushButton {"
            "   border: none;"
            "   background: transparent;"
            "   padding: 6px;"
            "   text-align: left;"
            "}"
            "QPushButton:hover {"
            "   background-color: #E6F0FF;"
            "   border-radius: 4px;"
            "}"
            );
        return btn;
    };

    // ===== 填充按钮（按列排列）=====
    QStringList names = {
        tr("解析"), tr("插值"), tr("分段"),
        tr("高斯脉冲"), tr("斜坡"), tr("矩形波"),
        tr("阶跃"), tr("三角波"), tr("波形"),
        tr("随机"), tr("外部"), tr("MATLAB"),
        tr("高程(DEM)"), tr("图像"), tr("函数Switch")
    };
    const int COLS = 3;  // 每行显示3列
    int row = 0, col = 0;
    for (const QString& name : names) {
        QPushButton* btn = createGridButton(name, ":/icon/res/icon/icon_app_help.png");
        gridLayout->addWidget(btn, row, col);
        connect(btn, &QPushButton::clicked, this, [=]() {
            qDebug() << "Clicked function:" << name;
            // TODO: 在这里处理点击事件
        });
        if (++col >= COLS) {
            col = 0;
            ++row;
        }
    }

    // ===== 把 gridWidget 封装成 QWidgetAction 加入菜单 =====
    QWidgetAction* gridAction = new QWidgetAction(menuFunction);
    gridAction->setDefaultWidget(gridWidget);

    // ===== 添加标题和网格到菜单 =====
    menuFunction->addAction(actionSelectFunction);
    menuFunction->addAction(gridAction);

    // ===== 绑定菜单到 Ribbon 面板按钮 =====
    actionFunction->setMenu(menuFunction);
    panelDefine->addSmallAction(actionFunction, QToolButton::InstantPopup);

    QAction* actionParameterEntry = createAction(tr("参数实例"), ":/icon/res/icon/icon_app_help.png");
    actionParameterEntry->setEnabled(false);
    panelDefine->addSmallAction(actionParameterEntry, QToolButton::InstantPopup);


    // 4. 几何panel
    SARibbonPanel* panelGeometry = page->addPanel(tr("几何"));
    QAction* actionBuildAll = createAction(tr("全部构建"), ":/icon/res/icon/icon_app_help.png");
    panelGeometry->addAction(actionBuildAll);
    QAction* actionImport = createAction(tr("导入"), ":/icon/res/icon/icon_app_help.png");
    panelGeometry->addAction(actionImport);

    // 5. 材料panel
    SARibbonPanel* panelMaterial = page->addPanel(tr("材料"));
    QAction* actionAddMateriall = createAction(tr("添加材料"), ":/icon/res/icon/icon_app_help.png");
    panelMaterial->addAction(actionAddMateriall);



    // 6. 物理场panel
    SARibbonPanel* panelPhysic = page->addPanel(tr("物理场"));
    SARibbonMenu* menuPhysic = new SARibbonMenu(this);
    QAction* actionSelectPhysic = createAction(tr("选择物理场"), ":/icon/res/icon/icon_app_help.png");
    // TODO: 动态获取
    menuPhysic->addAction(createAction(tr("物理场1"), ":/icon/res/icon/icon_app_help.png"));
    actionSelectPhysic->setMenu(menuPhysic);
    panelPhysic->addAction(actionSelectPhysic);

    QAction* actionAddPhysic = createAction(tr("添加物理场"), ":/icon/res/icon/icon_app_help.png");
    panelPhysic->addAction(actionAddPhysic);


    // 7. 网格panel
    SARibbonPanel* panelGrid = page->addPanel(tr("网格"));
    QAction* actionAddGrid = createAction(tr("添加网格"), ":/icon/res/icon/icon_app_help.png");
    panelGrid->addAction(actionAddGrid);
    SARibbonMenu* menuGrid = new SARibbonMenu(this);
    QAction* actionSelectGrid = createAction(tr("选择网格"), ":/icon/res/icon/icon_app_help.png");
    // TODO: 动态获取
    menuGrid->addAction(createAction(tr("网格1"), ":/icon/res/icon/icon_app_help.png"));
    actionSelectGrid->setMenu(menuGrid);
    panelGrid->addAction(actionSelectGrid);


    // 7. 研究panel
    SARibbonPanel* panelStudy = page->addPanel(tr("研究"));
    QAction* actionCaculate = createAction(tr("计算"), ":/icon/res/icon/icon_app_help.png");
    panelStudy->addAction(actionCaculate);

    SARibbonMenu* menuStudy = new SARibbonMenu(this);
    QAction* actionSelectStudy = createAction(tr("选择研究"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionStudy1 = createAction(tr("研究1"), ":/icon/res/icon/icon_app_help.png");
    menuStudy->addAction(actionStudy1);
    actionSelectStudy->setMenu(menuStudy);
    panelStudy->addAction(actionSelectStudy);

    QAction* actionAddStudy = createAction(tr("添加研究"), ":/icon/res/icon/icon_app_help.png");
    panelStudy->addAction(actionAddStudy);

    // 8. 结果panel
    SARibbonPanel* panelResult = page->addPanel(tr("结果"));

    SARibbonMenu* menuSelectViewGrp = new SARibbonMenu(this);
    QAction* actionSelectViewGrp = createAction(tr("选择绘图组"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionView1 = createAction(tr("绘图1"), ":/icon/res/icon/icon_app_help.png");
    menuSelectViewGrp->addAction(actionView1);
    actionSelectViewGrp->setMenu(menuSelectViewGrp);
    panelResult->addAction(actionSelectViewGrp);

    SARibbonMenu* menuAddViewGrp = new SARibbonMenu(this);
    QAction* actionAddViewGrp = createAction(tr("添加绘图组"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionView3D = createAction(tr("三维绘图组"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionView2D = createAction(tr("二维绘图组"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionView1D = createAction(tr("一维绘图组"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionView0D = createAction(tr("极坐标绘图组"), ":/icon/res/icon/icon_app_help.png");

    menuAddViewGrp->addAction(actionView3D);
    menuAddViewGrp->addAction(actionView2D);
    menuAddViewGrp->addAction(actionView1D);
    menuAddViewGrp->addAction(actionView0D);
    // menuAddViewGrp->addAction(actionView3D);

    actionAddViewGrp->setMenu(menuAddViewGrp);
    panelResult->addAction(actionAddViewGrp);


    // 9. 布局panel
    SARibbonPanel* panelLayout = page->addPanel(tr("布局"));

    SARibbonMenu* menuView = new SARibbonMenu(this);
    QAction* actionView = createAction(tr("窗口"), ":/icon/res/icon/icon_app_help.png");
    // TODO： 动态获取窗口
    QAction* actionWindow1 = createAction(tr("窗口1"), ":/icon/res/icon/icon_app_help.png");
    menuView->addAction(actionWindow1);
    actionView->setMenu(menuView);
    panelLayout->addAction(actionView);


    SARibbonMenu* menuResetView = new SARibbonMenu(this);

    // 主动作
    QAction* actionRestView = createAction(tr("重置桌面"), ":/icon/res/icon/icon_app_help.png");

    // 普通动作
    QAction* actionRestLayout = createAction(tr("重置布局"), ":/icon/res/icon/icon_app_help.png");

    // === 创建互斥分组 ===
    QActionGroup* layoutGroup = new QActionGroup(this);
    layoutGroup->setExclusive(true);

    // 自定义勾选图标（你也可以换成自己的图标路径）
    QIcon checkedIcon(":/icon/res/icon/icon_app_help.png");

    // 创建两个布局动作
    QAction* actionLayout1 = createAction(tr("宽屏布局"), "");
    QAction* actionLayout2 = createAction(tr("常规屏幕布局"), "");

    // 允许勾选
    actionLayout1->setCheckable(true);
    actionLayout2->setCheckable(true);

    // 添加到组中
    layoutGroup->addAction(actionLayout1);
    layoutGroup->addAction(actionLayout2);

    // 默认选中宽屏布局
    actionLayout1->setChecked(true);
    actionLayout1->setIcon(checkedIcon);
    actionLayout2->setIcon(QIcon());

    // === 构建菜单 ===
    menuResetView->addAction(actionRestLayout);
    menuResetView->addSeparator();
    menuResetView->addAction(actionLayout1);
    menuResetView->addAction(actionLayout2);

    actionRestView->setMenu(menuResetView);
    panelLayout->addAction(actionRestView);

    // === 当用户点击切换布局时，动态更新图标 ===
    connect(layoutGroup, &QActionGroup::triggered, this, [=](QAction* act) {
        for (QAction* a : layoutGroup->actions()) {
            a->setIcon(a == act ? checkedIcon : QIcon());
        }
        qDebug() << "布局切换到:" << act->text();
    });
}

/**
 * @brief 创建定义页面内容
 * @param page 定义分类页面指针
 */
void MainWindow::createCategoryDefine(SARibbonCategory *page)
{
    //  1. 变量 panel
    SARibbonPanel* panelVar = page->addPanel(tr("变量"));

    QAction* actionLocalVar = createAction(tr("局部变量"), ":/icon/res/icon/icon_app_help.png");

    // connect(actionApp, &QAction::triggered, this, [=](){
    //     this->ribbonBar()->repaint();
    // });
    // addAction(actionLocalVar);
    panelVar->addLargeAction(actionLocalVar);

    //  2. 函数 panel
    SARibbonPanel* panelFunction = page->addPanel(tr("函数"));

    QAction* actionAnalytic = createAction(tr("解析"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionInterpolation = createAction(tr("插值"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionSegement = createAction(tr("分段"), ":/icon/res/icon/icon_app_help.png");
    panelFunction->addSmallAction(actionAnalytic);
    panelFunction->addSmallAction(actionInterpolation);
    panelFunction->addSmallAction(actionSegement);

    QAction* actionMoreFunction = createAction(tr("更多函数"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuMoreFunction = new SARibbonMenu(this);

    // ===== 创建网格区域 QWidget =====
    QWidget* gridWidget = new QWidget(menuMoreFunction);
    QGridLayout* gridLayout = new QGridLayout(gridWidget);
    gridLayout->setContentsMargins(8, 8, 8, 8);
    gridLayout->setSpacing(6);

    auto createGridButton = [&](const QString& text, const QString& iconPath) {
        QPushButton* btn = new QPushButton(QIcon(iconPath), text);
        btn->setIconSize(QSize(24, 24));
        btn->setFlat(true);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        btn->setStyleSheet(
            "QPushButton {"
            "   border: none;"
            "   background: transparent;"
            "   padding: 6px;"
            "   text-align: left;"
            "}"
            "QPushButton:hover {"
            "   background-color: #E6F0FF;"
            "   border-radius: 4px;"
            "}"
            );
        return btn;
    };

    // ===== 填充按钮（按列排列）=====
    QStringList names = {
        tr("解析"), tr("插值"), tr("分段"),
        tr("高斯脉冲"), tr("斜坡"), tr("矩形波"),
        tr("阶跃"), tr("三角波"), tr("波形"),
        tr("随机"), tr("外部"), tr("MATLAB"),
        tr("高程(DEM)"), tr("图像"), tr("函数Switch")
    };
    const int COLS = 3;  // 每行显示3列
    int row = 0, col = 0;
    for (const QString& name : names) {
        QPushButton* btn = createGridButton(name, ":/icon/res/icon/icon_app_help.png");
        gridLayout->addWidget(btn, row, col);
        connect(btn, &QPushButton::clicked, this, [=]() {
            qDebug() << "Clicked function:" << name;
            // TODO: 在这里处理点击事件
        });
        if (++col >= COLS) {
            col = 0;
            ++row;
        }
    }

    // ===== 把 gridWidget 封装成 QWidgetAction 加入菜单 =====
    QWidgetAction* gridAction = new QWidgetAction(menuMoreFunction);
    gridAction->setDefaultWidget(gridWidget);

    menuMoreFunction->addAction(gridAction);
    actionMoreFunction->setMenu(menuMoreFunction);
    panelFunction->addAction(actionMoreFunction);

    //3. 选择 panel
    SARibbonPanel* panelChose = page->addPanel(tr("选择"));

    QStringList OpNames = {
        tr("显式"), tr("补集"), tr("相邻"),
        tr("球/圆盘"), tr("框"), tr("圆柱体"),
        tr("并集"), tr("交集"), tr("求差")
    };
    QStringList OpIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    for(int i = 0; i < OpNames.size(); ++i) {
        QAction* actionOp = createAction(OpNames[i], ":/icon/res/icon/icon_app_help.png");
        panelChose->addSmallAction(actionOp);
    }

    QAction* actionColor = createAction(tr("颜色"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuColor = new SARibbonMenu(this);
    QAction* actionChoseColor = createAction(tr("颜色选择"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionResetColor = createAction(tr("根据主题重置颜色"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionDeletColor = createAction(tr("移除选择颜色"), ":/icon/res/icon/icon_app_help.png");
    menuColor->addAction(actionChoseColor);
    menuColor->addAction(actionResetColor);
    menuColor->addAction(actionDeletColor);
    actionColor->setMenu(menuColor);
    panelChose->addAction(actionColor);


    //4. 探针 panel
    SARibbonPanel* panelProbe = page->addPanel(tr("探针"));
    QAction* actionUpdateProbe = createAction(tr("更新探针"), ":/icon/res/icon/icon_app_help.png");
    panelProbe->addAction(actionUpdateProbe);

    QStringList probeNames = {
        tr("域探针"), tr("边界探针"), tr("边探针"),
        tr("点探针"), tr("域点探针"), tr("边界点探针"),
        tr("全局变量")
    };
    QStringList probeIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    
    SARibbonMenu* menuProbe = new SARibbonMenu(this);
    QAction* actionProbe = createAction(tr("探针"), ":/icon/res/icon/icon_app_help.png");
    
    for(int i = 0; i < probeNames.size(); ++i) {
        QAction* actionProbe = createAction(probeNames[i], ":/icon/res/icon/icon_app_help.png");
        menuProbe->addAction(actionProbe);
    }
    actionProbe->setMenu(menuProbe);
    panelProbe->addAction(actionProbe);
    
    
    //5. 物理场实用程序 panel
    SARibbonPanel* panelPhysic = page->addPanel(tr("物理场实用程序"));
    QAction* actionPhysic = createAction(tr("物理场实用程序"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuPhysic = new SARibbonMenu(this);
    QAction* actionMassProperties = createAction(tr("质量属性"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionMassContributions = createAction(tr("质量贡献"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionParticipationFactors = createAction(tr("参与因子"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionResponseSpectrum = createAction(tr("响应谱"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionBuckling = createAction(tr("屈曲缺陷"), ":/icon/res/icon/icon_app_help.png");
    menuPhysic->addAction(actionMassProperties);
    menuPhysic->addAction(actionMassContributions);
    menuPhysic->addAction(actionParticipationFactors);
    menuPhysic->addAction(actionResponseSpectrum);
    menuPhysic->addAction(actionBuckling);  
    
    actionPhysic->setMenu(menuPhysic);
    panelPhysic->addAction(actionPhysic);


    //6. 物理场实用程序 panel
    SARibbonPanel* panelCoupling = page->addPanel(tr("耦合"));

    SARibbonMenu* menuNonlocalCoupling = new SARibbonMenu(this);
    QAction* actionNonlocalCoupling = createAction(tr("非局部耦合"), ":/icon/res/icon/icon_app_help.png");

    QAction* actionIntegration = createAction(tr("积分"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionAverage = createAction(tr("平均值"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionMax = createAction(tr("最大值"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionMin = createAction(tr("最小值"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionGenaralExtrusion = createAction(tr("广义拉伸"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionLinearExtrusion = createAction(tr("线性拉伸"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionBoundarySim = createAction(tr("边界相似性"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionIdentityMapping = createAction(tr("一致映射"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionGeneralProjection = createAction(tr("广义投影"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionLinearProjection = createAction(tr("线性投影"), ":/icon/res/icon/icon_app_help.png");

    menuNonlocalCoupling->addAction(actionIntegration);
    menuNonlocalCoupling->addAction(actionAverage);
    menuNonlocalCoupling->addAction(actionMax);
    menuNonlocalCoupling->addAction(actionMin);
    menuNonlocalCoupling->addAction(actionGenaralExtrusion);
    menuNonlocalCoupling->addAction(actionLinearExtrusion);
    menuNonlocalCoupling->addAction(actionBoundarySim);
    menuNonlocalCoupling->addAction(actionIdentityMapping);
    menuNonlocalCoupling->addAction(actionGeneralProjection);
    menuNonlocalCoupling->addAction(actionLinearProjection);
    actionNonlocalCoupling->setMenu(menuNonlocalCoupling);
    panelCoupling->addAction(actionNonlocalCoupling);

    SARibbonMenu* menuPairs = new SARibbonMenu(this);
    QAction* actionPairs = createAction(tr("对"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionContactPair = createAction(tr("接触对"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionIdentityBoundaryPair= createAction(tr("一致边界对"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionIdentityEdgeyPair = createAction(tr("一致边对"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionIdentityPointPair = createAction(tr("一致点对"), ":/icon/res/icon/icon_app_help.png");
    menuPairs->addAction(actionContactPair);
    menuPairs->addAction(actionIdentityBoundaryPair);
    menuPairs->addAction(actionIdentityEdgeyPair);
    menuPairs->addAction(actionIdentityPointPair);
    actionPairs->setMenu(menuPairs);
    panelCoupling->addAction(actionPairs);


    //6. 坐标系 panel
    SARibbonPanel* panelCoordinateSys = page->addPanel(tr("坐标系"));
    QStringList coordinateSysNames = {
        tr("基矢坐标系"), tr("组合坐标系"), tr("映射坐标系"),
        tr("缩放坐标系"), tr("边界坐标系"), tr("旋转坐标系"),
        tr("柱坐标系"), tr("球坐标系"), tr("几何中的坐标系"),
        tr("复合系统")
    };
    QStringList coordinateSysIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    SARibbonMenu* menuCoordinateSys = new SARibbonMenu(this);
    QAction* actionCoordinateSys = createAction(tr("坐标系"), ":/icon/res/icon/icon_app_help.png");
    for(int i = 0; i < coordinateSysNames.size(); ++i) {
        QAction* action = createAction(coordinateSysNames[i], ":/icon/res/icon/icon_app_help.png");
        menuCoordinateSys->addAction(action);
    }
    actionCoordinateSys->setMenu(menuCoordinateSys);
    panelCoordinateSys->addAction(actionCoordinateSys);

    QAction* actionPerfectMatchLayer = createAction(tr("完美匹配层"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionInfiniteEleDomain = createAction(tr("无限元域"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionAbsorbingLayer = createAction(tr("吸收层"), ":/icon/res/icon/icon_app_help.png");
    panelCoordinateSys->addSmallAction(actionPerfectMatchLayer);
    panelCoordinateSys->addSmallAction(actionInfiniteEleDomain);
    panelCoordinateSys->addSmallAction(actionAbsorbingLayer);


    //7. 变形网格 panel
    SARibbonPanel* panelDeformedMesh = page->addPanel(tr("变形网格"));

    QStringList movingMeshNames = {
        tr("指定变形"), tr("旋转域"), tr("变形域"),
        tr("固定边界"), tr("指定网格位移"), tr("指定法向网格位移"),
        tr("指定法向网格速度"), tr("旋转边界"), tr("网格滑移"),
        tr("对称")
    };
    QStringList movingMeshIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    SARibbonMenu* menuMovingMesh = new SARibbonMenu(this);
    QAction* actionMovingMesh = createAction(tr("变形网格"), ":/icon/res/icon/icon_app_help.png");
    for(int i = 0; i < movingMeshNames.size(); ++i) {
        QAction* action = createAction(movingMeshNames[i], ":/icon/res/icon/icon_app_help.png");
        menuMovingMesh->addAction(action);
    }
    actionMovingMesh->setMenu(menuMovingMesh);
    panelDeformedMesh->addAction(actionMovingMesh);


    SARibbonMenu* menuDeformedGeometry = new SARibbonMenu(this);
    QAction* actionDeformedGeometry = createAction(tr("变形几何"), ":/icon/res/icon/icon_app_help.png");
    for(int i = 0; i < movingMeshNames.size(); ++i) {
        QAction* action = createAction(movingMeshNames[i], ":/icon/res/icon/icon_app_help.png");
        menuDeformedGeometry->addAction(action);
    }
    actionDeformedGeometry->setMenu(menuDeformedGeometry);
    panelDeformedMesh->addAction(actionDeformedGeometry);


    //8. 优化 panel
    SARibbonPanel* panelOptimization = page->addPanel(tr("优化"));

    QStringList optimizationNames = {
        tr("密度模型"), tr("制动空域"), tr("指定材料"),
        tr("指定密度"), tr("指定空域边界"), tr("指定材料边界"),
        tr("指定密度边界"), tr("自由形状域"), tr("自由形状边界"),
        tr("自由形状壳")
    };
    QStringList optimizationIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    SARibbonMenu* menuOptimization = new SARibbonMenu(this);
    QAction* actionOptimization = createAction(tr("优化"), ":/icon/res/icon/icon_app_help.png");
    for(int i = 0; i < optimizationNames.size(); ++i) {
        QAction* action = createAction(optimizationNames[i], ":/icon/res/icon/icon_app_help.png");
        menuOptimization->addAction(action);
    }
    actionOptimization->setMenu(menuOptimization);
    panelOptimization->addAction(actionOptimization);


    //8. 优化 panel
    SARibbonPanel* panelView = page->addPanel(tr("视图"));
    QAction* actionView = createAction(tr("视图"), ":/icon/res/icon/icon_app_help.png");
    panelView->addAction(actionView);

}

/**
 * @brief 创建几何页面内容
 * @param page 定义几何页面指针
 */
void MainWindow::createCategoryGeometry(SARibbonCategory *page)
{
    //1. 构建 panel
    SARibbonPanel* panelBuild = page->addPanel(tr("构建"));
    QAction* actionBuildAll = createAction(tr("全部构建"), ":/icon/res/icon/icon_app_help.png");
    panelBuild->addAction(actionBuildAll);

    //2. 导入/导出 panel
    SARibbonPanel* panelImportExport = page->addPanel(tr("导入/导出"));
    QAction* actionImport = createAction(tr("导入"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionInsertSeq = createAction(tr("插入序列"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionExport = createAction(tr("导出"), ":/icon/res/icon/icon_app_help.png");

    panelImportExport->addSmallAction(actionImport);
    panelImportExport->addSmallAction(actionInsertSeq);
    panelImportExport->addSmallAction(actionExport);



    //3. 清理 panel
    SARibbonPanel* panelCleanup = page->addPanel(tr("清理"));

    QStringList cleanupNames = {
        tr("端盖面"), tr("检测干涉"), tr("删除圆角"),
        tr("删除孔"), tr("删除短边"), tr("删除长短面"),
        tr("删除小面"), tr("删除尖峰"), tr("删除面"),
        tr("分离面")
    };
    QStringList cleanupIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    SARibbonMenu* menuDefeaturingRepair = new SARibbonMenu(this);
    QAction* actionDefeaturingRepair = createAction(tr("特征去除和修复"), ":/icon/res/icon/icon_app_help.png");
    for(int i = 0; i < cleanupNames.size(); ++i) {
        QAction* action = createAction(cleanupNames[i], ":/icon/res/icon/icon_app_help.png");
        menuDefeaturingRepair->addAction(action);
    }
    actionDefeaturingRepair->setMenu(menuDefeaturingRepair);
    panelCleanup->addAction(actionDefeaturingRepair);

    panelCleanup->addSeparator();
    QAction* actionRemoveDetail = createAction(tr("移除细节"), ":/icon/res/icon/icon_app_help.png");
    panelCleanup->addAction(actionRemoveDetail);


    QStringList virtualOpNames = {
        tr("忽略顶点"), tr("忽略边"), tr("忽略面"),
        tr("形成复合边"), tr("形成复合面"), tr("形成复合域"),
        tr("塌陷边"), tr("塌陷面"), tr("塌陷面区域"),
        tr("合并顶点")
    };
    QStringList virtualOpIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    SARibbonMenu* menuVirtualOp = new SARibbonMenu(this);
    QAction* actionVirtualOp = createAction(tr("特征去除和修复"), ":/icon/res/icon/icon_app_help.png");
    for(int i = 0; i < virtualOpNames.size(); ++i) {
        QAction* action = createAction(virtualOpNames[i], ":/icon/res/icon/icon_app_help.png");
        menuVirtualOp->addAction(action);
    }
    actionVirtualOp->setMenu(menuVirtualOp);
    panelCleanup->addAction(actionVirtualOp);

    //4. 体素 panel
    SARibbonPanel* panelPrimitives = page->addPanel(tr("体素"));

    QStringList primitiveNames = {
        tr("长方体"), tr("圆锥体"), tr("圆柱体"),
        tr("球体"), tr("圆环"), tr("螺旋")
    };
    QStringList primitiveIcons = {
        ":/icon/res/icon/icon_app_help.png"
    };
    for(int i = 0; i < primitiveNames.size(); ++i) {
        QAction* action = createAction(primitiveNames[i], ":/icon/res/icon/icon_app_help.png");
        panelPrimitives->addSmallAction(action);
    }
    QAction* actionMorePrimitive = createAction(tr("特征去除和修复"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuMorePrimitive = new SARibbonMenu(this);

    QStringList morePrimitiveNames = {
        tr("三次贝塞尔曲线"), tr("偏心圆锥"), tr("椭球"),
        tr("六面体"), tr("插值曲线"), tr("线端"),
        tr("参数化曲线"), tr("参数化曲面"), tr("点"),
        tr("多边形"), tr("二次贝塞尔曲线"), tr("四面体")
    };
    for(int i = 0; i < morePrimitiveNames.size(); ++i) {
        QAction* action = createAction(morePrimitiveNames[i], ":/icon/res/icon/icon_app_help.png");
        menuMorePrimitive->addAction(action);
    }

    actionMorePrimitive->setMenu(menuMorePrimitive);
    panelPrimitives->addAction(actionMorePrimitive);


    //5. 工作平面 panel
    SARibbonPanel* panelWorkPlane = page->addPanel(tr("工作平面"));

    QAction* actionSelectPlane = createAction(tr("选择工作平面"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuSelectPlanee = new SARibbonMenu(this);

    actionSelectPlane->setEnabled(false);
    actionSelectPlane->setMenu(menuSelectPlanee);

    panelWorkPlane->addAction(actionSelectPlane);

    QAction* actionWorkPlane = createAction(tr("工作平面"), ":/icon/res/icon/icon_app_help.png");
    panelWorkPlane->addAction(actionWorkPlane);

    //6. 操作 panel
    SARibbonPanel* panelOperations = page->addPanel(tr("操作"));

    QAction* actionExtrude = createAction(tr("拉伸"), ":/icon/res/icon/icon_app_help.png");
    panelOperations->addAction(actionExtrude);

    QAction* actionRevolve = createAction(tr("回转"), ":/icon/res/icon/icon_app_help.png");
    panelOperations->addSmallAction(actionRevolve);
    QAction* actionSweep = createAction(tr("扫掠"), ":/icon/res/icon/icon_app_help.png");
    panelOperations->addSmallAction(actionSweep);
    QAction* actionLoft = createAction(tr("放样"), ":/icon/res/icon/icon_app_help.png");
    panelOperations->addSmallAction(actionLoft);
    panelOperations->addSeparator();

    QAction* actionBooleanPartition = createAction(tr("布尔操作和分割"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuBooleanPartition = new SARibbonMenu(this);

    QStringList booleanPartitionNames = {
        tr("并集"), tr("交集"), tr("求差"),
        tr("组合"), tr("分割对象"), tr("分割域"),
        tr("分割面"), tr("分割边")
    };
    for(int i = 0; i < booleanPartitionNames.size(); ++i) {
        QAction* action = createAction(booleanPartitionNames[i], ":/icon/res/icon/icon_app_help.png");
        menuBooleanPartition->addAction(action);
    }
    actionBooleanPartition->setMenu(menuBooleanPartition);
    panelOperations->addAction(actionBooleanPartition);


    QAction* actionTransforms = createAction(tr("变换"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuTransforms  = new SARibbonMenu(this);

    QStringList transformsNames = {
        tr("刚性变换"), tr("复制"), tr("旋转"),
        tr("比例因子"), tr("移动"), tr("镜像"),
        tr("阵列"), tr("分割边")
    };
    for(int i = 0; i < transformsNames.size(); ++i) {
        QAction* action = createAction(transformsNames[i], ":/icon/res/icon/icon_app_help.png");
        menuTransforms->addAction(action);
    }
    actionTransforms->setMenu(menuTransforms);
    panelOperations->addAction(actionTransforms);

    QAction* actionConversions = createAction(tr("变换"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuConversions  = new SARibbonMenu(this);

    QStringList conversionNames = {
        tr("转换为实体"), tr("转换为表面"), tr("转换为曲线"),
        tr("转换为点"), tr("转换为..对象"), tr("抽取中面"),
        tr("拆分"), tr("加厚")
    };
    for(int i = 0; i < conversionNames.size(); ++i) {
        QAction* action = createAction(conversionNames[i], ":/icon/res/icon/icon_app_help.png");
        menuConversions->addAction(action);
    }
    actionConversions->setMenu(menuConversions);
    panelOperations->addAction(actionConversions);

    QAction* actionChamfer = createAction(tr("倒斜角"), ":/icon/res/icon/icon_app_help.png");
    panelOperations->addSmallAction(actionChamfer);
    QAction* actionFillet = createAction(tr("倒圆角"), ":/icon/res/icon/icon_app_help.png");
    panelOperations->addSmallAction(actionFillet);
    QAction* actionDelete = createAction(tr("删除"), ":/icon/res/icon/icon_app_help.png");
    panelOperations->addSmallAction(actionDelete);

    //7. 操作 panel
    SARibbonPanel* panelOther = page->addPanel(tr("其他"));

    QAction* actionParts = createAction(tr("零件"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuParts  = new SARibbonMenu(this);
    QAction* actionCreatePart = createAction(tr("创建零件"), ":/icon/res/icon/icon_app_help.png");
    menuParts->addAction(actionCreatePart);
    QAction* actionLoadPart = createAction(tr("加载零件"), ":/icon/res/icon/icon_app_help.png");
    menuParts->addAction(actionLoadPart);
    QAction* actionPartLib = createAction(tr("零件库"), ":/icon/res/icon/icon_app_help.png");
    menuParts->addAction(actionPartLib);

    actionParts->setMenu(menuParts);
    panelOther->addAction(actionParts);


    QAction* actionSelections = createAction(tr("选择"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuSelections  = new SARibbonMenu(this);

    QStringList selectionsNames = {
        tr("显式选择"), tr("球选择"), tr("框选择"),
        tr("圆柱体选择"), tr("并集选择"), tr("交集选择"),
        tr("差集选择"), tr("补集选择"), tr("相邻选择"),
        tr("累积选择")
    };
    for(int i = 0; i < selectionsNames.size(); ++i) {
        QAction* action = createAction(selectionsNames[i], ":/icon/res/icon/icon_app_help.png");
        menuSelections->addAction(action);
    }
    actionSelections->setMenu(menuSelections);
    panelOther->addAction(actionSelections);

    QAction* actionColor = createAction(tr("颜色"), ":/icon/res/icon/icon_app_help.png");
    SARibbonMenu* menuColor = new SARibbonMenu(this);
    QAction* actionChoseColor = createAction(tr("颜色选择"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionResetColor = createAction(tr("根据主题重置颜色"), ":/icon/res/icon/icon_app_help.png");
    QAction* actionDeletColor = createAction(tr("移除选择颜色"), ":/icon/res/icon/icon_app_help.png");
    menuColor->addAction(actionChoseColor);
    menuColor->addAction(actionResetColor);
    menuColor->addAction(actionDeletColor);
    actionColor->setMenu(menuColor);
    panelOther->addAction(actionColor);


}

void MainWindow::createCategoryMaterial(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryPhysic(SARibbonCategory *page)
{
    
}

void MainWindow::createCategoryMesh(SARibbonCategory *page)
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
