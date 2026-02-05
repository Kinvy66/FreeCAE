/**
 * @file FCGeoEnum.h
 * @brief 几何命令与虚拓扑等枚举（移植自 FITKInterfaceGeometry）
 */
#ifndef FCGEOENUM_H
#define FCGEOENUM_H

#include "FCGeometryInterfaceAPI.h"
#include <QObject>

namespace FC {

class FCGEOMETRYINTERFACE_API FCGeoEnum : public QObject
{
    Q_OBJECT
public:
    enum FITKGeoEngine
    {
        FGENone = -1,
        FGEOCC,
        FGEOGG,
        FGEPS,
        FGEACIS,
        FGECGM,
    };
    Q_ENUM(FITKGeoEngine);

    enum VTopoShapeType
    {
        VSNone = 0,
        VSPoint,
        VSEdge,
        VSWire,
        VSFace,
        VSShell,
        VSSolid,
        VSLump,
        VSAssembly,
    };
    Q_ENUM(VTopoShapeType);

    enum FITKDatumType
    {
        FDTNone = -1,
        FDTPoint = 0,
        FDTCurveRadio = 1,
        FDTLine = 1000,
        FDTSegment = 1001,
        FDTLineByIntersectPlanes = 1002,
        FDTLineByRotateLine = 1003,
        FDTPlane = 2000,
        FDTOffsetPlane = 2001,
        FDTThreePointsPlane = 2002,
        FDTEquationPlane = 2003,
        FDTPointAndDirectionPlane = 2004,
        FDTRotateFromPlanePlane = 2005
    };
    Q_ENUM(FITKDatumType);

    enum FITKGeoStatus
    {
        FGSNone = -1,
        FGSNormal,
        FGSModified,
        FGSDeleted,
    };
    Q_ENUM(FITKGeoStatus);

