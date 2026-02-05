/**
 * @file FCModelSet.cpp
 * @brief 集合实现（移植自 FITKInterfaceModel）
 */
#include "FCModelSet.h"
#include <algorithm>

namespace FC {

FCModelSet::FCModelSet(FCModelEnum::FITKModelSetType type)
    : _setType(type)
{}

FCModelEnum::FITKModelSetType FCModelSet::getModelSetType()
{
    if (!isCombination())
        return _setType;
    const int n = getDataCount();
    if (n < 2) return FCModelEnum::FMSNone;
    FCModelSet* set = getDataByIndex(0);
    if (!set) return FCModelEnum::FMSNone;
    FCModelEnum::FITKModelSetType firstType = set->getModelSetType();
    for (int i = 0; i < n; ++i) {
        set = getDataByIndex(i);
        if (!set) continue;
        if (set->getModelSetType() != firstType) return FCModelEnum::FMSMIX;
    }
    return firstType;
}

bool FCModelSet::containsType(FCModelEnum::FITKModelSetType t)
{
    if (t == getModelSetType()) return true;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCModelSet* set = getDataByIndex(i);
        if (!set) continue;
        if (set->getModelSetType() == t) return true;
    }
    return false;
}

void FCModelSet::setModelSetType(FCModelEnum::FITKModelSetType t)
{
    _setType = t;
}

bool FCModelSet::isInternal()
{
    if (!isCombination()) return _internal;
    const int n = getDataCount();
    for (int i = 1; i < n; ++i) {
        FCModelSet* set = getDataByIndex(i);
        if (!set || !set->isInternal()) return false;
    }
    return true;
}

bool FCModelSet::isGenerated()
{
    return _generated;
}

void FCModelSet::setInternal(bool internal)
{
    _internal = internal;
}

void FCModelSet::setGenerated(bool generated)
{
    _generated = generated;
}

void FCModelSet::clearMember()
{
    _setMember.clear();
}

void FCModelSet::appendMember(const int& mem)
{
    _setMember.append(mem);
}

void FCModelSet::setAbsoluteMember(const QList<int>& mem)
{
    _setMember = mem;
    std::sort(_setMember.begin(), _setMember.end());
    const int nCount = _setMember.size();
    if (nCount < 3) return;
    const int start = _setMember.first();
    const int end = _setMember.last();
    const int intv = _setMember.at(1) - start;
    _generated = true;
    for (int i = 2; i < nCount; ++i) {
        if (intv != _setMember[i] - _setMember[i - 1]) {
            _generated = false;
            return;
        }
    }
    if (_generated) {
        _setMember.clear();
        _setMember.append(start);
        _setMember.append(end);
        _setMember.append(intv);
    }
}

const QList<int>& FCModelSet::getMember() const
{
    return _setMember;
}

int FCModelSet::getMemberCount()
{
    if (_generated) return getAbsoluteMember().size();
    return _setMember.size();
}

const QList<int> FCModelSet::getAbsoluteMember() const
{
    if (!_generated) return _setMember;
    if (_setMember.size() % 3 != 0) return QList<int>();
    const int groupCount = _setMember.size() / 3;
    QList<int> mem;
    for (int i = 0; i < groupCount; i++) {
        const int start = _setMember.at(i * 3);
        const int end = _setMember.at(i * 3 + 1);
        const int step = _setMember.at(i * 3 + 2);
        for (int num = start; num <= end; num += step)
            mem.append(num);
    }
    return mem;
}

bool FCModelSet::isCombination()
{
    return getDataCount() > 0;
}

void FCModelSet::appendDataObj(FCAbstractDataObject* item)
{
    FCAbstractDataManager<FCModelSet>::appendDataObj(item);
    _internal = true;
    for (int i = 0; i < getDataCount(); ++i) {
        FCModelSet* set = getDataByIndex(i);
        if (!set || set->isInternal()) continue;
        _internal = false;
        break;
    }
}

void FCModelSet::getAbsoluteSet(QList<FCModelSet*>& sets, FCModelEnum::FITKModelSetType t)
{
    if (!isCombination()) {
        if (_setType == t) sets.append(this);
    } else {
        const int nSet = getDataCount();
        for (int i = 0; i < nSet; ++i) {
            FCModelSet* asubSet = getDataByIndex(i);
            if (!asubSet) continue;
            asubSet->getAbsoluteSet(sets, t);
        }
    }
}

void FCModelSet::setDataObjectName(const QString& name)
{
    FCAbstractNamedDataObject::setDataObjectName(name);
    const int nSet = getDataCount();
    for (int i = 0; i < nSet; ++i) {
        FCModelSet* asubSet = getDataByIndex(i);
        if (!asubSet) continue;
        asubSet->setDataObjectName(name);
    }
}

int FCModelSet::getFirstMemberID()
{
    if (_setMember.isEmpty()) return -1;
    return _setMember.first();
}

} // namespace FC
