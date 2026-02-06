/**
 * @file FCGeoModelExtractCenter.h
 * @brief 抽中面/圆心命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelExtractCenter）
 */
#ifndef FCGEOMODELEXTRACTCENTER_H
#define FCGEOMODELEXTRACTCENTER_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <FCData/FCMacros.h>

namespace FC {

/**
 * @brief 提取球面或圆弧的圆心
 */
class FCGEOMETRYINTERFACE_API FCGeoModelExtractCenter : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelExtractCenter);
    Q_OBJECT
public:
    enum ShapeType { Sphere, Arc };
    Q_ENUM(ShapeType)

    explicit FCGeoModelExtractCenter() = default;
    ~FCGeoModelExtractCenter() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setShape(const VirtualShape& shape) { m_shape = shape; }
    VirtualShape getShape() const { return m_shape; }
    void setShapeType(ShapeType type) { m_shapeType = type; }
    ShapeType getShapeType() const { return m_shapeType; }
    void setCenter(const double* center);
    void getCenter(double* center) const;
protected:
    VirtualShape m_shape{};
    ShapeType m_shapeType{ Sphere };
    double m_center[3]{ 0., 0., 0. };
};

} // namespace FC

#endif // FCGEOMODELEXTRACTCENTER_H
