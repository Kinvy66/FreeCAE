/**
 * @file FCUnstructuredMeshTopo.cpp
 * @brief 非结构网格拓扑实现（移植自 FITKInterfaceModel）
 */
#include "FCUnstructuredMeshTopo.h"
#include "FCUnstructuredMesh.h"
#include "FCAbstractElement.h"
#include <FCData/FCPoint.h>

namespace FC {

FCNodeElements::FCNodeElements(FCNode* node)
    : _node(node)
{}

void FCNodeElements::appendElement(FCAbstractElement* element)
{
    if (!element || _elements.contains(element)) return;
    _elements.append(element);
    connect(element, &FCAbstractElement::elementDestroiedSignal, this, &FCNodeElements::onElementDestroiedSlot);
}

FCNode* FCNodeElements::getNode() const
{
    return _node;
}

QList<FCAbstractElement*> FCNodeElements::getElements() const
{
    return _elements;
}

QList<int> FCNodeElements::getElementIDs() const
{
    QList<int> ids;
    for (FCAbstractElement* ele : _elements) {
        if (ele) ids.append(ele->getEleID());
    }
    return ids;
}

void FCNodeElements::onElementDestroiedSlot(FCAbstractElement* ele)
{
    if (ele) _elements.removeAll(ele);
}

FCUnstructuredMeshTopo::FCUnstructuredMeshTopo(FCUnstructuredMesh* mesh)
    : _mesh(mesh)
{}

FCUnstructuredMeshTopo::~FCUnstructuredMeshTopo()
{
    for (FCNodeElements* ne : _nodeElementHash) {
        if (ne) delete ne;
    }
    _nodeElementHash.clear();
}

bool FCUnstructuredMeshTopo::buildUnstructuredMeshTopo()
{
    if (!_mesh) return false;
    const int nEle = _mesh->getElementCount();
    for (int i = 0; i < nEle; i++) {
        FCAbstractElement* ele = _mesh->getElementAt(i);
        if (!ele) continue;
        addElementTopo(ele);
    }
    return true;
}

bool FCUnstructuredMeshTopo::addElementTopo(FCAbstractElement* element)
{
    if (!element) return false;
    const int nNode = element->getNodeCount();
    for (int i = 0; i < nNode; i++) {
        const int nodeID = element->getNodeID(i);
        if (nodeID < 0) continue;
        FCNode* node = _mesh->getNodeByID(nodeID);
        if (!node) continue;
        FCNodeElements* ne = nullptr;
        if (!_nodeElementHash.contains(nodeID)) {
            ne = new FCNodeElements(node);
            _nodeElementHash.insert(nodeID, ne);
            connect(node, &FCNode::nodeDestroiedSignal, this, &FCUnstructuredMeshTopo::onNodeDestroiedSlot);
        } else {
            ne = _nodeElementHash.value(nodeID);
        }
        if (ne) ne->appendElement(element);
    }
    return true;
}

QList<FCAbstractElement*> FCUnstructuredMeshTopo::getNodeElements(int nodeID) const
{
    FCNodeElements* ne = _nodeElementHash.value(nodeID);
    if (ne) return ne->getElements();
    return QList<FCAbstractElement*>();
}

void FCUnstructuredMeshTopo::onNodeDestroiedSlot(FCNode* node)
{
    if (!node) return;
    int id = node->getNodeID();
    if (_nodeElementHash.contains(id)) {
        FCNodeElements* ne = _nodeElementHash.value(id);
        if (ne) delete ne;
        _nodeElementHash.remove(id);
    }
}

} // namespace FC
