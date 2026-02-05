/**
 * @file FCAbstractElement.cpp
 * @brief 单元抽象实现（移植自 FITKInterfaceModel）
 */
#include "FCAbstractElement.h"

namespace FC {

FCAbstractElement::~FCAbstractElement()
{
    emit elementDestroiedSignal(this);
}

FCModelEnum::FITKEleType FCAbstractElement::getEleType()
{
    return FCModelEnum::EleNone;
}

int FCAbstractElement::getNodeCount()
{
    return -1;
}

void FCAbstractElement::setEleID(int id)
{
    _eleID = id;
}

int FCAbstractElement::getEleID() const
{
    return _eleID;
}

int FCAbstractElement::getNodeID(int index) const
{
    if (index < 0 || index >= _elemenTopo.size()) return -1;
    return _elemenTopo.at(index);
}

int FCAbstractElement::getNodeIndex(int id) const
{
    return _elemenTopo.indexOf(id);
}

void FCAbstractElement::setNodeID(int index, int id)
{
    if (index < 0 || index >= getNodeCount()) return;
    _elemenTopo[index] = id;
}

void FCAbstractElement::setNodeID(const QList<int>& ids)
{
    if (ids.size() != getNodeCount()) return;
    _elemenTopo = ids;
}

int FCAbstractElement::getFaceCount()
{
    return -1;
}

int FCAbstractElement::getElementDim()
{
    return -1;
}

int FCAbstractElement::getEdgeCount()
{
    return -1;
}

QList<int> FCAbstractElement::getFace(int index, bool lowerOrder)
{
    Q_UNUSED(index);
    Q_UNUSED(lowerOrder);
    return QList<int>();
}

QList<int> FCAbstractElement::getEdge(int index)
{
    Q_UNUSED(index);
    return QList<int>();
}

void FCAbstractElement::reverse()
{
    const int c = _elemenTopo.size();
    for (int k = 0; k < c / 2; ++k)
        _elemenTopo.swapItemsAt(k, c - 1 - k);
}

int FCAbstractElement::getFaceIndex(const QList<int>& face, bool normalSens)
{
    return normalSens ? getFaceIndexNorSen(face) : getFaceIndexNorUnSen(face);
}

int FCAbstractElement::getEdgeIndex(const QList<int>& edge)
{
    const int nEdge = getEdgeCount();
    for (int i = 0; i < nEdge; i++) {
        QList<int> cEdge = getEdge(i);
        if (cEdge.size() != edge.size() || cEdge.isEmpty()) continue;
        QList<int> tmp = cEdge;
        for (int iNode : edge)
            tmp.removeOne(iNode);
        if (tmp.isEmpty()) return i;
    }
    return -1;
}

int FCAbstractElement::getFaceIndexNorSen(const QList<int>& face, bool lowerOrder)
{
    const int nFace = getFaceCount();
    for (int i = 0; i < nFace; i++) {
        QList<int> cFace = getFace(i, lowerOrder);
        const int nNode = cFace.size();
        if (nNode != face.size() || nNode == 0) continue;
        int cFNodeIndex = cFace.indexOf(face.at(0));
        if (cFNodeIndex == -1) continue;
        bool isthisFace = true;
        for (int iNode = 0; iNode < nNode; ++iNode) {
            if (face.at(iNode) != cFace.at((iNode + cFNodeIndex) % nNode)) {
                isthisFace = false;
                break;
            }
        }
        if (isthisFace) return i;
    }
    return -1;
}

int FCAbstractElement::getFaceIndexNorUnSen(const QList<int>& face, bool lowerOrder)
{
    const int nFace = getFaceCount();
    for (int i = 0; i < nFace; i++) {
        QList<int> cFace = getFace(i, lowerOrder);
        if (cFace.size() != face.size() || cFace.isEmpty()) continue;
        QList<int> tmp = cFace;
        for (int iNode : face)
            tmp.removeOne(iNode);
        if (tmp.isEmpty()) return i;
    }
    return -1;
}

int FCAbstractElement::getPreviousNodeID(int id)
{
    const int index = _elemenTopo.indexOf(id);
    if (index < 0) return -1;
    if (index == 0) return _elemenTopo.last();
    return _elemenTopo.at(index - 1);
}

int FCAbstractElement::getNextNodeID(int id)
{
    const int index = _elemenTopo.indexOf(id);
    if (index < 0) return -1;
    if (index == _elemenTopo.size() - 1) return _elemenTopo.first();
    return _elemenTopo.at(index + 1);
}

QList<int> FCAbstractElement::getAllNodes()
{
    return _elemenTopo;
}

void FCAbstractElement::replaceNodeID(const int& oldID, const int& newID)
{
    for (int i = 0; i < _elemenTopo.size(); ++i) {
        if (_elemenTopo[i] == oldID)
            _elemenTopo[i] = newID;
    }
}

void FCAbstractElement::setNodeCount(int c)
{
    _elemenTopo.clear();
    for (int i = 0; i < c; ++i)
        _elemenTopo.append(-1);
}

void FCAbstractElement::setTag(unsigned int tag)
{
    _tag = tag;
}

unsigned int FCAbstractElement::getTag() const
{
    return _tag;
}

void FCAbstractElement::setNativeFlag(bool flag)
{
    _nativeFlag = flag;
}

bool FCAbstractElement::getNativeFlag()
{
    return _nativeFlag;
}

} // namespace FC
