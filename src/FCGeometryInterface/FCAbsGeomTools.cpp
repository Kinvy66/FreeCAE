/**
 * @file FCAbstractGeomTools.cpp
 * @brief 几何查询工具集实现
 */
#include "FCAbsGeomTools.h"

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
bool FCAbstractGeomEdgeTool::getStartPointDirection(FCAbsVirtualTopo*, double*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::getEndPointDirection(FCAbsVirtualTopo*, double*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::getDirection(FCAbsVirtualTopo*, double*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::getCoedgeDirection(FCAbsVirtualTopo*, FCAbsVirtualTopo*, double*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::isPosOnEdge(FCAbsVirtualTopo*, double*) { return false; }
bool FCAbstractGeomEdgeTool::isLine(FCAbsVirtualTopo*) { return false; }
bool FCAbstractGeomEdgeTool::getLength(FCAbsVirtualTopo*, double&) { return false; }
bool FCAbstractGeomEdgeTool::getMidPoint(FCAbsVirtualTopo*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::getMidPointDirection(FCAbsVirtualTopo*, double*, double*, int) { return false; }
bool FCAbstractGeomEdgeTool::getFeaturePoints(FCAbsVirtualTopo*, QList<QList<double>>&, bool, int) { return false; }
bool FCAbstractGeomEdgeTool::evalPoint(FCAbsVirtualTopo*, double, double*, bool, int) { return false; }
QList<int> FCAbstractGeomEdgeTool::getFreeEdgeIDs(int) { return {}; }
QList<int> FCAbstractGeomEdgeTool::getEdgesOnPlane(int, double*, double*) { return {}; }

FCAbstractGeomFaceTool* FCAbstractGeomFaceTool::createTool()
{
    return new FCAbstractGeomFaceTool;
}
bool FCAbstractGeomFaceTool::getAreaByID(const int&, double&) { return false; }
bool FCAbstractGeomFaceTool::getPlaneByID(const int&, double*, double*, double*, int) { return false; }
bool FCAbstractGeomFaceTool::getNormalByID(const int&, double*, double*, int) { return false; }
bool FCAbstractGeomFaceTool::isPlaneByID(const int&) { return false; }
bool FCAbstractGeomFaceTool::getArea(FCAbsVirtualTopo*, double&) { return false; }
bool FCAbstractGeomFaceTool::getMeanCurvatureAtPos(FCAbsVirtualTopo*, double*, double&) { return false; }
bool FCAbstractGeomFaceTool::getCurvatureAtPos(FCAbsVirtualTopo*, double*, double&, double&, double*, double*) { return false; }
bool FCAbstractGeomFaceTool::getPlane(FCAbsVirtualTopo*, double*, double*, double*, int) { return false; }
bool FCAbstractGeomFaceTool::getNormal(FCAbsVirtualTopo*, double*, double*, int) { return false; }
bool FCAbstractGeomFaceTool::getNormalAtPos(FCAbsVirtualTopo*, double*, double*, int) { return false; }
bool FCAbstractGeomFaceTool::isPlane(FCAbsVirtualTopo*) { return false; }
bool FCAbstractGeomFaceTool::isCylind(FCAbsVirtualTopo*) { return false; }
bool FCAbstractGeomFaceTool::getCylindSurf(FCAbsVirtualTopo*, double*, double*) { return false; }
bool FCAbstractGeomFaceTool::isCone(FCAbsVirtualTopo*) { return false; }
bool FCAbstractGeomFaceTool::getConeSurf(FCAbsVirtualTopo*, double*, double*) { return false; }
bool FCAbstractGeomFaceTool::isCoplanar(QList<int>) { return false; }
bool FCAbstractGeomFaceTool::isSameSurface(QList<int>) { return false; }

FCAbstractGeomCommonTool* FCAbstractGeomCommonTool::createTool()
{
    return new FCAbstractGeomCommonTool;
}
bool FCAbstractGeomCommonTool::getDistanceByTopoIDs(int, int, double&, double[3], double[3]) { return false; }
bool FCAbstractGeomCommonTool::getFaceSamplePointsByID(int, QList<QList<double>>&, int, int) { return false; }
bool FCAbstractGeomCommonTool::getDistanceByTopos(FCAbsVirtualTopo*, FCAbsVirtualTopo*, double&, double[3], double[3]) { return false; }
bool FCAbstractGeomCommonTool::getDistanceByTopoAndPos(FCAbsVirtualTopo*, double[3], double&, double[3]) { return false; }
void FCAbstractGeomCommonTool::canCheckTopos(bool&, bool&) {}
bool FCAbstractGeomCommonTool::getInvalidTopos(int, QList<VirtualShape>&, bool) { return false; }
bool FCAbstractGeomCommonTool::getImpreciseTopos(int, QList<VirtualShape>&) { return false; }
bool FCAbstractGeomCommonTool::calcCircleInfoBy3Points(QVector<double>, QVector<double>, QVector<double>, QVector<double>&, QVector<double>&, double&) { return false; }
bool FCAbstractGeomCommonTool::getFaceSamplePoints(FCAbsVirtualTopo*, QList<QList<double>>&, int, int) { return false; }

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
