/**
 * @file FCElementList.cpp
 * @brief 单元列表实现（移植自 FITKInterfaceModel）
 */
#include "FCElementList.h"
#include "FCAbstractElement.h"
#include <QMutexLocker>
#include <QSet>

namespace FC {

FCElementList::~FCElementList()
{
    removeAllElement();
}

void FCElementList::appendElement(FCAbstractElement* element)
{
    if (!element) return;
    _elementList.append(element);
    const int id = element->getEleID();
    const int index = _elementList.size() - 1;
    _idIndexMap.insert(id, index);
    if (id > _eleMaxID) _eleMaxID = id;
}

int FCElementList::getEleIDByIndex(int index)
{
    if (index < 0 || index >= _elementList.size()) return -1;
    FCAbstractElement* ele = _elementList[index];
    return ele ? ele->getEleID() : -1;
}

int FCElementList::getEleIndexByID(int id) const
{
    return _idIndexMap.value(id, -1);
}

FCAbstractElement* FCElementList::getElementAt(int index)
{
    if (index < 0 || index >= _elementList.size()) return nullptr;
    return _elementList.at(index);
}

FCAbstractElement* FCElementList::getElementByID(int id)
{
    const int index = _idIndexMap.value(id, -1);
    if (index < 0 || index >= _elementList.size()) return nullptr;
    return _elementList.at(index);
}

int FCElementList::getElementCount()
{
    return _elementList.size();
}

void FCElementList::reverseEleByID(int id)
{
    FCAbstractElement* ele = getElementByID(id);
    if (ele) ele->reverse();
}

void FCElementList::reverseEleByIndex(int index)
{
    FCAbstractElement* ele = getElementAt(index);
    if (ele) ele->reverse();
}

int FCElementList::getElementMaxID()
{
    return _eleMaxID;
}

void FCElementList::removeAllElement()
{
    for (FCAbstractElement* ele : _elementList)
        delete ele;
    _elementList.clear();
    _idIndexMap.clear();
    _eleMaxID = 0;
}

int FCElementList::removeElementAt(int index)
{
    QMutexLocker locker(&_mutex);
    if (index < 0 || index >= _elementList.size()) return -1;
    FCAbstractElement* ele = _elementList[index];
    _elementList.removeAt(index);
    if (!ele) return -1;
    const int id = ele->getEleID();
    _idIndexMap.remove(id);
    delete ele;
    updateElementIDIndexMap();
    return id;
}

int FCElementList::removeElementByID(int id)
{
    QMutexLocker locker(&_mutex);
    const int index = _idIndexMap.value(id, -1);
    if (index < 0 || index >= _elementList.size()) return -1;
    FCAbstractElement* ele = _elementList[index];
    _elementList.removeAt(index);
    _idIndexMap.remove(id);
    if (!ele) return -1;
    delete ele;
    updateElementIDIndexMap();
    return index;
}

QList<FCAbstractElement*> FCElementList::getNeighborElement(int nodeID)
{
    QList<FCAbstractElement*> eles;
    for (FCAbstractElement* ele : _elementList) {
        if (!ele) continue;
        if (ele->getNodeIndex(nodeID) >= 0)
            eles.append(ele);
    }
    return eles;
}

QList<int> FCElementList::getAllNodeIDs() const
{
    QSet<int> all;
    for (FCAbstractElement* e : _elementList) {
        if (!e) continue;
        for (int id : e->getAllNodes())
            all.insert(id);
    }
    QList<int> list;
    for (int id : all)
        list.append(id);
    return list;
}

void FCElementList::replaceNodesID(const QHash<int, int>& nodes)
{
    for (FCAbstractElement* ele : _elementList) {
        if (!ele) continue;
        for (auto it = nodes.begin(); it != nodes.end(); ++it)
            ele->replaceNodeID(it.key(), it.value());
    }
}

void FCElementList::updateElementIDIndexMap()
{
    _idIndexMap.clear();
    _eleMaxID = 0;
    int index = 0;
    for (FCAbstractElement* ele : _elementList) {
        if (!ele) continue;
        int eId = ele->getEleID();
        _idIndexMap[eId] = index++;
        if (eId > _eleMaxID) _eleMaxID = eId;
    }
}

void FCElementList::fastRemoveElementAt(int index)
{
    if (index < 0 || index >= _elementList.size()) return;
    FCAbstractElement* ele = _elementList[index];
    _elementList.removeAt(index);
    if (ele) delete ele;
}

void FCElementList::fastRemoveElementByID(int id)
{
    const int index = _idIndexMap.value(id, -1);
    if (index < 0 || index >= _elementList.size()) return;
    FCAbstractElement* ele = _elementList[index];
    _elementList.removeAt(index);
    if (ele) delete ele;
}

} // namespace FC
