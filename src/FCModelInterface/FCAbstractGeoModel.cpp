/**
 * @file FCAbstractGeoModel.cpp
 * @brief 抽象几何模型实现（移植自 FITKInterfaceModel）
 */
#include "FCAbstractGeoModel.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCAbstractNamedDataObject.h>

namespace FC {

FCAbstractGeoModel::FCAbstractGeoModel()
{}

FCAbstractGeoModel::~FCAbstractGeoModel()
{}

int FCAbstractGeoModel::getShapeCount(FCModelEnum::FITKModelSetType type)
{
    Q_UNUSED(type);
    return 0;
}

void FCAbstractGeoModel::getTriangleMeshVS(FCAbstractDataObject* mesh, int id, void* additional)
{
    Q_UNUSED(mesh);
    Q_UNUSED(id);
    Q_UNUSED(additional);
}

bool FCAbstractGeoModel::writeSTLFile(QString filePath)
{
    Q_UNUSED(filePath);
    return false;
}

FCGeoComponentManager* FCAbstractGeoModel::getGeoComponentManager()
{
    return _compManager;
}

bool FCAbstractGeoModel::getBoundaryBox(double* minPt, double* maxPt)
{
    Q_UNUSED(minPt);
    Q_UNUSED(maxPt);
    return false;
}

void FCAbstractGeoModel::setAbsDataID(int id)
{
    FCAbstractDataObject::setAbsDataID(id);
}

// --- FCGeoComponent ---
FCGeoComponent::FCGeoComponent(FCModelEnum::FITKModelSetType type)
    : _type(type)
{}

void FCGeoComponent::setMember(QList<int>& member)
{
    _member = member;
}

void FCGeoComponent::appendMember(int mem)
{
    _member.append(mem);
}

void FCGeoComponent::clear()
{
    _member.clear();
}

void FCGeoComponent::removeMember(int mem)
{
    _member.removeOne(mem);
}

QList<int> FCGeoComponent::getMember() const
{
    return _member;
}

FCModelEnum::FITKModelSetType FCGeoComponent::getGeoType() const
{
    return _type;
}

// --- FCGeoComponentManager ---
void FCGeoComponentManager::appendDataObj(FCAbstractDataObject* item)
{
    FCAbstractDataManager<FCGeoComponent>::appendDataObj(item);
}

QList<FCGeoComponent*> FCGeoComponentManager::getGeoComponent(FCModelEnum::FITKModelSetType type)
{
    QList<FCGeoComponent*> list;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCGeoComponent* c = getDataByIndex(i);
        if (c && c->getGeoType() == type) list.append(c);
    }
    return list;
}

QList<int> FCGeoComponentManager::getAllComponentMember(FCModelEnum::FITKModelSetType type)
{
    QList<int> mem;
    QList<FCGeoComponent*> list = getGeoComponent(type);
    for (FCGeoComponent* c : list) {
        if (c) mem.append(c->getMember());
    }
    return mem;
}

} // namespace FC
