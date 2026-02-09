/**
 * @file FCGeoModelOperFace.h
 * @brief 面操作命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelOperFace）
 */
#ifndef FCGEOMODELOPERFACE_H
#define FCGEOMODELOPERFACE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 面操作（填孔/填缝/删悬浮边/点线圆映射到面等）
 */
class FCGEOMETRYINTERFACE_API FCGeoModelOperFace : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelOperFace);
    Q_OBJECT
public:
    enum GeoFaceOperType {
        GBTNone,
        GBTFillHoles,
        GBTFillGaps,
        GBTDeleteFloatingEdge,
        GBTImprintMapPointToFace,
        GBTImprintMapLineToFace,
        GBTImprintMapCircleToFace
    };
    Q_ENUM(GeoFaceOperType)

    FCGeoModelOperFace() = default;
    ~FCGeoModelOperFace() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void add(const VirtualShape& shape);
    void add(const QList<VirtualShape>& shapes);
    void set(const QList<VirtualShape>& shapes);
    void remove(int index);
    void clear();
    void setType(GeoFaceOperType t) { m_operType = t; }
    GeoFaceOperType getType() const { return m_operType; }
    QList<VirtualShape> getVShapes() const { return m_shapes; }
protected:
    GeoFaceOperType m_operType{ GBTNone };
    QList<VirtualShape> m_shapes;
};

/** 移除面（OCC 实现用） */
class FCGEOMETRYINTERFACE_API FCGeoModelOperFaceRemoveFace : public FCGeoModelOperFace
{
    FC_CLASS(FC, FCGeoModelOperFaceRemoveFace);
public:
    FCGeoModelOperFaceRemoveFace() = default;
    ~FCGeoModelOperFaceRemoveFace() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/** 延伸面（OCC 实现用） */
class FCGEOMETRYINTERFACE_API FCGeoModelOperFaceExtendFace : public FCGeoModelOperFace
{
    FC_CLASS(FC, FCGeoModelOperFaceExtendFace);
public:
    FCGeoModelOperFaceExtendFace() = default;
    ~FCGeoModelOperFaceExtendFace() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace FC

#endif // FCGEOMODELOPERFACE_H
