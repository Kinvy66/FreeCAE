/**
 * @file FCActionCreateConeOperator.h
 * @brief 创建圆锥/圆台操作器
 */
#ifndef FCACTIONCREATECONEOPERATOR_H
#define FCACTIONCREATECONEOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"
#include <FCData/FCType.h>

namespace FC
{
class FCGeoModelCone;

class FCACTIONSOPERATOR_API FCActionCreateConeOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCActionCreateConeOperator();
    ~FCActionCreateConeOperator() override = default;

    bool execGUI() override;
    bool execProfession() override;

private:
    FCID mCurrentNodeId{ FCID_INVALID };
    FCGeoModelCone* mCurrentConeCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATECONEOPERATOR_H
