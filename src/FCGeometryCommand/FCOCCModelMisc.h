/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelMisc.h
 * @brief OCC 其余几何命令桩实现（移植自 FITKGeoCompOCC，update 暂为 stub）
 */
#ifndef FCOCCMODELMISC_H
#define FCOCCMODELMISC_H

#include "FCAbstractOCCModel.h"
#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoModelImportFlattenAssembly.h>
#include <FCGeometryInterface/FCAbsGeoModelOperLine.h>
#include <FCGeometryInterface/FCAbsGeoModelOperFace.h>
#include <FCGeometryInterface/FCAbsGeoModelOperImprintSolid.h>
#include <FCGeometryInterface/FCAbsGeoModelOperCompound.h>
#include <FCGeometryInterface/FCAbsGeoModelPartitionEdge.h>
#include <FCGeometryInterface/FCAbsGeoModelPartitionFace.h>
#include <FCGeometryInterface/FCAbsGeoModelPartitionSolid.h>
#include <FCGeometryInterface/FCAbsGeoStitch.h>
#include <FCGeometryInterface/FCAbsGeoModelExportTopos.h>
#include <FCGeometryInterface/FCAbsGeoSketchDeleter.h>
#include <FCGeometryInterface/FCAbsGeoTransformation.h>
#include <FCGeometryInterface/FCAbsGeoSplitter.h>
#include <FCGeometryInterface/FCGeoStiffener.h>
#include <FCGeometryInterface/FCAbsGeoModelNearestPointLine.h>
#include <FCGeometryInterface/FCAbsGeoModelExtractCenter.h>
#include <FCGeometryInterface/FCAbsGeoModelFromMesh.h>
#include <FCGeometryInterface/FCAbsGeoRepairSmallEdges.h>
#include <FCGeometryInterface/FCAbsGeoRepairSmallFaces.h>
#include <FCGeometryInterface/FCAbsGeoCoverEdges.h>
#include <FCGeometryInterface/FCAbsGeoReplaceFaces.h>

