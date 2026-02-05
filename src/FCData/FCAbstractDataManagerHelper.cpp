/**
 * @file FCAbstractDataManagerHelper.cpp
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAbstractDataManagerHelper.h"
#include <algorithm>

namespace FC {

FCAbstractDataManagerHelper::FCAbstractDataManagerHelper(QList<FCAbstractDataObject*>& list)
    : _dataList(list)
{
}

void FCAbstractDataManagerHelper::appendData(FCAbstractDataObject* item)
{
    if (!item) return;
    if (isContains(item)) { delete item; return; }
    _dataList.append(item);
    item->setParentDataID(_parentDataID);
    connectHelper(item);
}

void FCAbstractDataManagerHelper::appendData(const QList<FCAbstractDataObject*>& objs)
{
    for (auto* item : objs) {
        if (!item || isContains(item)) continue;
        _dataList.append(item);
        item->setParentDataID(_parentDataID);
        connectHelper(item);
    }
}

int FCAbstractDataManagerHelper::getDataCount() const
{
    return _dataList.size();
}

bool FCAbstractDataManagerHelper::isContains(FCAbstractDataObject* obj) const
{
    return _dataList.contains(obj);
}

void FCAbstractDataManagerHelper::clear()
{
    while (!_dataList.isEmpty()) {
        FCAbstractDataObject* obj = _dataList.takeFirst();
        if (obj) { disconnect(obj, nullptr, this, nullptr); delete obj; }
    }
}

void FCAbstractDataManagerHelper::insertDataObj(int index, FCAbstractDataObject* item)
{
    if (!item) return;
    if (index >= _dataList.size()) { appendData(item); return; }
    if (index < 0) index = 0;
    _dataList.insert(index, item);
    item->setParentDataID(_parentDataID);
    connectHelper(item);
}

FCAbstractDataObject* FCAbstractDataManagerHelper::getDataByIndex(int index)
{
    if (index < 0 || index >= _dataList.size()) return nullptr;
    return _dataList.at(index);
}

FCAbstractDataObject* FCAbstractDataManagerHelper::getDataByID(int id)
{
    for (auto* obj : _dataList)
        if (obj && obj->getDataObjectID() == id) return obj;
    return nullptr;
}

FCAbstractDataObject* FCAbstractDataManagerHelper::getDataByName(const QString& name, bool compSens)
{
    for (auto* obj : _dataList) {
        auto* nobj = dynamic_cast<FCAbstractNamedDataObject*>(obj);
        if (!nobj) continue;
        if (compSens) { if (name == nobj->getDataObjectName()) return obj; }
        else { if (name.toLower() == nobj->getDataObjectName().toLower()) return obj; }
    }
    return nullptr;
}

QList<FCAbstractDataObject*> FCAbstractDataManagerHelper::getDataListByName(const QString& name, bool compSens)
{
    QList<FCAbstractDataObject*> out;
    for (auto* obj : _dataList) {
        auto* nobj = dynamic_cast<FCAbstractNamedDataObject*>(obj);
        if (!nobj) continue;
        if (compSens) { if (name == nobj->getDataObjectName()) out.append(obj); }
        else { if (name.toLower() == nobj->getDataObjectName().toLower()) out.append(obj); }
    }
    return out;
}

int FCAbstractDataManagerHelper::getDataIndex(FCAbstractDataObject* obj) const
{
    return _dataList.indexOf(obj);
}

void FCAbstractDataManagerHelper::removeDataObj(FCAbstractDataObject* obj)
{
    int i = _dataList.indexOf(obj);
    if (i >= 0) { disconnect(obj, nullptr, this, nullptr); _dataList.removeAt(i); delete obj; }
}

void FCAbstractDataManagerHelper::removeDataObjWithoutRelease(FCAbstractDataObject* obj)
{
    _dataList.removeOne(obj);
}

void FCAbstractDataManagerHelper::removeDataByIndex(int index)
{
    if (index >= 0 && index < _dataList.size()) {
        FCAbstractDataObject* obj = _dataList.takeAt(index);
        if (obj) { disconnect(obj, nullptr, this, nullptr); delete obj; }
    }
}

void FCAbstractDataManagerHelper::removeDataByID(int id)
{
    for (int i = 0; i < _dataList.size(); ++i)
        if (_dataList.at(i) && _dataList.at(i)->getDataObjectID() == id) {
            FCAbstractDataObject* obj = _dataList.takeAt(i);
            disconnect(obj, nullptr, this, nullptr);
            delete obj;
            return;
        }
}

void FCAbstractDataManagerHelper::removeDataByName(const QString& name, bool compSens)
{
    for (int i = _dataList.size() - 1; i >= 0; --i) {
        auto* nobj = dynamic_cast<FCAbstractNamedDataObject*>(_dataList.at(i));
        if (!nobj) continue;
        bool match = compSens ? (name == nobj->getDataObjectName()) : (name.toLower() == nobj->getDataObjectName().toLower());
        if (match) {
            FCAbstractDataObject* obj = _dataList.takeAt(i);
            disconnect(obj, nullptr, this, nullptr);
            delete obj;
            return;
        }
    }
}

void FCAbstractDataManagerHelper::setParentDataID(int id)
{
    _parentDataID = id;
}

int FCAbstractDataManagerHelper::getParentDataID() const
{
    return _parentDataID;
}

QString FCAbstractDataManagerHelper::checkName(const QString& name)
{
    QString base = name;
    int n = 0;
    while (getDataByName(base, false)) base = name + QString::number(++n);
    return base;
}

int FCAbstractDataManagerHelper::getIndexByID(int dataId) const
{
    for (int i = 0; i < _dataList.size(); ++i)
        if (_dataList.at(i) && _dataList.at(i)->getDataObjectID() == dataId) return i;
    return -1;
}

int FCAbstractDataManagerHelper::getIDByIndex(int dataIndex) const
{
    if (dataIndex < 0 || dataIndex >= _dataList.size()) return -1;
    FCAbstractDataObject* obj = _dataList.at(dataIndex);
    return obj ? obj->getDataObjectID() : -1;
}

void FCAbstractDataManagerHelper::sortObject(ObjectSortType type)
{
    if (type == ObjectSortType::DataObjID)
        std::sort(_dataList.begin(), _dataList.end(), [](FCAbstractDataObject* a, FCAbstractDataObject* b) { return a && b && a->getDataObjectID() < b->getDataObjectID(); });
    else
        std::sort(_dataList.begin(), _dataList.end(), [](FCAbstractDataObject* a, FCAbstractDataObject* b) {
            auto* na = dynamic_cast<FCAbstractNamedDataObject*>(a);
            auto* nb = dynamic_cast<FCAbstractNamedDataObject*>(b);
            if (!na || !nb) return false;
            return na->getDataObjectName() < nb->getDataObjectName();
        });
}

void FCAbstractDataManagerHelper::sortObject(std::function<bool(FCAbstractDataObject*, FCAbstractDataObject*)> function)
{
    std::sort(_dataList.begin(), _dataList.end(), function);
}

bool FCAbstractDataManagerHelper::isUsedDataObject(const QList<int>& data) const
{
    for (auto* obj : _dataList) {
        if (!obj) continue;
        for (int id : data) if (obj->getDataObjectID() == id) return true;
    }
    return false;
}

void FCAbstractDataManagerHelper::dataObjectDestoried(FCAbstractDataObject* obj)
{
    _dataList.removeOne(obj);
}

void FCAbstractDataManagerHelper::connectHelper(FCAbstractDataObject* obj, bool con)
{
    if (!obj) return;
    if (con)
        connect(obj, &FCAbstractDataObject::dataObjectDestoried, this, &FCAbstractDataManagerHelper::dataObjectDestoried);
    else
        disconnect(obj, &FCAbstractDataObject::dataObjectDestoried, this, nullptr);
}

} // namespace FC
