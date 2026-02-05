/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraphObjManager.h
 * @brief 渲染对象管理器（仅 Qt，与 FITKGraphObjManager 接口一致）
 */
#ifndef FCGRAPHOBJMANAGER_H
#define FCGRAPHOBJMANAGER_H

#include "FCRenderWindowVTKAPI.h"
#include <QList>

namespace FC
{
class FCGraphObjectVTK;

class FCRENDERWINDOWVTK_API FCGraphObjManager
{
public:
    FCGraphObjManager();
    ~FCGraphObjManager();

    void appendGraphObj(FCGraphObjectVTK* obj);
    int getGraphObjCount() const;
    FCGraphObjectVTK* getGraphObjAt(int index) const;
    template<typename T>
    T* getGraphObjTAt(int index) const
    {
        return dynamic_cast<T*>(getGraphObjAt(index));
    }
    void removeGraphObj(FCGraphObjectVTK* obj);
    void clear();
    bool isContains(FCGraphObjectVTK* obj) const;

private:
    QList<FCGraphObjectVTK*> m_list;
};
} // namespace FC
#endif
