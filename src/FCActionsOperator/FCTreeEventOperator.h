/**
 * @file FCTreeEventOperator.h
 * @brief 树形菜单事件接口类
 * @date 2026-02-24
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCTREEEVENTOPERATOR_H
#define FCTREEEVENTOPERATOR_H
#include "FCActionsOperatorAPI.h"
#include "FCAbstractOperator.h"

namespace FC 
{
class  FCACTIONSOPERATOR_API FCTreeEventOperator: public FCAbstractOperator
{
    Q_OBJECT
public:
    explicit FCTreeEventOperator();
    virtual ~FCTreeEventOperator();
    
    virtual void updateTree();
    
    
};
} // namespace FC


#endif // FCTREEEVENTOPERATOR_H
