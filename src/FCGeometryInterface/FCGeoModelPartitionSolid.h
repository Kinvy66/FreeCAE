/**
 * @file FCGeoModelPartitionSolid.h
 * @brief 实体分块命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelPartitionSolid）
 */
#ifndef FCGEOMODELPARTITIONSOLID_H
#define FCGEOMODELPARTITIONSOLID_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>
#include <QVector>

namespace FC {

/**
 * @brief 体分块基类
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionSolid : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelPartitionSolid);
public:
    FCGeoModelPartitionSolid() = default;
    ~FCGeoModelPartitionSolid() override = default;

    void setSolids(const QList<VirtualShape>& solids) { m_solids = solids; }
    QList<VirtualShape> solids() const { return m_solids; }
protected:
    QList<VirtualShape> m_solids;
};

/**
 * @brief 使用平面进行实体分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionSolidWithPlane : public FCGeoModelPartitionSolid
{
    FC_CLASS(FC, FCGeoModelPartitionSolidWithPlane);
public:
    FCGeoModelPartitionSolidWithPlane() = default;
    ~FCGeoModelPartitionSolidWithPlane() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setPlane(const QVector<double>& origin, const QVector<double>& normal) { m_origin = origin; m_normal = normal; }
    QVector<double> planeOrigin() const { return m_origin; }
    QVector<double> planeNormal() const { return m_normal; }
protected:
    QVector<double> m_origin;
    QVector<double> m_normal;
};

/**
 * @brief 使用扩展面进行实体分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionSolidWithExtendFace : public FCGeoModelPartitionSolid
{
    FC_CLASS(FC, FCGeoModelPartitionSolidWithExtendFace);
public:
    FCGeoModelPartitionSolidWithExtendFace() = default;
    ~FCGeoModelPartitionSolidWithExtendFace() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setExtendFace(const VirtualShape& face) { m_extendFace = face; }
    VirtualShape extendFace() const { return m_extendFace; }
protected:
    VirtualShape m_extendFace{};
};

/**
 * @brief 使用草绘进行实体分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionSolidWithSketchPlanar : public FCGeoModelPartitionSolid
{
    FC_CLASS(FC, FCGeoModelPartitionSolidWithSketchPlanar);
public:
    FCGeoModelPartitionSolidWithSketchPlanar() = default;
    ~FCGeoModelPartitionSolidWithSketchPlanar() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setSketch(const VirtualShape& sketch) { m_sketch = sketch; }
    VirtualShape sketch() const { return m_sketch; }
protected:
    VirtualShape m_sketch{};
};

} // namespace FC

#endif // FCGEOMODELPARTITIONSOLID_H
