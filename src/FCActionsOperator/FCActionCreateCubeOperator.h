#ifndef FCACTIONCREATECUBEOPERATOR_H
#define FCACTIONCREATECUBEOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"
#include <QList>

namespace FC 
{
class FCGeoModelBox;
class FCAbsGeoCommand;
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

private slots:
    /** 属性页「构建」后：将单个几何体加入 VTK 渲染窗口（命令列表路径） */
    void onGeometryBuilt(FC::FCAbsGeoCommand* cmd);
    /** 属性页「构建所有」后：按序列刷新几何层（命令列表路径） */
    void onGeometrySequenceBuilt(const QList<FC::FCAbsGeoCommand*>& cmds);

private:
    /** 本次 execProfession 创建的 DAG 节点 ID（COMSOL 式） */
    int _currentNodeId{ -1 };
    /** 兼容旧版：Box 命令（当前以 DAG 为主，此项多为 nullptr） */
    FCGeoModelBox* _currentBoxCmd{ nullptr };
};

} // namespace FC

#endif // FCACTIONCREATECUBEOPERATOR_H
