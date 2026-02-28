/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCGeoCompRegister.cpp
 * @brief OCC 几何命令注册实现（移植自 FITKGeoCompOCC FITKGeoCompOCCInterface）
 */
#include "FCOCCGeoCompRegister.h"
#include "FCOCCModelSimpleShape.h"
#include "FCOCCModelSolid.h"
#include "FCOCCModelCut.h"
#include "FCOCCModelOperSolid.h"
#include "FCOCCModelImport.h"
#include "FCOCCModelExport.h"
#include "FCOCCOperBool.h"
#include "FCOCCNBodyUnite.h"
#include "FCOCCTransformation.h"
#include "FCOCCModelPart.h"
#include "FCOCCModelPoint.h"
#include "FCOCCSplitter.h"
#include "FCOCCModelPartitionEdge.h"
#include "FCOCCModelPartitionFace.h"
#include "FCOCCModelPartitionSolid.h"
#include "FCOCCModelImportFlattenAssembly.h"
#include "FCOCCModelOperLine.h"
#include "FCOCCModelOperFace.h"
#include "FCOCCModelOperImprintSolid.h"
#include "FCOCCModelOperCompound.h"
#include "FCOCCModelStitch.h"
#include "FCOCCModelExportTopos.h"
#include "FCOCCGeoSketchDeleter.h"
#include "FCOCCStiffener.h"
#include "FCOCCModelNearestPointLine.h"
#include "FCOCCModelExtractCenter.h"
#include "FCOCCModelFromMesh.h"
#include "FCOCCModelRepairSmallEdges.h"
#include "FCOCCModelRepairSmallFaces.h"
#include "FCOCCModelCoverEdges.h"
#include "FCOCCModelReplaceFaces.h"
#include "FCOCCModelCurve.h"
#include "FCOCCModelSurface.h"
#include "FCOCCModelSketch2D.h"
#include "FCOCCModelImportedPart.h"
#include "FCOCCSampleCurve.h"
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCGeometryInterface/FCAbsGeoDelete.h>
#include <FCGeometryInterface/FCAbsGeoMultiDelete.h>
#include <FCGeometryInterface/FCAbsConstruction.h>
#include <FCGeometryInterface/FCAbsGeoReferencePoint.h>
#include <FCGeometryInterface/FCAbsGeoReferenceLine.h>
#include <FCGeometryInterface/FCAbsGeoReferencePlane.h>

namespace OCC {

void registerOCCGeometryCommands()
{
    FC::FCGeoInterfaceFactory* fac = FC::FCGeoInterfaceFactory::instance();
    if (!fac) return;

    // 无 OCC 实现：Delete / MultiDelete / Construction
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTDelete, []() { return new FC::FCGeoDelete; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTMultiDelete, []() { return new FC::FCGeoMultiDelete; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTConstruction, []() { return new FC::FCGeoConstruction; });

    // 导入导出
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTImport, []() { return new FCOCCModelImport; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTExport, []() { return new FCOCCModelExport; });

    // 部件
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPart, []() { return new FCOCCModelPart; });

    // 2D 草图
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSketch2D, []() { return new FCOCCModelSketch2D; });

