/**
 * @file FCGmshMeshReader.h
 * @brief Gmsh MSH 格式网格读取（移植自 OpenFOAM mesh IO 的通用读取逻辑）
 */
#ifndef FCGMSHMESHREADER_H
#define FCGMSHMESHREADER_H

#include "FCMeshGenGmshAPI.h"
#include <QString>
#include <QVector>
#include <QHash>

namespace FC {

class FCUnstructuredMeshVTK;

/**
 * @brief Gmsh MSH 文件读取器
 * 支持 MSH2 与 MSH4 格式，将网格读入 FCUnstructuredMeshVTK
 */
class FCMESHGENGMSH_API FCGmshMeshReader
{
public:
    FCGmshMeshReader();
    ~FCGmshMeshReader();

    void setFileName(const QString& file);
    QString getFileName() const { return _fileName; }

    void setMeshObject(FCUnstructuredMeshVTK* mesh);
    FCUnstructuredMeshVTK* getMeshObject() const { return _mesh; }

    bool read();

private:
    bool readMSH2();
    bool readMSH4();
    int parseVersion(QString* errMsg = nullptr);

    QString _fileName;
    FCUnstructuredMeshVTK* _mesh{};
};

} // namespace FC

#endif // FCGMSHMESHREADER_H
