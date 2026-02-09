/**
 * @file FCActionEventHandler.cpp
 * @brief action点击事件处理
 * @date 2026-02-04
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCActionEventHandler.h"

#include <QMessageBox>
#include <QCoreApplication>

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
    
    FCAPPCONTROLLER_PASS();
    
  
    QObject* obj = this->sender();
    if (obj == nullptr) return;
    QString objName = obj->objectName();
    
    // TODO
    
    // Core::FITKActionOperator* actOper = this->getOperator(obj);
    // if (actOper != nullptr) actOper->setEmitter(obj);
    
    // if (actOper == nullptr)
    // {
    //     //错误提示
    //     QString error = QString("%1 Operator create failed !").arg(objName);
    //     AppFrame::FITKMessageError(error);
    // }
    // else
    //     actOper->actionTriggered();

}

FCActionOperator *FCActionEventHandler::getOperator(QObject *object)
{    
    return nullptr;
}

} // namespace FC
