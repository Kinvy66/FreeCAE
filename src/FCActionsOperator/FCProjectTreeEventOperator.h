/**
 * @file FCProjectTreeEventOperator.h
 * @brief 工程树操作器
 * @date 2026-02-24
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCPROJECTTREEEVENTOPERATOR_H
#define FCPROJECTTREEEVENTOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCTreeEventOperator.h"
#include "FCOperatorRepo.h"

namespace FC 
{

class FCACTIONSOPERATOR_API FCProjectTreeEventOperator : public FCTreeEventOperator
{
    Q_OBJECT
public:
    explicit FCProjectTreeEventOperator();
    ~FCProjectTreeEventOperator();
    
    /**
     * @brief 更新整个工程树
     */
    void updateTree() override;
};



} // namespace FC


#endif // FCPROJECTTREEEVENTOPERATOR_H
