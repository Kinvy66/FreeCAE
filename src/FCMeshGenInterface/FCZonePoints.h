/**
 * @file FCZonePoints.h
 * @brief 区域材料点（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCZONEPOINTS_H
#define FCZONEPOINTS_H

#include "FCMeshGenInterfaceAPI.h"
#include <QObject>
#include <QList>
#include <FCData/FCPoint.h>

namespace FC {

/**
 * @brief 区域点（用于局部加密等）
 */
class FCMESHGENINTERFACE_API FCZonePoint : public FCPoint
{
public:
    using FCPoint::FCPoint;
};

/**
 * @brief 区域点管理器
 */
class FCMESHGENINTERFACE_API FCZonePointManager : public QObject
{
    Q_OBJECT
public:
    explicit FCZonePointManager(QObject* parent = nullptr);
    virtual ~FCZonePointManager() override;

    void append(FCZonePoint* item);
    void clear();
    int count() const { return _list.size(); }
    FCZonePoint* at(int index) const;

private:
    QList<FCZonePoint*> _list;
};

} // namespace FC

#endif // FCZONEPOINTS_H
