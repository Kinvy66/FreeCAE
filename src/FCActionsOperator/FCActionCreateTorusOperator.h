/**
 * @file FCActionCreateTorusOperator.h
 * @brief 创建圆环操作器
 */
#ifndef FCACTIONCREATETORUSOPERATOR_H
#define FCACTIONCREATETORUSOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"
#include <FCData/FCType.h>

namespace FC
{
class FCGeoModelTorus;

class FCACTIONSOPERATOR_API FCActionCreateTorusOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCActionCreateTorusOperator();
    ~FCActionCreateTorusOperator() override = default;

    bool execGUI() override;
    bool execProfession() override;

private:
    FCID mCurrentNodeId{ FCID_INVALID };
    FCGeoModelTorus* mCurrentTorusCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATETORUSOPERATOR_H
