/**
 * @file FCVTKGraphObjectModelCmd.cpp
 * @brief 几何命令 -> VTK 图元实现
 */
#include "FCVTKGraphObjectModelCmd.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryInterface/FCAbsGeoShapeAgent.h>
#include <vtkActor.h>
#include <vtkMapper.h>


namespace FC {

FCVTKGraphObjectModelCmd::FCVTKGraphObjectModelCmd(FCAbsGeoCommand* modelData)
    : FCVTKGraphObjectModelBase(modelData)
{
    if (!modelData) return;
    FCAbsGeoShapeAgent* shapeAgent = modelData->getShapeAgent();
    if (!shapeAgent) return;
    init();
    if (!generateDataSet(shapeAgent)) return;
    generateGraph();
}

void FCVTKGraphObjectModelCmd::update(bool forceUpdate)
{
    FCAbsGeoCommand* modelData = dynamic_cast<FCAbsGeoCommand*>(static_cast<FCAbstractDataObject*>(mDataObject));
    if (!modelData) return;
    FCAbsGeoShapeAgent* shapeAgent = modelData->getShapeAgent();
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
