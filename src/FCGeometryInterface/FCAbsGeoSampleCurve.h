/**
 * @file FCGeoSampleCurve.h
 * @brief 曲线采样命令（移Щ自 FITKInterfaceGeometry FITKAbsGeoSampleCurve）
 */
#ifndef FCGEOSAMPLECURVE_H
#define FCGEOSAMPLECURVE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <FCData/FCMacros.h>

namespace FC {

/**
 * @brief 曲线采样（移瓑鍙?等弧长/等弧长匡級
 */
class FCGEOMETRYINTERFACE_API FCGeoSampleCurve : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoSampleCurve);
    Q_OBJECT
public:
    enum SampleMethod { Parametric, ArcLength, ChordLength };
    Q_ENUM(SampleMethod)

    explicit FCGeoSampleCurve() = default;
    ~FCGeoSampleCurve() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setSampleMethod(SampleMethod method) { m_sampleMethod = method; }
    SampleMethod getSampleMethod() const { return m_sampleMethod; }
    void setSamplePointCount(int count) { m_samplePointCount = count; }
    int getSamplePointCount() const { return m_samplePointCount; }
    void setSampleCurve(const VirtualShape& curve) { m_sampleCurve = curve; }
    VirtualShape getSampleCurve() const { return m_sampleCurve; }
protected:
    SampleMethod m_sampleMethod{ Parametric };
    int m_samplePointCount{};
    VirtualShape m_sampleCurve{};
};

} // namespace FC

#endif // FCGEOSAMPLECURVE_H
