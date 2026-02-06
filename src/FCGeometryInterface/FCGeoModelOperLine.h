/**
 * @file FCGeoModelOperLine.h
 * @brief 线操作命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelOperLine）
 */
#ifndef FCGEOMODELOPERLINE_H
#define FCGEOMODELOPERLINE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 线操作（合并/按中点分割/按角度分割）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelOperLine : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelOperLine);
    Q_OBJECT
public:
    enum GeoLineOperType { GBTNone, GBTMerge, GBTSplitByMiddlePosition, GBTSplitByAngle };
    Q_ENUM(GeoLineOperType)

    FCGeoModelOperLine() = default;
    ~FCGeoModelOperLine() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void add(const VirtualShape& shape);
    void add(const QList<VirtualShape>& shapes);
    void set(const QList<VirtualShape>& shapes);
    void remove(int index);
    void clear();
    void setType(GeoLineOperType t) { m_operType = t; }
    GeoLineOperType getType() const { return m_operType; }
    QList<VirtualShape> getVShapes() const { return m_shapes; }
    void setAngle(double angle) { m_angle = angle; }
    double getAngle() const { return m_angle; }
protected:
    GeoLineOperType m_operType{ GBTNone };
    QList<VirtualShape> m_shapes;
    double m_angle{};
};

} // namespace FC

#endif // FCGEOMODELOPERLINE_H
