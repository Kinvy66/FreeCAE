/**
 * @file FCActionEventHandler.h
 * @brief action点击事件处理
 * @date 2026-01-29
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCACTIONEVENTHANDLER_H
#define FCACTIONEVENTHANDLER_H

#include <QObject>

namespace FC 
{

class FCUIInterface;
class FCActionOperator;

/** 通过注入 FCUIInterface，操作器可统一访问主窗口与 Docking，无需 APP 多重 setX。 */
class FCActionEventHandler : public QObject
{
public:
    FCActionEventHandler() = default;
    ~FCActionEventHandler() = default;

    /** 设置 UI 上下文，触发操作时会注入到 Operator（setUIInterface），供其访问 mainWindow/dockingArea 等 */
    void setUIInterface(FCUIInterface* ui) { m_uiInterface = ui; }
    FCUIInterface* uiInterface() const { return m_uiInterface; }
    
public slots:
    
    /**
      * @brief 执行处理操作
      */
    void execOperator();
    
private:
    FCActionOperator* getOperator(QObject* object);

    FCUIInterface* m_uiInterface{ nullptr };
};
} // namespace FC



#endif // FCACTIONEVENTHANDLER_H
