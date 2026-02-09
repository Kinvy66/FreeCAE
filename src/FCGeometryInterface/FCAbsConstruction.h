/**
 * @file FCGeoConstruction.h
 * @brief 构造线命令接口（移植自 FITKInterfaceGeometry FITKAbsConstruction）
 */
#ifndef FCGEOCONSTRUCTION_H
#define FCGEOCONSTRUCTION_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCGeoEnum.h"
#include <FCData/FCMacros.h>

namespace FC {

/**
 * @brief 构造线命令
 */
class FCGEOMETRYINTERFACE_API FCGeoConstruction : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoConstruction);
public:
    explicit FCGeoConstruction(FCGeoEnum::FCConstructionType type = FCGeoEnum::FCTLine);
    ~FCGeoConstruction() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setConstructionType(FCGeoEnum::FCConstructionType type) { m_type = type; }
    FCGeoEnum::FCConstructionType getConstructionType() const { return m_type; }
    void getPositions(double* point1, double* point2) const;
    void setPositions(const double* point1, const double* point2);
    void setPosition1(double x, double y, double z);
    void setPosition1(const double* point);
    void setPosition2(double x, double y, double z);
    void setPosition2(const double* point);
protected:
    FCGeoEnum::FCConstructionType m_type{ FCGeoEnum::FCTLine };
    double m_point1[3]{ 0., 0., 0. };
    double m_point2[3]{ 0., 0., 0. };
};

} // namespace FC

#endif // FCGEOCONSTRUCTION_H
