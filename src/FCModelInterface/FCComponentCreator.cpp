/**
 * @file FCComponentCreator.cpp
 * @brief 模型部件创建器实现（移植自 FITKInterfaceModel）
 */
#include "FCComponentCreator.h"
#include "FCAbstractModel.h"
#include "FCModelSet.h"
#include "FCComponentManager.h"
#include "FCMeshSurfaceElement.h"
#include "FCMeshSurfaceNode.h"
#include "FCModelEnum.h"
#include <FCData/FCDataRepo.h>

namespace FC {

FCComponentCreator::FCComponentCreator(FCComponentManager* manager)
    : _compManager(manager)
{}

void FCComponentCreator::setName(const QString& name)
{
    _name = name;
}

void FCComponentCreator::setInternal(bool internal)
{
    _internal = internal;
}

FCModelSet* FCComponentCreator::createNodeSet(const int& parentID, const QList<int>& member, bool addCompManager)
{
    FCModelSet* set = createSet(parentID, member, addCompManager);
    if (set) set->setModelSetType(FCModelEnum::FMSNode);
    return set;
}

FCModelSet* FCComponentCreator::createEleSet(const int& parentID, const QList<int>& member, bool addCompManager)
{
    FCModelSet* set = createSet(parentID, member, addCompManager);
    if (set) set->setModelSetType(FCModelEnum::FMSElem);
    return set;
}

FCModelSet* FCComponentCreator::createGeometryVertexSet(const int& parentID, const QList<int>& member, bool addCompManager)
{
    FCModelSet* set = createSet(parentID, member, addCompManager);
    if (set) set->setModelSetType(FCModelEnum::FMSPoint);
    return set;
}

FCModelSet* FCComponentCreator::createGeometryEdgeSet(const int& parentID, const QList<int>& member, bool addCompManager)
{
    FCModelSet* set = createSet(parentID, member, addCompManager);
    if (set) set->setModelSetType(FCModelEnum::FMSEdge);
    return set;
}

FCModelSet* FCComponentCreator::createGeometryFaceSet(const int& parentID, const QList<int>& member, bool addCompManager)
{
    FCModelSet* set = createSet(parentID, member, addCompManager);
    if (set) set->setModelSetType(FCModelEnum::FMSSurface);
    return set;
}

FCModelSet* FCComponentCreator::createGeometrySolidSet(const int& parentID, const QList<int>& member, bool addCompManager)
{
    FCModelSet* set = createSet(parentID, member, addCompManager);
    if (set) set->setModelSetType(FCModelEnum::FMSSolid);
    return set;
}

FCModelSet* FCComponentCreator::editNodeSet(const int& parentID, const QList<int>& member, int originId)
{
    if (!_compManager) return nullptr;
    if (FCDATAREPO->getDataAs<FCAbstractModel>(parentID) == nullptr) return nullptr;
    if (member.isEmpty()) return nullptr;
    FCModelSet* set = dynamic_cast<FCModelSet*>(_compManager->getDataByID(originId));
    if (!set) return nullptr;
    set->setModel(parentID);
    set->setAbsoluteMember(member);
    return set;
}

FCModelSet* FCComponentCreator::editEleSet(const int& parentID, const QList<int>& member, int originId)
{
    if (!_compManager) return nullptr;
    if (FCDATAREPO->getDataAs<FCAbstractModel>(parentID) == nullptr) return nullptr;
    if (member.isEmpty()) return nullptr;
    FCModelSet* set = dynamic_cast<FCModelSet*>(_compManager->getDataByID(originId));
    if (!set) return nullptr;
    set->setModel(parentID);
    set->setAbsoluteMember(member);
    return set;
}

FCMeshSurface* FCComponentCreator::createEleSurface(QHash<FCModelSet*, int>& setSurfaceIndex)
{
    if (!_compManager || setSurfaceIndex.isEmpty()) return nullptr;
    QList<FCModelSet*> sets = setSurfaceIndex.keys();
    FCMeshSurfaceElement* surf = new FCMeshSurfaceElement;
    surf->setModel(_compManager->getMParentDataIDM());
    surf->setDataObjectName(_name);
    for (FCModelSet* set : sets) {
        if (!set) continue;
        int surfIndex = setSurfaceIndex.value(set);
        if (surfIndex < 0) continue;
        surf->addMeshSet(set->getAbstractModel(), set, surfIndex);
    }
    _compManager->appendDataObj(surf);
    return surf;
}

FCMeshSurface* FCComponentCreator::editEleSurface(const QHash<FCModelSet*, int>& setSurfaceIndex, int originId)
{
    if (!_compManager || setSurfaceIndex.isEmpty()) return nullptr;
    FCMeshSurfaceElement* surf = dynamic_cast<FCMeshSurfaceElement*>(_compManager->getDataByID(originId));
    if (!surf) return nullptr;
    surf->clearMeshSet();
    QList<FCModelSet*> sets = setSurfaceIndex.keys();
    for (FCModelSet* set : sets) {
        if (!set) continue;
        int surfIndex = setSurfaceIndex.value(set);
        if (surfIndex < 0) continue;
        surf->addMeshSet(set->getAbstractModel(), set, surfIndex);
    }
    return surf;
}

FCMeshSurface* FCComponentCreator::createNodeSurface(QHash<FCModelSet*, int>& setSurfaceIndex)
{
    if (!_compManager || setSurfaceIndex.isEmpty()) return nullptr;
    QList<FCModelSet*> sets = setSurfaceIndex.keys();
    FCMeshSurfaceNode* surf = new FCMeshSurfaceNode;
    surf->setModel(_compManager->getMParentDataIDM());
    surf->setDataObjectName(_name);
    for (FCModelSet* set : sets) {
        if (!set) continue;
        int surfIndex = setSurfaceIndex.value(set);
        if (surfIndex < 0) continue;
        surf->addMeshSet(set->getAbstractModel(), set, static_cast<double>(surfIndex));
    }
    _compManager->appendDataObj(surf);
    return surf;
}

FCMeshSurface* FCComponentCreator::editNodeSurface(const QHash<FCModelSet*, int>& setSurfaceIndex, int originId)
{
    if (!_compManager || setSurfaceIndex.isEmpty()) return nullptr;
    FCMeshSurfaceNode* surf = dynamic_cast<FCMeshSurfaceNode*>(_compManager->getDataByID(originId));
    if (!surf) return nullptr;
    surf->clearMeshSet();
    QList<FCModelSet*> sets = setSurfaceIndex.keys();
    for (FCModelSet* set : sets) {
        if (!set) continue;
        int surfIndex = setSurfaceIndex.value(set);
        if (surfIndex < 0) continue;
        surf->addMeshSet(set->getAbstractModel(), set, static_cast<double>(surfIndex));
    }
    return surf;
}

FCModelSet* FCComponentCreator::createSet(const int& parentID, const QList<int>& member, bool addCompManager)
{
    if (!_compManager) return nullptr;
    if (FCDATAREPO->getDataAs<FCAbstractModel>(parentID) == nullptr) return nullptr;
    if (member.isEmpty()) return nullptr;
    FCModelSet* set = new FCModelSet;
    set->setModel(parentID);
    set->setAbsoluteMember(member);
    if (addCompManager) {
        set->setDataObjectName(_name);
        set->setInternal(_internal);
        _compManager->appendDataObj(set);
    }
    return set;
}

} // namespace FC
