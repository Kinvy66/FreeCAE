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

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setExtendFace(const VirtualShape& face) { m_extendFace = face; }
    VirtualShape extendFace() const { return m_extendFace; }
protected:
    VirtualShape m_extendFace{};
};

} // namespace FC

#endif // FCGEOMODELPARTITIONFACE_H
