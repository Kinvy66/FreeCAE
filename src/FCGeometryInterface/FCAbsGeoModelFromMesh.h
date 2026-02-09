/**
 * @file FCGeoModelFromMesh.h
 * @brief 从网格生成几何命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelFromMesh，接口层不依赖具体网格类型）
 */
#ifndef FCGEOMODELFROMMESH_H
#define FCGEOMODELFROMMESH_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QHash>
#include <QList>

namespace FC {

/**
 * @brief 从网格数据生成几何模型的命令（接口层：仅存储节点与三角面数据）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelFromMesh : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelFromMesh);
public:
    struct MeshNodeCoordinates
    {
        double x{};
        double y{};
        double z{};
    };
    struct MeshTri
    {
        int node1{};
        int node2{};
        int node3{};
        double normal[3]{};
    };

    explicit FCGeoModelFromMesh() = default;
    ~FCGeoModelFromMesh() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setMeshNodeCoordinates(const QHash<int, MeshNodeCoordinates>& coords) { m_meshNodeCoordinates = coords; }
    const QHash<int, MeshNodeCoordinates>& getMeshNodeCoordinates() const { return m_meshNodeCoordinates; }
    void setMeshTriangles(const QList<MeshTri>& tris) { m_meshTriangles = tris; }
    const QList<MeshTri>& getMeshTriangles() const { return m_meshTriangles; }
protected:
    QHash<int, MeshNodeCoordinates> m_meshNodeCoordinates;
    QList<MeshTri> m_meshTriangles;
};

} // namespace FC

#endif // FCGEOMODELFROMMESH_H
