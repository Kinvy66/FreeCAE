/**
 * @file FCGeoOperBool.h
 * @brief 布尔运算几何命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoOperBool）
 */
#ifndef FCGEOOPERBOOL_H
#define FCGEOOPERBOOL_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"

namespace FC {

class FCAbsGeoShapeAgent;

/**
 * @brief 布尔运算几何命令（交集/差集/并集）
 */
class FCGEOMETRYINTERFACE_API FCGeoOperBool : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoOperBool);
    Q_OBJECT
public:
    enum GeoBoolOperType
    {
        GBTNone,
        GBTCommon,  // 交集
        GBTCut,     // 差集
        GBTAdd,     // 并集
    };
    Q_ENUM(GeoBoolOperType)

    explicit FCGeoOperBool() = default;
    ~FCGeoOperBool() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setBoolOperType(GeoBoolOperType t);
    GeoBoolOperType getBoolOperType() const;

    void setGeoShape1(FCAbsGeoShapeAgent* s1);
    void setGeoShape2(FCAbsGeoShapeAgent* s2);
    FCAbsGeoShapeAgent* getGeoShape1();
    FCAbsGeoShapeAgent* getGeoShape2();

    VirtualShape target() const { return m_target; }
    void setTarget(const VirtualShape& shape) { m_target = shape; }
    VirtualShape tool() const { return m_tool; }
    void setTool(const VirtualShape& shape) { m_tool = shape; }

protected:
    GeoBoolOperType _boolOperType{ GBTNone };
    FCAbsGeoShapeAgent* _shape1{};
    FCAbsGeoShapeAgent* _shape2{};
    VirtualShape m_target{};
    VirtualShape m_tool{};
};

} // namespace FC

#endif // FCGEOOPERBOOL_H
