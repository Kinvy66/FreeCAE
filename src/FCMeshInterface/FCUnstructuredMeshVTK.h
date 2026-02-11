/**
 * @file FCUnstructuredMeshVTK.h
 * @brief 非结构网格 VTK 实现（移植自 FITKInterfaceMesh FITKUnstructuredMeshVTK）
 */
#ifndef FCUNSTRUCTUREDMESHVTK_H
#define FCUNSTRUCTUREDMESHVTK_H

#include "FCMeshInterfaceAPI.h"
#include "FCUnstructuredMesh.h"
#include "FCMeshVTKMap.h"

class vtkUnstructuredGrid;
class vtkIntArray;

namespace FC {

class FCMeshFeatureVTK;
class FCGeoToMeshMapper;

/**
 * @brief 非结构网格 VTK 实现
 */
class FCMESHINTERFACE_API FCUnstructuredMeshVTK : public FCUnstructuredMesh
{
public:
    explicit FCUnstructuredMeshVTK();
    virtual ~FCUnstructuredMeshVTK();

    static QString GetOrphanMeshTagName();

    virtual FCModelEnum::AbsModelType getAbsModelType() override;
    virtual void update() override;

    const QList<int> getCellIndiceByDim(int dim);

    virtual int addNode(FCNode* node) override;
    virtual int addNode(double x, double y, double z, bool isNative = false, unsigned int tag = 0) override;
    virtual void addNode(int id, double x, double y, double z, bool isNative = false, unsigned int tag = 0) override;

    virtual void appendElement(FCAbstractElement* element) override;
    virtual void removeAllElement() override;
    virtual int removeElementAt(int index) override;
    virtual int removeElementByID(int id) override;

    virtual bool writeToFile(const QString& file);

    virtual void reverseEleByID(int id) override;
    virtual void reverseEleByIndex(int index) override;

    FCMeshFeatureVTK* getMeshFeature();
    vtkUnstructuredGrid* getGrid();
    FCGeoToMeshMapper* getGeoToMeshMapper();

    bool getBounds(double bds6[6]);
    int getNumberOfCells();
    void reConstructure();
    void transformVTKMesh(vtkUnstructuredGrid* grid);

    void clearMesh();
    virtual void clearNativeMesh() override;
    virtual void clearOrphanMesh() override;
    virtual void replaceNodesID(const QHash<int, int>& nodes) override;

private:
    vtkUnstructuredGrid* _vtkDataSet{};
    vtkIntArray* _orphanMeshTagArrayPt{};
    vtkIntArray* _orphanMeshTagArrayCell{};
    FCMeshFeatureVTK* _feature{};
    FCGeoToMeshMapper* _geoToMeshMapper{};
    bool _modified{false};
};

} // namespace FC

#endif // FCUNSTRUCTUREDMESHVTK_H
