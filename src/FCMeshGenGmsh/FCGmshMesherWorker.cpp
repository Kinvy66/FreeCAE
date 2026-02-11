/**
 * @file FCGmshMesherWorker.cpp
 * @brief Gmsh 网格生成工作线程实现
 */
#include "FCGmshMesherWorker.h"
#include "FCAbstractMesherDriver.h"
#include <gmsh.h>
#include <QDir>
#include <QFileInfo>
#include <QVariant>

namespace FC {

FCGmshMesherWorker::FCGmshMesherWorker(QObject* parent) : QObject(parent) {}

void FCGmshMesherWorker::run(const QStringList& geometryFiles, const QString& outputFile,
                             int numThreads, const QVariantMap& sizeParams, const QVariantMap& algParams)
{
    QString errMsg;
    bool success = false;

    try {
        gmsh::initialize();

        // 多线程：0 表示自动，否则设置线程数
        if (numThreads > 0) {
            gmsh::option::setNumber("General.NumThreads", static_cast<double>(numThreads));
        }

        // 全局网格尺寸
        double minS = sizeParams.value("MinSize", 0.1).toDouble();
        double maxS = sizeParams.value("MaxSize", 10.0).toDouble();
        double factor = sizeParams.value("SizeFactor", 1.0).toDouble();
        if (sizeParams.contains("GlobalSize")) {
            double s = sizeParams["GlobalSize"].toDouble();
            if (s > 0) {
                minS = maxS = s;
            }
        }
        gmsh::option::setNumber("Mesh.CharacteristicLengthMin", minS);
        gmsh::option::setNumber("Mesh.CharacteristicLengthMax", maxS);
        gmsh::option::setNumber("Mesh.CharacteristicLengthFactor", factor);

        // 网格算法
        int dim = algParams.value("Dimension", 3).toInt();
        int alg2D = algParams.value("2DAlgorithm", 3).toInt();   // Frontal-Delaunay default
        int alg3D = algParams.value("3DAlgorithm", 1).toInt();    // Frontal default
        int order = algParams.value("ElementOrder", 1).toInt();

        gmsh::option::setNumber("Mesh.ElementOrder", order);

        if (dim == 2) {
            gmsh::option::setNumber("Mesh.Algorithm", static_cast<double>(alg2D));
        } else {
            gmsh::option::setNumber("Mesh.Algorithm3D", static_cast<double>(alg3D));
        }

        // 合并几何文件 (STEP, BREP, IGES, STL)
        gmsh::model::add("mesh_model");
        for (const QString& geoFile : geometryFiles) {
            QString path = QDir::fromNativeSeparators(geoFile);
            if (!QFileInfo::exists(path)) {
                errMsg = QString("Geometry file not found: %1").arg(path);
                gmsh::finalize();
                emit finished(false, errMsg);
                return;
            }
            gmsh::merge(path.toStdString());
        }

        if (geometryFiles.isEmpty()) {
            errMsg = "No geometry file specified";
            gmsh::finalize();
            emit finished(false, errMsg);
            return;
        }

        // 生成网格（多线程在 Gmsh 内部使用）
        gmsh::model::mesh::generate(dim);

        // 写出网格
        QFileInfo outInfo(outputFile);
        QDir::root().mkpath(outInfo.absolutePath());
        std::string outPath = QDir::toNativeSeparators(outputFile).toStdString();
        gmsh::write(outPath);

        success = true;
        gmsh::finalize();
    } catch (const std::exception& e) {
        errMsg = QString::fromUtf8(e.what());
        try { gmsh::finalize(); } catch (...) {}
    } catch (...) {
        errMsg = "Unknown Gmsh error";
        try { gmsh::finalize(); } catch (...) {}
    }

    emit finished(success, errMsg);
}

} // namespace FC
