/**
 * @file FCActionCreateCylinderOperator.h
 * @brief 创建圆柱操作器（与立方体一致：DAG addCylinder，构建在手动「构建」时执行）
 */
#ifndef FCACTIONCREATECYLINDEROPERATOR_H
#define FCACTIONCREATECYLINDEROPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"
#include <FCData/FCType.h>

namespace FC
{
class FCGeoModelCylinder;

class FCACTIONSOPERATOR_API FCActionCreateCylinderOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCActionCreateCylinderOperator();
    ~FCActionCreateCylinderOperator() override = default;

    bool execGUI() override;
    bool execProfession() override;

private:
    FCID mCurrentNodeId{ FCID_INVALID };
    FCGeoModelCylinder* mCurrentCylinderCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATECYLINDEROPERATOR_H
