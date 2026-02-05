/**
 * @file FCUnstructuredMesh.cpp
 * @brief 非结构网格实现（移植自 FITKInterfaceModel）
 */
#include "FCUnstructuredMesh.h"
#include "FCAbstractElement.h"
#include "FCUnstructuredMeshTopo.h"
#include <FCData/FCVec3Alg.h>
#include <FCData/FCPoint.h>
#include <algorithm>

namespace FC {

FCUnstructuredMesh::FCUnstructuredMesh()
{}

FCUnstructuredMesh::~FCUnstructuredMesh()
{
    if (_meshTopo) {
        delete _meshTopo;
        _meshTopo = nullptr;
    }
}

FCModelEnum::AbsModelType FCUnstructuredMesh::getAbsModelType()
{
    return FCModelEnum::AMTunstructuredMesh;
}

void FCUnstructuredMesh::update()
{}

FCModelEnum::FITKMeshDim FCUnstructuredMesh::getMeshDim()
{
    if (_elementList.isEmpty()) return FCModelEnum::FMDimNone;
    FCAbstractElement* ele = _elementList.at(0);
    if (!ele) return FCModelEnum::FMDimNone;
    FCModelEnum::FITKMeshDim dim = FCModelEnum::GetElementDim(ele->getEleType());
    const int n = getElementCount();
    for (int i = 1; i < n; ++i) {
        FCAbstractElement* elei = _elementList.at(i);
        if (!elei) continue;
        FCModelEnum::FITKMeshDim dimi = FCModelEnum::GetElementDim(elei->getEleType());
        if (dimi != dim) return FCModelEnum::FMDimMix;
    }
    return dim;
}

unsigned int FCUnstructuredMesh::getMeshDimBit()
{
    unsigned int dimBit = 0;
    if (_elementList.isEmpty()) return dimBit;
    const int n = getElementCount();
    for (int i = 0; i < n; ++i) {
        FCAbstractElement* elei = _elementList.at(i);
        if (!elei) continue;
        dimBit |= static_cast<unsigned int>(FCModelEnum::GetElementDim(elei->getEleType()));
    }
    return dimBit;
}

FCElementQuality FCUnstructuredMesh::checkElementQuality(int eleIndex)
{
    FCElementQuality q;
    FCAbstractElement* ele = getElementAt(eleIndex);
    if (!ele) return q;
    QList<double> edgeLength;
    const int n = ele->getEdgeCount();
    for (int i = 0; i < n; ++i) {
        QList<int> edge = ele->getEdge(i);
        double l = calMeshEdgeLength(edge);
        if (l > 0) edgeLength.append(l);
    }
    if (edgeLength.isEmpty()) return q;
    std::sort(edgeLength.begin(), edgeLength.end());
    q.minEdgeLength = edgeLength.first();
    q.maxEdgeLength = edgeLength.last();
    if (q.minEdgeLength > 1e-20)
        q.aspectRatio = q.maxEdgeLength / q.minEdgeLength;
    return q;
}

bool FCUnstructuredMesh::getElementDirection(double* dir, int id)
{
    FCAbstractElement* ele = getElementByID(id);
    if (!ele) return false;
    switch (ele->getElementDim()) {
    case 1: return getLineEleDirection(dir, ele);
    case 2: return getShellEleDirection(dir, ele);
    default: break;
    }
    return false;
}

bool FCUnstructuredMesh::hasOrphanMesh()
{
    for (FCNode* node : _nodeList) {
        if (node && !node->getNativeFlag()) return true;
    }
    for (FCAbstractElement* ele : _elementList) {
        if (ele && !ele->getNativeFlag()) return true;
    }
    return false;
}

bool FCUnstructuredMesh::hasNativeMesh()
{
    for (FCNode* node : _nodeList) {
        if (node && node->getNativeFlag()) return true;
    }
    for (FCAbstractElement* ele : _elementList) {
        if (ele && ele->getNativeFlag()) return true;
    }
    return false;
}

void FCUnstructuredMesh::clearNativeMesh()
{
    int nEles = getElementCount();
    for (int i = nEles - 1; i >= 0; i--) {
        FCAbstractElement* element = getElementAt(i);
        if (!element || !element->getNativeFlag()) continue;
        fastRemoveElementAt(i);
    }
    int nPoints = getNodeCount();
    for (int i = nPoints - 1; i >= 0; i--) {
        FCNode* node = getNodeAt(i);
        if (!node || !node->getNativeFlag()) continue;
        fastRemoveNodeAt(i);
    }
    updateNodeIDIndexMap();
    updateElementIDIndexMap();
    update();
}

void FCUnstructuredMesh::clearOrphanMesh()
{
    int nEles = getElementCount();
    for (int i = nEles - 1; i >= 0; i--) {
        FCAbstractElement* element = getElementAt(i);
        if (!element || element->getNativeFlag()) continue;
        fastRemoveElementAt(i);
    }
    int nPoints = getNodeCount();
    for (int i = nPoints - 1; i >= 0; i--) {
        FCNode* node = getNodeAt(i);
        if (!node || node->getNativeFlag()) continue;
        fastRemoveNodeAt(i);
    }
    updateNodeIDIndexMap();
    updateElementIDIndexMap();
    update();
}

bool FCUnstructuredMesh::buildUnstructuredMeshTopo()
{
    if (!_meshTopo)
        _meshTopo = new FCUnstructuredMeshTopo(this);
    return _meshTopo->buildUnstructuredMeshTopo();
}

double FCUnstructuredMesh::calMeshEdgeLength(const QList<int>& edge)
{
    double length = 0;
    if (edge.size() < 2) return -1;
    for (int i = 0; i < edge.size() - 1; ++i) {
        FCNode* node1 = getNodeByID(edge[i]);
        FCNode* node2 = getNodeByID(edge[i + 1]);
        if (node1 && node2)
            length += Distance(*node2, *node1);
    }
    return length;
}

bool FCUnstructuredMesh::getLineEleDirection(double* dir, FCAbstractElement* ele)
{
    if (!ele) return false;
    if (ele->getEdgeCount() <= 0) return false;
    QList<int> ed = ele->getEdge(0);
    if (ed.size() < 2) return false;
    FCNode* n1 = getNodeByID(ed.first());
    FCNode* n2 = getNodeByID(ed.last());
    if (!n1 || !n2) return false;
    FCVec3 v(*n2, *n1);
    dir[0] = v.x(); dir[1] = v.y(); dir[2] = v.z();
    return true;
}

bool FCUnstructuredMesh::getShellEleDirection(double* dir, FCAbstractElement* ele)
{
    if (!ele || ele->getEdgeCount() <= 2) return false;
    QList<int> ed1 = ele->getEdge(0);
    if (ed1.size() < 2) return false;
    FCNode* n1 = getNodeByID(ed1.first());
    FCNode* n2 = getNodeByID(ed1.last());
    QList<int> ed2 = ele->getEdge(1);
    if (ed2.size() < 2) return false;
    FCNode* n3 = getNodeByID(ed2.last());
    if (!n1 || !n2 || !n3) return false;
    FCPoint p = CrossProduct(Subtract(*n2, *n1), Subtract(*n3, *n1));
    dir[0] = p.x(); dir[1] = p.y(); dir[2] = p.z();
    return true;
}

FCUnstructuredMeshTopo* FCUnstructuredMesh::getUnstructuredMeshTopo()
{
    return _meshTopo;
}

bool FCUnstructuredMesh::getPointCoor(int pointID, double* coor, int modelIndex)
{
    Q_UNUSED(modelIndex);
    FCNode* node = getNodeByID(pointID);
    if (!node) return false;
    node->getCoor(coor);
    return true;
}

} // namespace FC
