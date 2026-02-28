/**
 * @file FCAbsGeoModelTorus.h
 * @brief 圆环几何命令实现（接口层）
 * @date 2026-02-28
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCABSGEOMODELTORUS_H
#define FCABSGEOMODELTORUS_H
#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC 
{
class FCGEOMETRYINTERFACE_API FCGeoModelTorus : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelTorus)
public:
    explicit FCGeoModelTorus() = default;
    ~FCGeoModelTorus() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setLocation(const double* p);
    void getLocation(double* p) const;
    void setAxis(const double* axis);
    void getAxis(double* axis) const;
    void setMajorRadius(double r);
    double getMajorRadius() const;
    void setMinorRadius(double r);
    double getMinorRadius() const;
    void setAngle(double angle);
    double getAngle() const;

protected:
    double mLocation[3] = { 0, 0, 0 };
    double mAxis[3] = { 0, 0, 1 };
    double mMajorRadius = 0;   ///< 大半径
    double mMinorRadius = 0;   ///< 小半径
    double mAngle = 360.0;    ///< 旋转角度(度), 360为完整圆环
};
} // namespace FC



#endif // FCABSGEOMODELTORUS_H
