/**
 * @file FCElementTet.h
 * @brief 四面体单元（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTTET_H
#define FCELEMENTTET_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementTet4 : public FCAbstractElement
{
public:
    explicit FCElementTet4();
    ~FCElementTet4() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

class FCMODELINTERFACE_API FCElementTet10 : public FCAbstractElement
{
public:
    explicit FCElementTet10();
    ~FCElementTet10() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

} // namespace FC

#endif // FCELEMENTTET_H
