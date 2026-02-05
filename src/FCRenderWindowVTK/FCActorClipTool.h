/**
 * @file FCActorClipTool.h
 * @brief 支持裁切平面的 Actor
 */
#ifndef FCACTORCLIPTOOL_H
#define FCACTORCLIPTOOL_H

#include "FCRenderWindowVTKAPI.h"
#include <vtkActor.h>
#include <vtkNew.h>

class vtkPlane;
class vtkAlgorithmOutput;

namespace FC
{
enum class ClipType
{
    NoneType = -1,
    Clip = 0,
    Cut,
    ExtractGeometry
};

/**
 * @brief 支持裁切的 Actor，可与 vtkPlane 配合用于裁切显示
 */
class FCRENDERWINDOWVTK_API FCActorClipTool : public vtkActor
{
public:
    static FCActorClipTool* New();
    vtkTypeMacro(FCActorClipTool, vtkActor);

    void setClipType(ClipType type);
    void setClipImplicitFunction(vtkPlane* plane);

protected:
    FCActorClipTool();
    ~FCActorClipTool() override;

private:
    FCActorClipTool(const FCActorClipTool&) = delete;
    void operator=(const FCActorClipTool&) = delete;

    ClipType mClipType = ClipType::NoneType;
    vtkPlane* mClipPlane = nullptr;
};
} // namespace FC
#endif
