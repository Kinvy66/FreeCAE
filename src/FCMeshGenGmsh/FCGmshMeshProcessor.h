/**
 * @file FCGmshMeshProcessor.h
 * @brief Gmsh 网格后处理器（读取 MSH 到 FC 网格）
 */
#ifndef FCGMSHMESHPROCESSOR_H
#define FCGMSHMESHPROCESSOR_H

#include "FCMeshGenGmshAPI.h"
#include "FCAbstractMeshProcessor.h"

namespace FC {

/**
 * @brief Gmsh 网格后处理器
 * 从 WorkDir/OutputMeshFile 读取 MSH 文件并填充到 FCUnstructuredMeshVTK
 */
class FCMESHGENGMSH_API FCGmshMeshProcessor : public FCAbstractMeshProcessor
{
public:
    explicit FCGmshMeshProcessor() = default;
    ~FCGmshMeshProcessor() override = default;

    void start(QStringList info = QStringList()) override;
};

} // namespace FC

#endif // FCGMSHMESHPROCESSOR_H
