/**
 * @file FCGeoModelCurve.h
 * @brief 曲线几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelCurve，接口层）
 */
#ifndef FCGEOMODELCURVE_H
#define FCGEOMODELCURVE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <array>
#include <QList>

namespace FC {

/**
 * @brief 曲线抽象基类（局部坐标系接口）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCurve : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelCurve);
public:
    FCGeoModelCurve() = default;
    ~FCGeoModelCurve() override = default;

    std::array<double, 3> origin() const { return m_origin; }
    void setOrigin(double x, double y, double z);
    void setOrigin(std::array<double, 3> xyz) { m_origin = xyz; }
    std::array<double, 3> xDirection() const { return m_xDirection; }
    std::array<double, 3> yDirection() const { return m_yDirection; }
    void setYDirection(std::array<double, 3> xyz);
    void setYDirection(double x, double y, double z);
    std::array<double, 3> zDirection() const { return m_zDirection; }
    void setZDirection(std::array<double, 3> xyz);
    void setZDirection(double x, double y, double z);
protected:
    std::array<double, 3> m_origin{ { 0, 0, 0 } };
    std::array<double, 3> m_xDirection{ { 1, 0, 0 } };
    std::array<double, 3> m_yDirection{ { 0, 1, 0 } };
    std::array<double, 3> m_zDirection{ { 0, 0, 1 } };
};

/**
 * @brief 无限直线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelLine : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelLine);
public:
    FCGeoModelLine() = default;
    ~FCGeoModelLine() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> dir() const { return m_dir; }
    void setDir(double x, double y, double z);
protected:
    std::array<double, 3> m_dir{};
};

/**
 * @brief 有限线段
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSegment : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelSegment);
public:
    FCGeoModelSegment() = default;
    ~FCGeoModelSegment() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> startPoint() const { return m_startPoint; }
    void setStartPoint(double x, double y, double z);
    void setStartPoint(std::array<double, 3> xyz) { m_startPoint = xyz; }
    std::array<double, 3> endPoint() const { return m_endPoint; }
    void setEndPoint(double x, double y, double z);
    void setEndPoint(std::array<double, 3> xyz) { m_endPoint = xyz; }
protected:
    std::array<double, 3> m_startPoint{};
    std::array<double, 3> m_endPoint{};
};

/**
 * @brief 线串（多段线）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelWire : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelWire);
public:
    FCGeoModelWire() = default;
    ~FCGeoModelWire() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setCurves(const QList<VirtualShape>& curves) { m_curves = curves; }
    QList<VirtualShape> curves() const { return m_curves; }
protected:
    QList<VirtualShape> m_curves;
};

/**
 * @brief 圆
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCircle : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelCircle);
public:
    FCGeoModelCircle() = default;
    ~FCGeoModelCircle() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> center() const { return m_center; }
    void setCenter(double x, double y, double z);
    void setCenter(std::array<double, 3> xyz) { m_center = xyz; }
    double radius() const { return m_radius; }
    void setRadius(double r) { m_radius = r; }
    std::array<double, 3> normal() const { return m_normal; }
    void setNormal(std::array<double, 3> n) { m_normal = n; }
protected:
    std::array<double, 3> m_center{};
    double m_radius{};
    std::array<double, 3> m_normal{ { 0, 0, 1 } };
};

/**
 * @brief 三点圆
 */
class FCGEOMETRYINTERFACE_API FCGeoModelThreePointsCircle : public FCGeoModelCircle
{
    FC_CLASS(FC, FCGeoModelThreePointsCircle);
public:
    FCGeoModelThreePointsCircle() = default;
    ~FCGeoModelThreePointsCircle() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setPoints(const QList<std::array<double, 3>>& points) { m_points = points; }
    QList<std::array<double, 3>> points() const { return m_points; }
protected:
    QList<std::array<double, 3>> m_points;
};

/**
 * @brief 圆弧
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCircleArc : public FCGeoModelCircle
{
    FC_CLASS(FC, FCGeoModelCircleArc);
public:
    FCGeoModelCircleArc() = default;
    ~FCGeoModelCircleArc() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> startPoint() const { return m_startPoint; }
    void setStartPoint(double x, double y, double z);
    void setStartPoint(std::array<double, 3> xyz) { m_startPoint = xyz; }
    std::array<double, 3> endPoint() const { return m_endPoint; }
    void setEndPoint(double x, double y, double z);
    void setEndPoint(std::array<double, 3> xyz) { m_endPoint = xyz; }
protected:
    std::array<double, 3> m_startPoint{};
    std::array<double, 3> m_endPoint{};
};

/**
 * @brief 椭圆
 */
class FCGEOMETRYINTERFACE_API FCGeoModelEllipse : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelEllipse);
public:
    FCGeoModelEllipse() = default;
    ~FCGeoModelEllipse() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    double majorRadius() const { return m_majorRadius; }
    void setMajorRadius(double r) { m_majorRadius = r; }
    double minorRadius() const { return m_minorRadius; }
    void setMinorRadius(double r) { m_minorRadius = r; }
