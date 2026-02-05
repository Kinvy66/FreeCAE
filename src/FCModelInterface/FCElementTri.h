/**
 * @file FCElementTri.h
 * @brief 三角形单元（移植自 FITKInterfaceModel）
 */
#ifndef FCELEMENTTRI_H
#define FCELEMENTTRI_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractElement.h"

namespace FC {

class FCMODELINTERFACE_API FCElementTri3 : public FCAbstractElement
{
public:
    explicit FCElementTri3();
    ~FCElementTri3() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

class FCMODELINTERFACE_API FCElementTri6 : public FCAbstractElement
{
public:
    explicit FCElementTri6();
    ~FCElementTri6() override = default;
    FCModelEnum::FITKEleType getEleType() override;
    int getNodeCount() override;
    int getFaceCount() override;
    int getEdgeCount() override;
    QList<int> getFace(int index, bool lowerOrder = false) override;
    QList<int> getEdge(int index) override;
    int getElementDim() override;
};

} // namespace FC

#endif // FCELEMENTTRI_H
