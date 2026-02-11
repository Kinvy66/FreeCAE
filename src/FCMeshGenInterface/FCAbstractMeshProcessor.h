/**
 * @file FCAbstractMeshProcessor.h
 * @brief 网格后处理器抽象类（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCABSTRACTMESHPROCESSOR_H
#define FCABSTRACTMESHPROCESSOR_H

#include "FCMeshGenInterfaceAPI.h"
#include <QStringList>
#include <QHash>
#include <FCData/FCVariantParams.h>

namespace FC {

class FCAbstractDataObject;

/**
 * @brief 网格生成后处理器（读取/转换网格文件等）
 */
class FCMESHGENINTERFACE_API FCAbstractMeshProcessor : public FCVariantParams
{
public:
    explicit FCAbstractMeshProcessor() = default;
    virtual ~FCAbstractMeshProcessor();

    virtual void setArgs(const QStringList& args);
    virtual void insertDataObject(const QString& key, FCAbstractDataObject* value);
    FCAbstractDataObject* getDataObject(const QString& key) const;

    template<class T>
    T* getDataObjectAs(const QString& key) const
    {
        return dynamic_cast<T*>(getDataObject(key));
    }

    virtual void start(QStringList info = QStringList()) = 0;

protected:
    QStringList _args;
    QHash<QString, FCAbstractDataObject*> _dataObject;
};

} // namespace FC

#endif // FCABSTRACTMESHPROCESSOR_H
