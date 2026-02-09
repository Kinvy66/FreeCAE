/**
 * @file FCAbstractGeomTools.h
 * @brief 几何工具集接口（移植自 FITKInterfaceGeometry FITKAbsGeomTools）
 */
#ifndef FCABSTRACTGEOMTOOLS_H
#define FCABSTRACTGEOMTOOLS_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoEnum.h"
#include "FCAbsGeoCommand.h"
#include "FCAbsVirtualTopo.h"
#include <FCData/FCAbstractDataObject.h>
#include <QList>
#include <QVector>

namespace FC {

/** 点工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomPointTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomPointTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomPointTool() = default;
    static FCAbstractGeomPointTool* createTool();
    virtual bool getXYZByID(const int& id, double* oXyz, int modelId = -1);
    virtual bool getXYZ(FCAbsVirtualTopo* vtp, double* oXyz, int modelId = -1);
};

/** 边工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomEdgeTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomEdgeTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomEdgeTool() = default;
    static FCAbstractGeomEdgeTool* createTool();
    virtual bool getStartPointDirectionByID(const int& id, double* oPos, double* oDir, int iModelId = -1);
    virtual bool getEndPointDirectionByID(const int& id, double* oPos, double* oDir, int iModelId = -1);
    virtual bool getLengthByID(const int& id, double& oLen);
    virtual bool getMidPointByID(const int& id, double* oPos, int iModelId = -1);
    virtual bool isLineByID(const int& id);
    // vtp-based API
    virtual bool getStartPointDirection(FCAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1);
    virtual bool getEndPointDirection(FCAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1);
    virtual bool getDirection(FCAbsVirtualTopo* vtp, double* iPos, double* oDir, int iModelId = -1);
    virtual bool getCoedgeDirection(FCAbsVirtualTopo* vtpFace, FCAbsVirtualTopo* vtpEdge, double* iPos, double* oDir, int iModelId = -1);
    virtual bool isPosOnEdge(FCAbsVirtualTopo* vtp, double* iPos);
    virtual bool isLine(FCAbsVirtualTopo* vtp);
    virtual bool getLength(FCAbsVirtualTopo* vtp, double& oLen);
    virtual bool getMidPoint(FCAbsVirtualTopo* vtp, double* oPos, int iModelId = -1);
    virtual bool getMidPointDirection(FCAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1);
    virtual bool getFeaturePoints(FCAbsVirtualTopo* vtp, QList<QList<double>>& oPoses, bool skipMidPt = false, int iModelId = -1);
    virtual bool evalPoint(FCAbsVirtualTopo* vtp, double iParam, double* oPos, bool iNormalized = false, int iModelId = -1);
    virtual QList<int> getFreeEdgeIDs(int commandId);
    virtual QList<int> getEdgesOnPlane(int cmdId, double* iOrg, double* iNor);
};

/** 面工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomFaceTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomFaceTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomFaceTool() = default;
    static FCAbstractGeomFaceTool* createTool();
    virtual bool getAreaByID(const int& id, double& oArea);
    virtual bool getPlaneByID(const int& id, double* oPos, double* oNormal, double* oUp, int iModelId = -1);
    virtual bool getNormalByID(const int& id, double* oPos, double* oNormal, int iModelId = -1);
    virtual bool isPlaneByID(const int& id);
    // vtp-based API
    virtual bool getArea(FCAbsVirtualTopo* vtp, double& oArea);
    virtual bool getMeanCurvatureAtPos(FCAbsVirtualTopo* vtp, double* iPos, double& oCur);
    virtual bool getCurvatureAtPos(FCAbsVirtualTopo* vtp, double* iPos, double& oCur1, double& oCur2, double* oDir1, double* oDir2);
    virtual bool getPlane(FCAbsVirtualTopo* vtp, double* oPos, double* oNor, double* oUp, int iModelId = -1);
    virtual bool getNormal(FCAbsVirtualTopo* vtp, double* oPos, double* oNormal, int iModelId = -1);
    virtual bool getNormalAtPos(FCAbsVirtualTopo* vtp, double* iPos, double* oNormal, int iModelId = -1);
    virtual bool isPlane(FCAbsVirtualTopo* vtp);
    virtual bool isCylind(FCAbsVirtualTopo* vtp);
    virtual bool getCylindSurf(FCAbsVirtualTopo* vtp, double* oCt, double* oDir);
    virtual bool isCone(FCAbsVirtualTopo* vtp);
    virtual bool getConeSurf(FCAbsVirtualTopo* vtp, double* oCt, double* oDir);
    virtual bool isCoplanar(QList<int> ids);
    virtual bool isSameSurface(QList<int> ids);
};

/** 通用工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomCommonTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomCommonTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomCommonTool() = default;
    static FCAbstractGeomCommonTool* createTool();
    virtual bool getDistanceByTopoIDs(int vTopoId_1, int vTopoId_2, double& oDist, double oPos_1[3], double oPos_2[3]);
    virtual bool getFaceSamplePointsByID(int vTopoId, QList<QList<double>>& oPoses, int iSpacing = 5, int iModelId = -1);
    // vtp-based API
    virtual bool getDistanceByTopos(FCAbsVirtualTopo* vtp_1, FCAbsVirtualTopo* vtp_2, double& oDist, double oPos_1[3], double oPos_2[3]);
    virtual bool getDistanceByTopoAndPos(FCAbsVirtualTopo* vtp, double iPos3[3], double& oDist, double oPos[3]);
    virtual void canCheckTopos(bool& oInvalid, bool& oImprecise);
    virtual bool getInvalidTopos(int cmdId, QList<VirtualShape>& oTopos, bool warnings = false);
    virtual bool getImpreciseTopos(int cmdId, QList<VirtualShape>& oTopos);
    virtual bool calcCircleInfoBy3Points(QVector<double> pt1, QVector<double> pt2, QVector<double> pt3,
        QVector<double>& center, QVector<double>& normal, double& radius);
    virtual bool getFaceSamplePoints(FCAbsVirtualTopo* vtp, QList<QList<double>>& oPoses, int iSpacing = 5, int iModelId = -1);
};

/** 特征工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomFeatureTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomFeatureTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomFeatureTool() = default;
    static FCAbstractGeomFeatureTool* createTool();
    virtual bool getFeatures(int comId, FCGeoEnum::FITKFeatureType featureType, QList<QList<FCAbsVirtualTopo*>>& oFeatures);
};

/** 几何工具创建器 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomToolsCreator
{
public:
    explicit FCAbstractGeomToolsCreator() = default;
    virtual ~FCAbstractGeomToolsCreator() = 0;
    virtual FCAbstractGeomPointTool* createPointTool();
    virtual FCAbstractGeomEdgeTool* createEdgeTool();
    virtual FCAbstractGeomFaceTool* createFaceTool();
    virtual FCAbstractGeomCommonTool* createCommonTool();
    virtual FCAbstractGeomFeatureTool* createFeatureTool();
};

inline FCAbstractGeomToolsCreator::~FCAbstractGeomToolsCreator() {}

} // namespace FC

#endif // FCABSTRACTGEOMTOOLS_H
