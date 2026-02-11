/**
 * @file FCGmshMeshGenInterface.h
 * @brief Gmsh 网格生成模块注册接口
 */
#ifndef FCGMSHMESHGENINTERFACE_H
#define FCGMSHMESHGENINTERFACE_H

#include "FCMeshGenGmshAPI.h"

namespace FC {

/**
 * @brief 向 FCMeshGenInterface 注册 Gmsh 引擎
 * 在应用启动时调用 initialize()
 */
class FCMESHGENGMSH_API FCGmshMeshGenInterface
{
public:
    static void initialize();
    static void finalize();
};

} // namespace FC

#endif // FCGMSHMESHGENINTERFACE_H