protected:
    double m_majorRadius{};
    double m_minorRadius{};
};

/**
 * @brief 三点椭圆
 */
class FCGEOMETRYINTERFACE_API FCGeoModelThreePointsEllipse : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelThreePointsEllipse);
public:
    FCGeoModelThreePointsEllipse() = default;
    ~FCGeoModelThreePointsEllipse() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> majorPoint() const { return m_majorPoint; }
    void setMajorPoint(double x, double y, double z);
    void setMajorPoint(std::array<double, 3> xyz) { m_majorPoint = xyz; }
    std::array<double, 3> minorPoint() const { return m_minorPoint; }
    void setMinorPoint(double x, double y, double z);
    void setMinorPoint(std::array<double, 3> xyz) { m_minorPoint = xyz; }
protected:
    std::array<double, 3> m_majorPoint{};
    std::array<double, 3> m_minorPoint{};
};

/**
 * @brief 椭圆弧
 */
class FCGEOMETRYINTERFACE_API FCGeoModelEllipseArc : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelEllipseArc);
public:
    FCGeoModelEllipseArc() = default;
    ~FCGeoModelEllipseArc() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    double majorRadius() const { return m_majorRadius; }
    void setMajorRadius(double r) { m_majorRadius = r; }
    double minorRadius() const { return m_minorRadius; }
    void setMinorRadius(double r) { m_minorRadius = r; }
    std::array<double, 3> startPoint() const { return m_startPoint; }
    void setStartPoint(double x, double y, double z);
    void setStartPoint(std::array<double, 3> xyz) { m_startPoint = xyz; }
    std::array<double, 3> endPoint() const { return m_endPoint; }
    void setEndPoint(double x, double y, double z);
    void setEndPoint(std::array<double, 3> xyz) { m_endPoint = xyz; }
protected:
    double m_majorRadius{};
    double m_minorRadius{};
    std::array<double, 3> m_startPoint{};
    std::array<double, 3> m_endPoint{};
};

/**
 * @brief 双曲线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelHyperbola : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelHyperbola);
public:
    FCGeoModelHyperbola() = default;
    ~FCGeoModelHyperbola() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    double majorRadius() const { return m_majorRadius; }
    void setMajorRadius(double r) { m_majorRadius = r; }
    double minorRadius() const { return m_minorRadius; }
    void setMinorRadius(double r) { m_minorRadius = r; }
protected:
    double m_majorRadius{};
    double m_minorRadius{};
};

/**
 * @brief 三点双曲线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelThreePointsHyperbola : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelThreePointsHyperbola);
public:
    FCGeoModelThreePointsHyperbola() = default;
    ~FCGeoModelThreePointsHyperbola() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    std::array<double, 3> majorPoint() const { return m_majorPoint; }
    void setMajorPoint(double x, double y, double z);
    void setMajorPoint(std::array<double, 3> xyz) { m_majorPoint = xyz; }
    std::array<double, 3> minorPoint() const { return m_minorPoint; }
    void setMinorPoint(double x, double y, double z);
    void setMinorPoint(std::array<double, 3> xyz) { m_minorPoint = xyz; }
    double majorRadius() const { return m_majorRadius; }
    void setMajorRadius(double r) { m_majorRadius = r; }
    double minorRadius() const { return m_minorRadius; }
    void setMinorRadius(double r) { m_minorRadius = r; }
protected:
    std::array<double, 3> m_majorPoint{};
    std::array<double, 3> m_minorPoint{};
    double m_majorRadius{};
    double m_minorRadius{};
};

/**
 * @brief 双曲线弧
 */
