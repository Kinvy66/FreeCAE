/**
 * @file FCMeshSurface.cpp
 * @brief 网格表面实现（移植自 FITKInterfaceModel）
 */
#include "FCMeshSurface.h"
#include "FCModelSet.h"
#include <FCData/FCDataRepo.h>

namespace FC {

FCMeshSurface::~FCMeshSurface()
{}

int FCMeshSurface::getMeshSetCount() const
{
    return _modelSetIDs.size();
}

FCModelSet* FCMeshSurface::getMeshSetAt(int index) const
{
    if (index < 0 || index >= _modelSetIDs.size()) return nullptr;
    const QPair<int, int>& p = _modelSetIDs.at(index);
    return FCDataRepo::instance()->getDataAs<FCModelSet>(p.second);
}

} // namespace FC
