/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraphObjManager.h"
#include "FCGraphObjectVTK.h"

namespace FC
{
FCGraphObjManager::FCGraphObjManager() = default;

FCGraphObjManager::~FCGraphObjManager() = default;

void FCGraphObjManager::appendGraphObj(FCGraphObjectVTK* obj)
{
    if (obj && !mList.contains(obj))
        mList.append(obj);
}

int FCGraphObjManager::getGraphObjCount() const
{
    return mList.size();
}

FCGraphObjectVTK* FCGraphObjManager::getGraphObjAt(int index) const
{
    if (index < 0 || index >= mList.size())
        return nullptr;
    return mList.at(index);
}

void FCGraphObjManager::removeGraphObj(FCGraphObjectVTK* obj)
{
    mList.removeOne(obj);
}

void FCGraphObjManager::clear()
{
    mList.clear();
}

bool FCGraphObjManager::isContains(FCGraphObjectVTK* obj) const
{
    return obj && mList.contains(obj);
}
} // namespace FC
