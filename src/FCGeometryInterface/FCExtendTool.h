/**
 * @file FCExtendTool.h
 * @brief 构造辅助静态工具（移植自 FITKInterfaceGeometry FITKAbsExtendTool）
 */
#ifndef FC_EXTEND_TOOL_H
#define FC_EXTEND_TOOL_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCVirtualTopoMapper.h"
#include <QtCore/QtGlobal>
#include <cmath>
#include <queue>

namespace FC {

class FCExtendTool
{
public:
    FCExtendTool() = delete;
    ~FCExtendTool() = delete;

    static void crossVector(const double* first, const double* second, double* result);
    static double dotVector(const double* first, const double* second);
    static bool isZeroVector(const double* vec);
    static double vectorNorm(const double* vec);
    static bool isFlipDirection(const double* first, const double* second);
    static bool isEdgeOnFaceTopo(const VirtualShape& edge, const VirtualShape& face);
    static bool isConnectedFace(const VirtualShape& face1, const VirtualShape& face2);
    static bool isConnectedFaces(QList<VirtualShape>& faces);
};

} // namespace FC

#endif // FC_EXTEND_TOOL_H
