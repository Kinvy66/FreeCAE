/**
 * @file FCGeoModelOperImprintSolid.h
 * @brief 压印体操作命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelOperImprintSolid）
 */
#ifndef FCGEOMODELOPERIMPRINTSOLID_H
#define FCGEOMODELOPERIMPRINTSOLID_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 压印体操作
 */
class FCGEOMETRYINTERFACE_API FCGeoModelOperImprintSolid : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelOperImprintSolid);
    Q_OBJECT
public:
    enum GeoSolidOperType { GBTNone, GBTImprintSolid };
    Q_ENUM(GeoSolidOperType)

    FCGeoModelOperImprintSolid() = default;
    ~FCGeoModelOperImprintSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void add(const VirtualShape& shape);
    void set(const VirtualShape& shape);
    void add(const QList<VirtualShape>& shapes);
    void set(const QList<VirtualShape>& shapes);
    void remove(int index);
    void clear();
    void setType(GeoSolidOperType t) { m_operType = t; }
    GeoSolidOperType getType() const { return m_operType; }
    QList<VirtualShape> getVShapes() const { return m_shapes; }
protected:
    GeoSolidOperType m_operType{ GBTImprintSolid };
    QList<VirtualShape> m_shapes;
};

} // namespace FC

#endif // FCGEOMODELOPERIMPRINTSOLID_H
