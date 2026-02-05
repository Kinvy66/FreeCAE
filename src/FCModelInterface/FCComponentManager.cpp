/**
 * @file FCComponentManager.cpp
 * @brief 组件管理器实现（移植自 FITKInterfaceModel）
 */
#include "FCComponentManager.h"
#include "FCModelSet.h"
#include "FCMeshSurface.h"
#include "FCModelSurface.h"
#include "FCAbstractModel.h"

namespace FC {

QList<FCModelSet*> FCComponentManager::getModelSet(FCModelEnum::FITKModelSetType t)
{
    QList<FCModelSet*> sets;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCModelSet* set = dynamic_cast<FCModelSet*>(getDataByIndex(i));
        if (!set) continue;
        if (set->getModelSetType() == t) sets.append(set);
        if (!set->isCombination()) continue;
        const int nSub = set->getDataCount();
        for (int j = 0; j < nSub; ++j) {
            FCModelSet* subSet = set->getDataByIndex(j);
            if (!subSet) continue;
            if (subSet->getModelSetType() == t) sets.append(subSet);
        }
    }
    return sets;
}

QList<FCModelSet*> FCComponentManager::getUnInternalModelSet(FCModelEnum::FITKModelSetType t)
{
    QList<FCModelSet*> sets = getModelSet(t);
    QList<FCModelSet*> unSets;
    for (FCModelSet* s : sets) {
        if (!s || s->isInternal()) continue;
        unSets.append(s);
    }
    return unSets;
}

QList<FCMeshSurface*> FCComponentManager::getAllMeshSurface()
{
    QList<FCMeshSurface*> surfs;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCMeshSurface* surf = dynamic_cast<FCMeshSurface*>(getDataByIndex(i));
        if (surf) surfs.append(surf);
    }
    return surfs;
}

QList<FCMeshSurface*> FCComponentManager::getMeshSurface(FCMeshSurface::MeshSurfaceType t)
{
    QList<FCMeshSurface*> surfs;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCMeshSurface* surf = dynamic_cast<FCMeshSurface*>(getDataByIndex(i));
        if (!surf || surf->getMeshSurfaceType() != t) continue;
        surfs.append(surf);
    }
    return surfs;
}

QList<FCModelSurface*> FCComponentManager::getModelSurface(FCModelSurface::ModelSurfaceType t)
{
    QList<FCModelSurface*> surfs;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCModelSurface* surf = dynamic_cast<FCModelSurface*>(getDataByIndex(i));
        if (!surf || surf->getModelSurfaceType() != t) continue;
        surfs.append(surf);
    }
    return surfs;
}

QList<FCMeshSurface*> FCComponentManager::getUnInternalMeshSurface(FCMeshSurface::MeshSurfaceType t)
{
    QList<FCMeshSurface*> surfs = getMeshSurface(t);
    QList<FCMeshSurface*> unSurfs;
    for (FCMeshSurface* s : surfs) {
        if (!s || s->isInternal()) continue;
        unSurfs.append(s);
    }
    return unSurfs;
}

bool FCComponentManager::isModelSet(int index)
{
    return getAsModelSet(index) != nullptr;
}

FCModelSet* FCComponentManager::getAsModelSet(int index)
{
    return dynamic_cast<FCModelSet*>(getDataByIndex(index));
}

FCMeshSurface* FCComponentManager::getAsMeshSurf(int index)
{
    return dynamic_cast<FCMeshSurface*>(getDataByIndex(index));
}

FCModelSurface* FCComponentManager::getSurf(int index)
{
    return dynamic_cast<FCModelSurface*>(getDataByIndex(index));
}

QList<FCModelSet*> FCComponentManager::getAllSet(bool recur)
{
    QList<FCModelSet*> sets;
    const int n = getDataCount();
    if (!recur) {
        for (int i = 0; i < n; ++i) {
            FCModelSet* s = getAsModelSet(i);
            if (s) sets.append(s);
        }
        return sets;
    }
    for (int i = 0; i < n; ++i) {
        FCModelSet* s = getAsModelSet(i);
        if (!s) continue;
        if (s->isCombination()) {
            const int nsub = s->getDataCount();
            for (int j = 0; j < nsub; ++j) {
                FCModelSet* sub = s->getDataByIndex(j);
                if (sub) sets.append(sub);
            }
        } else {
            sets.append(s);
        }
    }
    return sets;
}

