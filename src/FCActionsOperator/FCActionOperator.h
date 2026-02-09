#ifndef FCACTIONOPERATOR_H
#define FCACTIONOPERATOR_H
#include "FCActionsOperatorAPI.h"
#include "FCAbstractOperator.h"

namespace FC 
{
class FCACTIONSOPERATOR_API FCActionOperator : public FCAbstractOperator
{
    Q_OBJECT
public:
    FCActionOperator();
    virtual ~FCActionOperator() = 0;

    /**
     * @brief 点击事件处理方法
     * @return true 执行成功
     * @return false 执行失败
     */
    virtual bool actionTriggered();

    /**
     * @brief 界面逻辑，生成UI交互
     * @return true 执行成功
     * @return false 执行失败
     */
    virtual bool execGUI();

    /**
     * @brief 业务处理逻辑，在execGUI后执行
     * @return true 执行成功
     * @return false 执行失败
     */
    virtual bool execProfession();

protected:
    /**
     * @brief 参数预处理
     */
    virtual void preArgs();
};
} // namespace FC

#endif // FCACTIONOPERATOR_H
