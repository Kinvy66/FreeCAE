/**
 * @file FCVTKGraphObjectModelBase.h
 * @brief OCC 几何形状 -> VTK 图元 基类（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKGraphObjectModelBase）
 */
#ifndef FCVTKGRAPHOBJECTMODELBASE_H
#define FCVTKGRAPHOBJECTMODELBASE_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKGraphObject3D.h"
#include "FCVTKCommons.h"
#include <FCData/FCIdHash.h>
#include <QVector>
#include <QVariant>
#include <QHash>

class vtkPolyData;
class vtkUnstructuredGrid;
class vtkDataSet;
class vtkDoubleArray;

namespace FC {

class FCAbsGeoShapeAgent;
class FCGeometryMeshVS;
class FCVirtualTopoManager;
class FCAbsVirtualTopo;
class FCVTKShapeColorMap;
class FCVTKGraphObjectSelect;

class FCVTKGRAPHADAPTOR_API FCVTKGraphObjectModelBase : public FCVTKGraphObject3D
{
public:
    explicit FCVTKGraphObjectModelBase(FCAbstractDataObject* dataObj);
    ~FCVTKGraphObjectModelBase() override;

    void setVisible(bool visibility) override;
    void setViewMode(FCVTKCommons::ShapeMeshViewMode type, bool visible) override;
    void setTransparent(bool isOn) override;
    void setColor(QColor color, FCVTKCommons::ShapeType type, FCID shapeId) override;
    void highlight(FCVTKCommons::ShapeType type = FCVTKCommons::ShapeTypeNone, QColor color = QColor()) override;
    void disHighlight() override;
    void advanceHighlight(FCVTKCommons::ShapeType type, QVector<FCID> indice, QColor color = QColor()) override;
    void disAdvanceHighlight() override;
    int getNumberOf(FCVTKCommons::ShapeType type) override;
    void setPickMode(FCVTKCommons::ShapePickMode mode) override;
    FCID getShapeIdByVTKCellId(int vtkCellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType) override;
    const QVector<int> getVTKCellIdsByShapeId(FCID shapeId, FCVTKCommons::ShapeAbsEnum topAbsShapeType) override;
    const QVector<int> getVTKCellIdsByVTKCellId(int cellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType) override;
    vtkDataSet* getMesh(FCVTKCommons::ShapeType type) override;

protected:
    void init();
    bool meshVSToVtkDataSet();
    void createVertexGrid();
    void createEdgeGrid();
    void createFaceGrid();
    void initializeSolidFaceMap();
    bool generateDataSet(FCAbsGeoShapeAgent* shapeAgent);
    void generateGraph();
    void clearData();

protected:
    FCGeometryMeshVS* m_triData = nullptr;
    FCVirtualTopoManager* m_topoMgr = nullptr;
    const QString c_defaultArrName = QStringLiteral("Default");

    FCVTKGraphObjectSelect* m_highlightSelector = nullptr;
    FCVTKShapeColorMap* m_colorMapVertex = nullptr;
    FCVTKShapeColorMap* m_colorMapEdge = nullptr;
    FCVTKShapeColorMap* m_colorMapFace = nullptr;

    vtkUnstructuredGrid* m_gridVertex = nullptr;
    vtkUnstructuredGrid* m_gridEdge = nullptr;
    vtkPolyData* m_gridFace = nullptr;
    vtkDoubleArray* m_normalFacePoint = nullptr;

    vtkProp* m_fActorVertex = nullptr;
    vtkProp* m_fActorEdge = nullptr;
    vtkProp* m_fActorFace = nullptr;

    QVector<FCID> m_cellIdVertexIdMap;
    QVector<FCID> m_cellIdEdgeIdMap;
    QVector<FCID> m_cellIdFaceIdMap;
    QHash<FCID, FCID> m_faceSolidIdMap;
    QHash<FCID, int> m_vertexIdCellIdMap;
    QHash<FCID, int> m_edgeIdCellIdMap;
    QHash<FCID, QVector<FCID>> m_solidFaceIdsHash;
    QHash<FCID, QVector<int>> m_faceCellIdsHash;

    QVariant m_shapeState;
};

} // namespace FC

#endif // FCVTKGRAPHOBJECTMODELBASE_H
