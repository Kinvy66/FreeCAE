/**
 * @file FCGeoSketchDeleter.h
 * @brief 草图删除命令（移植自 FITKInterfaceGeometry FITKAbsGeoSketchDeleter）
 */
#ifndef FCGEOSKETCHDELETER_H
#define FCGEOSKETCHDELETER_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 草图删除：移除指定草图实体
 */
class FCGEOMETRYINTERFACE_API FCGeoSketchDeleter : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoSketchDeleter);
public:
    FCGeoSketchDeleter() = default;
    ~FCGeoSketchDeleter() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    QList<VirtualShape> toBeRemovedEntities() const { return m_toBeRemovedEntities; }
    void setToBeRemovedEntities(const QList<VirtualShape>& entities) { m_toBeRemovedEntities = entities; }
protected:
    QList<VirtualShape> m_toBeRemovedEntities;
};

} // namespace FC

#endif // FCGEOSKETCHDELETER_H
