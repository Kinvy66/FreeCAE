/**
 * @file FCVTKGraphObjectModelBase.cpp
 * @brief OCC 几何 MeshVS -> VTK 网格与 Actor 实现
 */
#include "FCVTKGraphObjectModelBase.h"
#include "FCVTKShapeColorMap.h"
#include "FCVTKGraphObjectSelect.h"
#include <FCGeometryInterface/FCAbsGeoShapeAgent.h>
#include <FCGeometryInterface/FCGeometryMeshVS.h>
#include <FCGeometryInterface/FCGeometryMeshEntity.h>
#include <FCGeometryInterface/FCVirtualTopoManager.h>
#include <FCGeometryInterface/FCGeoEnum.h>

#include <vtkObject.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkCellType.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPolygon.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProp.h>
#include <vtkProperty.h>

#include <QtCore/QHashIterator>
#include <cmath>

#if VTK_MAJOR_VERSION >= 9
#define FC_VTK_ID_TYPE const vtkIdType*
#else
#define FC_VTK_ID_TYPE vtkIdType*
#endif

namespace FC {

FCVTKGraphObjectModelBase::FCVTKGraphObjectModelBase(FCAbstractDataObject* dataObj)
    : FCVTKGraphObject3D(dataObj)
{
    m_polygonOffset = FCVTKCommons::s_polygonOffsetGeom;
    m_shapeInfo.Type = FCVTKCommons::ModelShape;
    setRenderLayer(0, 2);
}

FCVTKGraphObjectModelBase::~FCVTKGraphObjectModelBase()
{
    deleteVtkObj(m_gridVertex);
    deleteVtkObj(m_gridEdge);
    deleteVtkObj(m_gridFace);
    deleteVtkObj(m_normalFacePoint);
    deleteObj(m_colorMapVertex);
    deleteObj(m_colorMapEdge);
    deleteObj(m_colorMapFace);
    m_highlightSelector = nullptr;
    m_solidFaceIdsHash.clear();
    m_faceSolidIdMap.clear();
    m_cellIdVertexIdMap.clear();
    m_cellIdEdgeIdMap.clear();
    m_cellIdFaceIdMap.clear();
    m_vertexIdCellIdMap.clear();
    m_edgeIdCellIdMap.clear();
    m_faceCellIdsHash.clear();
    m_triData = nullptr;
    m_topoMgr = nullptr;
}

void FCVTKGraphObjectModelBase::init()
{
    m_gridVertex = vtkUnstructuredGrid::New();
    m_gridVertex->SetPoints(vtkSmartPointer<vtkPoints>::New());
    m_gridEdge = vtkUnstructuredGrid::New();
    m_gridEdge->SetPoints(vtkSmartPointer<vtkPoints>::New());
    m_gridFace = vtkPolyData::New();
    m_gridFace->SetPoints(vtkSmartPointer<vtkPoints>::New());
    m_gridFace->SetPolys(vtkSmartPointer<vtkCellArray>::New());
    m_normalFacePoint = vtkDoubleArray::New();
    m_normalFacePoint->SetNumberOfComponents(3);
    m_gridFace->GetPointData()->SetNormals(m_normalFacePoint);

    m_highlightSelector = new FCVTKGraphObjectSelect;
    m_addinGraphObjList.push_back(m_highlightSelector);
}

bool FCVTKGraphObjectModelBase::meshVSToVtkDataSet()
{
    if (!m_triData || !m_gridVertex || !m_gridEdge || !m_gridFace)
        return false;
    createVertexGrid();
    createEdgeGrid();
    createFaceGrid();
    initializeSolidFaceMap();
    return true;
}

void FCVTKGraphObjectModelBase::createVertexGrid()
{
    if (!m_gridVertex || !m_triData) return;
    vtkPoints* points = m_gridVertex->GetPoints();
    if (!points) return;
    const QHash<FCID, FCGeoMeshVSPt*>& pointsHash = m_triData->getPointItems();
    for (auto it = pointsHash.constBegin(); it != pointsHash.constEnd(); ++it) {
        FCID meshPtId = it.key();
        FCGeoMeshVSPt* pt = it.value();
        vtkIdType ptIndex = points->InsertNextPoint(pt->x(), pt->y(), pt->z());
        vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
        cell->InsertNextId(ptIndex);
        m_gridVertex->InsertNextCell(VTK_VERTEX, cell);
        m_cellIdVertexIdMap.push_back(meshPtId);
        m_vertexIdCellIdMap.insert(meshPtId, static_cast<int>(ptIndex));
    }
}

void FCVTKGraphObjectModelBase::createEdgeGrid()
{
    if (!m_gridEdge || !m_triData) return;
    vtkPoints* points = m_gridEdge->GetPoints();
    if (!points) return;
    const QHash<FCID, FCGeoMeshVSEdgeEntity*>& edgesHash = m_triData->getEdgeItems();
    for (auto it = edgesHash.constBegin(); it != edgesHash.constEnd(); ++it) {
        FCID meshEdgeId = it.key();
        FCGeoMeshVSEdgeEntity* edge = it.value();
        const QList<FCGeoMeshVSPt*>& pts = edge->getPoints();
        vtkSmartPointer<vtkIdList> polyLineCell = vtkSmartPointer<vtkIdList>::New();
        for (FCGeoMeshVSPt* pt : pts) {
            vtkIdType ptIndex = points->InsertNextPoint(pt->x(), pt->y(), pt->z());
            polyLineCell->InsertNextId(ptIndex);
        }
        int cellIndex = static_cast<int>(m_gridEdge->InsertNextCell(VTK_POLY_LINE, polyLineCell));
        m_cellIdEdgeIdMap.push_back(meshEdgeId);
        m_edgeIdCellIdMap.insert(meshEdgeId, cellIndex);
    }
}

void FCVTKGraphObjectModelBase::createFaceGrid()
{
    if (!m_gridFace || !m_triData) return;
    vtkPoints* points = m_gridFace->GetPoints();
    if (!points) return;
    const QHash<FCID, FCGeoMeshVSFaceEntity*>& facesHash = m_triData->getFaceItems();
    vtkDoubleArray* tempCellNormals = vtkDoubleArray::New();
    tempCellNormals->SetNumberOfComponents(3);
    vtkPoints* tempPts = vtkPoints::New();
    tempPts->SetNumberOfPoints(3);
    double tempNor3[3] = { 0., 0., 0. };
    int ptIndexStart = 0;

    for (auto it = facesHash.constBegin(); it != facesHash.constEnd(); ++it) {
        FCID meshFaceId = it.key();
        FCGeoMeshVSFaceEntity* face = it.value();
        const QList<FCGeoMeshVSPt*>& facePoints = face->getPoints();
        for (FCGeoMeshVSPt* fp : facePoints)
            points->InsertNextPoint(fp->x(), fp->y(), fp->z());
        const QList<FCGeoMeshVSTri*>& tris = face->getTriangles();
        for (FCGeoMeshVSTri* tri : tris) {
            const int* nodes = tri->getNode();
            int ptIndex1 = ptIndexStart + nodes[0];
            int ptIndex2 = ptIndexStart + nodes[1];
            int ptIndex3 = ptIndexStart + nodes[2];
            tempPts->SetPoint(0, points->GetPoint(ptIndex1));
            tempPts->SetPoint(1, points->GetPoint(ptIndex2));
            tempPts->SetPoint(2, points->GetPoint(ptIndex3));
            vtkSmartPointer<vtkIdList> triCell = vtkSmartPointer<vtkIdList>::New();
            triCell->InsertNextId(ptIndex1);
            triCell->InsertNextId(ptIndex2);
            triCell->InsertNextId(ptIndex3);
            int cellIndex = static_cast<int>(m_gridFace->InsertNextCell(VTK_TRIANGLE, triCell));
            vtkPolygon::ComputeNormal(tempPts, tempNor3);
            tempCellNormals->InsertNextTuple3(tempNor3[0], tempNor3[1], tempNor3[2]);
            m_cellIdFaceIdMap.push_back(meshFaceId);
            m_faceCellIdsHash[meshFaceId].push_back(cellIndex);
        }
        ptIndexStart += facePoints.count();
    }

    m_normalFacePoint->SetNumberOfTuples(m_gridFace->GetNumberOfPoints());
    m_normalFacePoint->FillComponent(0, 0);
    m_normalFacePoint->FillComponent(1, 0);
    m_normalFacePoint->FillComponent(2, 0);
    vtkIdType cellId = 0;
    vtkIdType npts = 0;
    FC_VTK_ID_TYPE pts = nullptr;
    for (m_gridFace->GetPolys()->InitTraversal(); m_gridFace->GetPolys()->GetNextCell(npts, pts); ++cellId) {
        double* norCell = tempCellNormals->GetTuple3(cellId);
        for (vtkIdType i = 0; i < npts; ++i) {
            double* norPt = m_normalFacePoint->GetTuple3(pts[i]);
            double r[3] = { norPt[0] + norCell[0], norPt[1] + norCell[1], norPt[2] + norCell[2] };
            m_normalFacePoint->SetTuple3(pts[i], r[0], r[1], r[2]);
        }
    }
    for (vtkIdType i = 0; i < m_gridFace->GetNumberOfPoints(); ++i) {
        double* ptNormal = m_normalFacePoint->GetTuple3(i);
        double length = std::sqrt(ptNormal[0]*ptNormal[0] + ptNormal[1]*ptNormal[1] + ptNormal[2]*ptNormal[2]);
        if (length > 1e-12) {
            ptNormal[0] /= length;
            ptNormal[1] /= length;
            ptNormal[2] /= length;
            m_normalFacePoint->SetTuple3(i, ptNormal[0], ptNormal[1], ptNormal[2]);
        }
    }
    tempCellNormals->Delete();
    tempPts->Delete();
}

void FCVTKGraphObjectModelBase::initializeSolidFaceMap()
{
    if (!m_topoMgr) return;
    FCShapeVirtualTopoManager* tpSolidMgr = m_topoMgr->getShapeVirtualTopoManager(FCGeoEnum::VSSolid);
    if (!tpSolidMgr) return;
    int nSolids = tpSolidMgr->getDataCount();
    for (int i = 0; i < nSolids; ++i) {
        FCAbsVirtualTopo* vSolid = tpSolidMgr->getDataByIndex(i);
        FCID solidId = vSolid->getDataObjectID();
        int nSub = vSolid->getSubTopoCount();
        for (int j = 0; j < nSub; ++j) {
            FCAbsVirtualTopo* vFace = vSolid->getSubTopo(j);
            FCID faceId = vFace->getDataObjectID();
            m_solidFaceIdsHash[solidId].push_back(faceId);
            m_faceSolidIdMap[faceId] = solidId;
        }
    }
}

bool FCVTKGraphObjectModelBase::generateDataSet(FCAbsGeoShapeAgent* shapeAgent)
{
    if (!shapeAgent) return false;
    FCGeometryMeshVS* triData = shapeAgent->getMeshVS();
    if (!triData || !triData->isValid()) return false;
    m_triData = triData;
    m_topoMgr = shapeAgent->getVirtualTopoManager();
    QVariant newState;
    shapeAgent->createShapeState(newState);
    m_shapeState = newState;
    return meshVSToVtkDataSet();
}

void FCVTKGraphObjectModelBase::generateGraph()
{
    if (!m_gridVertex || !m_gridEdge || !m_gridFace) return;
    double colorV[3], colorE[3], colorF[3];
    FCVTKCommons::QColorToDouble3(FCVTKCommons::s_geoVertexColor, colorV);
    FCVTKCommons::QColorToDouble3(FCVTKCommons::s_geoEdgeColor, colorE);
    FCVTKCommons::QColorToDouble3(FCVTKCommons::s_geoFaceColor, colorF);

    m_colorMapVertex = new FCVTKShapeColorMap;
    m_colorMapEdge = new FCVTKShapeColorMap;
    m_colorMapFace = new FCVTKShapeColorMap;
    m_colorMapVertex->setInputData(m_gridVertex);
    m_colorMapEdge->setInputData(m_gridEdge);
    m_colorMapFace->setInputData(m_gridFace);
    m_colorMapVertex->setColor(c_defaultArrName, FCVTKCommons::s_geoVertexColor);
    m_colorMapEdge->setColor(c_defaultArrName, FCVTKCommons::s_geoEdgeColor);
    m_colorMapFace->setColor(c_defaultArrName, FCVTKCommons::s_geoFaceColor);
    m_colorMapVertex->update();
    m_colorMapEdge->update();
    m_colorMapFace->update();

    vtkSmartPointer<vtkDataSetMapper> mapperV = vtkSmartPointer<vtkDataSetMapper>::New();
    mapperV->SetInputData(m_gridVertex);
    mapperV->SetScalarVisibility(true);
    mapperV->SetScalarModeToUseCellFieldData();
    mapperV->SelectColorArray(c_defaultArrName.toUtf8().constData());
    vtkActor* actorV = vtkActor::New();
    actorV->SetMapper(mapperV);
    actorV->GetProperty()->SetPointSize(5);
    actorV->SetPickable(0);
    m_fActorVertex = actorV;
    addActor(actorV);

    vtkSmartPointer<vtkDataSetMapper> mapperE = vtkSmartPointer<vtkDataSetMapper>::New();
    mapperE->SetInputData(m_gridEdge);
    mapperE->SetScalarVisibility(true);
    mapperE->SetScalarModeToUseCellFieldData();
    mapperE->SelectColorArray(c_defaultArrName.toUtf8().constData());
    vtkActor* actorE = vtkActor::New();
    actorE->SetMapper(mapperE);
    actorE->GetProperty()->SetLineWidth(2);
    actorE->SetPickable(0);
    m_fActorEdge = actorE;
    addActor(actorE);

    vtkSmartPointer<vtkPolyDataMapper> mapperF = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperF->SetInputData(m_gridFace);
    mapperF->SetScalarVisibility(true);
    mapperF->SetScalarModeToUseCellFieldData();
    mapperF->SelectColorArray(c_defaultArrName.toUtf8().constData());
    vtkActor* actorF = vtkActor::New();
    actorF->SetMapper(mapperF);
    actorF->GetProperty()->SetEdgeVisibility(false);
    actorF->GetProperty()->SetOpacity(1.0 - FCVTKCommons::s_transparency);
    actorF->SetPickable(0);
    m_fActorFace = actorF;
    addActor(actorF);
}

void FCVTKGraphObjectModelBase::clearData()
{
    if (m_gridVertex) { m_gridVertex->Reset(); m_gridVertex->GetPoints()->Reset(); }
    if (m_gridEdge) { m_gridEdge->Reset(); m_gridEdge->GetPoints()->Reset(); }
    if (m_gridFace) { m_gridFace->Reset(); m_gridFace->GetPoints()->Reset(); m_gridFace->GetPolys()->Reset(); }
    if (m_normalFacePoint) m_normalFacePoint->Reset();
    if (m_colorMapVertex) m_colorMapVertex->clear();
    if (m_colorMapEdge) m_colorMapEdge->clear();
    if (m_colorMapFace) m_colorMapFace->clear();
    if (m_highlightSelector) m_highlightSelector->clearData();
    m_solidFaceIdsHash.clear();
    m_faceSolidIdMap.clear();
    m_cellIdVertexIdMap.clear();
    m_cellIdEdgeIdMap.clear();
    m_cellIdFaceIdMap.clear();
    m_vertexIdCellIdMap.clear();
    m_edgeIdCellIdMap.clear();
    m_faceCellIdsHash.clear();
}

void FCVTKGraphObjectModelBase::setVisible(bool visibility)
{
    if (m_fActorFace) m_fActorFace->SetVisibility(m_viewModes[FCVTKCommons::SMVM_Shade] && visibility ? 1 : 0);
    if (m_fActorEdge) m_fActorEdge->SetVisibility(m_viewModes[FCVTKCommons::SMVM_Wireframe] && visibility ? 1 : 0);
    if (m_fActorVertex) m_fActorVertex->SetVisibility(m_viewModes[FCVTKCommons::SMVM_Vertex] && visibility ? 1 : 0);
    if (m_highlightSelector)
        m_highlightSelector->setVisible(visibility && getDataVisibility() && (m_isHighlighting || m_isAdvHighlighting));
}

void FCVTKGraphObjectModelBase::setViewMode(FCVTKCommons::ShapeMeshViewMode type, bool visible)
{
    vtkProp* fActor = nullptr;
    switch (type) {
    case FCVTKCommons::SMVM_Shade:   fActor = m_fActorFace; break;
    case FCVTKCommons::SMVM_Wireframe: fActor = m_fActorEdge; break;
    case FCVTKCommons::SMVM_Vertex:  fActor = m_fActorVertex; break;
    default: return;
    }
    if (fActor) fActor->SetVisibility(visible ? 1 : 0);
    FCVTKGraphObject3D::setViewMode(type, visible);
}

void FCVTKGraphObjectModelBase::setTransparent(bool isOn)
{
    // 只设置面透明度，点和线始终不透明
    double opa = isOn ? 1.0 - FCVTKCommons::s_transparency : 1.0;
    if (vtkActor* a = vtkActor::SafeDownCast(m_fActorFace)) a->GetProperty()->SetOpacity(opa);
}

void FCVTKGraphObjectModelBase::setColor(QColor color, FCVTKCommons::ShapeType type, FCID shapeId)
{
    switch (type) {
    case FCVTKCommons::ModelVertex: {
        int cellId = m_vertexIdCellIdMap.value(shapeId, -1);
        if (cellId >= 0 && m_colorMapVertex)
            m_colorMapVertex->setCellsColor(c_defaultArrName, QList<int>() << cellId, color);
        break;
    }
    case FCVTKCommons::ModelEdge: {
        int cellId = m_edgeIdCellIdMap.value(shapeId, -1);
        if (cellId >= 0 && m_colorMapEdge)
            m_colorMapEdge->setCellsColor(c_defaultArrName, QList<int>() << cellId, color);
        break;
    }
    case FCVTKCommons::ModelFace:
        if (m_faceCellIdsHash.contains(shapeId) && m_colorMapFace)
            m_colorMapFace->setCellsColor(c_defaultArrName, m_faceCellIdsHash[shapeId].toList(), color);
        break;
    case FCVTKCommons::ModelSolid:
        if (m_solidFaceIdsHash.contains(shapeId) && m_colorMapFace) {
            QList<int> cellIds;
            for (FCID fId : m_solidFaceIdsHash[shapeId])
                cellIds.append(m_faceCellIdsHash.value(fId));
            m_colorMapFace->setCellsColor(c_defaultArrName, cellIds, color);
        }
        break;
    default: break;
    }
    if (m_colorMapVertex) m_colorMapVertex->update();
    if (m_colorMapEdge) m_colorMapEdge->update();
    if (m_colorMapFace) m_colorMapFace->update();
}

void FCVTKGraphObjectModelBase::highlight(FCVTKCommons::ShapeType type, QColor color)
{
    if (!m_highlightSelector) return;
    if (type == FCVTKCommons::ShapeTypeNone) type = FCVTKCommons::ModelFace;
    vtkDataSet* meshDataSet = getMesh(type);
    if (!meshDataSet) return;
    QVector<int> indice;
    for (int i = 0; i < meshDataSet->GetNumberOfCells(); ++i)
        indice.push_back(i);
    if (!color.isValid()) color = Qt::red;
    m_highlightSelector->setColor(color);
    m_highlightSelector->setSelectData(meshDataSet, indice, type);
    m_highlightSelector->setVisible(getDataVisibility());
    FCVTKGraphObject3D::highlight(type, color);
}

void FCVTKGraphObjectModelBase::disHighlight()
{
    if (m_highlightSelector) m_highlightSelector->setVisible(false);
    FCVTKGraphObject3D::disHighlight();
}

void FCVTKGraphObjectModelBase::advanceHighlight(FCVTKCommons::ShapeType type, QVector<FCID> indice, QColor color)
{
    if (!m_highlightSelector || indice.isEmpty()) return;
    vtkDataSet* meshDataSet = getMesh(type);
    if (!meshDataSet) return;
    FCVTKCommons::ShapeAbsEnum shapeEnum = FCVTKCommons::STA_SHAPE;
    switch (type) {
    case FCVTKCommons::ModelVertex: shapeEnum = FCVTKCommons::STA_VERTEX; break;
    case FCVTKCommons::ModelEdge:   shapeEnum = FCVTKCommons::STA_EDGE; break;
    case FCVTKCommons::ModelFace:  shapeEnum = FCVTKCommons::STA_FACE; break;
    case FCVTKCommons::ModelSolid: shapeEnum = FCVTKCommons::STA_SOLID; break;
    default: return;
    }
    int nCells = meshDataSet->GetNumberOfCells();
    QVector<int> cellFlags(nCells, 0);
    for (FCID shapeId : indice) {
        for (int vtkId : getVTKCellIdsByShapeId(shapeId, shapeEnum))
            cellFlags[vtkId] = 1;
    }
    QVector<int> vtkIndice;
    for (int i = 0; i < cellFlags.size(); ++i)
        if (cellFlags[i]) vtkIndice.push_back(i);
    if (!color.isValid()) color = Qt::red;
    m_highlightSelector->setColor(color);
    m_highlightSelector->setSelectData(meshDataSet, vtkIndice, type);
    m_highlightSelector->setVisible(getDataVisibility());
    FCVTKGraphObject3D::advanceHighlight(type, indice, color);
}

void FCVTKGraphObjectModelBase::disAdvanceHighlight()
{
    if (m_highlightSelector) {
        m_highlightSelector->clearData();
        m_highlightSelector->setVisible(false);
    }
    FCVTKGraphObject3D::disAdvanceHighlight();
}

int FCVTKGraphObjectModelBase::getNumberOf(FCVTKCommons::ShapeType type)
{
    if (!m_triData) return 0;
    switch (type) {
    case FCVTKCommons::ModelVertex: return m_triData->getPointItems().count();
    case FCVTKCommons::ModelEdge:  return m_triData->getEdgeItems().count();
    case FCVTKCommons::ModelFace:  return m_triData->getFaceItems().count();
    case FCVTKCommons::ModelSolid: return 0;
    default: return 0;
    }
}

void FCVTKGraphObjectModelBase::setPickMode(FCVTKCommons::ShapePickMode mode)
{
    int pv = 0, pe = 0, pf = 0;
    switch (mode) {
    case FCVTKCommons::PickVertex:  pv = 1; break;
    case FCVTKCommons::PickEdge:    pe = 1; break;
    case FCVTKCommons::PickFace:
    case FCVTKCommons::PickSolid:   pf = 1; break;
    case FCVTKCommons::PickAll:     pv = pe = pf = 1; break;
    default: break;
    }
    if (m_fActorVertex) m_fActorVertex->SetPickable(pv);
    if (m_fActorEdge) m_fActorEdge->SetPickable(pe);
    if (m_fActorFace) m_fActorFace->SetPickable(pf);
    FCVTKGraphObject3D::setPickMode(mode);
}

FCID FCVTKGraphObjectModelBase::getShapeIdByVTKCellId(int vtkCellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType)
{
    if (vtkCellId < 0) return FCID_INVALID;
    switch (topAbsShapeType) {
    case FCVTKCommons::STA_SOLID:
        if (vtkCellId < m_cellIdFaceIdMap.size()) {
            FCID faceId = m_cellIdFaceIdMap[vtkCellId];
            return m_faceSolidIdMap.value(faceId, FCID_INVALID);
        }
        break;
    case FCVTKCommons::STA_FACE:
        if (vtkCellId < m_cellIdFaceIdMap.size()) return m_cellIdFaceIdMap[vtkCellId];
        break;
    case FCVTKCommons::STA_EDGE:
        if (vtkCellId < m_cellIdEdgeIdMap.size()) return m_cellIdEdgeIdMap[vtkCellId];
        break;
    case FCVTKCommons::STA_VERTEX:
        if (vtkCellId < m_cellIdVertexIdMap.size()) return m_cellIdVertexIdMap[vtkCellId];
        break;
    default: break;
    }
    return FCID_INVALID;
}

const QVector<int> FCVTKGraphObjectModelBase::getVTKCellIdsByShapeId(FCID shapeId, FCVTKCommons::ShapeAbsEnum topAbsShapeType)
{
    QVector<int> cellIds;
    switch (topAbsShapeType) {
    case FCVTKCommons::STA_SOLID: {
        if (!m_solidFaceIdsHash.contains(shapeId)) break;
        for (FCID fId : m_solidFaceIdsHash[shapeId])
            cellIds.append(getVTKCellIdsByShapeId(fId, FCVTKCommons::STA_FACE));
        break;
    }
    case FCVTKCommons::STA_FACE:
        if (m_faceCellIdsHash.contains(shapeId)) cellIds = m_faceCellIdsHash[shapeId];
        break;
    case FCVTKCommons::STA_EDGE:
        if (m_edgeIdCellIdMap.contains(shapeId)) cellIds.push_back(m_edgeIdCellIdMap[shapeId]);
        break;
    case FCVTKCommons::STA_VERTEX:
        if (m_vertexIdCellIdMap.contains(shapeId)) cellIds.push_back(m_vertexIdCellIdMap[shapeId]);
        break;
    default: break;
    }
    return cellIds;
}

const QVector<int> FCVTKGraphObjectModelBase::getVTKCellIdsByVTKCellId(int cellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType)
{
    QVector<int> cellIds;
    switch (topAbsShapeType) {
    case FCVTKCommons::STA_SOLID:
        if (cellId < m_cellIdFaceIdMap.size()) {
            FCID faceId = m_cellIdFaceIdMap[cellId];
            FCID solidId = m_faceSolidIdMap.value(faceId, FCID_INVALID);
            if (solidId != FCID_INVALID && m_solidFaceIdsHash.contains(solidId))
                for (FCID fId : m_solidFaceIdsHash[solidId])
                    cellIds.append(m_faceCellIdsHash.value(fId));
        }
        break;
    case FCVTKCommons::STA_FACE:
        if (cellId < m_cellIdFaceIdMap.size())
            cellIds = m_faceCellIdsHash.value(m_cellIdFaceIdMap[cellId]);
        break;
    case FCVTKCommons::STA_EDGE:
    case FCVTKCommons::STA_VERTEX:
        cellIds.push_back(cellId);
        break;
    default: break;
    }
    return cellIds;
}

vtkDataSet* FCVTKGraphObjectModelBase::getMesh(FCVTKCommons::ShapeType type)
{
    switch (type) {
    case FCVTKCommons::ModelVertex: return m_gridVertex;
    case FCVTKCommons::ModelEdge:  return m_gridEdge;
    case FCVTKCommons::ModelFace:
    case FCVTKCommons::ModelSolid: return m_gridFace;
    default: break;
    }
    return nullptr;
}

} // namespace FC
