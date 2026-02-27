/**
 * @file FCActionCreateSphereOperator.h
 * @brief 创建球体操作器（与立方体一致：DAG addSphere，构建在手动「构建」时执行）
 */
#ifndef FCACTIONCREATESPHEREOPERATOR_H
#define FCACTIONCREATESPHEREOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"
#include <FCData/FCType.h>

namespace FC
{
class FCGeoModelSphere;

class FCACTIONSOPERATOR_API FCActionCreateSphereOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCActionCreateSphereOperator();
    ~FCActionCreateSphereOperator() override = default;

    bool execGUI() override;
    bool execProfession() override;

private:
    FCID mCurrentNodeId{ FCID_INVALID };
    FCGeoModelSphere* mCurrentSphereCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATESPHEREOPERATOR_H
