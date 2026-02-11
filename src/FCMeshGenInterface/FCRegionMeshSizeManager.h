/**
 * @file FCRegionMeshSizeManager.h
 * @brief 局部区域网格尺寸管理器（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCREGIONMESHSIZEMANAGER_H
#define FCREGIONMESHSIZEMANAGER_H

#include "FCMeshGenInterfaceAPI.h"
#include "FCAbstractRegionMeshSize.h"
#include <QList>

namespace FC {

/**
 * @brief 局部区域网格尺寸管理器
 */
class FCMESHGENINTERFACE_API FCRegionMeshSizeManager : public QObject
{
    Q_OBJECT
public:
    explicit FCRegionMeshSizeManager(QObject* parent = nullptr);
    virtual ~FCRegionMeshSizeManager() override;

    void append(FCAbstractRegionMeshSize* item);
    void remove(FCAbstractRegionMeshSize* item);
    void clear();
    int count() const { return _list.size(); }
    FCAbstractRegionMeshSize* at(int index) const;

    QList<FCAbstractRegionMeshSize*> getRegionByType(FCAbstractRegionMeshSize::RegionType rt) const;

private:
    QList<FCAbstractRegionMeshSize*> _list;
};

} // namespace FC

#endif // FCREGIONMESHSIZEMANAGER_H
