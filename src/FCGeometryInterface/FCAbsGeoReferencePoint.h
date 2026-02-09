/**
 * @file FCGeoReferencePoint.h
  * @brief 参考点命令（移植自 FITKInterfaceGeometry FITKAbsGeoReferencePoint）
 */
#ifndef FCGEOREFERENCEPOINT_H
#define FCGEOREFERENCEPOINT_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoDatum.h"
#include <array>

namespace FC {

/**
  * @brief 参考点
 */
class FCGEOMETRYINTERFACE_API FCGeoReferencePoint : public FCAbsGeoDatumPoint
{
    FC_CLASS(FC, FCGeoReferencePoint);
public:
    FCGeoReferencePoint() = default;
    ~FCGeoReferencePoint() override = default;
    FCGeoEnum::FCDatumType getDatumType() override;

    double x() const { return _pos[0]; }
    void setX(double x) { _pos[0] = x; }
    double y() const { return _pos[1]; }
    void setY(double y) { _pos[1] = y; }
    double z() const { return _pos[2]; }
    void setZ(double z) { _pos[2] = z; }
    void setCoord(double x, double y, double z);
};

/**
 * @brief 指定曲线比例参考点
 */
class FCGEOMETRYINTERFACE_API FCGeoReferenceCurveRadio : public FCGeoReferencePoint
{
    FC_CLASS(FC, FCGeoReferenceCurveRadio);
public:
    FCGeoReferenceCurveRadio() = default;
    ~FCGeoReferenceCurveRadio() override = default;
    FCGeoEnum::FCDatumType getDatumType() override;

    VirtualShape sourceCurve() const { return m_sourceCurve; }
    void setSourceCurve(const VirtualShape& curve) { m_sourceCurve = curve; }
    double radio() const { return m_radio; }
    void setRadio(double r) { m_radio = r; }
protected:
    VirtualShape m_sourceCurve{};
    double m_radio{};
};

} // namespace FC

#endif // FCGEOREFERENCEPOINT_H
