/**
 * @file FCElementWedge.h
 * @brief 三棱柱单元（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTWEDGE_H
#define FCELEMENTWEDGE_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementWedge6 : public FCAbstractElement
{
public:
    explicit FCElementWedge6();
    ~FCElementWedge6() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

} // namespace FC

#endif // FCELEMENTWEDGE_H