    // 点
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPoint, []() { return new FCOCCModelPoint; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCurveEnd, []() { return new FCOCCModelCurveEnd; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCurveEndDistance, []() { return new FCOCCModelCurveEndDistance; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCurveRadio, []() { return new FCOCCModelCurveRadio; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPointProjectionOnCurve, []() { return new FCOCCModelPointProjectionOnCurve; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTTwoCurveIntersectPoint, []() { return new FCOCCModelTwoCurveIntersectPoint; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPointProjectionOnSurface, []() { return new FCOCCModelPointProjectionOnSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCurveIntersectSurface, []() { return new FCOCCModelCurveIntersectSurface; });

    // 曲线
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTLine, []() { return new FCOCCModelLine; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSegment, []() { return new FCOCCModelSegment; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTWire, []() { return new FCOCCModelWire; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCircle, []() { return new FCOCCModelCircle; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTThreePointsCircle, []() { return new FCOCCModelThreePointsCircle; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCircleArc, []() { return new FCOCCModelCircleArc; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTEllipse, []() { return new FCOCCModelEllipse; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTThreePointsEllipse, []() { return new FCOCCModelThreePointsEllipse; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTEllipseArc, []() { return new FCOCCModelEllipseArc; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTHyperbola, []() { return new FCOCCModelHyperbola; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTThreePointsHyperbola, []() { return new FCOCCModelThreePointsHyperbola; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTHyperbolaArc, []() { return new FCOCCModelHyperbolaArc; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTParabola, []() { return new FCOCCModelParabola; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTThreePointsParabola, []() { return new FCOCCModelThreePointsParabola; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTParabolaArc, []() { return new FCOCCModelParabolaArc; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTBezierByControlPoints, []() { return new FCOCCModelBezierByControlPoints; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTBSplineByThroughPoints, []() { return new FCOCCModelBsplineByThroughPoints; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTTrimmed, []() { return new FCOCCModelTrimmedCurve; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTOffsetCurve, []() { return new FCOCCModelOffsetCurve; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCurveProjectionOnSurface, []() { return new FCOCCModelCurveProjectionOnSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSurfaceIntersectSurface, []() { return new FCOCCModelSurfaceIntersectionSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTBridgeCurve, []() { return new FCOCCModelBridgeCurve; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCurveFromSurface, []() { return new FCOCCModelSurfaceEdge; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSampleCurve, []() { return new FCOCCSampleCurve; });

    // 曲面
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTClosedWireSurface, []() { return new FCOCCModelClosedWireSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTOffsetSurface, []() { return new FCOCCModelOffsetSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTExtrudeSurface, []() { return new FCOCCModelExtrudeSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRevolSurface, []() { return new FCOCCModelRevolSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSweepSurface, []() { return new FCOCCModelSweepSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTMultiSectionSurface, []() { return new FCOCCModelMultiSectionSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTBridgeSurface, []() { return new FCOCCModelBridgeSurface; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSurfaceFromSolid, []() { return new FCOCCModelSolidSurface; });

    // 导入部件
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTImportPart, []() { return new FCOCCModelImportedPart; });

    // 基本体
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTBox, []() { return new FCOCCModelBox; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCylinder, []() { return new FCOCCModelCylinder; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSphere, []() { return new FCOCCModelSphere; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCone, []() { return new FCOCCModelCone; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTTorus, []() { return new FCOCCModelTorus; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTHelix, []() { return new FCOCCModelHelix; });

    // 布尔与多体
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTBool, []() { return new FCOCCOperBool; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTNBodyUnite, []() { return new FCOCCNBodyUnite; });

    // 实体
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTClosedSurfaceSolid, []() { return new FCOCCModelClosedSurfaceSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTExtrudeSolid, []() { return new FCOCCModelExtrudeSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRevolSolid, []() { return new FCOCCModelRevolSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSweepSolid, []() { return new FCOCCModelSweepSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTMultiSectionSolid, []() { return new FCOCCModelMultiSectionSolid; });

    // 切削
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTExtrudeCut, []() { return new FCOCCModelExtrudeCut; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRevolCut, []() { return new FCOCCModelRevolCut; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSweepCut, []() { return new FCOCCModelSweepCut; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTMultiSectionCut, []() { return new FCOCCModelMultiSectionCut; });

    // 体特征
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTChamferSolid, []() { return new FCOCCModelChamferSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTFilletSolid, []() { return new FCOCCModelFilletSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTDefeature, []() { return new FCOCCModelDefeatureSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRemoveChamfer, []() { return new FCOCCModelRemoveChamferSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRemoveFillet, []() { return new FCOCCModelRemoveFilletSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTFillHole, []() { return new FCOCCModelFillHoleSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCircularHole, []() { return new FCOCCModelCircularHole; });

    // 变换（FCOCCTransformation，移植自 FITKOCCTransformation）
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTTransform, []() { return new FCOCCModelTransform; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTTransformByTwoPoints, []() { return new FCOCCModelTransformByTwoPoints; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTTransformByDirAndDis, []() { return new FCOCCModelTransformByDirAndDis; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRotate, []() { return new FCOCCModelRotate; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTScale, []() { return new FCOCCModelScale; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTMirror, []() { return new FCOCCModelMirror; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRectangularPattern, []() { return new FCOCCModelRectangularPattern; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCircularPattern, []() { return new FCOCCModelCircularPattern; });

    // 分割（FCOCCSplitter，移植自 FITKOCCSplitter）
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCurveSplitter, []() { return new FCOCCCurveSplitter; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSurfaceSplitter, []() { return new FCOCCSurfaceSplitter; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSolidSplitter, []() { return new FCOCCSolidSplitter; });

    // 边分块（FCOCCModelPartitionEdge，移植自 FITKOCCModelPartitionEdge）
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionEdgeWithParameter, []() { return new FCOCCModelPartitionEdgeWithParameter; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionEdgeWithPoint, []() { return new FCOCCModelPartitionEdgeWithPoint; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionEdgeWithDatumPlane, []() { return new FCOCCModelPartitionEdgeWithDatumPlane; });

    // 面分块（FCOCCModelPartitionFace，移植自 FITKOCCModelPartitionFace）
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionFaceWithSketch, []() { return new FCOCCModelPartitionFaceWithSketch; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionFaceWithTwoPoints, []() { return new FCOCCModelPartitionFaceWithTwoPoints; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionFaceWithDatumPlane, []() { return new FCOCCModelPartitionFaceWithDatumPlane; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionFaceWithCurvedPath, []() { return new FCOCCModelPartitionFaceWithCurvedPath; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionFaceWithExtendFace, []() { return new FCOCCModelPartitionFaceWithExtendFace; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionFaceWithIntersectFace, []() { return new FCOCCModelPartitionFaceWithIntersectFace; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionFaceWithProjectEdges, []() { return new FCOCCModelPartitionFaceWithProjectEdges; });

    // 体分块（FCOCCModelPartitionSolid，移植自 FITKOCCModelPartitionSolid）
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionSolidWithPlane, []() { return new FCOCCModelPartitionSolidWithPlane; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionSolidWithExtendFace, []() { return new FCOCCModelPartitionSolidWithExtendFace; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionSolidWithSweepEdge, []() { return new FCOCCModelPartitionSolidWithSweepEdge; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTPartitionSolidWithSketchPlanar, []() { return new FCOCCModelPartitionSolidWithSketchPlanar; });

    // 其余 OCC 命令（ImportFlattenAssembly / OperLine / OperFace / OperImprintSolid / OperCompound / Stitch / ExportTopos / SketchDeleter / Stiffener / NearestPointLine / ExtractCenter / FromMesh / Repair* / CoverEdges / ReplaceFaces，各见对应 .h）
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTImportFlattenAssembly, []() { return new FCOCCModelImportFlattenAssembly; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTMergeLine, []() { return new FCOCCModelOperLine; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRemoveFace, []() { return new FCOCCModelOperFaceRemoveFace; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTFillGapsFace, []() { return new FCOCCModelOperFaceFillGaps; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTFillHolesFace, []() { return new FCOCCModelOperFaceFillHoles; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTExtendFace, []() { return new FCOCCModelExtendFace; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTDeleteFloatingEdge, []() { return new FCOCCModelOperFaceDeleteFloatingEdge; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTImprintSolid, []() { return new FCOCCModelOperImprintSolid; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCompoundAppendShape, []() { return new FCOCCModelOperAddCompound; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCompoundRemoveShape, []() { return new FCOCCModelOperRemoveCompound; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSplitByMiddlePositionLine, []() { return new FCOCCModelMiddleOperLine; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTStitch, []() { return new FCOCCModelStitch; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTExportTopos, []() { return new FCOCCModelExportTopos; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTSketchDeleter, []() { return new FCOCCGeoSketchDeleter; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTStiffener, []() { return new FCOCCStiffener; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTNearestPointLine, []() { return new FCOCCModelNearestPointLine; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTExtractCenter, []() { return new FCOCCModelExtractCenter; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTModelFromMesh, []() { return new FCOCCModelFromMesh; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTInteractionPoint, []() { return new FCOCCModelInteractionPoint; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRepairSmallEdges, []() { return new FCOCCModelRepairSmallEdges; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTRepairSmallFaces, []() { return new FCOCCModelRepairSmallFaces; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTCoverEdges, []() { return new FCOCCModelCoverEdges; });
    fac->insertCommandCreateFun(FC::FCGeoEnum::FGTReplaceFaces, []() { return new FCOCCModelReplaceFaces; });

    // 基准（使用 FC 层实现，供 OCC 等引擎通过 createDatum 创建）
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTPoint, []() { return new FC::FCGeoReferencePoint; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTCurveRadio, []() { return new FC::FCGeoReferenceCurveRadio; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTLine, []() { return new FC::FCGeoReferenceLine; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTSegment, []() { return new FC::FCGeoReferenceSegment; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTLineByIntersectPlanes, []() { return new FC::FCGeoReferenceLineByIntersectPlanes; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTLineByRotateLine, []() { return new FC::FCGeoReferenceLineByRotateLine; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTPlane, []() { return new FC::FCGeoReferencePlane; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTOffsetPlane, []() { return new FC::FCGeoReferenceOffsetPlane; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTThreePointsPlane, []() { return new FC::FCGeoReferenceThreePointsPlane; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTEquationPlane, []() { return new FC::FCGeoReferenceEquationPlane; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTPointAndDirectionPlane, []() { return new FC::FCGeoReferencePointAndDirectionPlane; });
    fac->insertDatumCreateFun(FC::FCGeoEnum::FDTRotateFromPlanePlane, []() { return new FC::FCGeoReferenceRotateFromPlanePlane; });
}

} // namespace OCC
