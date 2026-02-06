/**
 * @file FCAbstractGeomTools.h
 * @brief 几何查询工具抽象接口（移植自 FITKInterfaceGeometry FITKAbsGeomTools，接口层桩实现）
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

/** 点查询工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomPointTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomPointTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomPointTool() = default;
    static FCAbstractGeomPointTool* createTool();
    virtual bool getXYZByID(const int& id, double* oXyz, int modelId = -1);
    virtual bool getXYZ(FCAbsVirtualTopo* vtp, double* oXyz, int modelId = -1);
};

/** 边查询工具 */
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
};

/** 面查询工具 */
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
};

/** 通用查询工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomCommonTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomCommonTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomCommonTool() = default;
    static FCAbstractGeomCommonTool* createTool();
    virtual bool getDistanceByTopoIDs(int vTopoId_1, int vTopoId_2, double& oDist, double oPos_1[3], double oPos_2[3]);
    virtual bool getFaceSamplePointsByID(int vTopoId, QList<QList<double>>& oPoses, int iSpacing = 5, int iModelId = -1);
};

/** 特征查询工具 */
class FCGEOMETRYINTERFACE_API FCAbstractGeomFeatureTool : public FCAbstractDataObject
{
public:
    explicit FCAbstractGeomFeatureTool() : FCAbstractDataObject(-1) {}
    virtual ~FCAbstractGeomFeatureTool() = default;
    static FCAbstractGeomFeatureTool* createTool();
    virtual bool getFeatures(int comId, FCGeoEnum::FITKFeatureType featureType, QList<QList<FCAbsVirtualTopo*>>& oFeatures);
};

/** 几何查询工具创建器 */
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
