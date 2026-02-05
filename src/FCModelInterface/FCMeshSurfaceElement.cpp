/**
 * @file FCMeshSurfaceElement.cpp
 * @brief 网格单元表面实现（移植自 FITKInterfaceModel）
 */
#include "FCMeshSurfaceElement.h"
#include "FCModelSet.h"
#include "FCAbstractModel.h"
#include "FCModelEnum.h"
#include <FCData/FCDataRepo.h>

namespace FC {

FCMeshSurface::MeshSurfaceType FCMeshSurfaceElement::getMeshSurfaceType()
{
    return FCMeshSurface::SurEle;
}

FCModelSurface::ModelSurfaceType FCMeshSurfaceElement::getModelSurfaceType()
{
    return FCModelSurface::MST_MeshEle;
}

void FCMeshSurfaceElement::addMeshSet(int modelid, int setID, int surfIndex)
{
    const int index = indexOf(modelid, setID);
    if (index >= 0) {
        _modelSetIDs[index] = QPair<int, int>(modelid, setID);
        if (index >= 0 && index < _surfIndex.size())
            _surfIndex[index] = surfIndex;
    } else {
        _modelSetIDs.append(QPair<int, int>(modelid, setID));
        _surfIndex.append(surfIndex);
    }
}

void FCMeshSurfaceElement::addMeshSet(FCAbstractModel* model, FCModelSet* set, int surfIndex)
{
    if (!set || set->getModelSetType() != FCModelEnum::FMSElem) return;
    int modelID = model ? model->getDataObjectID() : -1;
    addMeshSet(modelID, set->getDataObjectID(), surfIndex);
}

void FCMeshSurfaceElement::clearMeshSet()
{
    clearSets();
    _surfIndex.clear();
}

int FCMeshSurfaceElement::getSurfaceIndexAt(int index) const
{
    if (index < 0 || index >= _surfIndex.size()) return -1;
    return _surfIndex.at(index);
}

void FCMeshSurfaceElement::setSurfaceIndex(int index, int surfIndex)
{
    if (index < 0 || index >= _surfIndex.size()) return;
    _surfIndex[index] = surfIndex;
}

QList<SurfaceAbsSetInfo> FCMeshSurfaceElement::getAbsoluteSetInfo()
{
    QList<SurfaceAbsSetInfo> eleSurfInfo;
    const int n = _modelSetIDs.size();
    if (n != _surfIndex.size()) return eleSurfInfo;

    for (int i = 0; i < n; ++i) {
        FCAbstractModel* model = getModelAt(i);
        FCModelSet* set = getMeshSetAt(i);
        const int surfIndex = _surfIndex.at(i);
        if (!model || !set || surfIndex < 0) continue;

        QList<FCModelSet*> eleSets;
        set->getAbsoluteSet(eleSets, FCModelEnum::FMSElem);
        bool isAss = model->getAbsModelType() == FCModelEnum::Assembly;
        for (FCModelSet* s : eleSets) {
            if (!s) continue;
            SurfaceAbsSetInfo info;
            info._model = model;
            if (isAss) info._model = s->getAbstractModel();
            info._set = s;
            info._surfaceIndex = surfIndex;
            eleSurfInfo.append(info);
        }
    }
    return eleSurfInfo;
}

} // namespace FC
