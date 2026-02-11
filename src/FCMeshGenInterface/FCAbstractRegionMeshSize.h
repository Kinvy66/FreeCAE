/**
 * @file FCAbstractRegionMeshSize.h
 * @brief 局部区域网格尺寸抽象类（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCABSTRACTREGIONMESHSIZE_H
#define FCABSTRACTREGIONMESHSIZE_H

#include "FCMeshGenInterfaceAPI.h"
#include "FCGlobalMeshSizeInfo.h"
#include <QObject>
#include <QHash>

namespace FC {

/**
 * @brief 局部区域网格尺寸抽象类
 */
class FCMESHGENINTERFACE_API FCAbstractRegionMeshSize : public FCGlobalMeshSizeInfo
{
    Q_OBJECT
public:
    enum RegionType {
        RegionNone,
        RegionFromFile,
        RegionGeom,
        RegionBox,
        RegionCylinder,
        RegionSphere,
        RegionUserDef1,
        RegionUserDef2,
        RegionUserDef3,
        RegionUserDef4,
        RegionUserDef5,
    };
    Q_ENUM(RegionType);

    enum BoundaryType {
        BTEmpty,
        BTPatch,
        BTWall,
        BTSymmetry,
        BTUDF1, BTUDF2, BTUDF3, BTUDF4, BTUDF5,
    };
    Q_ENUM(BoundaryType);

    explicit FCAbstractRegionMeshSize(QObject* parent = nullptr);
    virtual ~FCAbstractRegionMeshSize();

    virtual RegionType getRegionType() = 0;

    void setDivision(int dim, int divNum);
    void setGrading(int dim, double grad);
    int getDivision(int dim) const;
    double getGrading(int dim) const;

    void insertBoundary(int faceID, BoundaryType bt);
    BoundaryType getBoundary(int faceID) const;

    void setInternalSize(double v);
    double getInternalSize() const;
    void setExternalSize(double v);
    double getExternalSize() const;
    void setThickness(double v);
    double getThickness() const;

protected:
    int _division[3]{10, 10, 10};
    double _grading[3]{1, 1, 1};
    QHash<int, BoundaryType> _boundarys;
    double _internalSize{0};
    double _externalSize{0};
    double _thickness{0};
};

} // namespace FC

#endif // FCABSTRACTREGIONMESHSIZE_H
