/**
 * @file FCGmshMeshWriter.h
 * @brief 网格写出（MSH/VTK 格式）
 */
#ifndef FCGMSHMESHWRITER_H
#define FCGMSHMESHWRITER_H

#include "FCMeshGenGmshAPI.h"
#include <QString>

namespace FC {

class FCUnstructuredMeshVTK;

/**
 * @brief 将 FC 网格写出为 MSH 或 VTK 文件
 */
class FCMESHGENGMSH_API FCGmshMeshWriter
{
public:
    FCGmshMeshWriter();
    ~FCGmshMeshWriter();

    void setMeshObject(FCUnstructuredMeshVTK* mesh);
    void setFileName(const QString& file);

    bool write();  // 根据扩展名选择格式
    bool writeMSH();
    bool writeVTK();

private:
    FCUnstructuredMeshVTK* _mesh{};
    QString _fileName;
};

} // namespace FC

#endif // FCGMSHMESHWRITER_H
