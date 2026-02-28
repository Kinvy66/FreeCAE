/**
 * @file FCAbsGeoModelCone.h
 * @brief  圆锥(台)几何命令实现（接口层）
 * @date 2026-02-28
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCABSGEOMODELCONE_H
#define FCABSGEOMODELCONE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC 
{
class FCGEOMETRYINTERFACE_API FCGeoModelCone : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelCone);
    
public:
    explicit FCGeoModelCone() = default;
    ~FCGeoModelCone() override = default;
    
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setLocation(const double* p);
    void getLocation(double* p) const;
    void setAxis(const double* axis);
    void getAxis(double* axis) const;
    void setHeight(double h);
    double getHeight() const;
    void setBottomRadius(double r);
    double getBottomRadius() const;
    void setTopRadius(double r);
    double getTopRadius() const;

protected:
    double mLocation[3] = { 0, 0, 0 };
    double mAxis[3] = { 0, 0, 1 };
    double mHeight = 0;
    double mBottomRadius = 0;
    double mTopRadius = 0;
};
} // namespace FC



#endif // FCABSGEOMODELCONE_H
