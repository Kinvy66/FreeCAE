/**
 * @file FCGmshMeshGenInterface.cpp
 * @brief Gmsh 网格生成模块注册
 */
#include "FCGmshMeshGenInterface.h"
#include "FCMeshGenInterface.h"
#include "FCGmshMesherDriver.h"
#include "FCGmshMeshProcessor.h"
#include "FCGmshMeshSizeGenerator.h"
#include "FCGmshGeometryMeshSizeGenerator.h"
#include "FCGmshMeshAlgorithmInfoGenerator.h"

namespace FC {

void FCGmshMeshGenInterface::initialize()
{
    FCMeshGenInterface* iface = FCMeshGenInterface::instance();
    if (!iface) return;

    iface->regMeshSizeGenerator(new FCGmshMeshSizeGenerator, "Gmsh");
    iface->regGeometryMeshSizeGenerator(new FCGmshGeometryMeshSizeGenerator, "Gmsh");
    iface->regMeshAlgorithmGenerator(new FCGmshMeshAlgorithmInfoGenerator, "Gmsh");
    iface->regMesherDriver(new FCGmshMesherDriver, "Gmsh");
    iface->regMeshProcessor(new FCGmshMeshProcessor, "Gmsh");
}

void FCGmshMeshGenInterface::finalize()
{
    FCMeshGenInterface* iface = FCMeshGenInterface::instance();
    if (iface) iface->removeMeshGenInterfaceData("Gmsh");
}

} // namespace FC
