/**
 * @file FCGmshMeshProcessor.cpp
 * @brief Gmsh 网格后处理器实现
 */
#include "FCGmshMeshProcessor.h"
#include "FCGmshMeshReader.h"
#include "FCUnstructuredMeshVTK.h"
#include <QDir>
#include <QFileInfo>

namespace FC {

void FCGmshMeshProcessor::start(QStringList info)
{
    Q_UNUSED(info);

    QString workDir = getValue("WorkDir").toString();
    QString meshFile = getValue("OutputMeshFile").toString();
    if (meshFile.isEmpty()) meshFile = "mesh.msh";
    if (QFileInfo(meshFile).isRelative() && !workDir.isEmpty()) {
        meshFile = workDir + "/" + meshFile;
    }

    FCUnstructuredMeshVTK* mesh = getDataObjectAs<FCUnstructuredMeshVTK>("Mesh");
    if (!mesh) return;

    if (!QFileInfo::exists(meshFile)) return;

    mesh->clearMesh();

    FCGmshMeshReader reader;
    reader.setFileName(meshFile);
    reader.setMeshObject(mesh);
    reader.read();
}

} // namespace FC