namespace OCC {

class FCGEOMETRYCMD_API FCOCCModelImportFlattenAssembly : public FC::FCGeoModelImportFlattenAssembly, public OCCShapeAgent {
public: FCOCCModelImportFlattenAssembly() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelImportFlattenAssembly() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperLine : public FC::FCGeoModelOperLine, public OCCShapeAgent {
public: FCOCCModelOperLine() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperLine() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperFaceRemoveFace : public FC::FCGeoModelOperFaceRemoveFace, public OCCShapeAgent {
public: FCOCCModelOperFaceRemoveFace() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperFaceRemoveFace() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperFaceFillGaps : public FC::FCGeoModelOperFace, public OCCShapeAgent {
public: FCOCCModelOperFaceFillGaps() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperFaceFillGaps() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperFaceDeleteFloatingEdge : public FC::FCGeoModelOperFace, public OCCShapeAgent {
public: FCOCCModelOperFaceDeleteFloatingEdge() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperFaceDeleteFloatingEdge() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperImprintSolid : public FC::FCGeoModelOperImprintSolid, public OCCShapeAgent {
public: FCOCCModelOperImprintSolid() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperImprintSolid() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperAddCompound : public FC::FCGeoModelOperCompound, public OCCShapeAgent {
public: FCOCCModelOperAddCompound() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperAddCompound() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelOperRemoveCompound : public FC::FCGeoModelOperCompound, public OCCShapeAgent {
public: FCOCCModelOperRemoveCompound() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelOperRemoveCompound() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelMiddleOperLine : public FC::FCGeoModelOperLine, public OCCShapeAgent {
public: FCOCCModelMiddleOperLine() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelMiddleOperLine() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelStitch : public FC::FCGeoStitch, public OCCShapeAgent {
public: FCOCCModelStitch() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelStitch() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelExportTopos : public FC::FCGeoModelExportTopos, public OCCShapeAgent {
public: FCOCCModelExportTopos() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelExportTopos() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCGeoSketchDeleter : public FC::FCGeoSketchDeleter, public OCCShapeAgent {
public: FCOCCGeoSketchDeleter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCGeoSketchDeleter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRectangularPattern : public FC::FCGeoModelRectangularPattern, public OCCShapeAgent {
public: FCOCCModelRectangularPattern() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelRectangularPattern() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelCircularPattern : public FC::FCGeoModelCircularPattern, public OCCShapeAgent {
public: FCOCCModelCircularPattern() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelCircularPattern() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCCurveSplitter : public FC::FCGeoCurveSplitter, public OCCShapeAgent {
public: FCOCCCurveSplitter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCCurveSplitter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCSurfaceSplitter : public FC::FCGeoSurfaceSplitter, public OCCShapeAgent {
public: FCOCCSurfaceSplitter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCSurfaceSplitter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCSolidSplitter : public FC::FCGeoSolidSplitter, public OCCShapeAgent {
public: FCOCCSolidSplitter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCSolidSplitter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCStiffener : public FC::FCGeoStiffener, public OCCShapeAgent {
public: FCOCCStiffener() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCStiffener() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelNearestPointLine : public FC::FCGeoModelNearestPointLine, public OCCShapeAgent {
public: FCOCCModelNearestPointLine() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelNearestPointLine() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelExtractCenter : public FC::FCGeoModelExtractCenter, public OCCShapeAgent {
public: FCOCCModelExtractCenter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelExtractCenter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelFromMesh : public FC::FCGeoModelFromMesh, public OCCShapeAgent {
public: FCOCCModelFromMesh() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelFromMesh() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRepairSmallEdges : public FC::FCGeoRepairSmallEdges, public OCCShapeAgent {
public: FCOCCModelRepairSmallEdges() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelRepairSmallEdges() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelRepairSmallFaces : public FC::FCGeoRepairSmallFaces, public OCCShapeAgent {
public: FCOCCModelRepairSmallFaces() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelRepairSmallFaces() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelCoverEdges : public FC::FCGeoCoverEdges, public OCCShapeAgent {
public: FCOCCModelCoverEdges() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelCoverEdges() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelReplaceFaces : public FC::FCGeoReplaceFaces, public OCCShapeAgent {
public: FCOCCModelReplaceFaces() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelReplaceFaces() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionEdgeWithParameter : public FC::FCGeoModelPartitionEdgeWithParameter, public OCCShapeAgent {
public: FCOCCModelPartitionEdgeWithParameter() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionEdgeWithParameter() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionEdgeWithPoint : public FC::FCGeoModelPartitionEdgeWithPoint, public OCCShapeAgent {
public: FCOCCModelPartitionEdgeWithPoint() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionEdgeWithPoint() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionEdgeWithDatumPlane : public FC::FCGeoModelPartitionEdgeWithDatumPlane, public OCCShapeAgent {
public: FCOCCModelPartitionEdgeWithDatumPlane() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionEdgeWithDatumPlane() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithSketch : public FC::FCGeoModelPartitionFaceWithSketch, public OCCShapeAgent {
public: FCOCCModelPartitionFaceWithSketch() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionFaceWithSketch() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithTwoPoints : public FC::FCGeoModelPartitionFaceWithTwoPoints, public OCCShapeAgent {
public: FCOCCModelPartitionFaceWithTwoPoints() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionFaceWithTwoPoints() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithDatumPlane : public FC::FCGeoModelPartitionFaceWithDatumPlane, public OCCShapeAgent {
public: FCOCCModelPartitionFaceWithDatumPlane() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionFaceWithDatumPlane() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithCurvedPath : public FC::FCGeoModelPartitionFaceWithCurvedPath, public OCCShapeAgent {
public: FCOCCModelPartitionFaceWithCurvedPath() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionFaceWithCurvedPath() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithExtendFace : public FC::FCGeoModelPartitionFaceWithExtendFace, public OCCShapeAgent {
public: FCOCCModelPartitionFaceWithExtendFace() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionFaceWithExtendFace() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithIntersectFace : public FC::FCGeoModelPartitionFaceWithIntersectFace, public OCCShapeAgent {
public: FCOCCModelPartitionFaceWithIntersectFace() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionFaceWithIntersectFace() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionFaceWithProjectEdges : public FC::FCGeoModelPartitionFaceWithProjectEdges, public OCCShapeAgent {
public: FCOCCModelPartitionFaceWithProjectEdges() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionFaceWithProjectEdges() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithPlane : public FC::FCGeoModelPartitionSolidWithPlane, public OCCShapeAgent {
public: FCOCCModelPartitionSolidWithPlane() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionSolidWithPlane() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithExtendFace : public FC::FCGeoModelPartitionSolidWithExtendFace, public OCCShapeAgent {
public: FCOCCModelPartitionSolidWithExtendFace() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionSolidWithExtendFace() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithSweepEdge : public FC::FCGeoModelPartitionSolidWithSweepEdge, public OCCShapeAgent {
public: FCOCCModelPartitionSolidWithSweepEdge() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionSolidWithSweepEdge() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

class FCGEOMETRYCMD_API FCOCCModelPartitionSolidWithSketchPlanar : public FC::FCGeoModelPartitionSolidWithSketchPlanar, public OCCShapeAgent {
public: FCOCCModelPartitionSolidWithSketchPlanar() : OCCShapeAgent(this) { _shapeAgent = _occShapeAgent; }
    ~FCOCCModelPartitionSolidWithSketchPlanar() override = default;
    FC::FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace OCC

#endif // FCOCCMODELMISC_H
