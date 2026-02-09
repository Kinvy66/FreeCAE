/**
 * @file FCGeoModelCylinder.h
 * @brief 圆柱体几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelCylinder）
 */
#ifndef FCGEOMODELCYLINDER_H
#define FCGEOMODELCYLINDER_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 圆柱体几何命令
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCylinder : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelCylinder);
public:
    explicit FCGeoModelCylinder() = default;
    ~FCGeoModelCylinder() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setLocation(const double* p1);
    void getLocation(double* p1) const;
    void setDirection(const double* dir);
    void getDirection(double* dir) const;
    void setRadius(double r);
    double getRadius() const;
    void setLength(double length);
    double getLength() const;

protected:
    double _location[3] = { 0, 0, 0 };
    double _direction[3] = { 0, 0, 0 };
    double _radius = 0;
    double _length = 0;
};

} // namespace FC

#endif // FCGEOMODELCYLINDER_H
