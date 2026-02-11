/**
 * @file FCZonePoints.cpp
 */
#include "FCZonePoints.h"

namespace FC {

FCZonePointManager::FCZonePointManager(QObject* parent) : QObject(parent) {}

FCZonePointManager::~FCZonePointManager()
{
    for (FCZonePoint* p : _list) delete p;
    _list.clear();
}

void FCZonePointManager::append(FCZonePoint* item)
{
    if (item && !_list.contains(item)) _list.append(item);
}

void FCZonePointManager::clear()
{
    for (FCZonePoint* p : _list) delete p;
    _list.clear();
}

FCZonePoint* FCZonePointManager::at(int index) const
{
    return (index >= 0 && index < _list.size()) ? _list.at(index) : nullptr;
}

} // namespace FC
