/**
 * @file FCGeoModelBox.h
 * @brief 长方体几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelBox）
 */
#ifndef FCGEOMODELBOX_H
#define FCGEOMODELBOX_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

/**
 * @brief 长方体几何命令
 */
class FCGEOMETRYINTERFACE_API FCGeoModelBox : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelBox);
public:
    explicit FCGeoModelBox() = default;
    ~FCGeoModelBox() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setPoint1(const double* p1);
    void setLength(const double* length);
    void getPoint1(double* p1) const;
    void getLength(double* length) const;

protected:
    double _point1[3] = { 0, 0, 0 };
    double _lengthParam[3] = { 0, 0, 0 };
};

} // namespace FC

#endif // FCGEOMODELBOX_H
