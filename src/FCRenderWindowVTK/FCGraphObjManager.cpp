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
    if (obj && !m_list.contains(obj))
        m_list.append(obj);
}

int FCGraphObjManager::getGraphObjCount() const
{
    return m_list.size();
}

FCGraphObjectVTK* FCGraphObjManager::getGraphObjAt(int index) const
{
    if (index < 0 || index >= m_list.size())
        return nullptr;
    return m_list.at(index);
}

void FCGraphObjManager::removeGraphObj(FCGraphObjectVTK* obj)
{
    m_list.removeOne(obj);
}

void FCGraphObjManager::clear()
{
    m_list.clear();
}

bool FCGraphObjManager::isContains(FCGraphObjectVTK* obj) const
{
    return obj && m_list.contains(obj);
}
} // namespace FC
