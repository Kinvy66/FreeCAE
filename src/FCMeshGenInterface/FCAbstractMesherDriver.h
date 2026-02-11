/**
 * @file FCAbstractMesherDriver.h
 * @brief 网格驱动器抽象类（移植自 FITKInterfaceMeshGen，适配 Gmsh 等网格引擎）
 */
#ifndef FCABSTRACTMESHERDRIVER_H
#define FCABSTRACTMESHERDRIVER_H

#include "FCMeshGenInterfaceAPI.h"
#include <QObject>
#include <QStringList>
#include <QHash>
#include <FCData/FCVariantParams.h>

namespace FC {

class FCAbstractDataObject;

/**
 * @brief 网格驱动器抽象类（Gmsh/snappyHexMesh 等具体实现继承此类）
 */
class FCMESHGENINTERFACE_API FCAbstractMesherDriver : public QObject, public FCVariantParams
{
    Q_OBJECT
public:
    explicit FCAbstractMesherDriver(QObject* parent = nullptr);
    virtual ~FCAbstractMesherDriver() override;

    virtual void setDataObject(FCAbstractDataObject* data);
    virtual void setArgs(const QStringList& args);
    virtual void insertDataObject(const QString& key, FCAbstractDataObject* value);

    virtual void startMesher(QStringList info = QStringList()) = 0;
    virtual void stopMesher(QStringList info = QStringList()) = 0;

signals:
    void mesherFinished();

protected:
    FCAbstractDataObject* _data{};
    QStringList _args;
    QHash<QString, FCAbstractDataObject*> _dataObject;
};

} // namespace FC

#endif // FCABSTRACTMESHERDRIVER_H
