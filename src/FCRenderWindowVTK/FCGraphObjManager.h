/**
 * @file FCGraphObjManager.h
 * @brief 渲染对象管理器
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
    QList<FCGraphObjectVTK*> mList;
};
} // namespace FC
#endif
