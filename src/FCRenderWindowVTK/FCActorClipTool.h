/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCActorClipTool.h
 * @brief 支持裁切平面的 Actor（仅 VTK，与 FITKActorClipTool 接口一致）
 */
#ifndef FCACTORCLIPTOOL_H
#define FCACTORCLIPTOOL_H

#include "FCRenderWindowVTKAPI.h"
#include <vtkActor.h>
#include <vtkNew.h>

class vtkPlane;
class vtkAlgorithmOutput;

namespace Comp
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

    ClipType m_clipType = ClipType::NoneType;
    vtkPlane* m_clipPlane = nullptr;
};
} // namespace Comp
#endif