class FCGEOMETRYINTERFACE_API FCGeoModelHyperbolaArc : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelHyperbolaArc);
public:
    FCGeoModelHyperbolaArc() = default;
    ~FCGeoModelHyperbolaArc() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    double majorRadius() const { return m_majorRadius; }
    void setMajorRadius(double r) { m_majorRadius = r; }
    double minorRadius() const { return m_minorRadius; }
    void setMinorRadius(double r) { m_minorRadius = r; }
    std::array<double, 3> startPoint() const { return m_startPoint; }
    void setStartPoint(double x, double y, double z);
    void setStartPoint(std::array<double, 3> xyz) { m_startPoint = xyz; }
    std::array<double, 3> endPoint() const { return m_endPoint; }
    void setEndPoint(double x, double y, double z);
    void setEndPoint(std::array<double, 3> xyz) { m_endPoint = xyz; }
protected:
    double m_majorRadius{};
    double m_minorRadius{};
    std::array<double, 3> m_startPoint{};
    std::array<double, 3> m_endPoint{};
};

/**
 * @brief 抛物线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelParabola : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelParabola);
public:
    FCGeoModelParabola() = default;
    ~FCGeoModelParabola() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    double focalLength() const { return m_focalLength; }
    void setFocalLength(double f) { m_focalLength = f; }
protected:
    double m_focalLength{};
};

/**
 * @brief 三点抛物线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelThreePointsParabola : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelThreePointsParabola);
public:
    FCGeoModelThreePointsParabola() = default;
    ~FCGeoModelThreePointsParabola() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setPoints(const QList<std::array<double, 3>>& points) { m_points = points; }
    QList<std::array<double, 3>> points() const { return m_points; }
    double focalLength() const { return m_focalLength; }
    void setFocalLength(double f) { m_focalLength = f; }
protected:
    QList<std::array<double, 3>> m_points;
    double m_focalLength{};
};

/**
 * @brief 抛物线弧
 */
class FCGEOMETRYINTERFACE_API FCGeoModelParabolaArc : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelParabolaArc);
public:
    FCGeoModelParabolaArc() = default;
    ~FCGeoModelParabolaArc() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    double focalLength() const { return m_focalLength; }
    void setFocalLength(double f) { m_focalLength = f; }
    std::array<double, 3> startPoint() const { return m_startPoint; }
    void setStartPoint(double x, double y, double z);
    void setStartPoint(std::array<double, 3> xyz) { m_startPoint = xyz; }
    std::array<double, 3> endPoint() const { return m_endPoint; }
    void setEndPoint(double x, double y, double z);
    void setEndPoint(std::array<double, 3> xyz) { m_endPoint = xyz; }
protected:
    double m_focalLength{};
    std::array<double, 3> m_startPoint{};
    std::array<double, 3> m_endPoint{};
};