    enum FITKGeometryComType
    {
        FGTNone = -1,
        FGTDelete,
        FGTMultiDelete,
        FGTDatum,
        FGTSketch2D,
        FGTConstruction,
        FGTImport,
        FGTImportMergePart,
        FGTImportSplitPart,
        FGTImportFlattenAssembly,
        FGTImportSplitAssembly,
        FGTImportPart,
        FGTExport,
        FGTExportTopos,
        FGTBox,
        FGTCylinder,
        FGTSphere,
        FGTBool,
        FGTNBodyUnite,
        FGTPoint,
        FGTCurveEnd,
        FGTCurveEndDistance,
        FGTCurveRadio,
        FGTPointProjectionOnCurve,
        FGTTwoCurveIntersectPoint,
        FGTPointProjectionOnSurface,
        FGTCurveIntersectSurface,
        FGTInteractionPoint,
        FGTLine,
        FGTSegment,
        FGTWire,
        FGTCircle,
        FGTCircleArc,
        FGTThreePointsCircle,
        FGTThreePointsEllipse,
        FGTThreePointsHyperbola,
        FGTThreePointsParabola,
        FGTCurveProjectionOnSurface,
        FGTSurfaceIntersectSurface,
        FGTEllipse,
        FGTEllipseArc,
        FGTHyperbola,
        FGTHyperbolaArc,
        FGTParabola,
        FGTParabolaArc,
        FGTOffsetCurve,
        FGTBezierByControlPoints,
        FGTBezierByThroughPoints,
        FGTBSplineByControlPoints,
        FGTBSplineByThroughPoints,
        FGTTrimmed,
        FGTBridgeCurve,
        FGTCurveFromSurface,
        FGTClosedWireSurface,
        FGTOffsetSurface,
        FGTExtrudeSurface,
        FGTRevolSurface,
        FGTSweepSurface,
        FGTClosedSurfaceSolid,
        FGTBridgeSurface,
        FGTSurfaceFromSolid,
        FGTMultiSectionSurface,
        FGTExtrudeSolid,
        FGTRevolSolid,
        FGTSweepSolid,
        FGTMultiSectionSolid,
        FGTExtrudeCut,
        FGTRevolCut,
        FGTSweepCut,
        FGTMultiSectionCut,
        FGTChamferSolid,
        FGTFilletSolid,
        FGTDefeature,
        FGTRemoveChamfer,
        FGTRemoveFillet,
        FGTFillHole,
        FGTMergeLine,
        FGTSplitByMiddlePositionLine,
        FGTSplitByAngleLine,
        FGTDeleteFloatingEdge,
        FGTFillHolesFace,
        FGTFillGapsFace,
        FGTImprintMapPointToFace,
        FGTImprintMapLineToFace,
        FGTImprintSolid,
        FGTCompoundAppendShape,
        FGTCompoundRemoveShape,
        FGTTransform,
        FGTTransformByTwoPoints,
        FGTTransformByDirAndDis,
        FGTRotate,
        FGTScale,
        FGTMirror,
        FGTPattern,
        FGTRectangularPattern,
        FGTCircularPattern,
        FGTCurveSplitter,
        FGTSurfaceSplitter,
        FGTSolidSplitter,
        FGTStiffener,
        FGTCircularHole,
        FGTPart,
        FGTBody,
        FGTInstance,
        FGTRemoveFace,
        FGTExtendFace,
        FGTRepairFace,
        FGTStitch,
        FGTSampleCurve,
        FGTNearestPointLine,
        FGTExtractCenter,
        FGTModelFromMesh,
        FGTCoverEdges,
        FGTRepairSmallEdges,
        FGTRepairSmallFaces,
        FGTPartitionEdgeWithParameter,
        FGTPartitionEdgeWithPoint,
        FGTPartitionEdgeWithDatumPlane,
        FGTPartitionFaceWithSketch,
        FGTPartitionFaceWithTwoPoints,
        FGTPartitionFaceWithDatumPlane,
        FGTPartitionFaceWithCurvedPath,
        FGTPartitionFaceWithExtendFace,
        FGTPartitionFaceWithIntersectFace,
        FGTPartitionFaceWithProjectEdges,
        FGTPartitionSolidWithPlane,
        FGTPartitionSolidWithExtendFace,
        FGTPartitionSolidWithSweepEdge,
        FGTPartitionSolidWithNSidedPatch,
        FGTPartitionSolidWithSketchPlanar,
        FGTSketchDeleter,
        FGTSketchFillet,
        FGTReplaceFaces,
        FGTVirtualTopoCombineFaces,
        FGTVirtualTopoCombineEdges,
        FGTModifyCylinder,
        FGTTransformFace,
        FGTRotateFace,
        FGTModifyBlend,
    };
    Q_ENUM(FITKGeometryComType);

    enum FITKConstructionType
    {
        FCTNone = -1,
        FCTLine,
        FCTCircle
    };
    Q_ENUM(FITKConstructionType);

    enum FITKFeatureType
    {
        FeatureTypeNone = 0,
        FeatureTypeChamfer = 1,
        FeatureTypeCutOut = 2,
        FeatureTypeFillet = 4,
        FeatureTypeHole = 8,
        FeatureTypeLogo = 16,
        FeatureTypeNotch = 32,
        FeatureTypePad = 64,
        FeatureTypePocket = 128,
        FeatureTypeSlot = 256,
        FeatureTypeWallAndBend = 512,
        FeatureTypeAll = 1024,
    };
    Q_ENUM(FITKFeatureType);

    enum DatumPlnType
    {
        DPT_Principal_XOY = 0,
        DPT_Principal_YOZ,
        DPT_Principal_ZOX,
        DPT_UserDefine
    };

    enum DatumAxisType
    {
        DAT_Principal_X = 0,
        DAT_Principal_Y,
        DAT_Principal_Z,
        DAT_UserDefine
    };

private:
    FCGeoEnum() = default;
};

} // namespace FC

#endif // FCGEOENUM_H