QList<FCModelSurface*> FCComponentManager::getAllSurface()
{
    QList<FCModelSurface*> surf;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCModelSurface* s = getSurf(i);
        if (s) surf.append(s);
    }
    return surf;
}

void FCComponentManager::appendDataObj(FCAbstractDataObject* item)
{
    if (dynamic_cast<FCModelSurface*>(item)) {
        FCAbstractDataManager<FCAbstractModelComponent>::appendDataObj(item);
        return;
    }
    FCModelSet* set = dynamic_cast<FCModelSet*>(item);
    if (!set) return;
    FCModelSet* existSet = dynamic_cast<FCModelSet*>(getDataByName(set->getDataObjectName()));
    if (!existSet)
        FCAbstractDataManager<FCAbstractModelComponent>::appendDataObj(item);
    else
        mergeSameNameSet(existSet, set);
}

void FCComponentManager::insertDataObj(int index, FCAbstractDataObject* item)
{
    if (dynamic_cast<FCMeshSurface*>(item)) {
        FCAbstractDataManager<FCAbstractModelComponent>::insertDataObj(index, item);
        return;
    }
    FCModelSet* set = dynamic_cast<FCModelSet*>(item);
    if (!set) return;
    FCModelSet* existSet = dynamic_cast<FCModelSet*>(getDataByName(set->getDataObjectName()));
    if (!existSet)
        FCAbstractDataManager<FCAbstractModelComponent>::insertDataObj(index, item);
    else
        mergeSameNameSet(existSet, set);
}

FCAbstractModelComponent* FCComponentManager::getDataByID(int id)
{
    FCAbstractModelComponent* comp = FCAbstractDataManager<FCAbstractModelComponent>::getDataByID(id);
    if (comp) return comp;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCModelSet* set = dynamic_cast<FCModelSet*>(getDataByIndex(i));
        if (!set || !set->isCombination()) continue;
        comp = set->getDataByID(id);
        if (comp) return comp;
    }
    return nullptr;
}

FCModelSet* FCComponentManager::getModelSetByMember(FCModelEnum::FITKModelSetType t,
    int modelID, const QList<int>& member)
{
    if (member.isEmpty()) return nullptr;
    QList<FCModelSet*> sets = getModelSet(t);
    for (FCModelSet* set : sets) {
        if (!set || set->getAbsModelID() != modelID) continue;
        if (set->getAbsoluteMember() == member) return set;
    }
    FCModelSet* set = new FCModelSet(t);
    set->setInternal(true);
    set->setModel(modelID);
    for (int i : member) set->appendMember(i);
    set->setDataObjectName(checkName(QStringLiteral("Set-1")));
    appendDataObj(set);
    return set;
}

FCModelSet* FCComponentManager::getSingleNodeSet(int modelID, int memID)
{
    QList<FCModelSet*> sets = getModelSet(FCModelEnum::FMSNode);
    for (FCModelSet* set : sets) {
        if (!set || set->getAbsModelID() != modelID) continue;
        QList<int> mem = set->getMember();
        if (mem.size() == 1 && mem.at(0) == memID) return set;
    }
    return nullptr;
}

void FCComponentManager::mergeSameNameSet(FCModelSet* existSet, FCModelSet* newSet)
{
    if (!existSet || !newSet) return;
    if (existSet->isCombination()) {
        existSet->appendDataObj(newSet);
        return;
    }
    FCModelSet* comSet = new FCModelSet(FCModelEnum::FMSMIX);
    comSet->setDataObjectName(existSet->getDataObjectName());
    comSet->setModel(getMParentDataIDM());
    int index = getDataIndex(existSet);
    removeDataObjWithoutRelease(existSet);
    comSet->appendDataObj(existSet);
    comSet->appendDataObj(newSet);
    insertDataObj(index, comSet);
}

} // namespace FC