/**
 * @brief 通过控制点创建贝塞尔曲线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelBezierByControlPoints : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelBezierByControlPoints);
public:
    FCGeoModelBezierByControlPoints() = default;
    ~FCGeoModelBezierByControlPoints() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setControlPoints(const QList<std::array<double, 3>>& points) { m_controlPoints = points; }
    QList<std::array<double, 3>> controlPoints() const { return m_controlPoints; }
protected:
    QList<std::array<double, 3>> m_controlPoints;
};

/**
 * @brief 通过线上的点创建B样条曲线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelBsplineByThroughPoints : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelBsplineByThroughPoints);
public:
    FCGeoModelBsplineByThroughPoints() = default;
    ~FCGeoModelBsplineByThroughPoints() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setThroughPoints(const QList<std::array<double, 3>>& points) { m_throughPoints = points; }
    QList<std::array<double, 3>> throughPoints() const { return m_throughPoints; }
    void setThroughPointShapes(const QList<VirtualShape>& shapes) { m_throughPointShapes = shapes; }
    QList<VirtualShape> throughPointShapes() const { return m_throughPointShapes; }
protected:
    QList<std::array<double, 3>> m_throughPoints;
    QList<VirtualShape> m_throughPointShapes;
};

/**
 * @brief 裁剪曲线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelTrimmedCurve : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelTrimmedCurve);
public:
    FCGeoModelTrimmedCurve() = default;
    ~FCGeoModelTrimmedCurve() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& curve) { m_sourceCurve = curve; }
    int edgeVirtualTopoId() const { return m_edgeVirtualTopoId; }
    void setEdgeVirtualTopoId(int id) { m_edgeVirtualTopoId = id; }
protected:
    VirtualShape m_sourceCurve{};
    int m_edgeVirtualTopoId{ -1 };
};

/**
 * @brief 偏移曲线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelOffsetCurve : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelOffsetCurve);
public:
    FCGeoModelOffsetCurve() = default;
    ~FCGeoModelOffsetCurve() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& curve) { m_sourceCurve = curve; }
    std::array<double, 3> dir() const { return m_dir; }
    void setDir(double x, double y, double z);
    void setDir(std::array<double, 3> d) { m_dir = d; }
    double offset() const { return m_offset; }
    void setOffset(double o) { m_offset = o; }
protected:
    VirtualShape m_sourceCurve{};
    std::array<double, 3> m_dir{};
    double m_offset{};
};

/**
 * @brief 线在面上的投影
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCurveProjectionOnSurface : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelCurveProjectionOnSurface);
public:
    FCGeoModelCurveProjectionOnSurface() = default;
    ~FCGeoModelCurveProjectionOnSurface() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& curve) { m_sourceCurve = curve; }
    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(const VirtualShape& surface) { m_sourceSurface = surface; }
protected:
    VirtualShape m_sourceCurve{};
    VirtualShape m_sourceSurface{};
};

/**
 * @brief 面和面的交线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSurfaceIntersectionSurface : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelSurfaceIntersectionSurface);
public:
    FCGeoModelSurfaceIntersectionSurface() = default;
    ~FCGeoModelSurfaceIntersectionSurface() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceSurface1() const { return m_sourceSurface1; }
    void setSourceSurface1(const VirtualShape& surface) { m_sourceSurface1 = surface; }
    VirtualShape sourceSurface2() const { return m_sourceSurface2; }
    void setSourceSurface2(const VirtualShape& surface) { m_sourceSurface2 = surface; }
protected:
    VirtualShape m_sourceSurface1{};
    VirtualShape m_sourceSurface2{};
};

/**
 * @brief 桥接曲线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelBridgeCurve : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelBridgeCurve);
public:
    FCGeoModelBridgeCurve() = default;
    ~FCGeoModelBridgeCurve() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceCurve1() const { return m_sourceCurve1; }
    void setSourceCurve1(const VirtualShape& curve) { m_sourceCurve1 = curve; }
    VirtualShape sourceCurve2() const { return m_sourceCurve2; }
    void setSourceCurve2(const VirtualShape& curve) { m_sourceCurve2 = curve; }
    double parameter1() const { return m_parameter1; }
    void setParameter1(double p) { m_parameter1 = p; }
    double parameter2() const { return m_parameter2; }
    void setParameter2(double p) { m_parameter2 = p; }
protected:
    VirtualShape m_sourceCurve1{};
    VirtualShape m_sourceCurve2{};
    double m_parameter1{};
    double m_parameter2{};
};

/**
 * @brief 面的边线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSurfaceEdge : public FCGeoModelCurve
{
    FC_CLASS(FC, FCGeoModelSurfaceEdge);
public:
    FCGeoModelSurfaceEdge() = default;
    ~FCGeoModelSurfaceEdge() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    VirtualShape sourceSurface() const { return m_sourceSurface; }
    void setSourceSurface(const VirtualShape& surface) { m_sourceSurface = surface; }
    int edgeVirtualTopoId() const { return m_edgeVirtualTopoId; }
    void setEdgeVirtualTopoId(int id) { m_edgeVirtualTopoId = id; }
protected:
    VirtualShape m_sourceSurface{};
    int m_edgeVirtualTopoId{ -1 };
};

} // namespace FC

#endif // FCGEOMODELCURVE_H
