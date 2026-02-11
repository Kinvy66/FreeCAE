/**
 * @file FCUnstructuredMeshVTK.cpp
 * @brief 非结构网格 VTK 实现（移植自 FITKInterfaceMesh FITKUnstructuredMeshVTK）
 */
#include "FCUnstructuredMeshVTK.h"
#include "FCMeshFeatureVTK.h"
#include "FCGeoToMeshMapper.h"
#include "FCElementFactory.h"
#include <FCData/FCPoint.h>

#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkSmartPointer.h>
#include <vtkIdList.h>
#include <vtkCell.h>
#include <vtkCellType.h>
#include <vtkIntArray.h>

#include <QHash>

namespace FC {

QString FCUnstructuredMeshVTK::GetOrphanMeshTagName()
{
    return QStringLiteral("OrphanElementTag");
}

FCUnstructuredMeshVTK::FCUnstructuredMeshVTK()
{
    _vtkDataSet = vtkUnstructuredGrid::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    _vtkDataSet->SetPoints(points);

    _orphanMeshTagArrayPt = vtkIntArray::New();
    _orphanMeshTagArrayPt->SetName(GetOrphanMeshTagName().toUtf8());
    _orphanMeshTagArrayPt->SetNumberOfComponents(1);

    _orphanMeshTagArrayCell = vtkIntArray::New();
    _orphanMeshTagArrayCell->SetName(GetOrphanMeshTagName().toUtf8());
    _orphanMeshTagArrayCell->SetNumberOfComponents(1);

    _vtkDataSet->GetPointData()->AddArray(_orphanMeshTagArrayPt);
    _vtkDataSet->GetCellData()->AddArray(_orphanMeshTagArrayCell);

    _feature = new FCMeshFeatureVTK(_vtkDataSet);
    _geoToMeshMapper = new FCGeoToMeshMapper();
}

FCUnstructuredMeshVTK::~FCUnstructuredMeshVTK()
{
    if (_feature) { delete _feature; _feature = nullptr; }
    if (_vtkDataSet) { _vtkDataSet->Delete(); _vtkDataSet = nullptr; }
    if (_orphanMeshTagArrayPt) { _orphanMeshTagArrayPt->Delete(); _orphanMeshTagArrayPt = nullptr; }
    if (_orphanMeshTagArrayCell) { _orphanMeshTagArrayCell->Delete(); _orphanMeshTagArrayCell = nullptr; }
    if (_geoToMeshMapper) { delete _geoToMeshMapper; _geoToMeshMapper = nullptr; }
}

FCModelEnum::AbsModelType FCUnstructuredMeshVTK::getAbsModelType()
{
    return FCModelEnum::AMTunstructuredMeshvtk;
}

void FCUnstructuredMeshVTK::update()
{
    if (!_modified) return;
    _vtkDataSet->Modified();
    if (_feature) _feature->update();
    _modified = false;
}

const QList<int> FCUnstructuredMeshVTK::getCellIndiceByDim(int dim)
{
    QList<int> indice;
    if (dim < 0 || dim > 3) return indice;
    const int n = getElementCount();
    for (int i = 0; i < n; ++i) {
        FCAbstractElement* element = getElementAt(i);
        if (element && element->getElementDim() == dim)
            indice.push_back(i);
    }
    return indice;
}

int FCUnstructuredMeshVTK::addNode(FCNode* node)
{
    if (node)
        _orphanMeshTagArrayPt->InsertNextValue(node->getNativeFlag() ? 0 : 1);
    else
        _orphanMeshTagArrayPt->InsertNextValue(1);
    return FCNodeList::addNode(node);
}

int FCUnstructuredMeshVTK::addNode(double x, double y, double z, bool isNative, unsigned int tag)
{
    FCNode* node = new FCNode(0, x, y, z);
    node->setNativeFlag(isNative);
    node->setTag(tag);
    const int id = FCNodeList::addNode(node);
    _orphanMeshTagArrayPt->InsertNextValue(isNative ? 0 : 1);
    vtkPoints* points = _vtkDataSet->GetPoints();
    if (points) points->InsertNextPoint(x, y, z);
    _modified = true;
    return id;
}

void FCUnstructuredMeshVTK::addNode(int id, double x, double y, double z, bool isNative, unsigned int tag)
{
    FCNode* node = new FCNode(id, x, y, z);
    node->setNativeFlag(isNative);
    node->setTag(tag);
    FCNodeList::addNode(node);
    _orphanMeshTagArrayPt->InsertNextValue(isNative ? 0 : 1);
    vtkPoints* points = _vtkDataSet->GetPoints();
    if (points) points->InsertNextPoint(x, y, z);
    _modified = true;
}

void FCUnstructuredMeshVTK::appendElement(FCAbstractElement* element)
{
    if (!element) return;
    FCModelEnum::FITKEleType type = element->getEleType();
    if (!fcmeshEleTypeToVTK.contains(type)) return;

    FCElementList::appendElement(element);

    const int count = element->getNodeCount();
    vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
    for (int i = 0; i < count; ++i) {
        const int nodeID = element->getNodeID(i);
        const int index = getNodeIndexByID(nodeID);
        idList->InsertNextId(index);
    }
    _vtkDataSet->InsertNextCell(fcmeshEleTypeToVTK[type], idList);
    _orphanMeshTagArrayCell->InsertNextValue(element->getNativeFlag() ? 0 : 1);
    _modified = true;
}

void FCUnstructuredMeshVTK::removeAllElement()
{
    FCElementList::removeAllElement();
    if (_orphanMeshTagArrayCell) {
        _orphanMeshTagArrayCell->Reset();
        _orphanMeshTagArrayCell->Resize(0);
    }
}

int FCUnstructuredMeshVTK::removeElementAt(int index)
{
    const int eId = FCElementList::removeElementAt(index);
    if (eId > 0 && _orphanMeshTagArrayCell) {
        const int nVals = _orphanMeshTagArrayCell->GetNumberOfValues();
        for (int i = index; i < nVals - 1; ++i)
            _orphanMeshTagArrayCell->SetValue(i, _orphanMeshTagArrayCell->GetValue(i + 1));
        _orphanMeshTagArrayCell->Resize(nVals - 1);
    }
    return eId;
}

int FCUnstructuredMeshVTK::removeElementByID(int id)
{
    const int iEle = FCElementList::removeElementByID(id);
    if (iEle >= 0 && _orphanMeshTagArrayCell) {
        const int nVals = _orphanMeshTagArrayCell->GetNumberOfValues();
        for (int i = iEle; i < nVals - 1; ++i)
            _orphanMeshTagArrayCell->SetValue(i, _orphanMeshTagArrayCell->GetValue(i + 1));
        _orphanMeshTagArrayCell->Resize(nVals - 1);
    }
    return iEle;
}

bool FCUnstructuredMeshVTK::writeToFile(const QString& file)
{
    vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
    writer->SetFileName(file.toUtf8().constData());
    writer->SetFileTypeToASCII();
    writer->SetInputData(_vtkDataSet);
    return writer->Write() != 0;
}

void FCUnstructuredMeshVTK::reverseEleByID(int id)
{
    reverseEleByIndex(getEleIndexByID(id));
}

void FCUnstructuredMeshVTK::reverseEleByIndex(int index)
{
    FCElementList::reverseEleByIndex(index);
    vtkCell* cell = _vtkDataSet->GetCell(index);
    if (!cell) return;
    vtkIdList* pts = cell->GetPointIds();
    if (!pts) return;
    const int np = pts->GetNumberOfIds();
    vtkIdType* pids = new vtkIdType[np];
    for (int i = 0; i < np / 2; ++i) {
        pids[i] = pts->GetId(np - 1 - i);
        pids[np - 1 - i] = pts->GetId(i);
    }
    if (np % 2) pids[np / 2] = pts->GetId(np / 2);
    _vtkDataSet->ReplaceCell(index, np, pids);
    delete[] pids;
    _modified = true;
}

FCMeshFeatureVTK* FCUnstructuredMeshVTK::getMeshFeature()
{
    return _feature;
}

vtkUnstructuredGrid* FCUnstructuredMeshVTK::getGrid()
{
    return _vtkDataSet;
}

FCGeoToMeshMapper* FCUnstructuredMeshVTK::getGeoToMeshMapper()
{
    return _geoToMeshMapper;
}

bool FCUnstructuredMeshVTK::getBounds(double bds6[6])
{
    if (!_vtkDataSet) return false;
    if (_vtkDataSet->GetNumberOfCells() == 0 && _vtkDataSet->GetNumberOfPoints() == 0)
        return false;
    const double* bds = _vtkDataSet->GetBounds();
    for (int i = 0; i < 6; ++i) bds6[i] = bds[i];
    return true;
}

int FCUnstructuredMeshVTK::getNumberOfCells()
{
    return _vtkDataSet ? _vtkDataSet->GetNumberOfCells() : 0;
}

void FCUnstructuredMeshVTK::reConstructure()
{
    _vtkDataSet->Reset();
    int nPtArrs = _vtkDataSet->GetPointData()->GetNumberOfArrays();
    int nCellArrs = _vtkDataSet->GetCellData()->GetNumberOfArrays();
    for (int i = nPtArrs - 1; i >= 0; --i)
        _vtkDataSet->GetPointData()->RemoveArray(i);
    for (int i = nCellArrs - 1; i >= 0; --i)
        _vtkDataSet->GetCellData()->RemoveArray(i);

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    _vtkDataSet->SetPoints(points);
    if (_orphanMeshTagArrayPt) {
        _orphanMeshTagArrayPt->Resize(0);
        _orphanMeshTagArrayPt->SetNumberOfComponents(1);
        _vtkDataSet->GetPointData()->AddArray(_orphanMeshTagArrayPt);
    }
    if (_orphanMeshTagArrayCell) {
        _orphanMeshTagArrayCell->Resize(0);
        _orphanMeshTagArrayCell->SetNumberOfComponents(1);
        _vtkDataSet->GetCellData()->AddArray(_orphanMeshTagArrayCell);
    }

    double pos3[3] = {0, 0, 0};
    const int nNodes = getNodeCount();
    for (int i = 0; i < nNodes; ++i) {
        FCNode* node = getNodeAt(i);
        if (!node) continue;
        node->getCoor(pos3);
        points->InsertNextPoint(pos3);
        _orphanMeshTagArrayPt->InsertNextValue(node->getNativeFlag() ? 0 : 1);
    }

    const int nEles = getElementCount();
    for (int i = 0; i < nEles; ++i) {
        FCAbstractElement* element = getElementAt(i);
        if (!element) continue;
        const int count = element->getNodeCount();
        vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
        for (int j = 0; j < count; ++j) {
            const int nodeID = element->getNodeID(j);
            idList->InsertNextId(getNodeIndexByID(nodeID));
        }
        _vtkDataSet->InsertNextCell(fcmeshEleTypeToVTK[element->getEleType()], idList);
        _orphanMeshTagArrayCell->InsertNextValue(element->getNativeFlag() ? 0 : 1);
    }
}

void FCUnstructuredMeshVTK::transformVTKMesh(vtkUnstructuredGrid* grid)
{
    if (!grid) return;
    clearMesh();  // 清空网格（含 VTK）
    _vtkDataSet->DeepCopy(grid);
    const int npt = _vtkDataSet->GetNumberOfPoints();
    for (int i = 0; i < npt; ++i) {
        double coor[3] = {0, 0, 0};
        _vtkDataSet->GetPoint(i, coor);
        FCNode* node = new FCNode(0, coor[0], coor[1], coor[2]);
        FCNodeList::addNode(node);
    }
    const int ncell = _vtkDataSet->GetNumberOfCells();
    for (int i = 0; i < ncell; ++i) {
        vtkCell* cell = _vtkDataSet->GetCell(i);
        if (!cell) continue;
        const int cT = cell->GetCellType();
        FCModelEnum::FITKEleType t = fcmeshEleTypeToVTK.key(cT, FCModelEnum::EleNone);
        FCAbstractElement* ele = FCElementFactory::createElement(t);
        if (!ele) continue;
        QList<int> cellID;
        const int ncellPt = cell->GetNumberOfPoints();
        for (int cpt = 0; cpt < ncellPt; ++cpt)
            cellID.append(cell->GetPointId(cpt) + 1);  // VTK 0-based -> FC 1-based
        ele->setNodeID(cellID);
        FCElementList::appendElement(ele);
    }
    reConstructure();  // 根据节点/单元列表重建 VTK 数据
}

void FCUnstructuredMeshVTK::clearMesh()
{
    removeAllNode();
    removeAllElement();
    _vtkDataSet->GetPoints()->Reset();
    int nPtArrs = _vtkDataSet->GetPointData()->GetNumberOfArrays();
    int nCellArrs = _vtkDataSet->GetCellData()->GetNumberOfArrays();
    for (int i = nPtArrs - 1; i >= 0; --i)
        _vtkDataSet->GetPointData()->RemoveArray(i);
    for (int i = nCellArrs - 1; i >= 0; --i)
        _vtkDataSet->GetCellData()->RemoveArray(i);
    _vtkDataSet->Reset();
    if (_orphanMeshTagArrayPt) {
        _orphanMeshTagArrayPt->Reset();
        _orphanMeshTagArrayPt->Resize(0);
        _vtkDataSet->GetPointData()->AddArray(_orphanMeshTagArrayPt);
    }
    if (_orphanMeshTagArrayCell) {
        _orphanMeshTagArrayCell->Reset();
        _orphanMeshTagArrayCell->Resize(0);
        _vtkDataSet->GetCellData()->AddArray(_orphanMeshTagArrayCell);
    }
}

void FCUnstructuredMeshVTK::clearNativeMesh()
{
    _vtkDataSet->GetPoints()->Reset();
    _vtkDataSet->Reset();
    int nPtArrs = _vtkDataSet->GetPointData()->GetNumberOfArrays();
    int nCellArrs = _vtkDataSet->GetCellData()->GetNumberOfArrays();
    for (int i = nPtArrs - 1; i >= 0; --i)
        _vtkDataSet->GetPointData()->RemoveArray(i);
    for (int i = nCellArrs - 1; i >= 0; --i)
        _vtkDataSet->GetCellData()->RemoveArray(i);

    int nEles = getElementCount();
    for (int i = nEles - 1; i >= 0; --i) {
        FCAbstractElement* element = getElementAt(i);
        if (!element || !element->getNativeFlag()) continue;
        fastRemoveElementAt(i);
    }
    int nPoints = getNodeCount();
    for (int i = nPoints - 1; i >= 0; --i) {
        FCNode* node = getNodeAt(i);
        if (!node || !node->getNativeFlag()) continue;
        fastRemoveNodeAt(i);
    }
    updateNodeIDIndexMap();
    updateElementIDIndexMap();
    reConstructure();
    update();
    if (_geoToMeshMapper) _geoToMeshMapper->clear();
}

void FCUnstructuredMeshVTK::clearOrphanMesh()
{
    _vtkDataSet->GetPoints()->Reset();
    _vtkDataSet->Reset();
    int nPtArrs = _vtkDataSet->GetPointData()->GetNumberOfArrays();
    int nCellArrs = _vtkDataSet->GetCellData()->GetNumberOfArrays();
    for (int i = nPtArrs - 1; i >= 0; --i)
        _vtkDataSet->GetPointData()->RemoveArray(i);
    for (int i = nCellArrs - 1; i >= 0; --i)
        _vtkDataSet->GetCellData()->RemoveArray(i);

    int nEles = getElementCount();
    for (int i = nEles - 1; i >= 0; --i) {
        FCAbstractElement* element = getElementAt(i);
        if (!element || element->getNativeFlag()) continue;
        fastRemoveElementAt(i);
    }
    int nPoints = getNodeCount();
    for (int i = nPoints - 1; i >= 0; --i) {
        FCNode* node = getNodeAt(i);
        if (!node || node->getNativeFlag()) continue;
        fastRemoveNodeAt(i);
    }
    updateNodeIDIndexMap();
    updateElementIDIndexMap();
    reConstructure();
    update();
}

void FCUnstructuredMeshVTK::replaceNodesID(const QHash<int, int>& nodes)
{
    FCElementList::replaceNodesID(nodes);
    reConstructure();
    update();
}

} // namespace FC
