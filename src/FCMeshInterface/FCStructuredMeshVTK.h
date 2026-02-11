/**
 * @file FCStructuredMeshVTK.h
 * @brief 结构化网格 VTK 实现（移植自 FITKInterfaceMesh FITKStructuredMeshVTK）
 */
#ifndef FCSTRUCTUREDMESHVTK_H
#define FCSTRUCTUREDMESHVTK_H

#include "FCMeshInterfaceAPI.h"
#include "FCStructuredMesh.h"

class vtkStructuredGrid;

namespace FC {

/**
 * @brief 结构化网格 VTK 实现
 */
class FCMESHINTERFACE_API FCStructuredMeshVTK : public FCStructuredMesh
{
public:
    explicit FCStructuredMeshVTK();
    virtual ~FCStructuredMeshVTK();

    virtual FCModelEnum::AbsModelType getAbsModelType() override;
    virtual void update() override;

    void setIJKDim(int i, int j, int k);

    vtkStructuredGrid* getGrid();
    bool getBounds(double bds6[6]);
    int getNumberOfCells();
    virtual bool writeToFile(const QString& file);
    void reConstructure();
    void clearMesh();

private:
    vtkStructuredGrid* _vtkDataSet{};
    bool _modified{false};
};

} // namespace FC

#endif // FCSTRUCTUREDMESHVTK_H
