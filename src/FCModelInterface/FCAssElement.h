/**
 * @file FCAssElement.h
 * @brief 装配单元、连接器、弹簧等（移植自 FITKInterfaceModel）
 */
#ifndef FCASSELEMENT_H
#define FCASSELEMENT_H

#include "FCModelInterfaceAPI.h"
#include "FCModelSet.h"
#include "FCModelEnum.h"
#include <QPair>

namespace FC {

class FCAbstractModel;

class FCMODELINTERFACE_API FCAbstractAssemblyElement
{
public:
    explicit FCAbstractAssemblyElement();
    virtual ~FCAbstractAssemblyElement() = default;

    int getElementID() const;
    void setElementID(int id);
    virtual void copyTo(FCAbstractAssemblyElement* e) = 0;

private:
    int _elementID{ -1 };
};

class FCMODELINTERFACE_API FCAssemblyElementVertex : public FCAbstractAssemblyElement
{
public:
    using FCAbstractAssemblyElement::FCAbstractAssemblyElement;
    ~FCAssemblyElementVertex() override = default;

    void copyTo(FCAbstractAssemblyElement* e) override;

    void setModelID(int mid);
    void setNodeID(int id);
    void getCoor(double* coor);
    void getNode(int& mid, int& id);

private:
    int _nodeModelID{ -1 };
    int _nodeID{ -1 };
};

class FCMODELINTERFACE_API FCAssemblyElementLine : public FCAbstractAssemblyElement
{
public:
    using FCAbstractAssemblyElement::FCAbstractAssemblyElement;
    ~FCAssemblyElementLine() override = default;

    void copyTo(FCAbstractAssemblyElement* e) override;

    void setNode1(int mid, int id);
    void setNode2(int mid, int id);
    void getNode1(int& mid, int& id);
    void getNode2(int& mid, int& id);
    void getCoor1(double* coor1);
    void getCoor2(double* coor2);
    QString getTextLabel1();
    QString getTextLabel2();
    void swap();

private:
    QPair<int, int> _node1;
    QPair<int, int> _node2;
};

class FCMODELINTERFACE_API FCAssemblyElementSet : public FCModelSet
{
public:
    explicit FCAssemblyElementSet() = default;
    ~FCAssemblyElementSet() override = default;

    FCModelEnum::FITKModelSetType getModelSetType() override;
};

} // namespace FC

#endif // FCASSELEMENT_H
