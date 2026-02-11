/**
 * @file FCMeshVTKMap.h
 * @brief 单元类型与 VTK 单元类型映射（移植自 FITKInterfaceMesh FITKMeshVTKMap）
 */
#ifndef FCMESHVTKMAP_H
#define FCMESHVTKMAP_H

#include "FCMeshInterfaceAPI.h"
#include "FCModelEnum.h"
#include <QHash>
#include <vtkCellType.h>

namespace FC {

/** 单元类型与 VTK 单元类型映射 */
const QHash<FCModelEnum::FITKEleType, int> fcmeshEleTypeToVTK = {
    {FCModelEnum::Line2,        VTK_LINE},
    {FCModelEnum::Line3,        VTK_QUADRATIC_EDGE},
    {FCModelEnum::Tri3,         VTK_TRIANGLE},
    {FCModelEnum::Tri6,         VTK_QUADRATIC_TRIANGLE},
    {FCModelEnum::Quad4,        VTK_QUAD},
    {FCModelEnum::Quad8,        VTK_QUADRATIC_QUAD},
    {FCModelEnum::Tet4,         VTK_TETRA},
    {FCModelEnum::Tet10,        VTK_QUADRATIC_TETRA},
    {FCModelEnum::Wedge6,       VTK_WEDGE},
    {FCModelEnum::Hex8,         VTK_HEXAHEDRON},
    {FCModelEnum::Hex20,        VTK_QUADRATIC_HEXAHEDRON},
    {FCModelEnum::Polygon,      VTK_POLYGON},
};

} // namespace FC

#endif // FCMESHVTKMAP_H
