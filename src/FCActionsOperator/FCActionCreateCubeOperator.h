#ifndef FCACTIONCREATECUBEOPERATOR_H
#define FCACTIONCREATECUBEOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"

namespace FC
{
class FCGeoModelBox;
/**
 * @brief 创建立方体操作器（COMSOL 式：使用 DAG addBlock）
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
    /** 本次 execProfession 创建的 DAG 节点 ID（COMSOL 式） */
    int _currentNodeId{ -1 };
    /** 兼容旧版：Box 命令（当前以 DAG 为主，此项多为 nullptr） */
    FCGeoModelBox* _currentBoxCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATECUBEOPERATOR_H
