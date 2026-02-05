/**
 * @file FCElementPolygon.h
 * @brief 多边形单元（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTPOLYGON_H
#define FCELEMENTPOLYGON_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementPolygon : public FCAbstractElement
{
public:
    explicit FCElementPolygon() = default;
    ~FCElementPolygon() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    void setNodeCount(int c) override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getElementDim() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
};

} // namespace FC

#endif // FCELEMENTPOLYGON_H
