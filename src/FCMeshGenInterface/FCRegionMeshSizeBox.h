/**
 * @file FCRegionMeshSizeBox.h
 * @brief 长方体区域网格尺寸（移植自 FITKInterfaceMeshGen，用于 Gmsh 等）
 */
#ifndef FCREGIONMESHSIZEBOX_H
#define FCREGIONMESHSIZEBOX_H

#include "FCAbstractRegionMeshSize.h"

namespace FC {

/**
 * @brief 长方体区域网格尺寸
 */
class FCMESHGENINTERFACE_API FCRegionMeshSizeBox : public FCAbstractRegionMeshSize
{
    Q_OBJECT
public:
    explicit FCRegionMeshSizeBox(QObject* parent = nullptr);
    virtual ~FCRegionMeshSizeBox() override = default;

    virtual RegionType getRegionType() override { return RegionBox; }

    void setPoint1(const double* p1);
    void setLength(const double* length);
    void getPoint1(double* p1) const;
    void getLength(double* length) const;

protected:
    double _point1[3]{0, 0, 0};
    double _lengthParam[3]{10, 10, 10};
};

} // namespace FC

#endif // FCREGIONMESHSIZEBOX_H
