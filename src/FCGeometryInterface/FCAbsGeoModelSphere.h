/**
 * @file FCGeoModelSphere.h
 * @brief 球体几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelSphere）
 */
#ifndef FCGEOMODELSPHERE_H
#define FCGEOMODELSPHERE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 球体几何命令
 */
class FCGEOMETRYINTERFACE_API FCGeoModelSphere : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelSphere);
public:
    explicit FCGeoModelSphere() = default;
    ~FCGeoModelSphere() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setLocation(const double* p1);
    void getLocation(double* p1) const;
    void setRadius(double r);
    double getRadius() const;

protected:
    double _location[3] = { 0, 0, 0 };
    double _radius = 0;
};

} // namespace FC

#endif // FCGEOMODELSPHERE_H
