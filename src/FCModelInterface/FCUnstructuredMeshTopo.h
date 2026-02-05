/**
 * @file FCUnstructuredMeshTopo.h
 * @brief 非结构网格拓扑（移植自 FITKInterfaceModel）
 */
#ifndef FCUNSTRUCTUREDMESHTOPO_H
#define FCUNSTRUCTUREDMESHTOPO_H

#include "FCModelInterfaceAPI.h"
#include <QObject>
#include <QHash>
#include <QList>

namespace FC {

class FCNode;
class FCAbstractElement;
class FCUnstructuredMesh;

class FCMODELINTERFACE_API FCNodeElements : public QObject
{
    Q_OBJECT
public:
    explicit FCNodeElements(FCNode* node);
    ~FCNodeElements() override = default;
    void appendElement(FCAbstractElement* element);
    FCNode* getNode() const;
    QList<FCAbstractElement*> getElements() const;
    QList<int> getElementIDs() const;

private slots:
    void onElementDestroiedSlot(FCAbstractElement* ele);

private:
    FCNode* _node{ nullptr };
    QList<FCAbstractElement*> _elements;
};

class FCMODELINTERFACE_API FCUnstructuredMeshTopo : public QObject
{
    Q_OBJECT
    friend class FCNodeElements;
public:
    explicit FCUnstructuredMeshTopo(FCUnstructuredMesh* mesh);
    ~FCUnstructuredMeshTopo() override;

    bool buildUnstructuredMeshTopo();
    bool addElementTopo(FCAbstractElement* element);
    QList<FCAbstractElement*> getNodeElements(int nodeID) const;

private slots:
    void onNodeDestroiedSlot(FCNode* node);

protected:
    FCUnstructuredMesh* _mesh{ nullptr };
    QHash<int, FCNodeElements*> _nodeElementHash;
};

} // namespace FC

#endif // FCUNSTRUCTUREDMESHTOPO_H
