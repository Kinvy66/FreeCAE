#ifndef FCABSTRACTOPERATOR_H
#define FCABSTRACTOPERATOR_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QWidget>
#include "FCActionsOperatorAPI.h"
#include "FCAbstractObject.hpp"
#include <FCUIInterface.h>

namespace FC 
{
/**
 * 操作器通过注入的 FCUIInterface 统一访问主界面与 Docking，避免对 APP 的依赖与多重 setX 注入。
 * 由 FCActionEventHandler 在 execOperator 时注入，子类可调用 mainWindow()、dockingArea()、uiInterface()。
 */
class FCACTIONSOPERATOR_API FCAbstractOperator : public QObject, public FCAbstractObject
{
    Q_OBJECT
public:
    FCAbstractOperator();
    virtual ~FCAbstractOperator() = 0;

    /** 设置 UI 上下文（由事件处理在触发时注入），子类通过 mainWindow/dockingArea/ribbonArea 访问主界面 */
    void setUIInterface(FCUIInterface* ui) { _uiInterface = ui; }
    /** 获取当前 UI 上下文，未设置时返回 nullptr */
    FCUIInterface* uiInterface() const { return _uiInterface; }
    /** 主窗口（弹窗父窗口、居中显示等），等价于 uiInterface() ? uiInterface()->getMainWindow() : nullptr */
    QMainWindow* mainWindow() const { return _uiInterface ? _uiInterface->getMainWindow() : nullptr; }
    /** Docking 区域（获取 CDockManager、各 Dock 窗口、raise 等），未设置时返回 nullptr */
    FCDockingAreaInterface* dockingArea() const { return _uiInterface ? _uiInterface->getDockingArea() : nullptr; }

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
    /** UI 上下文（由 APP 在触发操作时注入），用于 mainWindow/dockingArea 等访问 */
    FCUIInterface* _uiInterface{ nullptr };
};
} // namespace FC

#endif // FCABSTRACTOPERATOR_H
