/**
 * @file FCGlobalData.cpp
 * @brief 全局运行数据实现（移植自 APPFlow FITKGlobalData）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCGlobalData.h"
#include <QStringList>

namespace FC {

QMutex FCGlobalData::_mutex;

FCGlobalData::FCGlobalData()
{
    
}

FCGlobalData::~FCGlobalData()
{
    
}

QList<int> FCGlobalData::getGlobalDataIDs()
{
    //本身的全局数据
    const QList<FCAbstractDataObject*> objs = _gData.values();
    QList<int> ids;
    for (FCAbstractDataObject* obj : objs)
    {
        if(obj == nullptr) continue;
        ids.append(obj->getDataObjectID());
    }
    //framework中的全局数据
    // FITKProgramTaskManeger* ta = FITKAPP->getProgramTaskManager();
    // if (ta)
    // {
    //     ids.append(ta->getDataObjectID());
    // }
    return ids;
}

void FCGlobalData::insertData(int dt, FCAbstractDataObject *d)
{
    //空指针，不操作
    if (d == nullptr) return;
    //数据存在，先删除原来数据
    if (_gData.contains(dt))
    {
        auto d = _gData.value(dt);
        _gData.remove(dt);
        if (d) delete d;
    }
    //插入数据管理
    _gData.insert(dt, d);
}


} // namespace FC
