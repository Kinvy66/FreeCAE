/**
 * @file FCModelSurface.cpp
 * @brief 模型表面实现（移植自 FITKInterfaceModel）
 */
#include "FCModelSurface.h"
#include "FCModelSet.h"
#include "FCAbstractModel.h"
#include <FCData/FCDataRepo.h>

namespace FC {

FCModelSurface::~FCModelSurface()
{}

int FCModelSurface::getSetCount() const
{
    return _modelSetIDs.size();
}

FCModelSet* FCModelSurface::getSetAt(int index) const
{
    if (index < 0 || index >= _modelSetIDs.size()) return nullptr;
    const QPair<int, int>& p = _modelSetIDs.at(index);
    FCAbstractDataObject* obj = FCDataRepo::instance()->getDataByID(p.second);
    return dynamic_cast<FCModelSet*>(obj);
}

bool FCModelSurface::isInternal() const
{
    return _internal;
}

void FCModelSurface::setInternal(bool internal)
{
    _internal = internal;
}

FCAbstractModel* FCModelSurface::getModelAt(int index) const
{
    if (index < 0 || index >= _modelSetIDs.size()) return nullptr;
    const QPair<int, int>& p = _modelSetIDs.at(index);
    return FCDataRepo::instance()->getDataAs<FCAbstractModel>(p.first);
}

QList<SurfaceAbsSetInfo> FCModelSurface::getAbsoluteSetInfo()
{
    return QList<SurfaceAbsSetInfo>();
}

void FCModelSurface::clearSets()
{
    _modelSetIDs.clear();
}

int FCModelSurface::indexOf(int modelID, int setID)
{
    for (int i = 0; i < _modelSetIDs.size(); ++i) {
        if (_modelSetIDs.at(i).first == modelID && _modelSetIDs.at(i).second == setID)
            return i;
    }
    return -1;
}

} // namespace FC
