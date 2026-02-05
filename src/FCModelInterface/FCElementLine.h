/**
 * @file FCElementLine.h
 * @brief 线单元（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTLINE_H
#define FCELEMENTLINE_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementLine2 : public FCAbstractElement
{
public:
    explicit FCElementLine2();
    ~FCElementLine2() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

class FCMODELINTERFACE_API FCElementLine3 : public FCAbstractElement
{
public:
    explicit FCElementLine3();
    ~FCElementLine3() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

} // namespace FC

#endif // FCELEMENTLINE_H
