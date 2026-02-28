/**
 * @file FCAbsGeoModelHelix.h
 * @brief 螺旋体（接口层）
 * @date 2026-02-28
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCABSGEOMODELHELIX_H
#define FCABSGEOMODELHELIX_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC 
{
class FCGEOMETRYINTERFACE_API FCGeoModelHelix : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelHelix)
public:
    explicit FCGeoModelHelix() = default;
    ~FCGeoModelHelix() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setLocation(const double* p);
    void getLocation(double* p) const;
    void setNumberOfTurns(int n);
    int getNumberOfTurns() const;
    void setMajorRadius(double r);
    double getMajorRadius() const;
    void setMinorRadius(double r);
    double getMinorRadius() const;
    void setRadialPitch(double p);
    double getRadialPitch() const;
    void setAxialPitch(double p);
    double getAxialPitch() const;
    void setChirality(int c);
    int getChirality() const;

protected:
    double mLocation[3];
    int mNumberOfTurns;     ///< 匝数
    double mMajorRadius;    ///< 大半径
    double mMinorRadius;    ///< 小半径
    double mRadialPitch;    ///< 径向距
    double mAxialPitch;     ///< 轴向距
    int mChirality;   ///< 旋转方向
};

} // namespace FC


#endif // FCABSGEOMODELHELIX_H
