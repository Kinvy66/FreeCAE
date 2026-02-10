/**
 * @file FCVTKGraphObjectMeasureTools.h
 * @brief 测量工具图元：距离/角度（移植自 FITKFluidVTKGraphObjectMeasureTools）
 */
#ifndef FCVTKGRAPHOBJECTMEASURETOOLS_H
#define FCVTKGRAPHOBJECTMEASURETOOLS_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKCommons.h"
#include "FCVTKGraphObject3D.h"

class vtkAbstractWidget;
class vtkDistanceWidget;
class vtkAngleWidget;

namespace FC {

class FCVTKGRAPHADAPTOR_API FCVTKGraphObjectMeasureTools : public FCVTKGraphObject3D
{
public:
    FCVTKGraphObjectMeasureTools();
    ~FCVTKGraphObjectMeasureTools() override;

    void setVisible(bool visibility) override;
    void setMeasureType(FCVTKCommons::MeasureType type);
    /** 设置测量控件点的显示坐标。Distance: index 0=起点 1=终点；Angle: 0=起点 1=圆心 2=终点 */
    void setDisplayPosition(int index, double* pos);

private:
    void init();

    vtkDistanceWidget* m_distWidget = nullptr;
    vtkAngleWidget* m_angleWidget = nullptr;
    vtkAbstractWidget* m_currentWidget = nullptr;
    bool m_measureVisible = true;
};

} // namespace FC

#endif // FCVTKGRAPHOBJECTMEASURETOOLS_H
