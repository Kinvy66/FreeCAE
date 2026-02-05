/**
 * @file FCNodeList.cpp
 * @brief 节点列表实现（移植自 FITKInterfaceModel）
 */
#include "FCNodeList.h"
#include <FCData/FCPoint.h>
#include <QMutexLocker>

namespace FC {

FCNodeList::~FCNodeList()
{
    removeAllNode();
}

int FCNodeList::addNode(FCNode* node)
{
    QMutexLocker locker(&_mutex);
    if (!node) return -1;
    _nodeList.append(node);
    const int index = _nodeList.size() - 1;
    int id = node->getNodeID();
    if (id < 0) {
        id = ++_nodeMaxID;
        node->setNodeID(id);
    }
    _idIndexMap.insert(id, index);
    _nodeMaxID = qMax(_nodeMaxID, id);
    return id;
}

int FCNodeList::addNode(double x, double y, double z, bool isNative, unsigned int tag)
{
    Q_UNUSED(isNative);
    Q_UNUSED(tag);
    QMutexLocker locker(&_mutex);
    FCNode* node = new FCNode(0, x, y, z);
    _nodeList.append(node);
    const int index = _nodeList.size() - 1;
    const int id = ++_nodeMaxID;
    node->setNodeID(id);
    _idIndexMap.insert(id, index);
    return id;
}

void FCNodeList::addNode(int id, double x, double y, double z, bool isNative, unsigned int tag)
{
    Q_UNUSED(isNative);
    Q_UNUSED(tag);
    QMutexLocker locker(&_mutex);
    FCNode* node = new FCNode(id, x, y, z);
    _nodeList.append(node);
    const int index = _nodeList.size() - 1;
    _idIndexMap.insert(id, index);
    _nodeMaxID = qMax(_nodeMaxID, id);
}

void FCNodeList::removeNodeAt(int index)
{
    QMutexLocker locker(&_mutex);
    if (index < 0 || index >= _nodeList.size()) return;
    FCNode* node = _nodeList[index];
    _nodeList.removeAt(index);
    if (node) {
        _idIndexMap.remove(node->getNodeID());
        delete node;
    }
    updateNodeIDIndexMap();
}

void FCNodeList::removeNodeByID(int id)
{
    QMutexLocker locker(&_mutex);
    const int index = _idIndexMap.value(id, -1);
    if (index < 0 || index >= _nodeList.size()) return;
    FCNode* node = _nodeList[index];
    _nodeList.removeAt(index);
    _idIndexMap.remove(id);
    if (node) delete node;
    updateNodeIDIndexMap();
}

void FCNodeList::removeNodeByIDs(const QList<int>& ids)
{
    QMutexLocker locker(&_mutex);
    QList<FCNode*> nodes;
    for (int id : ids) {
        const int index = _idIndexMap.value(id, -1);
        if (index >= 0 && index < _nodeList.size())
            nodes.append(_nodeList[index]);
    }
    for (FCNode* node : nodes) {
        if (node) {
            _nodeList.removeOne(node);
            delete node;
        }
    }
    updateNodeIDIndexMap();
}

int FCNodeList::getNodeCount()
{
    QMutexLocker locker(&_mutex);
    return _nodeList.size();
}

FCNode* FCNodeList::getNodeAt(int index)
{
    QMutexLocker locker(&_mutex);
    if (index < 0 || index >= _nodeList.size()) return nullptr;
    return _nodeList[index];
}

FCNode* FCNodeList::getNodeByID(int id)
{
    QMutexLocker locker(&_mutex);
    if (!_idIndexMap.contains(id)) return nullptr;
    const int index = _idIndexMap.value(id);
    if (index < 0 || index >= _nodeList.size()) return nullptr;
    return _nodeList[index];
}

int FCNodeList::getNodeIDByIndex(int index)
{
    QMutexLocker locker(&_mutex);
    if (index < 0 || index >= _nodeList.size()) return -1;
    FCNode* node = _nodeList[index];
    return node ? node->getNodeID() : -1;
}

int FCNodeList::getNodeIndexByID(int id)
{
    QMutexLocker locker(&_mutex);
    return _idIndexMap.value(id, -1);
}

int FCNodeList::getNodeMaxID()
{
    return _nodeMaxID;
}

void FCNodeList::removeAllNode()
{
    for (FCNode* node : _nodeList)
        delete node;
    _nodeList.clear();
    _idIndexMap.clear();
    _nodeMaxID = 0;
}

void FCNodeList::updateNodeIDIndexMap()
{
    _idIndexMap.clear();
    int index = 0;
    _nodeMaxID = 0;
    for (FCNode* n : _nodeList) {
        if (!n) continue;
        _idIndexMap[n->getNodeID()] = index++;
        _nodeMaxID = qMax(_nodeMaxID, n->getNodeID());
    }
}

void FCNodeList::getNodeBndBox(double* bndBox) const
{
    if (!bndBox) return;
    bndBox[0] = bndBox[2] = bndBox[4] = 1e64;
    bndBox[1] = bndBox[3] = bndBox[5] = -1e64;
    for (FCNode* node : _nodeList) {
        if (!node) continue;
        double x = node->x(), y = node->y(), z = node->z();
        if (x < bndBox[0]) bndBox[0] = x;
        if (x > bndBox[1]) bndBox[1] = x;
        if (y < bndBox[2]) bndBox[2] = y;
        if (y > bndBox[3]) bndBox[3] = y;
        if (z < bndBox[4]) bndBox[4] = z;
        if (z > bndBox[5]) bndBox[5] = z;
    }
}

void FCNodeList::fastRemoveNodeAt(int index)
{
    if (index < 0 || index >= _nodeList.size()) return;
    FCNode* node = _nodeList[index];
    _nodeList.removeAt(index);
    if (node) delete node;
}

void FCNodeList::fastRemoveNodeByID(int id)
{
    const int index = _idIndexMap.value(id, -1);
    if (index < 0 || index >= _nodeList.size()) return;
    FCNode* node = _nodeList[index];
    _nodeList.removeAt(index);
    if (node) delete node;
}

} // namespace FC
