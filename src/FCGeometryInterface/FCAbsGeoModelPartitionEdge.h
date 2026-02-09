/**
 * @file FCGeoModelPartitionEdge.h
 * @brief 边分块命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelPartitionEdge）
 */
#ifndef FCGEOMODELPARTITIONEDGE_H
#define FCGEOMODELPARTITIONEDGE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCAbsGeoDatum.h"
#include <QList>
#include <QVector>

namespace FC {

/**
 * @brief 边分块基类
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionEdge : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelPartitionEdge);
public:
    FCGeoModelPartitionEdge() = default;
    ~FCGeoModelPartitionEdge() override = default;

    void setEdges(const QList<VirtualShape>& edges) { m_edges = edges; }
    QList<VirtualShape> edges() const { return m_edges; }
protected:
    QList<VirtualShape> m_edges;
};

/**
 * @brief 使用参数进行边分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionEdgeWithParameter : public FCGeoModelPartitionEdge
{
    FC_CLASS(FC, FCGeoModelPartitionEdgeWithParameter);
public:
    FCGeoModelPartitionEdgeWithParameter() = default;
    ~FCGeoModelPartitionEdgeWithParameter() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setParameter(double p) { m_parameter = p; }
    double parameter() const { return m_parameter; }
protected:
    double m_parameter{};
};

/**
 * @brief 使用点进行边分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionEdgeWithPoint : public FCGeoModelPartitionEdge
{
    FC_CLASS(FC, FCGeoModelPartitionEdgeWithPoint);
public:
    FCGeoModelPartitionEdgeWithPoint() = default;
    ~FCGeoModelPartitionEdgeWithPoint() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setPoint(const QVector<double>& p) { m_point = p; }
    QVector<double> point() const { return m_point; }
protected:
    QVector<double> m_point;
};

/**
 * @brief 使用基准面进行边分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionEdgeWithDatumPlane : public FCGeoModelPartitionEdge
{
    FC_CLASS(FC, FCGeoModelPartitionEdgeWithDatumPlane);
public:
    FCGeoModelPartitionEdgeWithDatumPlane() = default;
    ~FCGeoModelPartitionEdgeWithDatumPlane() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    FCAbsGeoDatumPlane* datumPlane() const { return m_datumPlane; }
    void setDatumPlane(FCAbsGeoDatumPlane* plane) { m_datumPlane = plane; }
protected:
    FCAbsGeoDatumPlane* m_datumPlane{};
};

} // namespace FC

#endif // FCGEOMODELPARTITIONEDGE_H
