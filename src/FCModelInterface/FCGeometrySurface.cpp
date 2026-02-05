/**
 * @file FCGeometrySurface.cpp
 * @brief 几何模型表面实现（移植自 FITKInterfaceModel）
 */
#include "FCGeometrySurface.h"
#include "FCModelSet.h"
#include "FCAbstractModel.h"
#include "FCModelEnum.h"

namespace FC {

FCModelSurface::ModelSurfaceType FCGeometrySurface::getModelSurfaceType()
{
    return FCModelSurface::MST_Geom;
}

void FCGeometrySurface::addGeometrySet(int modelid, int setID, int surfIndex)
{
    const int index = indexOf(modelid, setID);
    if (index >= 0) {
        _modelSetIDs[index] = QPair<int, int>(modelid, setID);
        if (index >= 0 && index < _surfIndice.size())
            _surfIndice[index] = surfIndex;
    } else {
        _modelSetIDs.append(QPair<int, int>(modelid, setID));
        _surfIndice.append(surfIndex);
    }
}

void FCGeometrySurface::addGeometrySet(FCAbstractModel* model, FCModelSet* set, int surfIndex)
{
    if (!set || set->getModelSetType() != FCModelEnum::FMSSurface) return;
    int modelID = model ? model->getDataObjectID() : -1;
    addGeometrySet(modelID, set->getDataObjectID(), surfIndex);
}

void FCGeometrySurface::clearGeometrySet()
{
    clearSets();
    _surfIndice.clear();
}

int FCGeometrySurface::getSurfaceIndexAt(int index) const
{
    if (index < 0 || index >= _surfIndice.size()) return -1;
    return _surfIndice.at(index);
}

void FCGeometrySurface::setSurfaceIndex(int index, int surfIndex)
{
    if (index < 0 || index >= _surfIndice.size()) return;
    _surfIndice[index] = surfIndex;
}

QList<SurfaceAbsSetInfo> FCGeometrySurface::getAbsoluteSetInfo()
{
    QList<SurfaceAbsSetInfo> surfInfo;
    const int n = _modelSetIDs.size();
    if (n != _surfIndice.size()) return surfInfo;

    for (int i = 0; i < n; ++i) {
        FCAbstractModel* model = getModelAt(i);
        FCModelSet* set = getSetAt(i);
        const int surfIndex = _surfIndice.at(i);
        if (!model || !set || surfIndex < 0) continue;

        QList<FCModelSet*> surfSets;
        set->getAbsoluteSet(surfSets, FCModelEnum::FMSSurface);
        bool isAss = model->getAbsModelType() == FCModelEnum::Assembly;
        for (FCModelSet* s : surfSets) {
            if (!s) continue;
            SurfaceAbsSetInfo info;
            info._model = model;
            if (isAss) info._model = s->getAbstractModel();
            info._set = s;
            info._surfaceIndex = surfIndex;
            surfInfo.append(info);
        }
    }
    return surfInfo;
}

} // namespace FC
