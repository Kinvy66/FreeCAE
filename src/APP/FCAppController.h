/**
 * @file FCAppController.h
 * @brief 控制层负责逻辑的对接
 * @date 2025-10-29
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCAPPCONTROLLER_H
#define FCAPPCONTROLLER_H

#include <QObject>
#include <QAction>
#include <QUndoStack>
#include <QScopedPointer>


class QComboBox;
class QToolBar;
class QMenuBar;
class QFontComboBox;
class QUndoStack;
class QGraphicsItem;
// qwt
class QwtPlotItem;
// Qt-Advanced-Docking-System 前置申明
namespace ads
{
class CDockWidget;
}

namespace FC 
{
class AppMainWindow;
class FCAppCore;
class FCProjectInterface;
class FCAppRibbonArea;
class FCAppDockingArea;
class FCAppCommand;
class FCAppActions;
class FCAppDataManager;
class FCActionEventHandler;

class FCAppController : public QObject
{
    Q_OBJECT
public:
    using IdType  = uint64_t;  ///< id类型
public:
    
    /**
	 * @brief 针对Operate窗口的最后焦点枚举，用于识别最后用户所在的操作窗口
	 */
    enum LastFocusedOpertateWidget
    {
        LastFocusedNoneOptWidget = 0x00,
        LastFocusedOnWorkflowOpt = 0x01,  ///< 最后焦点在工作流操作窗口
        LastFocusedOnDataOpt     = 0x02,  ///< 最后焦点在数据操作窗口
        LastFocusedOnChartOpt    = 0x04   ///< 最后焦点在绘图操作窗口
    };
    Q_DECLARE_FLAGS(LastFocusedOpertateWidgets, LastFocusedOpertateWidget)
    Q_FLAG(LastFocusedOpertateWidget)
    
public:
    FCAppController(QObject* par = nullptr);
    ~FCAppController();
    
    // 设置AppMainWindow
    FCAppController& setAppMainWindow(AppMainWindow* mainWindow);
    // 设置core
    FCAppController& setAppCore(FCAppCore* core);
    // 设置ribbon
    FCAppController& setAppRibbonArea(FCAppRibbonArea* ribbon);
    // 设置dock
    FCAppController& setAppDockingArea(FCAppDockingArea* dock);
    // 设置AppCommand
    FCAppController& setAppCommand(FCAppCommand* cmd);
    // 设置AppActions
    FCAppController& setAppActions(FCAppActions* act);
    // 设置action时间处理器
    FCAppController& setActionHandler(FCActionEventHandler* handler);
    // 设置AppDataManager
    FCAppController& setAppDataManager(FCAppDataManager* d);
    
    // 获取app
    AppMainWindow* app() const;
    // 初始化--必须初始化才能生效
    void initialize();
    
    
public:

    // 设置工程为dirty
    void setDirty(bool on = true);
    bool isDirty() const;
    // 导入数据
    // bool importData(const QString& filePath, const QVariantMap& args, QString* err = nullptr);
    // 更新窗口标题
    void updateWindowTitle();
    // 生成窗口标题
    static QString makeWindowTitle();
    static QString makeWindowTitle(FCProjectInterface* proj);
   
private slots:
    
    void testCreatorGeometry();
    
    void onFocusedDockWidgetChanged(ads::CDockWidget* old, ads::CDockWidget* now);    
    
private:
    // 初始化信号槽
    void initConnection();
    void registeActionsOperator();
  
private:
    AppMainWindow* mMainWindow { nullptr };
    FCAppCore* mCore { nullptr };
    FCProjectInterface* mProject { nullptr };
    
    FCAppRibbonArea* mRibbon { nullptr };
    FCAppDockingArea* mDock { nullptr };
    FCAppCommand* mCommand { nullptr };
    FCAppActions* mActions { nullptr };
    FCAppDataManager* mDatas { nullptr };
    
    FCActionEventHandler* mActionHandler;
    
    QStringList mFileReadFilters;  ///< 包含支持的文件[Images (*.png *.xpm *.jpg)] [Text files (*.txt)]
    //
    LastFocusedOpertateWidgets mLastFocusedOpertateWidget;  ///< 最后获取焦点的操作窗口
    //
    // DAAppSettingDialog* mSettingDialog { nullptr };         ///< 设置窗口
    // DAAppConfig* mConfig;                                   ///< 设置类
};
} // namespace FC



#endif // FCAPPCONTROLLER_H
