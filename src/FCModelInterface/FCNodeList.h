/**
 * @file FCNodeList.h
 * @brief 节点列表（移植自 FITKInterfaceModel，依赖 FCData）
 */
#ifndef FCNODELIST_H
#define FCNODELIST_H

#include "FCModelInterfaceAPI.h"
#include <FCData/FCPoint.h>
#include <QVector>
#include <QMap>
#include <QMutex>

namespace FC {

class FCMODELINTERFACE_API FCNodeList
{
public:
    explicit FCNodeList() = default;
    virtual ~FCNodeList();

    virtual int addNode(FCNode* node);
    virtual int addNode(double x, double y, double z, bool isNative = false, unsigned int tag = 0);
    virtual void addNode(int id, double x, double y, double z, bool isNative = false, unsigned int tag = 0);
    virtual void removeNodeAt(int index);
    virtual void removeNodeByID(int id);
    virtual void removeNodeByIDs(const QList<int>& ids);
    int getNodeCount();
    FCNode* getNodeAt(int index);

    template<class T>
    T* getTNodeAt(int index) { return dynamic_cast<T*>(getNodeAt(index)); }

    FCNode* getNodeByID(int id);

    template<class T>
    T* getTNodeByID(int id) { return dynamic_cast<T*>(getNodeByID(id)); }

    int getNodeIDByIndex(int index);
    int getNodeIndexByID(int id);
    int getNodeMaxID();
    void removeAllNode();
    void getNodeBndBox(double* bndBox) const;

protected:
    void updateNodeIDIndexMap();
    void fastRemoveNodeAt(int index);
    void fastRemoveNodeByID(int id);

    QMutex _mutex;
    QVector<FCNode*> _nodeList;
    QMap<int, int> _idIndexMap;
    int _nodeMaxID = 0;
};

} // namespace FC

#endif // FCNODELIST_H
