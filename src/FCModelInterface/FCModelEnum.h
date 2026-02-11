/**
 * @file FCModelEnum.h
 * @brief 模型相关枚举值（移植自 FITKInterfaceModel，不依赖 FITKCore）
 */
#ifndef FCMODELENUM_H
#define FCMODELENUM_H

#include "FCModelInterfaceAPI.h"
#include <QObject>

namespace FC {

class FCMODELINTERFACE_API FCModelEnum : public QObject
{
    Q_OBJECT
public:
    enum FITKEleType
    {
        EleNone,
        Line2,
        Line3,
        Tri3,
        Tri6,
        Quad4,
        Quad8,
        Tet4,
        Tet10,
        Wedge6,
        Hex8,
        Hex20,
        Polygon,
    };
    Q_ENUM(FITKEleType);

    enum FITKModelSetType
    {
        FMSNone,
        FMSNode,
        FMSElem,
        FMSMIX,
        FMSPoint,
        FMSEdge,
        FMSSurface,
        FMSSolid,
        FMSComb,
    };
    Q_ENUM(FITKModelSetType);

    enum FITKModelSetDim {
        FMSDimNone = 0,
        FMSDimD0 = 1,
        FMSDimD1 = 2,
        FMSDimD2 = 4,
        FMSDimD3 = 8,
        FMSDimMix = 16,
    };
    Q_ENUM(FITKModelSetDim);

    enum FITKMeshDim
    {
        FMDimNone = 0,
        FMDimD0 = 1,
        FMDimD1 = 2,
        FMDimD2 = 4,
        FMDimD3 = 8,
        FMDimMix = 16,
    };
    Q_ENUM(FITKMeshDim);

    enum FITKModelDim
    {
        FMDNone,
        FMD0,
        FMD1,
        FMD2,
        FMD3,
    };
    Q_ENUM(FITKModelDim);

    enum AbsModelType
    {
        AMTNone,
        AMTMesh = 100,
        AMTPartical,
        AMTunstructuredMesh,
        AMTStructuredMesh,
        AMTunstructuredMeshvtk,
        AMTStructuredMeshvtk,
        AMTGeometry = 200,
        AMTOCC,
        AssInstance = 300,
        Assembly,
        AMTMIX = 1000,
    };
    Q_ENUM(AbsModelType);

    static FITKMeshDim GetElementDim(FITKEleType eletype);
    static bool HasMeshDimFlag(const unsigned int flags, const FITKMeshDim& dim);

private:
    FCModelEnum() = default;
    FCModelEnum(const FCModelEnum&) = delete;
    ~FCModelEnum() = default;
};

} // namespace FC

#endif // FCMODELENUM_H
