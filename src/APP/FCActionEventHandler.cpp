/**
 * @file FCActionEventHandler.cpp
 * @brief action点击事件处理
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCActionEventHandler.h"
#include <FCUIInterface.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <FCActionsOperator/FCOperatorRepo.h>
#include <QAction>
#include <QDebug>
#include "FCActionOperator.h"

#define FCAPPCONTROLLER_PASS()                                                                                         \
QMessageBox::                                                                                                      \
    warning((nullptr),                                                           \
            QCoreApplication::translate("FCAppRibbonArea", "warning", nullptr),                                    \
            QCoreApplication::translate("FCAppRibbonArea",                                                         \
                                        "The current function is not implemented, only the UI is reserved, "       \
                                        "please pay attention: https://github.com/Kinvy66/FreeCAE",         \
                                        nullptr))


namespace FC 
{

void FCActionEventHandler::execOperator()
{
    QObject* obj = this->sender();
    if (obj == nullptr) return;
    
    QAction* action = qobject_cast<QAction*>(obj);
    if (action == nullptr) return;
    
    QString objName = action->objectName();
    if (objName.isEmpty()) return;
    
    // 获取操作器
    FCActionOperator* actOper = this->getOperator(action);
    if (actOper != nullptr) {
        actOper->setEmitter(action);
        actOper->setUIInterface(m_uiInterface);
        actOper->actionTriggered();
    } else {
        // 错误提示
        qWarning() << "FCActionEventHandler: Operator not found for action:" << objName;
        FCAPPCONTROLLER_PASS();
    }
}

FCActionOperator* FCActionEventHandler::getOperator(QObject* object)
{
    QAction* action = qobject_cast<QAction*>(object);
    if (action == nullptr) return nullptr;
    
    QString objName = action->objectName();
    if (objName.isEmpty()) return nullptr;
    
    // 从操作器仓库获取操作器
    FCOperatorRepo* repo = FCOPERATORREPO;
    if (!repo) return nullptr;
    
    FCActionOperator* oper = repo->getOperatorT<FCActionOperator>(objName);
    return oper;
}

} // namespace FC
