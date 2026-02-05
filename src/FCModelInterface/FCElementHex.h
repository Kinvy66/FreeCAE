/**
 * @file FCElementHex.h
 * @brief 六面体单元（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTHEX_H
#define FCELEMENTHEX_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementHex8 : public FCAbstractElement
{
public:
    explicit FCElementHex8();
    ~FCElementHex8() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

class FCMODELINTERFACE_API FCElementHex20 : public FCAbstractElement
{
public:
    explicit FCElementHex20();
    ~FCElementHex20() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

} // namespace FC

#endif // FCELEMENTHEX_H
