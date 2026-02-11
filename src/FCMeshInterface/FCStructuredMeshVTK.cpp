/**
 * @file FCStructuredMeshVTK.cpp
 * @brief 结构化网格 VTK 实现（移植自 FITKInterfaceMesh FITKStructuredMeshVTK）
 */
#include "FCStructuredMeshVTK.h"
#include <FCData/FCPoint.h>
#include <QtCore/QtGlobal>

#include <vtkStructuredGrid.h>
#include <vtkStructuredGridWriter.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>

namespace FC {

FCStructuredMeshVTK::FCStructuredMeshVTK()
{
    _vtkDataSet = vtkStructuredGrid::New();
}

FCStructuredMeshVTK::~FCStructuredMeshVTK()
{
    if (_vtkDataSet) {
        _vtkDataSet->Delete();
        _vtkDataSet = nullptr;
    }
}

FCModelEnum::AbsModelType FCStructuredMeshVTK::getAbsModelType()
{
    return FCModelEnum::AMTStructuredMeshvtk;
}

void FCStructuredMeshVTK::setIJKDim(int i, int j, int k)
{
    FCStructuredMesh::setIJKDim(i, j, k);
    _vtkDataSet->SetDimensions(i, j, k);
    _modified = true;
}

void FCStructuredMeshVTK::update()
{
    if (!_modified) return;
    reConstructure();
    _vtkDataSet->Modified();
    _modified = false;
}

vtkStructuredGrid* FCStructuredMeshVTK::getGrid()
{
    return _vtkDataSet;
}

bool FCStructuredMeshVTK::getBounds(double bds6[6])
{
    if (!_vtkDataSet) return false;
    if (_vtkDataSet->GetNumberOfPoints() == 0) return false;
    const double* bds = _vtkDataSet->GetBounds();
    for (int i = 0; i < 6; ++i) bds6[i] = bds[i];
    return true;
}

int FCStructuredMeshVTK::getNumberOfCells()
{
    if (!_vtkDataSet) return 0;
    return _vtkDataSet->GetNumberOfCells();
}

bool FCStructuredMeshVTK::writeToFile(const QString& file)
{
    vtkSmartPointer<vtkStructuredGridWriter> writer = vtkSmartPointer<vtkStructuredGridWriter>::New();
    writer->SetFileName(file.toUtf8().constData());
    writer->SetFileTypeToASCII();
    writer->SetInputData(_vtkDataSet);
    return writer->Write() != 0;
}

void FCStructuredMeshVTK::reConstructure()
{
    int dims[3];
    getIJKDim(dims);
    const int ni = dims[0];
    const int nj = dims[1];
    const int nk = dims[2];
    const int nPoints = ni * nj * nk;

    if (getNodeCount() < nPoints) return;

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    double pos[3] = {0, 0, 0};

    for (int k = 0; k < nk; ++k) {
        for (int j = 0; j < nj; ++j) {
            for (int i = 0; i < ni; ++i) {
                const int index = i + j * ni + k * ni * nj;
                FCNode* node = getNodeAt(index);
                if (node) {
                    node->getCoor(pos);
                } else {
                    pos[0] = pos[1] = pos[2] = 0.0;
                }
                points->InsertNextPoint(pos);
            }
        }
    }

    _vtkDataSet->SetDimensions(ni, nj, nk);
    _vtkDataSet->SetPoints(points);
}

void FCStructuredMeshVTK::clearMesh()
{
    removeAllNode();
    int dims[3];
    getIJKDim(dims);
    _vtkDataSet->SetDimensions(qMax(1, dims[0]), qMax(1, dims[1]), qMax(1, dims[2]));
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    _vtkDataSet->SetPoints(points);
}

} // namespace FC
