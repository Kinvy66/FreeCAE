/**
 * @file FCAppUI.cpp
 * @brief 总体界面接口，负责生成FCAppDockingArea和FCAppRibbonArea
 * @date 2025-10-28
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAppUI.h"
#include "FCAppDockingArea.h"
#include "FCAppRibbonArea.h"
#include "FCAppActions.h"
#include "FCAppCommand.h"
#include "FCAppCore.h"
#include "FCActionEventHandler.h"
// #include "FCAppDataManager.h"
#include "AppMainWindow.h"
#include "FCStatusBar.h"
//===================================================
// using FC namespace -- 禁止在头文件using！！
//===================================================

using namespace FC;


//===================================================
// FCAppUI
//===================================================
FCAppUI::FCAppUI(SARibbonMainWindow* m, FCCoreInterface* c) : FCUIInterface(m, c)
{
    //! 这里不进行createUi的调用，因为很多地方的窗口的构建需要FCAppActions，
    //! 而FCAppActions又依赖FCAppUI，在FCAppCore构建FCAppUI时，如果在FCAppUI的构造函数中调用createUi
    //! 那么会导致FCAppUI构造过程中调用createDockingArea，而createDockingArea是创建窗口的主要函数，
    //! 很多窗口的创建又依赖FCAppActions，虽然FCAppActions已经创建，但如果把createUi放到FCAppUI构造函数中，
    //! 此时FCAppUI还未构造完成，FCAppUI未构造完成就导致FCAppCore还无法持有FCAppUI指针，
    //! 那么createDockingArea构造各种窗口时就无法通过FC_APP_UI_ACTIONS宏（FC::FCAppCore::getInstance().getUi()->getActions()）来获取action
    //!
    //! 因此createUi要等FCAppCore持有FCAppUI指针后再调用
    //!
}

/**
 * @brief 获取主程序
 * @return 
 */
QMainWindow* FCAppUI::getMainWindow() const
{
    return static_cast< QMainWindow* >(mRibbonArea->app());
}

/**
 * @brief 获取界面的docking区域
 * @return 
 */
FCDockingAreaInterface* FCAppUI::getDockingArea()
{
    return mDockingArea;
}

/**
 * @brief 获取界面的ribbon区域
 * @return 
 */
FCRibbonAreaInterface* FCAppUI::getRibbonArea()
{
    return mRibbonArea;
}

/**
 * @brief 创建ui
 */
void FCAppUI::createUi()
{
    createCmd();      // cmd必须先创建，因为Actions会用到cmd的
    createActions();  // Actions第二个创建
    mActionHandler = new FCActionEventHandler();
    createDockingArea();
    createRibbonArea();
    mRibbonArea->setDockingArea(mDockingArea);
    if (mActionHandler)
        mActionHandler->setUIInterface(this);
    createStatusBar();
}

/**
 * @brief 获取app core
 * @return
 */
FCAppCore* FCAppUI::getAppCore()
{
    return qobject_cast< FCAppCore* >(core());
}

/**
 * @brief 获取app actions, 减少一次dynamic_cast
 * @return 
 */
FCAppActions* FCAppUI::getAppActions()
{
    return mActions;
}

FCActionEventHandler *FCAppUI::getActionHandler()
{
    return mActionHandler;
}

/**
 * @brief 获取app cmd, 减少一次dynamic_cast
 * @return 
 */
FCAppCommand* FCAppUI::getAppCmd()
{
    return mCmd;
}

/**
 * @brief //获取dock
 * @return 
 */
FCAppDockingArea* FCAppUI::getAppDockingArea()
{
    return mDockingArea;
}

/**
 * @brief 获取ribbon
 * @return 
 */
FCAppRibbonArea* FCAppUI::getAppRibbonArea()
{
    return mRibbonArea;
}

/**
 * @brief 获取状态栏
 * @return 
 */
FCStatusBar *FCAppUI::getStautsBar()
{
    return mStatusBar;
}

/**
 * @brief 创建action
 */
void FCAppUI::createActions()
{
    mActions = new FCAppActions(this);
    // m_actions->retranslateUi();  // 显示调用文字翻译
    registeAction(mActions);
}

/**
 * @brief 创建cmd
 */
void FCAppUI::createCmd()
{
    mCmd = new FCAppCommand(this);
    registeCommand(mCmd);
}

/**
 * @brief 创建docking 区域
 */
void FCAppUI::createDockingArea()
{
    mDockingArea = new FCAppDockingArea(this);
    registeExtend(mDockingArea);
}

/**
 * @brief 创建ribbon 区域
 */
void FCAppUI::createRibbonArea()
{
    mRibbonArea = new FCAppRibbonArea(this);
    registeExtend(mRibbonArea);
}

/**
 * @brief 创建statusBar
 */
void FCAppUI::createStatusBar()
{
    QMainWindow* m = getMainWindow();
    mStatusBar     = new FCStatusBar(m);
    m->setStatusBar(mStatusBar);
}
