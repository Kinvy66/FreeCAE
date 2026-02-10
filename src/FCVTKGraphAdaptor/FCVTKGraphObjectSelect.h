/**
 * @file FCVTKGraphObjectSelect.h
 * @brief 高亮显示部分几何/网格的图元（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKGraphObjectSelect）
 */
#ifndef FCVTKGRAPHOBJECTSELECT_H
#define FCVTKGRAPHOBJECTSELECT_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKGraphObject3D.h"
#include "FCVTKCommons.h"
#include <QVector>

class vtkDataSet;
class vtkSelectionNode;
class vtkSelection;
class vtkIdTypeArray;
class vtkExtractSelection;
class vtkProp;

namespace FC {

class FCVTKGRAPHADAPTOR_API FCVTKGraphObjectSelect : public FCVTKGraphObject3D
{
public:
    FCVTKGraphObjectSelect();
    ~FCVTKGraphObjectSelect() override;

    void setVisible(bool visibility) override;
    void setTransparent(bool isOn) override;
    void setViewMode(FCVTKCommons::ShapeMeshViewMode type, bool visible) override;
    void setSelectData(vtkDataSet* grid, QVector<int>& indice, FCVTKCommons::ShapeType type);
    void setColor(QColor color) override;
    void clearData();

private:
    void init();

    vtkProp* m_fActor = nullptr;
    vtkSelectionNode* m_selectNode = nullptr;
    vtkSelection* m_section = nullptr;
    vtkExtractSelection* m_extractSelection = nullptr;
    vtkIdTypeArray* m_idArray = nullptr;
};

} // namespace FC

#endif // FCVTKGRAPHOBJECTSELECT_H
