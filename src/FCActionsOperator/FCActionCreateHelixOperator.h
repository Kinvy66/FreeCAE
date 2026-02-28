/**
 * @file FCActionCreateHelixOperator.h
 * @brief 创建螺旋体操作器
 */
#ifndef FCACTIONCREATEHELIXOPERATOR_H
#define FCACTIONCREATEHELIXOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"
#include <FCData/FCType.h>

namespace FC
{
class FCGeoModelHelix;

class FCACTIONSOPERATOR_API FCActionCreateHelixOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCActionCreateHelixOperator();
    ~FCActionCreateHelixOperator() override = default;

    bool execGUI() override;
    bool execProfession() override;

private:
    FCID mCurrentNodeId{ FCID_INVALID };
    FCGeoModelHelix* mCurrentHelixCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATEHELIXOPERATOR_H
