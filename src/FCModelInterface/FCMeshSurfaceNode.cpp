/**
 * @file FCMeshSurfaceNode.cpp
 * @brief 网格节点表面实现（移植自 FITKInterfaceModel）
 */
#include "FCMeshSurfaceNode.h"
#include "FCModelSet.h"
#include "FCAbstractModel.h"
#include "FCModelEnum.h"
#include <FCData/FCDataRepo.h>

namespace FC {

FCMeshSurface::MeshSurfaceType FCMeshSurfaceNode::getMeshSurfaceType()
{
    return FCMeshSurface::SurNode;
}

FCModelSurface::ModelSurfaceType FCMeshSurfaceNode::getModelSurfaceType()
{
    return FCModelSurface::MST_MeshNode;
}

void FCMeshSurfaceNode::addMeshSet(int modelid, int setID, double v)
{
    const int index = indexOf(modelid, setID);
    if (index >= 0) {
        _modelSetIDs[index] = QPair<int, int>(modelid, setID);
        if (index >= 0 && index < _values.size())
            _values[index] = v;
    } else {
        _modelSetIDs.append(QPair<int, int>(modelid, setID));
        _values.append(v);
    }
}

void FCMeshSurfaceNode::addMeshSet(FCAbstractModel* model, FCModelSet* set, double v)
{
    if (!set || set->getModelSetType() != FCModelEnum::FMSElem) return;
    int modelID = model ? model->getDataObjectID() : -1;
    addMeshSet(modelID, set->getDataObjectID(), v);
}

void FCMeshSurfaceNode::clearMeshSet()
{
    clearSets();
    _values.clear();
}

double FCMeshSurfaceNode::getValueAt(int index) const
{
    if (index < 0 || index >= _values.size()) return -1;
    return _values.at(index);
}

QList<SurfaceAbsSetInfo> FCMeshSurfaceNode::getAbsoluteSetInfo()
{
    QList<SurfaceAbsSetInfo> nodeSurfInfo;
    const int n = _modelSetIDs.size();
    if (n != _values.size()) return nodeSurfInfo;

    for (int i = 0; i < n; ++i) {
        FCAbstractModel* model = getModelAt(i);
        FCModelSet* set = getMeshSetAt(i);
        const double v = _values.at(i);
        if (!model || !set || v < 0) continue;

        QList<FCModelSet*> nodeSets;
        set->getAbsoluteSet(nodeSets, FCModelEnum::FMSNode);
        bool isAss = model->getAbsModelType() == FCModelEnum::Assembly;
        for (FCModelSet* s : nodeSets) {
            if (!s) continue;
            SurfaceAbsSetInfo info;
            info._model = model;
            if (isAss) info._model = s->getAbstractModel();
            info._set = s;
            info._nodeWidght = v;
            nodeSurfInfo.append(info);
        }
    }
    return nodeSurfInfo;
}

} // namespace FC
