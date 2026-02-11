/**
 * @file FCRegionGeometryRefine.cpp
 */
#include "FCRegionGeometryRefine.h"

namespace FC {

FCRegionGeometryRefine::FCRegionGeometryRefine(QObject* parent) : QObject(parent) {}

void FCRegionGeometryRefine::setGeometryId(int id) { _geometryId = id; }
void FCRegionGeometryRefine::setRefineModel(RefineModelType t) { _refineModel = t; }
void FCRegionGeometryRefine::setDistance(double d) { _distance = d; }

FCRegionGeometryRefineManager::FCRegionGeometryRefineManager(QObject* parent) : QObject(parent) {}

FCRegionGeometryRefineManager::~FCRegionGeometryRefineManager()
{
    for (FCRegionGeometryRefine* p : _list) delete p;
    _list.clear();
}

void FCRegionGeometryRefineManager::append(FCRegionGeometryRefine* item)
{
    if (item && !_list.contains(item)) _list.append(item);
}

void FCRegionGeometryRefineManager::clear()
{
    for (FCRegionGeometryRefine* p : _list) delete p;
    _list.clear();
}

FCRegionGeometryRefine* FCRegionGeometryRefineManager::at(int index) const
{
    return (index >= 0 && index < _list.size()) ? _list.at(index) : nullptr;
}

} // namespace FC
