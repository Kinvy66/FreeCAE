/**
 * @file FCVTKGraphObjectModelShape.cpp
 * @brief 几何形状代理 -> VTK 图元实现
 */
#include "FCVTKGraphObjectModelShape.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCGeometryInterface/FCAbsGeoShapeAgent.h>
#include <vtkActor.h>
#include <vtkMapper.h>

namespace FC {

FCVTKGraphObjectModelShape::FCVTKGraphObjectModelShape(FCAbsGeoShapeAgent* modelData)
    : FCVTKGraphObjectModelBase(modelData)
{
    if (!modelData) return;
    init();
    if (!generateDataSet(modelData)) return;
    generateGraph();
}

void FCVTKGraphObjectModelShape::update(bool forceUpdate)
{
    FCAbsGeoShapeAgent* shapeAgent = dynamic_cast<FCAbsGeoShapeAgent*>(static_cast<FCAbstractDataObject*>(mDataObject));
    if (!shapeAgent) return;
    updateVisibility();
    QVariant newState;
    shapeAgent->createShapeState(newState);
    if (newState == m_shapeState && !forceUpdate) return;
    clearData();
    generateDataSet(shapeAgent);
    vtkActor* actorFace = vtkActor::SafeDownCast(m_fActorFace);
    if (m_gridFace && actorFace && actorFace->GetMapper())
        actorFace->GetMapper()->Update();
    vtkActor* actorEdge = vtkActor::SafeDownCast(m_fActorEdge);
    if (m_gridEdge && actorEdge && actorEdge->GetMapper())
        actorEdge->GetMapper()->Update();
    vtkActor* actorVertex = vtkActor::SafeDownCast(m_fActorVertex);
    if (m_gridVertex && actorVertex && actorVertex->GetMapper())
        actorVertex->GetMapper()->Update();
}

} // namespace FC
