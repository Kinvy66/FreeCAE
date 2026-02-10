/**
 * @file FCVTKGraphObjectMeasureTools.cpp
 * @brief 测量工具图元实现：vtkDistanceWidget、vtkAngleWidget
 */
#include "FCVTKGraphObjectMeasureTools.h"
#include <vtkDistanceWidget.h>
#include <vtkAngleWidget.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkAbstractWidget.h>
#include <vtkSmartPointer.h>

namespace FC {

FCVTKGraphObjectMeasureTools::FCVTKGraphObjectMeasureTools()
    : FCVTKGraphObject3D(nullptr)
{
    init();
    setRenderLayer(1);
}

FCVTKGraphObjectMeasureTools::~FCVTKGraphObjectMeasureTools()
{
    m_currentWidget = nullptr;
}

void FCVTKGraphObjectMeasureTools::init()
{
    m_distWidget = vtkDistanceWidget::New();
    vtkSmartPointer<vtkDistanceRepresentation2D> distRep2D = vtkSmartPointer<vtkDistanceRepresentation2D>::New();
    distRep2D->SetLabelFormat("L = %-#6.3g");
    m_distWidget->SetRepresentation(distRep2D);

    m_angleWidget = vtkAngleWidget::New();
    vtkSmartPointer<vtkAngleRepresentation2D> angleRep2D = vtkSmartPointer<vtkAngleRepresentation2D>::New();
    angleRep2D->SetLabelFormat("Deg = %-#3.3g");
    m_angleWidget->SetRepresentation(angleRep2D);

    m_distWidget->EnabledOff();
    m_angleWidget->EnabledOff();

    addWidget(m_distWidget);
    addWidget(m_angleWidget);
}

void FCVTKGraphObjectMeasureTools::setVisible(bool visibility)
{
    m_measureVisible = visibility;
    if (m_currentWidget)
        m_currentWidget->SetEnabled(visibility);
}

void FCVTKGraphObjectMeasureTools::setMeasureType(FCVTKCommons::MeasureType type)
{
    m_distWidget->EnabledOff();
    m_angleWidget->EnabledOff();
    switch (type)
    {
    case FCVTKCommons::MT_Distance:
        m_currentWidget = m_distWidget;
        break;
    case FCVTKCommons::MT_Angle:
        m_currentWidget = m_angleWidget;
        break;
    case FCVTKCommons::MT_None:
    default:
        m_currentWidget = nullptr;
        break;
    }
    if (m_currentWidget)
        m_currentWidget->SetEnabled(m_measureVisible);
}

void FCVTKGraphObjectMeasureTools::setDisplayPosition(int index, double* pos)
{
    if (index < 0 || index > 2 || !m_currentWidget || !pos)
        return;
    if (m_currentWidget == m_distWidget)
    {
        vtkDistanceRepresentation2D* rep = vtkDistanceRepresentation2D::SafeDownCast(m_distWidget->GetDistanceRepresentation());
        if (rep)
        {
            if (index == 0)
                rep->SetPoint1DisplayPosition(pos);
            else if (index == 1)
                rep->SetPoint2DisplayPosition(pos);
        }
    }
    else if (m_currentWidget == m_angleWidget)
    {
        vtkAngleRepresentation2D* rep = vtkAngleRepresentation2D::SafeDownCast(m_angleWidget->GetAngleRepresentation());
        if (rep)
        {
            if (index == 0)
                rep->SetPoint1DisplayPosition(pos);
            else if (index == 1)
                rep->SetCenterDisplayPosition(pos);
            else if (index == 2)
                rep->SetPoint2DisplayPosition(pos);
        }
    }
}

} // namespace FC
