 /**
 * @file FCGraphAreaPicker.h
 * @brief VTK 框选功能接口声明
 */
#ifndef FCGRAPHAREAPICKER_H
#define FCGRAPHAREAPICKER_H

#include "FCRenderWindowVTKAPI.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkActor2D.h>

class vtkRenderer;

namespace FC
{
class FCGraphInteractionStyle;

class FCRENDERWINDOWVTK_API FCGraphAreaPicker
{
public:
    explicit FCGraphAreaPicker(FCGraphInteractionStyle* style);
    ~FCGraphAreaPicker();

    void setLeftButtonDownPos(int* pos);
    void setPickerRender(vtkRenderer* render);
    void enable(bool state = true);
    bool isEnable();
    void drawRectangle();
    void pick();
    void setColor(double rf, double gf, double bf);

private:
    void initRectangle();
    void updateRectangle();

    FCGraphInteractionStyle* mStyle = nullptr;
    vtkRenderer* mRender = nullptr;
    int mStartPos[2] = { 0, 0 };
    int mEndPos[2] = { 0, 0 };
    vtkSmartPointer<vtkPolyData> mPickData;
    vtkSmartPointer<vtkActor2D> mPickActor;
    double mColor3[3] = { 1., 1., 1. };
};
} // namespace FC
#endif
