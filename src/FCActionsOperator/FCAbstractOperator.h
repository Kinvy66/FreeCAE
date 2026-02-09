#ifndef FCABSTRACTOPERATOR_H
#define FCABSTRACTOPERATOR_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include "FCActionsOperatorAPI.h"
#include "FCAbstractObject.hpp"

namespace FC 
{
class FCACTIONSOPERATOR_API FCAbstractOperator : public QObject, public FCAbstractObject
{
    Q_OBJECT
public:
    FCAbstractOperator();
    virtual ~FCAbstractOperator() = 0;

    /**
     * @brief 设置触发对象
     * @param obj 触发对象
     */
    virtual void setEmitter(QObject* obj);

    /**
     * @brief 设置参数
     * @param argName 参数名称
     * @param arg 参数对象
     */
    void setArgs(const QString& argName, QVariant arg);

    /**
     * @brief 移除参数
     * @param argName 参数名称
     */
    void removeArgs(const QString& argName);

    /**
     * @brief 是否包含参数
     * @param argName 参数名称
     * @return true/false
     */
    bool hasArgs(const QString& argName);

    /**
     * @brief 清空全部参数
     */
    virtual void clearArgs();

    /**
     * @brief 获取参数
     * @param name 参数名称
     * @param value 输出参数值
     * @return 是否成功获取
     */
    template <typename T>
    bool argValue(const QString& name, T& value)
    {
        if (!_operArgs.contains(name)) return false;
        auto arg = _operArgs.value(name);
        value = arg.value<T>();
        return true;
    }

    /**
     * @brief 获取参数，指针类型
     * @param name 参数名称
     * @return 参数指针
     */
    template <typename T>
    T* argValuePtr(const QString& name)
    {
        if (!_operArgs.contains(name)) return nullptr;
        auto arg = _operArgs.value(name);
        FCAbstractObject* absObj = arg.value<FCAbstractObject*>();
        return dynamic_cast<T*>(absObj);
    }

signals:
    /**
     * @brief 操作器被释放信号
     * @param oper 被释放的指针（this）
     */
    void operatorDestoryedSig(FCAbstractOperator* oper);

protected:
    /**
     * @brief 参数列表
     */
    QHash<QString, QVariant> _operArgs{};
    /**
     * @brief 触发器
     */
    QObject* _emitter{};
};
} // namespace FC

#endif // FCABSTRACTOPERATOR_H
