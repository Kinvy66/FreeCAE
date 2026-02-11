/**
 * @file FCGmshMesherWorker.h
 * @brief Gmsh 网格生成工作线程
 */
#ifndef FCGMSHMESHERWORKER_H
#define FCGMSHMESHERWORKER_H

#include "FCMeshGenGmshAPI.h"
#include <QObject>
#include <QStringList>

namespace FC {

class FCAbstractMesherDriver;

/**
 * @brief 在独立线程中执行 Gmsh 网格生成
 */
class FCMESHGENGMSH_API FCGmshMesherWorker : public QObject
{
    Q_OBJECT
public:
    explicit FCGmshMesherWorker(QObject* parent = nullptr);

    void setDriver(FCAbstractMesherDriver* driver) { _driver = driver; }
    void setStopFlag(bool* flag) { _stopFlag = flag; }

    void run(const QStringList& geometryFiles, const QString& outputFile,
             int numThreads, const QVariantMap& sizeParams, const QVariantMap& algParams);

signals:
    void finished(bool success, const QString& errorMsg);

private:
    FCAbstractMesherDriver* _driver{};
    bool* _stopFlag{};
};

} // namespace FC

#endif // FCGMSHMESHERWORKER_H
