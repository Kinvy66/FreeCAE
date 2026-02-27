/**
 * @file FCAbstractDataManagerHelper.cpp
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAbstractDataManagerHelper.h"
#include <algorithm>
#include <QRegularExpression>

namespace FC {

FCAbstractDataManagerHelper::FCAbstractDataManagerHelper(QList<FCAbstractDataObject*>& list)
    : mDataList(list)
{
}

void FCAbstractDataManagerHelper::appendData(FCAbstractDataObject* item)
{
    if (!item) return;
    if (isContains(item)) { delete item; return; }
    mDataList.append(item);
    item->setParentDataID(mParentDataID);
    connectHelper(item);
}

void FCAbstractDataManagerHelper::appendData(const QList<FCAbstractDataObject*>& objs)
{
    for (auto* item : objs) {
        if (!item || isContains(item)) continue;
        mDataList.append(item);
        item->setParentDataID(mParentDataID);
        connectHelper(item);
    }
}

int FCAbstractDataManagerHelper::getDataCount() const
{
    return mDataList.size();
}

bool FCAbstractDataManagerHelper::isContains(FCAbstractDataObject* obj) const
{
    return mDataList.contains(obj);
}

void FCAbstractDataManagerHelper::clear()
{
    while (!mDataList.isEmpty()) {
        FCAbstractDataObject* obj = mDataList.takeFirst();
        if (obj) { disconnect(obj, nullptr, this, nullptr); delete obj; }
    }
}

void FCAbstractDataManagerHelper::insertDataObj(int index, FCAbstractDataObject* item)
{
    if (!item) return;
    if (index >= mDataList.size()) { appendData(item); return; }
    if (index < 0) index = 0;
    mDataList.insert(index, item);
    item->setParentDataID(mParentDataID);
    connectHelper(item);
}

FCAbstractDataObject* FCAbstractDataManagerHelper::getDataByIndex(int index)
{
    if (index < 0 || index >= mDataList.size()) return nullptr;
    return mDataList.at(index);
}

FCAbstractDataObject* FCAbstractDataManagerHelper::getDataByID(FCID id)
{
    for (auto* obj : mDataList)
        if (obj && obj->getDataObjectID() == id) return obj;
    return nullptr;
}

FCAbstractDataObject* FCAbstractDataManagerHelper::getDataByName(const QString& name, bool compSens)
{
    for (auto* obj : mDataList) {
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
    for (auto* obj : mDataList) {
        auto* nobj = dynamic_cast<FCAbstractNamedDataObject*>(obj);
        if (!nobj) continue;
        if (compSens) { if (name == nobj->getDataObjectName()) out.append(obj); }
        else { if (name.toLower() == nobj->getDataObjectName().toLower()) out.append(obj); }
    }
    return out;
}

int FCAbstractDataManagerHelper::getDataIndex(FCAbstractDataObject* obj) const
{
    return mDataList.indexOf(obj);
}

void FCAbstractDataManagerHelper::removeDataObj(FCAbstractDataObject* obj)
{
    int i = mDataList.indexOf(obj);
    if (i >= 0) { disconnect(obj, nullptr, this, nullptr); mDataList.removeAt(i); delete obj; }
}

void FCAbstractDataManagerHelper::removeDataObjWithoutRelease(FCAbstractDataObject* obj)
{
    mDataList.removeOne(obj);
}

void FCAbstractDataManagerHelper::removeDataByIndex(int index)
{
    if (index >= 0 && index < mDataList.size()) {
        FCAbstractDataObject* obj = mDataList.takeAt(index);
        if (obj) { disconnect(obj, nullptr, this, nullptr); delete obj; }
    }
}

void FCAbstractDataManagerHelper::removeDataByID(FCID id)
{
    for (int i = 0; i < mDataList.size(); ++i)
        if (mDataList.at(i) && mDataList.at(i)->getDataObjectID() == id) {
            FCAbstractDataObject* obj = mDataList.takeAt(i);
            disconnect(obj, nullptr, this, nullptr);
            delete obj;
            return;
        }
}

void FCAbstractDataManagerHelper::removeDataByName(const QString& name, bool compSens)
{
    for (int i = mDataList.size() - 1; i >= 0; --i) {
        auto* nobj = dynamic_cast<FCAbstractNamedDataObject*>(mDataList.at(i));
        if (!nobj) continue;
        bool match = compSens ? (name == nobj->getDataObjectName()) : (name.toLower() == nobj->getDataObjectName().toLower());
        if (match) {
            FCAbstractDataObject* obj = mDataList.takeAt(i);
            disconnect(obj, nullptr, this, nullptr);
            delete obj;
            return;
        }
    }
}

void FCAbstractDataManagerHelper::setParentDataID(FCID id)
{
    mParentDataID = id;
}

FCID FCAbstractDataManagerHelper::getParentDataID() const
{
    return mParentDataID;
}

QString FCAbstractDataManagerHelper::checkName(const QString& name)
{
    // 与 AppFlow 一致：支持 "前缀+数字"（如 Box_1、Box-1），扫描现有名称取最大编号+1，保证连续创建时 1,2,3 递增
    QString simplified = name.simplified();
    if (simplified.isEmpty()) return name;

    QRegularExpression suffixRegex(QStringLiteral("^(.+)[_\\-](\\d+)$"));
    QRegularExpressionMatch m = suffixRegex.match(simplified);
    QString prefix;
    int suggestedNum = 1;
    if (m.hasMatch()) {
        prefix = m.captured(1);
        suggestedNum = m.captured(2).toInt();
    } else {
        prefix = simplified;
    }

    int maxId = 0;
    QRegularExpression prefixNumRegex(QStringLiteral("^(.+)[_\\-](\\d+)$"));
    for (FCAbstractDataObject* a : mDataList) {
        FCAbstractNamedDataObject* named = dynamic_cast<FCAbstractNamedDataObject*>(a);
        if (!named) continue;
        QString objName = named->getDataObjectName();
        QRegularExpressionMatch om = prefixNumRegex.match(objName);
        if (!om.hasMatch()) continue;
        QString objPrefix = om.captured(1);
        if (objPrefix.compare(prefix, Qt::CaseInsensitive) != 0) continue;
        int num = om.captured(2).toInt();
        if (num > maxId) maxId = num;
    }
    int nextNum = (maxId >= suggestedNum) ? (maxId + 1) : suggestedNum;
    QString candidate = prefix + QLatin1Char('_') + QString::number(nextNum);
    if (!getDataByName(candidate, false)) return candidate;
    while (getDataByName(candidate, false)) candidate = prefix + QLatin1Char('_') + QString::number(++nextNum);
    return candidate;
}

int FCAbstractDataManagerHelper::getIndexByID(FCID dataId) const
{
    for (int i = 0; i < mDataList.size(); ++i)
        if (mDataList.at(i) && mDataList.at(i)->getDataObjectID() == dataId) return i;
    return -1;
}

FCID FCAbstractDataManagerHelper::getIDByIndex(int dataIndex) const
{
    if (dataIndex < 0 || dataIndex >= mDataList.size()) return FCID_INVALID;
    FCAbstractDataObject* obj = mDataList.at(dataIndex);
    return obj ? obj->getDataObjectID() : FCID_INVALID;
}

void FCAbstractDataManagerHelper::sortObject(ObjectSortType type)
{
    if (type == ObjectSortType::DataObjID)
        std::sort(mDataList.begin(), mDataList.end(), [](FCAbstractDataObject* a, FCAbstractDataObject* b) { return a && b && a->getDataObjectID() < b->getDataObjectID(); });
    else
        std::sort(mDataList.begin(), mDataList.end(), [](FCAbstractDataObject* a, FCAbstractDataObject* b) {
            auto* na = dynamic_cast<FCAbstractNamedDataObject*>(a);
            auto* nb = dynamic_cast<FCAbstractNamedDataObject*>(b);
            if (!na || !nb) return false;
            return na->getDataObjectName() < nb->getDataObjectName();
        });
}

void FCAbstractDataManagerHelper::sortObject(std::function<bool(FCAbstractDataObject*, FCAbstractDataObject*)> function)
{
    std::sort(mDataList.begin(), mDataList.end(), function);
}

bool FCAbstractDataManagerHelper::isUsedDataObject(const QList<FCID>& data) const
{
    for (auto* obj : mDataList) {
        if (!obj) continue;
        for (FCID id : data) if (obj->getDataObjectID() == id) return true;
    }
    return false;
}

void FCAbstractDataManagerHelper::dataObjectDestoried(FCAbstractDataObject* obj)
{
    mDataList.removeOne(obj);
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
