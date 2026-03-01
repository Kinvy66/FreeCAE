/**
 * @file FCAppUI.h
 * @brief 总体界面接口，负责生成FCAppDockingArea和FCAppRibbonArea
 * @date 2025-10-28
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCAPPUI_H
#define FCAPPUI_H

#include <FCUIInterface.h>

namespace FC
{
class FCAppCore;
class FCAppCommand;
class FCAppActions;
class FCActionEventHandler;
class FCAppDockingArea;
class FCAppRibbonArea;
class FCAppEventBus;
class FCStatusBar;


class FCAppUI : public FCUIInterface
{
    Q_OBJECT
public:
    FCAppUI(SARibbonMainWindow* m, FCCoreInterface* c);
    
    //获取主程序
    virtual QMainWindow* getMainWindow() const override;
    
    //获取界面的docking区域
    virtual FCDockingAreaInterface* getDockingArea() override;
    
    //获取界面的ribbon区域
    virtual FCRibbonAreaInterface* getRibbonArea() override;

    virtual FCIEventBus* getEventBus() const override;

    //创建ui
    void createUi();
    
    
public:
    //获取core
    FCAppCore* getAppCore();
    //减少一次dynamic_cast
    FCAppActions* getAppActions();
    
    FCActionEventHandler* getActionHandler();
    
    //减少一次dynamic_cast
    FCAppCommand* getAppCmd();
    //获取dock
    FCAppDockingArea* getAppDockingArea();
    //获取ribbon
    FCAppRibbonArea* getAppRibbonArea();
    
    FCStatusBar* getStautsBar();
    
protected:
    void createActions();
    void createCmd();
    void createDockingArea();
    void createRibbonArea();
    void createStatusBar();
    
public:
    FCAppActions* mActions;

    FCAppCommand* mCmd;
    FCAppEventBus* mEventBus{ nullptr };
    FCAppDockingArea* mDockingArea;
    FCAppRibbonArea* mRibbonArea;
    FCStatusBar* mStatusBar;
    FCActionEventHandler* mActionHandler;
};

} // namespace FC

#ifndef FC_APP_UI
/**
 * @def 获取@sa FCAppCore 实例
 * @note 使用此宏需要以下头文件：
 * -# FCAppCore.h
 */
#define FC_APP_UI FC::FCAppCore::getInstance().getUi()
#endif

#ifndef FC_APP_EVENT_BUS
/** @def 获取应用事件总线，供操作器/窗口 emit 与 subscribe，实现多窗口联动 */
#define FC_APP_EVENT_BUS (FC::FCAppCore::getInstance().getUi()->getEventBus())
#endif


#endif // FCAPPUI_H
