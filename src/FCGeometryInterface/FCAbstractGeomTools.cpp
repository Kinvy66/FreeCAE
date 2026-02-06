/**
 * @file FCAbstractGeomTools.cpp
 * @brief 几何查询工具桩实现
 */
#include "FCAbstractGeomTools.h"

namespace FC {

FCAbstractGeomPointTool* FCAbstractGeomPointTool::createTool()
{
    return new FCAbstractGeomPointTool;
}
bool FCAbstractGeomPointTool::getXYZByID(const int&, double*, int) { return false; }
bool FCAbstractGeomPointTool::getXYZ(FCAbsVirtualTopo*, double*, int) { return false; }

FCAbstractGeomEdgeTool* FCAbstractGeomEdgeTool::createTool()
{
    return new FCAbstractGeomEdgeTool;
}
bool FCAbstractGeomEdgeTool::getStartPointDirectionByID(const int&, double*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::getEndPointDirectionByID(const int&, double*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::getLengthByID(const int&, double&) { return false; }
bool FCAbstractGeomEdgeTool::getMidPointByID(const int&, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::isLineByID(const int&) { return false; }

FCAbstractGeomFaceTool* FCAbstractGeomFaceTool::createTool()
{
    return new FCAbstractGeomFaceTool;
}
bool FCAbstractGeomFaceTool::getAreaByID(const int&, double&) { return false; }
bool FCAbstractGeomFaceTool::getPlaneByID(const int&, double*, double*, double*, int) { return false; }
bool FCAbstractGeomFaceTool::getNormalByID(const int&, double*, double*, int) { return false; }
bool FCAbstractGeomFaceTool::isPlaneByID(const int&) { return false; }

FCAbstractGeomCommonTool* FCAbstractGeomCommonTool::createTool()
{
    return new FCAbstractGeomCommonTool;
}
bool FCAbstractGeomCommonTool::getDistanceByTopoIDs(int, int, double&, double[3], double[3]) { return false; }
bool FCAbstractGeomCommonTool::getFaceSamplePointsByID(int, QList<QList<double>>&, int, int) { return false; }

FCAbstractGeomFeatureTool* FCAbstractGeomFeatureTool::createTool()
{
    return new FCAbstractGeomFeatureTool;
}
bool FCAbstractGeomFeatureTool::getFeatures(int, FCGeoEnum::FITKFeatureType, QList<QList<FCAbsVirtualTopo*>>&) { return false; }

FCAbstractGeomPointTool* FCAbstractGeomToolsCreator::createPointTool() { return FCAbstractGeomPointTool::createTool(); }
FCAbstractGeomEdgeTool* FCAbstractGeomToolsCreator::createEdgeTool() { return FCAbstractGeomEdgeTool::createTool(); }
FCAbstractGeomFaceTool* FCAbstractGeomToolsCreator::createFaceTool() { return FCAbstractGeomFaceTool::createTool(); }
FCAbstractGeomCommonTool* FCAbstractGeomToolsCreator::createCommonTool() { return FCAbstractGeomCommonTool::createTool(); }
FCAbstractGeomFeatureTool* FCAbstractGeomToolsCreator::createFeatureTool() { return FCAbstractGeomFeatureTool::createTool(); }

} // namespace FC
