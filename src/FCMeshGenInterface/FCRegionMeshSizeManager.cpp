/**
 * @file FCRegionMeshSizeManager.cpp
 */
#include "FCRegionMeshSizeManager.h"

namespace FC {

FCRegionMeshSizeManager::FCRegionMeshSizeManager(QObject* parent) : QObject(parent) {}

FCRegionMeshSizeManager::~FCRegionMeshSizeManager()
{
    for (FCAbstractRegionMeshSize* p : _list) delete p;
    _list.clear();
}

void FCRegionMeshSizeManager::append(FCAbstractRegionMeshSize* item)
{
    if (item && !_list.contains(item)) _list.append(item);
}

void FCRegionMeshSizeManager::remove(FCAbstractRegionMeshSize* item)
{
    _list.removeOne(item);
}

void FCRegionMeshSizeManager::clear()
{
    for (FCAbstractRegionMeshSize* p : _list) delete p;
    _list.clear();
}

FCAbstractRegionMeshSize* FCRegionMeshSizeManager::at(int index) const
{
    return (index >= 0 && index < _list.size()) ? _list.at(index) : nullptr;
}

QList<FCAbstractRegionMeshSize*> FCRegionMeshSizeManager::getRegionByType(FCAbstractRegionMeshSize::RegionType rt) const
{
    QList<FCAbstractRegionMeshSize*> out;
    for (FCAbstractRegionMeshSize* p : _list) {
        if (p && p->getRegionType() == rt) out.append(p);
    }
    return out;
}

} // namespace FC
