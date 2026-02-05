/**
 * @file FCElementQuad.h
 * @brief 四边形单元（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTQUAD_H
#define FCELEMENTQUAD_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementQuad4 : public FCAbstractElement
{
public:
    explicit FCElementQuad4();
    ~FCElementQuad4() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

class FCMODELINTERFACE_API FCElementQuad8 : public FCAbstractElement
{
public:
    explicit FCElementQuad8();
    ~FCElementQuad8() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

} // namespace FC

#endif // FCELEMENTQUAD_H
