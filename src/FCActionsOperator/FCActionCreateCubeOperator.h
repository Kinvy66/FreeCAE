#ifndef FCACTIONCREATECUBEOPERATOR_H
#define FCACTIONCREATECUBEOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"

namespace FC 
{
class FCGeoModelBox;
/**
 * @brief 创建立方体操作器
 */
class FCACTIONSOPERATOR_API FCActionCreateCubeOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCActionCreateCubeOperator();
    ~FCActionCreateCubeOperator() override = default;

    bool execGUI() override;
    bool execProfession() override;

private:
    /** 本次 execProfession 创建的 Box 命令，供 execGUI 绑定到 FCCubeInfoWidget */
    FCGeoModelBox* _currentBoxCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATECUBEOPERATOR_H
