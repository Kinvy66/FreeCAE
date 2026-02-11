/**
 * @file FCGlobalMeshGenerateAlgorithmInfo.h
 * @brief 全局网格生成算法信息（移植自 FITKInterfaceMeshGen，适配 Gmsh）
 */
#ifndef FCGLOBALMESHGENERATEALGORITHMINFO_H
#define FCGLOBALMESHGENERATEALGORITHMINFO_H

#include "FCMeshGenInterfaceAPI.h"
#include <QObject>
#include <FCData/FCVariantParams.h>

namespace FC {

/**
 * @brief 全局网格生成算法信息（Gmsh 2D/3D 算法等）
 */
class FCMESHGENINTERFACE_API FCGlobalMeshGenerateAlgorithmInfo : public QObject, public FCVariantParams
{
    Q_OBJECT
public:
    enum Algorithm2D {
        Alg2DAutomatic = 0,
        Alg2DMeshAdapt = 1,
        Alg2DDelaunay = 2,
        Alg2DFrontalDelaunay = 3,
        Alg2DBAMG = 4,
    };
    Q_ENUM(Algorithm2D);

    enum Algorithm3D {
        Alg3DDelaunay = 0,
        Alg3DFrontal = 1,
        Alg3DHXT = 2,
        Alg3DMMG3D = 3,
    };
    Q_ENUM(Algorithm3D);

    explicit FCGlobalMeshGenerateAlgorithmInfo(QObject* parent = nullptr);
    virtual ~FCGlobalMeshGenerateAlgorithmInfo() override = default;

    void setMeshGenerateDimension(int dim);
    int getMeshGenerateDimension() const { return _meshDim; }
    void set2DAlgorithm(Algorithm2D a);
    Algorithm2D get2DAlgorithm() const { return _alg2D; }
    void set3DAlgorithm(Algorithm3D a);
    Algorithm3D get3DAlgorithm() const { return _alg3D; }
    void setElementOrder(int order);
    int getElementOrder() const { return _elementOrder; }

protected:
    int _meshDim{3};
    Algorithm2D _alg2D{Alg2DFrontalDelaunay};
    Algorithm3D _alg3D{Alg3DFrontal};
    int _elementOrder{1};
};

} // namespace FC

#endif // FCGLOBALMESHGENERATEALGORITHMINFO_H
