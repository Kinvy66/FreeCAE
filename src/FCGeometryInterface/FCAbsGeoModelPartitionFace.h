/**
 * @file FCGeoModelPartitionFace.h
 * @brief 面分块命令（移植自 FITKInterfaceGeometry FITKAbsGeoModelPartitionFace）
 */
#ifndef FCGEOMODELPARTITIONFACE_H
#define FCGEOMODELPARTITIONFACE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCAbsGeoDatum.h"
#include <QList>
#include <QVector>

namespace FC {

/**
 * @brief 面分块基类
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFace : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoModelPartitionFace);
public:
    FCGeoModelPartitionFace() = default;
    ~FCGeoModelPartitionFace() override = default;

    void setFaces(const QList<VirtualShape>& faces) { m_faces = faces; }
    QList<VirtualShape> faces() const { return m_faces; }
protected:
    QList<VirtualShape> m_faces;
};

/**
 * @brief 使用草图进行面分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFaceWithSketch : public FCGeoModelPartitionFace
{
    FC_CLASS(FC, FCGeoModelPartitionFaceWithSketch);
public:
    FCGeoModelPartitionFaceWithSketch() = default;
    ~FCGeoModelPartitionFaceWithSketch() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setSketch(int id) { m_sketchId = id; }
    int sketch() const { return m_sketchId; }
protected:
    int m_sketchId{};
};

/**
 * @brief 使用两点进行面分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFaceWithTwoPoints : public FCGeoModelPartitionFace
{
    FC_CLASS(FC, FCGeoModelPartitionFaceWithTwoPoints);
public:
    FCGeoModelPartitionFaceWithTwoPoints() = default;
    ~FCGeoModelPartitionFaceWithTwoPoints() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setStartPoint(const QVector<double>& point) { m_startPoint = point; }
    QVector<double> startPoint() const { return m_startPoint; }
    void setEndPoint(const QVector<double>& point) { m_endPoint = point; }
    QVector<double> endPoint() const { return m_endPoint; }
protected:
    QVector<double> m_startPoint;
    QVector<double> m_endPoint;
};

/**
 * @brief 使用基准面进行面分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFaceWithDatumPlane : public FCGeoModelPartitionFace
{
    FC_CLASS(FC, FCGeoModelPartitionFaceWithDatumPlane);
public:
    FCGeoModelPartitionFaceWithDatumPlane() = default;
    ~FCGeoModelPartitionFaceWithDatumPlane() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    FCAbsGeoDatumPlane* datumPlane() const { return m_datumPlane; }
    void setDatumPlane(FCAbsGeoDatumPlane* plane) { m_datumPlane = plane; }
protected:
    FCAbsGeoDatumPlane* m_datumPlane{};
};

/**
 * @brief 使用扩展面进行面分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFaceWithExtendFace : public FCGeoModelPartitionFace
{
    FC_CLASS(FC, FCGeoModelPartitionFaceWithExtendFace);
public:
    FCGeoModelPartitionFaceWithExtendFace() = default;
    ~FCGeoModelPartitionFaceWithExtendFace() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setExtendFace(const VirtualShape& face) { m_extendFace = face; }
    VirtualShape extendFace() const { return m_extendFace; }
protected:
    VirtualShape m_extendFace{};
};

/**
 * @brief 使用曲线路径进行面分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFaceWithCurvedPath : public FCGeoModelPartitionFace
{
    FC_CLASS(FC, FCGeoModelPartitionFaceWithCurvedPath);
public:
    FCGeoModelPartitionFaceWithCurvedPath() = default;
    ~FCGeoModelPartitionFaceWithCurvedPath() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setFace(const VirtualShape& face) { m_faces = QList<VirtualShape>() << face; }
    VirtualShape face() const { return m_faces.isEmpty() ? VirtualShape() : m_faces.first(); }
    void setFirstEdge(const VirtualShape& e) { m_firstEdge = e; }
    VirtualShape firstEdge() const { return m_firstEdge; }
    void setSecondEdge(const VirtualShape& e) { m_secondEdge = e; }
    VirtualShape secondEdge() const { return m_secondEdge; }
    void setFirstPoint(const QVector<double>& p) { m_firstPoint = p; }
    QVector<double> firstPoint() const { return m_firstPoint; }
    void setSecondPoint(const QVector<double>& p) { m_secondPoint = p; }
    QVector<double> secondPoint() const { return m_secondPoint; }
protected:
    VirtualShape m_firstEdge{};
    VirtualShape m_secondEdge{};
    QVector<double> m_firstPoint;
    QVector<double> m_secondPoint;
};

/**
 * @brief 使用相交面进行面分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFaceWithIntersectFace : public FCGeoModelPartitionFace
{
    FC_CLASS(FC, FCGeoModelPartitionFaceWithIntersectFace);
public:
    FCGeoModelPartitionFaceWithIntersectFace() = default;
    ~FCGeoModelPartitionFaceWithIntersectFace() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setToolFaces(const QList<VirtualShape>& faces) { m_toolFaces = faces; }
    QList<VirtualShape> toolFaces() const { return m_toolFaces; }
protected:
    QList<VirtualShape> m_toolFaces;
};

/**
 * @brief 使用投影边进行面分块
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPartitionFaceWithProjectEdges : public FCGeoModelPartitionFace
{
    FC_CLASS(FC, FCGeoModelPartitionFaceWithProjectEdges);
public:
    FCGeoModelPartitionFaceWithProjectEdges() = default;
    ~FCGeoModelPartitionFaceWithProjectEdges() override = default;
    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
    void setProjectEdges(const QList<VirtualShape>& edges) { m_projectEdges = edges; }
    QList<VirtualShape> projectEdges() const { return m_projectEdges; }
protected:
    QList<VirtualShape> m_projectEdges;
};

} // namespace FC

#endif // FCGEOMODELPARTITIONFACE_H
