/**
 * @file FCAssElement.cpp
 * @brief 装配单元实现（移植自 FITKInterfaceModel）
 */
#include "FCAssElement.h"
#include "FCAbstractModel.h"
#include <FCData/FCDataRepo.h>

namespace FC {

static int AssEleID = 0;

FCAbstractAssemblyElement::FCAbstractAssemblyElement()
{
    _elementID = ++AssEleID;
}

int FCAbstractAssemblyElement::getElementID() const
{
    return _elementID;
}

void FCAbstractAssemblyElement::setElementID(int id)
{
    if (id > AssEleID) AssEleID = id;
    _elementID = id;
}

FCModelEnum::FITKModelSetType FCAssemblyElementSet::getModelSetType()
{
    return FCModelEnum::FMSNone;
}

void FCAssemblyElementVertex::copyTo(FCAbstractAssemblyElement* e)
{
    FCAssemblyElementVertex* v = dynamic_cast<FCAssemblyElementVertex*>(e);
    if (!v) return;
    v->setModelID(_nodeModelID);
    v->setNodeID(_nodeID);
}

void FCAssemblyElementVertex::setModelID(int mid)
{
    _nodeModelID = mid;
}

void FCAssemblyElementVertex::setNodeID(int id)
{
    _nodeID = id;
}

void FCAssemblyElementVertex::getCoor(double* coor)
{
    if (!coor) return;
    FCAbstractModel* m = FCDATAREPO->getDataAs<FCAbstractModel>(_nodeModelID);
    if (!m) return;
    m->getPointCoor(_nodeID, coor);
}

void FCAssemblyElementVertex::getNode(int& mid, int& id)
{
    mid = _nodeModelID;
    id = _nodeID;
}

void FCAssemblyElementLine::copyTo(FCAbstractAssemblyElement* e)
{
    FCAssemblyElementLine* l = dynamic_cast<FCAssemblyElementLine*>(e);
    if (!l) return;
    l->setNode1(_node1.first, _node1.second);
    l->setNode2(_node2.first, _node2.second);
}

void FCAssemblyElementLine::setNode1(int mid, int id)
{
    _node1.first = mid;
    _node1.second = id;
}

void FCAssemblyElementLine::setNode2(int mid, int id)
{
    _node2.first = mid;
    _node2.second = id;
}

void FCAssemblyElementLine::getNode1(int& mid, int& id)
{
    mid = _node1.first;
    id = _node1.second;
}

void FCAssemblyElementLine::getNode2(int& mid, int& id)
{
    mid = _node2.first;
    id = _node2.second;
}

void FCAssemblyElementLine::getCoor1(double* coor1)
{
    if (!coor1) return;
    FCAbstractModel* m = FCDATAREPO->getDataAs<FCAbstractModel>(_node1.first);
    if (!m) return;
    m->getPointCoor(_node1.second, coor1);
}

void FCAssemblyElementLine::getCoor2(double* coor2)
{
    if (!coor2) return;
    FCAbstractModel* m = FCDATAREPO->getDataAs<FCAbstractModel>(_node2.first);
    if (!m) return;
    m->getPointCoor(_node2.second, coor2);
}

QString FCAssemblyElementLine::getTextLabel1()
{
    FCAbstractModel* m = FCDATAREPO->getDataAs<FCAbstractModel>(_node1.first);
    if (!m) return QString();
    if (m->getAbsModelType() == FCModelEnum::Assembly)
        return QString("Node[%1]").arg(_node1.second);
    return QString("%1 Node[%2]").arg(m->getDataObjectName()).arg(_node1.second);
}

QString FCAssemblyElementLine::getTextLabel2()
{
    FCAbstractModel* m = FCDATAREPO->getDataAs<FCAbstractModel>(_node2.first);
    if (!m) return QString();
    if (m->getAbsModelType() == FCModelEnum::Assembly)
        return QString("Node[%1]").arg(_node2.second);
    return QString("%1 Node[%2]").arg(m->getDataObjectName()).arg(_node2.second);
}

void FCAssemblyElementLine::swap()
{
    QPair<int, int> t = _node1;
    _node1 = _node2;
    _node2 = t;
}

} // namespace FC
