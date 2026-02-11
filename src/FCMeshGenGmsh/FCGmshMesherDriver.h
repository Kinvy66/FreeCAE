/**
 * @file FCGmshMesherDriver.h
 * @brief Gmsh 网格驱动器（C++ API 调用，支持多线程）
 */
#ifndef FCGMSHMESHERDRIVER_H
#define FCGMSHMESHERDRIVER_H

#include "FCMeshGenGmshAPI.h"
#include "FCAbstractMesherDriver.h"
#include <QThread>

namespace FC {

/**
 * @brief Gmsh 网格驱动器（几何 -> Gmsh API -> 网格文件）
 * 通过 gmsh::merge 加载几何，gmsh::model::mesh::generate 生成网格
 * 使用 General.NumThreads 启用多线程
 */
class FCMESHGENGMSH_API FCGmshMesherDriver : public FCAbstractMesherDriver
{
    Q_OBJECT
public:
    explicit FCGmshMesherDriver(QObject* parent = nullptr);
    ~FCGmshMesherDriver() override;

    void startMesher(QStringList info = QStringList()) override;
    void stopMesher(QStringList info = QStringList()) override;

    void setNumThreads(int n) { _numThreads = n; }
    int getNumThreads() const { return _numThreads; }

signals:
    void mesherError(const QString& msg);
    void mesherProgress(const QString& msg);

private slots:
    void onWorkerFinished(bool success, const QString& errorMsg);

private:
    int _numThreads{0};  // 0 = 自动（默认所有核心）
    QThread* _workerThread{};
    class FCGmshMesherWorker* _worker{};
    bool _stopRequested{false};
};

} // namespace FC

#endif // FCGMSHMESHERDRIVER_H
