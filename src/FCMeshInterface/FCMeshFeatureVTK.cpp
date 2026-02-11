/**
 * @file FCMeshFeatureVTK.cpp
 * @brief 网格特征 VTK 管线实现（移植自 FITKInterfaceMesh FITKMeshFeatureVTK）
 */
#include "FCMeshFeatureVTK.h"
#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkAlgorithmOutput.h>
#include <vtkIdFilter.h>
#include <vtkPassThrough.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataNormals.h>

namespace FC {

const char* FCMeshFeatureVTK::GetPointIdArrayName()
{
    return "PointIds";
}

const char* FCMeshFeatureVTK::GetCellIdArrayName()
{
    return "CellIds";
}

const char* FCMeshFeatureVTK::GetFeatEdgeOriginalIdArrayName()
{
    return "OriIds";
}

FCMeshFeatureVTK::FCMeshFeatureVTK(vtkDataSet* dataSet)
{
    initialize();
    _dataSet = dataSet;
    _passThrough->SetInputData(dataSet);
}

FCMeshFeatureVTK::FCMeshFeatureVTK(vtkAlgorithmOutput* connection)
{
    initialize();
    _connection = connection;
    _passThrough->SetInputConnection(connection);
}

FCMeshFeatureVTK::~FCMeshFeatureVTK()
{
    if (_normalFilter) { _normalFilter->Delete(); _normalFilter = nullptr; }
    if (_featureEdgeFilter) { _featureEdgeFilter->Delete(); _featureEdgeFilter = nullptr; }
    if (_edgeFilter) { _edgeFilter->Delete(); _edgeFilter = nullptr; }
    if (_surfaceFilter) { _surfaceFilter->Delete(); _surfaceFilter = nullptr; }
    if (_idFilter) { _idFilter->Delete(); _idFilter = nullptr; }
    if (_passThrough) { _passThrough->Delete(); _passThrough = nullptr; }
}

void FCMeshFeatureVTK::initialize()
{
    _passThrough = vtkPassThrough::New();

    _idFilter = vtkIdFilter::New();
    _idFilter->SetInputConnection(_passThrough->GetOutputPort());
    _idFilter->SetCellIds(true);
    _idFilter->SetPointIds(true);
    _idFilter->SetPointIdsArrayName(GetPointIdArrayName());
    _idFilter->SetCellIdsArrayName(GetCellIdArrayName());
    _idFilter->SetFieldData(true);

    _surfaceFilter = vtkDataSetSurfaceFilter::New();
    _surfaceFilter->SetInputConnection(_idFilter->GetOutputPort());
    _surfaceFilter->SetNonlinearSubdivisionLevel(0);

    _normalFilter = vtkPolyDataNormals::New();
    _normalFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
    _normalFilter->SetComputeCellNormals(true);
    _normalFilter->SetComputePointNormals(false);

    _featureEdgeFilter = vtkFeatureEdges::New();
    _featureEdgeFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
    _featureEdgeFilter->BoundaryEdgesOn();
    _featureEdgeFilter->FeatureEdgesOn();

    _edgeFilter = vtkFeatureEdges::New();
    _edgeFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
    _edgeFilter->BoundaryEdgesOff();
    _edgeFilter->FeatureEdgesOff();
    _edgeFilter->NonManifoldEdgesOn();
    _edgeFilter->ManifoldEdgesOn();
}

void FCMeshFeatureVTK::update()
{
    if (_dataSet)
        _passThrough->SetInputData(_dataSet);
    if (_connection)
        _passThrough->SetInputConnection(_connection);

    _passThrough->Update();
    if (_idFilter) _idFilter->Update();
    if (_surfaceFilter) _surfaceFilter->Update();
    if (_normalFilter) _normalFilter->Update();
    if (_featureEdgeFilter) _featureEdgeFilter->Update();

    if (_dataSet) _dataSet->Modified();
}

void FCMeshFeatureVTK::reset(vtkDataSet* dataSet)
{
    _dataSet = dataSet;
    _connection = nullptr;
    if (_passThrough)
        _passThrough->SetInputData(_dataSet);
    update();
}

void FCMeshFeatureVTK::reset(vtkAlgorithmOutput* connection)
{
    _dataSet = nullptr;
    _connection = connection;
    if (_passThrough)
        _passThrough->SetInputConnection(_connection);
    update();
}

void FCMeshFeatureVTK::setCellAndPointIdsEnable(bool pointIdsOn, bool cellIdsOn)
{
    if (_idFilter) {
        _idFilter->SetPointIds(pointIdsOn ? 1 : 0);
        _idFilter->SetCellIds(cellIdsOn ? 1 : 0);
    }
}

void FCMeshFeatureVTK::setSurfEdgesEnable(bool isOn)
{
    if (!_edgeFilter) return;
    if (isOn && _surfaceFilter)
        _edgeFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
    else
        _edgeFilter->RemoveAllInputConnections(0);
}

vtkPassThrough* FCMeshFeatureVTK::getMeshPassThrough()
{
    return _passThrough;
}

vtkIdFilter* FCMeshFeatureVTK::getIdFilter()
{
    return _idFilter;
}

vtkDataSetSurfaceFilter* FCMeshFeatureVTK::getSurfaceFilter()
{
    return _surfaceFilter;
}

vtkFeatureEdges* FCMeshFeatureVTK::getFeatureEdgeFilter()
{
    return _featureEdgeFilter;
}

vtkFeatureEdges* FCMeshFeatureVTK::getShellEdgeFilter()
{
    return _edgeFilter;
}

vtkPolyDataNormals* FCMeshFeatureVTK::getNormalsFilter()
{
    return _normalFilter;
}

vtkPolyData* FCMeshFeatureVTK::getFeatureMesh(int type)
{
    switch (type) {
    case 1:
        if (_surfaceFilter) { _surfaceFilter->Update(); return _surfaceFilter->GetOutput(); }
        break;
    case 2:
        if (_normalFilter) { _normalFilter->Update(); return _normalFilter->GetOutput(); }
        break;
    case 3:
        if (_featureEdgeFilter) { _featureEdgeFilter->Update(); return _featureEdgeFilter->GetOutput(); }
        break;
    case 4:
        if (_edgeFilter) { _edgeFilter->Update(); return _edgeFilter->GetOutput(); }
        break;
    default:
        break;
    }
    return nullptr;
}

vtkDataArray* FCMeshFeatureVTK::getCellNormals()
{
    vtkPolyData* normalMesh = getFeatureMesh(2);
    if (!normalMesh) return nullptr;
    vtkCellData* cellData = normalMesh->GetCellData();
    if (!cellData) return nullptr;
    return cellData->GetNormals();
}

} // namespace FC
