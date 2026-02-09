#ifndef FCACTIONCREATECUBEOPERATOR_H
#define FCACTIONCREATECUBEOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"
#include "FCOperatorRepo.h"

namespace FC 
{
/**
 * @brief 创建立方体操作器
 */
class FCACTIONSOPERATOR_API FCActionCreateCubeOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCActionCreateCubeOperator();
    ~FCActionCreateCubeOperator() override = default;

    /**
     * @brief 界面逻辑，生成UI交互
     * @return true 执行成功
     * @return false 执行失败
     */
    bool execGUI() override;

    /**
     * @brief 业务处理逻辑，在execGUI后执行
     * @return true 执行成功
     * @return false 执行失败
     */
    bool execProfession() override;
};

// 注册操作器
Register2FCOperatorRepo(actionCreateCUbe, FCActionCreateCubeOperator);

} // namespace FC

#endif // FCACTIONCREATECUBEOPERATOR_H
