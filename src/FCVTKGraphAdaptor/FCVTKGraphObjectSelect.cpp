/**
 * @file FCVTKGraphObjectSelect.cpp
 * @brief 高亮选择图元实现
 */
#include "FCVTKGraphObjectSelect.h"
#include "FCVTKCommons.h"
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>

namespace FC {

FCVTKGraphObjectSelect::FCVTKGraphObjectSelect()
    : FCVTKGraphObject3D(nullptr)
{
    init();
    setRenderLayer(2, 2);
    mHasFixedBounds = false;
}

FCVTKGraphObjectSelect::~FCVTKGraphObjectSelect()
{
    if (m_selectNode) { m_selectNode->Delete(); m_selectNode = nullptr; }
    if (m_section) { m_section->Delete(); m_section = nullptr; }
    if (m_idArray) { m_idArray->Delete(); m_idArray = nullptr; }
    if (m_extractSelection) { m_extractSelection->Delete(); m_extractSelection = nullptr; }
}

void FCVTKGraphObjectSelect::init()
{
    m_selectNode = vtkSelectionNode::New();
    m_section = vtkSelection::New();
    m_extractSelection = vtkExtractSelection::New();
    m_selectNode->SetContentType(vtkSelectionNode::INDICES);
    m_selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
    m_section->AddNode(m_selectNode);
    m_extractSelection->SetInputData(1, m_section);

    m_idArray = vtkIdTypeArray::New();
    m_selectNode->SetSelectionList(m_idArray);

    vtkDataSetMapper* mapper = vtkDataSetMapper::New();
    mapper->SetInputConnection(m_extractSelection->GetOutputPort());
    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);
    mapper->Delete();
    actor->SetPickable(false);
    actor->GetProperty()->SetPointSize(static_cast<float>(FCVTKCommons::s_highlightPointSize));
    actor->GetProperty()->SetLineWidth(static_cast<float>(FCVTKCommons::s_highlightLineWidth));
    actor->GetProperty()->SetOpacity(1.0 - FCVTKCommons::s_transparency);
    m_fActor = actor;
    addActor(actor);
}

void FCVTKGraphObjectSelect::setVisible(bool visibility)
{
    if (m_fActor) m_fActor->SetVisibility(visibility ? 1 : 0);
}

void FCVTKGraphObjectSelect::setTransparent(bool isOn)
{
    // 选择高亮始终不透明，不随透明 action 改变
    Q_UNUSED(isOn);
}

void FCVTKGraphObjectSelect::setViewMode(FCVTKCommons::ShapeMeshViewMode type, bool visible)
{
    vtkActor* a = vtkActor::SafeDownCast(m_fActor);
    if (!a) return;
    a->SetVisibility(1);
    switch (type) {
    case FCVTKCommons::SMVM_Shade:
        a->GetProperty()->SetRepresentation(visible ? 2 : (m_viewModes[FCVTKCommons::SMVM_Wireframe] ? 1 : (m_viewModes[FCVTKCommons::SMVM_Vertex] ? 0 : -1)));
        if (!visible && !m_viewModes[FCVTKCommons::SMVM_Wireframe] && !m_viewModes[FCVTKCommons::SMVM_Vertex])
            a->SetVisibility(0);
        break;
    case FCVTKCommons::SMVM_Wireframe:
        a->GetProperty()->SetEdgeVisibility(visible ? 1 : 0);
        break;
    case FCVTKCommons::SMVM_Vertex:
        a->GetProperty()->SetVertexVisibility(visible ? 1 : 0);
        break;
    default: break;
    }
    FCVTKGraphObject3D::setViewMode(type, visible);
}

void FCVTKGraphObjectSelect::setSelectData(vtkDataSet* grid, QVector<int>& indice, FCVTKCommons::ShapeType type)
{
    if (!m_extractSelection || !m_selectNode || !m_idArray || !m_fActor || !grid) return;

    switch (type) {
    case FCVTKCommons::MeshNode:
        m_selectNode->SetFieldType(vtkSelectionNode::SelectionField::POINT);
        break;
    case FCVTKCommons::ModelVertex:
    case FCVTKCommons::ModelEdge:
    case FCVTKCommons::ModelFace:
    case FCVTKCommons::ModelSolid:
    case FCVTKCommons::MeshElement:
    case FCVTKCommons::Others:
        m_selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        break;
    default:
        return;
    }

    m_idArray->Reset();
    m_idArray->SetNumberOfTuples(0);
    for (int id : indice)
        m_idArray->InsertNextValue(static_cast<vtkIdType>(id));

    m_extractSelection->SetInputData(0, grid);
    m_selectNode->Modified();
    m_extractSelection->Update();

    vtkActor* a = vtkActor::SafeDownCast(m_fActor);
    if (a && a->GetMapper()) a->GetMapper()->Update();
    if (a) {
        a->GetProperty()->SetColor(1., 0., 0.);
        switch (type) {
        case FCVTKCommons::ModelVertex:
        case FCVTKCommons::MeshNode:
            a->GetProperty()->SetRepresentation(VTK_POINTS);
            break;
        case FCVTKCommons::ModelEdge:
        case FCVTKCommons::MeshElement:
            a->GetProperty()->SetRepresentation(VTK_WIREFRAME);
            a->GetProperty()->SetEdgeVisibility(1);
            break;
        case FCVTKCommons::ModelFace:
        case FCVTKCommons::ModelSolid:
            a->GetProperty()->SetRepresentation(VTK_SURFACE);
            a->GetProperty()->SetEdgeVisibility(0);
            break;
        default: break;
        }
    }
}

void FCVTKGraphObjectSelect::setColor(QColor color)
{
    vtkActor* a = vtkActor::SafeDownCast(m_fActor);
    if (!a || !color.isValid()) return;
    double c3[3];
    FCVTKCommons::QColorToDouble3(color, c3);
    a->GetProperty()->SetColor(c3);
    a->GetProperty()->SetEdgeColor(c3);
    a->GetProperty()->SetVertexColor(c3);
}

void FCVTKGraphObjectSelect::clearData()
{
    if (m_idArray) m_idArray->Reset();
    if (m_selectNode) m_selectNode->Modified();
    if (m_extractSelection) {
        m_extractSelection->RemoveAllInputs();
        m_extractSelection->RemoveAllInputConnections(0);
    }
    if (m_fActor) {
        if (vtkActor::SafeDownCast(m_fActor) && vtkActor::SafeDownCast(m_fActor)->GetMapper())
            vtkActor::SafeDownCast(m_fActor)->GetMapper()->Update();
        m_fActor->SetVisibility(0);
    }
}

} // namespace FC
