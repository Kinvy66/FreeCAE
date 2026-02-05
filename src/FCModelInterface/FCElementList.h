/**
 * @file FCElementList.h
 * @brief 单元列表（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTLIST_H
#define FCELEMENTLIST_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"
#include <QVector>
#include <QMap>
#include <QMutex>

namespace FC {

class FCMODELINTERFACE_API FCElementList
{
public:
    explicit FCElementList() = default;
    virtual ~FCElementList();

    virtual void appendElement(FCAbstractElement* element);
    int getEleIDByIndex(int index);
    int getEleIndexByID(int id) const;
    FCAbstractElement* getElementAt(int index);
    FCAbstractElement* getElementByID(int id);
    int getElementCount();
    virtual void reverseEleByID(int id);
    virtual void reverseEleByIndex(int index);

    template<class T>
    T* getTElementAt(int index) { return dynamic_cast<T*>(getElementAt(index)); }

    template<class T>
    T* getTElementByID(int id) { return dynamic_cast<T*>(getElementByID(id)); }

    int getElementMaxID();
    virtual void removeAllElement();
    virtual int removeElementAt(int index);
    virtual int removeElementByID(int id);
    QList<FCAbstractElement*> getNeighborElement(int nodeID);
    virtual QList<int> getAllNodeIDs() const;
    virtual void replaceNodesID(const QHash<int, int>& nodes);

protected:
    void updateElementIDIndexMap();
    void fastRemoveElementAt(int index);
    void fastRemoveElementByID(int id);

    QMutex _mutex;
    QVector<FCAbstractElement*> _elementList;
    QMap<int, int> _idIndexMap;
    int _eleMaxID = 0;
};

} // namespace FC

#endif // FCELEMENTLIST_H
