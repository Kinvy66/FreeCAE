#include "FCActionOperator.h"

namespace FC 
{

FCActionOperator::FCActionOperator()
{
}

FCActionOperator::~FCActionOperator()
{
}

bool FCActionOperator::actionTriggered()
{
    // 执行预处理
    this->preArgs();
    // 执行具体业务逻辑
    bool OK = this->execProfession();
    if (!OK) return false;
    // UI相关操作
    return this->execGUI();
}

bool FCActionOperator::execGUI()
{
    return true;
}

bool FCActionOperator::execProfession()
{
    return true;
}

void FCActionOperator::preArgs()
{
}

} // namespace FC
