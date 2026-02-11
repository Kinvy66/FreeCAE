/**
 * @file FCGmshMesherDriver.cpp
 * @brief Gmsh 网格驱动器实现
 */
#include "FCGmshMesherDriver.h"
#include "FCGmshMesherWorker.h"
#include "FCMeshGenInterface.h"
#include "FCGlobalMeshSizeInfo.h"
#include "FCGlobalMeshGenerateAlgorithmInfo.h"
#include <QDir>
#include <QFileInfo>

namespace FC {

FCGmshMesherDriver::FCGmshMesherDriver(QObject* parent) : FCAbstractMesherDriver(parent) {}

FCGmshMesherDriver::~FCGmshMesherDriver()
{
    _stopRequested = true;
    if (_workerThread && _workerThread->isRunning()) {
        _workerThread->quit();
        _workerThread->wait(3000);
    }
}

void FCGmshMesherDriver::startMesher(QStringList info)
{
    _stopRequested = false;

    QString workDir = getValue("WorkDir").toString();
    if (workDir.isEmpty()) {
        emit mesherError(tr("WorkDir is empty"));
        return;
    }

    QDir dir(workDir);
    if (!dir.exists() && !dir.mkpath(".")) {
        emit mesherError(tr("Cannot create work directory: %1").arg(workDir));
        return;
    }

    // 几何文件：从 GeometryFiles 或 GeometryFile 获取
    QStringList geometryFiles;
    QVariant geomVar = getValue("GeometryFiles");
    if (geomVar.canConvert<QStringList>()) {
        geometryFiles = geomVar.toStringList();
    }
    if (geometryFiles.isEmpty()) {
        QString single = getValue("GeometryFile").toString();
        if (!single.isEmpty()) geometryFiles << single;
    }

    if (geometryFiles.isEmpty()) {
        emit mesherError(tr("No geometry file specified (GeometryFile/GeometryFiles)"));
        return;
    }

    // 输出网格文件
    QString outputFile = getValue("OutputMeshFile").toString();
    if (outputFile.isEmpty()) {
        outputFile = workDir + "/mesh.msh";
    }
    if (QFileInfo(outputFile).isRelative()) {
        outputFile = workDir + "/" + outputFile;
    }

    // 从 FCMeshGenInterface 获取全局尺寸与算法参数
    QVariantMap sizeParams;
    QVariantMap algParams;
    FCMeshGenInterface* iface = FCMeshGenInterface::instance();
    if (iface) {
        FCGlobalMeshSizeInfo* gs = iface->getGlobalMeshSizeInfo("Gmsh");
        if (gs) {
            sizeParams["GlobalSize"] = gs->getGlobalSize();
            sizeParams["MinSize"] = gs->getMinSize();
            sizeParams["MaxSize"] = gs->getMaxSize();
            sizeParams["SizeFactor"] = gs->getSizeFactor();
        }
        FCGlobalMeshGenerateAlgorithmInfo* ga = iface->getGlobalMeshGenerateAlgorithmInfo("Gmsh");
        if (ga) {
            algParams["Dimension"] = ga->getMeshGenerateDimension();
            algParams["2DAlgorithm"] = ga->get2DAlgorithm();
            algParams["3DAlgorithm"] = ga->get3DAlgorithm();
            algParams["ElementOrder"] = ga->getElementOrder();
        }
    }

    int nThreads = _numThreads > 0 ? _numThreads : 0;

    _worker = new FCGmshMesherWorker();  // 无 parent，否则 moveToThread 报错
    _worker->setDriver(this);
    _worker->setStopFlag(&_stopRequested);

    _workerThread = new QThread(this);
    _worker->moveToThread(_workerThread);

    connect(_workerThread, &QThread::started, this, [this, geometryFiles, outputFile, nThreads, sizeParams, algParams]() {
        _worker->run(geometryFiles, outputFile, nThreads, sizeParams, algParams);
    });
    connect(_worker, &FCGmshMesherWorker::finished, this, &FCGmshMesherDriver::onWorkerFinished);

    _workerThread->start();
}

void FCGmshMesherDriver::stopMesher(QStringList info)
{
    _stopRequested = true;
    // Gmsh API 无直接中止，仅标记；worker 检测 _stopFlag（目前未在循环中检查）
}

void FCGmshMesherDriver::onWorkerFinished(bool success, const QString& errorMsg)
{
    if (_worker) {
        disconnect(_worker, &FCGmshMesherWorker::finished, this, &FCGmshMesherDriver::onWorkerFinished);
        _worker->deleteLater();  // 无 parent，需手动释放
        _worker = nullptr;
    }
    if (_workerThread && _workerThread->isRunning()) {
        _workerThread->quit();
        _workerThread->wait(1000);
    }
    if (!success && !errorMsg.isEmpty()) {
        emit mesherError(errorMsg);
    }
    emit mesherFinished();
}

} // namespace FC
