/**
 * @file FCAbstractElement.h
 * @brief 单元抽象类（移植自 FITKInterfaceModel）
 */
#ifndef FCABSTRACTELEMENT_H
#define FCABSTRACTELEMENT_H

#include "FCModelInterfaceAPI.h"
#include "FCModelEnum.h"
#include <FCData/FCAbstractObject.hpp>
#include <QObject>
#include <QList>

namespace FC {

class FCMODELINTERFACE_API FCAbstractElement : public QObject, public FCAbstractObject
{
    Q_OBJECT
public:
    explicit FCAbstractElement() = default;
    virtual ~FCAbstractElement();

    virtual FCModelEnum::FITKEleType getEleType() = 0;
    virtual int getNodeCount() = 0;
    void setEleID(int id);
    int getEleID() const;
    int getNodeID(int index) const;
    int getNodeIndex(int id) const;
    void setNodeID(int index, int nodeID);
    void setNodeID(const QList<int>& ids);
    virtual int getFaceCount() = 0;
    virtual int getElementDim() = 0;
    virtual int getEdgeCount() = 0;
    virtual QList<int> getFace(int index, bool lowerOrder = false) = 0;
    virtual QList<int> getEdge(int index) = 0;

    void reverse();
    int getFaceIndex(const QList<int>& face, bool normalSens = false);
    int getEdgeIndex(const QList<int>& edge);
    int getPreviousNodeID(int id);
    int getNextNodeID(int id);
    QList<int> getAllNodes();
    void replaceNodeID(const int& oldID, const int& newID);
    void setTag(unsigned int tag);
    unsigned int getTag() const;
    void setNativeFlag(bool flag);
    bool getNativeFlag();
    int getFaceIndexNorSen(const QList<int>& face, bool lowerOrder = false);
    int getFaceIndexNorUnSen(const QList<int>& face, bool lowerOrder = false);

signals:
    void elementDestroiedSignal(FCAbstractElement* element);

protected:
    virtual void setNodeCount(int c);

    int _eleID{ -1 };
    QList<int> _elemenTopo;
    unsigned int _tag{ 0 };
    bool _nativeFlag{ false };
};

} // namespace FC

#endif // FCABSTRACTELEMENT_H
