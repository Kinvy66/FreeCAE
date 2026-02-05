/**
 * @file FCGraphInteractionStyle.h
 * @brief 渲染窗口交互器类接口声明
 */
#ifndef FCGRAPHINTERACTIONSTYLE_H
#define FCGRAPHINTERACTIONSTYLE_H

#include "FCRenderWindowVTKAPI.h"
#include <QObject>
#include <vtkInteractorStyleRubberBandPick.h>

class vtkRenderer;
class vtkRenderWindowInteractor;

namespace FC
{
class FCGraphOperator;
class FCGraph3DWindowVTK;
class FCGraphAreaPicker;

class FCRENDERWINDOWVTK_API FCGraphInteractionStyle : public QObject, public vtkInteractorStyleRubberBandPick
{
    Q_OBJECT
public:
    static FCGraphInteractionStyle* New();
    vtkTypeMacro(FCGraphInteractionStyle, vtkInteractorStyleRubberBandPick);

    void setGraph3DWin(FCGraph3DWindowVTK* graphWin);
    void setInteractor(vtkRenderWindowInteractor* interactor);
    void setAreaPickRenderer(vtkRenderer* renderer);
    vtkRenderWindowInteractor* getInteractor();
    void setAredPickerState(bool state);
    virtual void areaPick(int* startPos, int* endPos);
    virtual void applyDolly(double factor);

protected:
    FCGraphInteractionStyle();
    ~FCGraphInteractionStyle() override;

    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMiddleButtonDown() override;
    void OnMiddleButtonUp() override;
    void OnMouseMove() override;
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnChar() override;
    bool isMouseMoved();

    FCGraph3DWindowVTK* mGraph3DWin = nullptr;
    FCGraphOperator* mOperactor = nullptr;
    int mLeftButtonDownPos[2] = { 0, 0 };
    int mLeftButtonUpPos[2] = { 0, 0 };
    FCGraphAreaPicker* mAreaPick = nullptr;
    bool mLeftButtonPressed = false;
};
} // namespace FC
#endif
