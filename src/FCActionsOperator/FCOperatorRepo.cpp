#include "FCOperatorRepo.h"
#include "FCAbstractOperator.h"
#include <QMutexLocker>

namespace FC 
{

FCOperatorRepo* FCOperatorRepo::_instance = nullptr;

FCOperatorRepo* FCOperatorRepo::instance()
{
    static FCOperatorRepo s_instance;
    return &s_instance;
}

FCOperatorRepo::FCOperatorRepo()
    : QObject(nullptr)
{
}

FCOperatorRepo::~FCOperatorRepo()
{
    QMutexLocker locker(&_mutex);
    // 遍历释放全部存在的operator
    QList<FCAbstractOperator*> opers = _existOperators.values();
    for (auto oper : opers) {
        if (oper != nullptr) {
            delete oper;
        }
    }
    // 容器清空
    _existOperators.clear();
}

FCAbstractOperator* FCOperatorRepo::getOperator(const QString& key)
{
    QMutexLocker locker(&_mutex);
    // 如果已经存在则返回原来的operator
    if (_existOperators.contains(key)) {
        return _existOperators.value(key);
    }
    // 不存在则直接创建
    return this->createOperator(key);
}

void FCOperatorRepo::removeOperator(const QString& key, bool removeFun)
{
    QMutexLocker locker(&_mutex);
    // 已经创建的对象
    if (_existOperators.contains(key)) {
        FCAbstractOperator* oper = _existOperators.value(key);
        _existOperators.remove(key);
        if (oper != nullptr) {
            delete oper;
        }
    }
    // 移除创建函数
    if (removeFun && _createFuns.contains(key)) {
        _createFuns.remove(key);
    }
}

void FCOperatorRepo::registerOperatorFunction(const QString& key, std::function<FCAbstractOperator*()> fun)
{
    QMutexLocker locker(&_mutex);
    _createFuns.insert(key, fun);
}

FCAbstractOperator* FCOperatorRepo::createOperator(const QString& key)
{
    // 查找创建函数
    auto fun = _createFuns.value(key);
    if (!fun) return nullptr;
    // 执行创建操作
    FCAbstractOperator* op = fun();
    if (op == nullptr) return nullptr;
    // 纳入到容器管理，并关联槽函数
    _existOperators.insert(key, op);
    connect(op, &FCAbstractOperator::operatorDestoryedSig,
            this, &FCOperatorRepo::operatorDestoryedSlot);
    return op;
}

void FCOperatorRepo::operatorDestoryedSlot(FCAbstractOperator* oper)
{
    QMutexLocker locker(&_mutex);
    // operator被释放，从管理器中移除，不要重复delete
    const QString name = _existOperators.key(oper);
    if (_existOperators.contains(name)) {
        _existOperators.remove(name);
    }
}

} // namespace FC
