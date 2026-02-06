/**
 * @file FCAbsGeoDatum.h
 * @brief 基准元素抽象（移植自 FITKInterfaceGeometry）
 */
#ifndef FCABSGEODATUM_H
#define FCABSGEODATUM_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCGeoEnum.h"
#include <FCData/FCMacros.h>
#include <FCData/FCAbstractDataManager.hpp>

namespace FC {

class FCGEOMETRYINTERFACE_API FCAbsGeoDatum : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCAbsGeoDatum);
public:
    enum DatumCreatedType { DCT_System = 0, DCT_UserDefine };

    FCAbsGeoDatum() = default;
    ~FCAbsGeoDatum() override = default;

    void setEditable(bool flag);
    bool editable();
    void setPosition(double* pos);
    void getPosition(double* pos);
    virtual FCGeoEnum::FITKDatumType getDatumType() = 0;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    virtual bool update() override { return true; }
    void printLog(QString msg, int type = 1);

protected:
    double _pos[3]{ 0., 0., 0. };
    bool _editable{ true };
};

/** 基准点基类 */
class FCGEOMETRYINTERFACE_API FCAbsGeoDatumPoint : public FCAbsGeoDatum
{
    FC_CLASS(FC, FCAbsGeoDatumPoint);
public:
    FCAbsGeoDatumPoint() = default;
    ~FCAbsGeoDatumPoint() override = default;
    FCGeoEnum::FITKDatumType getDatumType() override;
};

/** 基准线基类 */
class FCGEOMETRYINTERFACE_API FCAbsGeoDatumLine : public FCAbsGeoDatum
{
    FC_CLASS(FC, FCAbsGeoDatumLine);
public:
    FCAbsGeoDatumLine() = default;
    ~FCAbsGeoDatumLine() override = default;
    FCGeoEnum::FITKDatumType getDatumType() override;
    void setPosition2(const double* pos);
    void getPosition2(double* pos) const;
protected:
    double _pos2[3]{ 0., 0., 0. };
};

/** 基准面基类 */
class FCGEOMETRYINTERFACE_API FCAbsGeoDatumPlane : public FCAbsGeoDatum
{
    FC_CLASS(FC, FCAbsGeoDatumPlane);
public:
    FCAbsGeoDatumPlane() = default;
    ~FCAbsGeoDatumPlane() override = default;
    FCGeoEnum::FITKDatumType getDatumType() override;
    void setNormal(const double* nor);
    void getNormal(double* nor) const;
    void setUp(const double* up);
    void getUp(double* up) const;
protected:
    double _nor[3]{ 0., 0., 1. };
    double _up[3]{ 0., 1., 0. };
};

/** 系统默认基准元素管理器（平面/轴），接口层可返回 nullptr */
class FCGEOMETRYINTERFACE_API FCPrincipalDatumList
{
public:
    explicit FCPrincipalDatumList() = default;
    virtual ~FCPrincipalDatumList() = default;
    FCAbsGeoDatumPlane* getPrincipalPlane(FCGeoEnum::DatumPlnType type);
    FCAbsGeoDatumLine* getPrincipalAxis(FCGeoEnum::DatumAxisType type);
};

/** 基准元素列表（撤销列表用） */
class FCGEOMETRYINTERFACE_API FCDatumList : public FCAbstractDataManager<FCAbsGeoDatum>
{
public:
    explicit FCDatumList();
    virtual ~FCDatumList();
    FCAbsGeoDatumPlane* getPrincipalPlane(FCGeoEnum::DatumPlnType type);
    FCAbsGeoDatumLine* getPrincipalAxis(FCGeoEnum::DatumAxisType type);
protected:
    FCPrincipalDatumList* m_principalDatumMgr{ nullptr };
};

} // namespace FC

#endif // FCABSGEODATUM_H
