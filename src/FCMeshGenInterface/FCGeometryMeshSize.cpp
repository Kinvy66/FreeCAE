/**
 * @file FCGeometryMeshSize.cpp
 */
#include "FCGeometryMeshSize.h"

namespace FC {

FCGeometryMeshSize::FCGeometryMeshSize(QObject* parent) : FCGlobalMeshSizeInfo(parent) {}

void FCGeometryMeshSize::setGeoGroupComponentId(int id) { _geoGroupComponentId = id; }
void FCGeometryMeshSize::setLayerNumber(int n) { _layerNum = n; }
void FCGeometryMeshSize::setExpansionRatio(double r) { _expansionRatio = r; }
void FCGeometryMeshSize::setLayerThickness(double t) { _layerThickness = t; }

FCGeometryMeshSizeManager::FCGeometryMeshSizeManager(QObject* parent) : QObject(parent) {}

FCGeometryMeshSizeManager::~FCGeometryMeshSizeManager()
{
    for (FCGeometryMeshSize* p : _list) delete p;
    _list.clear();
}

void FCGeometryMeshSizeManager::append(FCGeometryMeshSize* item)
{
    if (item && !_list.contains(item)) _list.append(item);
}

void FCGeometryMeshSizeManager::clear()
{
    for (FCGeometryMeshSize* p : _list) delete p;
    _list.clear();
}

FCGeometryMeshSize* FCGeometryMeshSizeManager::at(int index) const
{
    return (index >= 0 && index < _list.size()) ? _list.at(index) : nullptr;
}

} // namespace FC
