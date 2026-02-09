#ifndef FCOPERATORREPO_H
#define FCOPERATORREPO_H

#include "FCActionsOperatorAPI.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <QMutex>
#include <functional>

namespace FC 
{
class FCAbstractOperator;

/**
 * @brief 操作器仓库，用于注册和获取操作器
 */
class FCACTIONSOPERATOR_API FCOperatorRepo : public QObject
{
    Q_OBJECT
public:
    static FCOperatorRepo* instance();

    /**
     * @brief 获取操作器，不存在的直接创建，存在的返回原有对象
     * @param key Operator注册的关键字
     * @return FCAbstractOperator*
     */
    FCAbstractOperator* getOperator(const QString& key);

    /**
     * @brief 获取操作器并转换为T类型
     * @param key Operator注册的关键字
     * @return T*
     */
    template <class T>
    T* getOperatorT(const QString& key)
    {
        return dynamic_cast<T*>(this->getOperator(key));
    }

    /**
     * @brief 移除操作器
     * @param key 操作器键值
     * @param removeFun 是否移除创建函数
     */
    void removeOperator(const QString& key, bool removeFun = true);

    /**
     * @brief 注册操作器创建函数
     * @param key 操作器键值
     * @param fun 创建函数
     */
    void registerOperatorFunction(const QString& key, std::function<FCAbstractOperator*()> fun);

private:
    FCOperatorRepo();
    ~FCOperatorRepo();
    FCOperatorRepo(const FCOperatorRepo&) = delete;
    FCOperatorRepo& operator=(const FCOperatorRepo&) = delete;

    /**
     * @brief 创建操作器
     * @param key 关键字
     * @return FCAbstractOperator*
     */
    FCAbstractOperator* createOperator(const QString& key);

private slots:
    /**
     * @brief operator被释放槽函数
     * @param oper 被释放的操作器
     */
    void operatorDestoryedSlot(FCAbstractOperator* oper);

private:
    /**
     * @brief 注册进来的创建operator函数
     */
    QHash<QString, std::function<FCAbstractOperator*()>> _createFuns{};
    /**
     * @brief 已经创建的operator
     */
    QHash<QString, FCAbstractOperator*> _existOperators{};
    /**
     * @brief 互斥锁
     */
    mutable QMutex _mutex;
    /**
     * @brief 单例实例
     */
    static FCOperatorRepo* _instance;

public:
    /**
     * @brief 操作器注册辅助类
     */
    template <typename T>
    class FCOperatorRepoRegHelper
    {
    public:
        FCOperatorRepoRegHelper(const QString& key)
        {
            auto ins = FCOperatorRepo::instance();
            // 注册构造函数，T类型必须有默认的无参构造函数
            ins->registerOperatorFunction(key, [] { return new T; });
        }
    };
};

} // namespace FC

/**
 * @brief 声明注册宏
 */
#ifndef Register2FCOperatorRepo
#define Register2FCOperatorRepo(key, Class, ...) \
    static FC::FCOperatorRepo::FCOperatorRepoRegHelper<Class> \
        _FCOperatorRepoRegHelper_##key##_##Class##__VA_ARGS__(#key)
#endif

/**
 * @brief 访问单例宏
 */
#ifndef FCOPERATORREPO
#define FCOPERATORREPO FC::FCOperatorRepo::instance()
#endif

#endif // FCOPERATORREPO_H
