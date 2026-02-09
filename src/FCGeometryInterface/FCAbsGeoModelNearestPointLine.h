/**
 * @file FCGeoModelNearestPointLine.h
 * @brief 最近点线命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelNearestPointLine）
 */
#ifndef FCGEOMODELNEARESTPOINTLINE_H
#define FCGEOMODELNEARESTPOINTLINE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <FCData/FCMacros.h>

namespace FC {

/**
 * @brief 给定三维点和曲线或面，生成点到曲线/面的最短连线
 */
class FCGEOMETRYINTERFACE_API FCGeoModelNearestPointLine : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelNearestPointLine);
    Q_OBJECT
public:
    enum ShapeType { Curve, Surface };
    Q_ENUM(ShapeType)

    explicit FCGeoModelNearestPointLine() = default;
    ~FCGeoModelNearestPointLine() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setPoint(const double* point);
    void getPoint(double* point) const;
    void setShape(const VirtualShape& shape) { m_shape = shape; }
    VirtualShape getShape() const { return m_shape; }
    void setShapeType(ShapeType type) { m_shapeType = type; }
    ShapeType getShapeType() const { return m_shapeType; }
    void setNearestPoint(const double* nearestPoint);
    void getNearestPoint(double* nearestPoint) const;
protected:
    double m_point[3]{};
    double m_nearestPoint[3]{};
    VirtualShape m_shape{};
    ShapeType m_shapeType{ Curve };
};

} // namespace FC

#endif // FCGEOMODELNEARESTPOINTLINE_H
