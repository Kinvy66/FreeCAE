/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCGraphOperator.h
 * @brief 绘图控制器接口声明（仅 VTK+Qt，与 FITKGraphOperator 接口一致）
 */
#ifndef FCGRAPHOPERATOR_H
#define FCGRAPHOPERATOR_H

#include "FCRenderWindowVTKAPI.h"

namespace FC
{
class FCGraph3DWindowVTK;

class FCRENDERWINDOWVTK_API FCGraphOperator
{
public:
    FCGraphOperator();
    virtual ~FCGraphOperator() = 0;

    void setGraph3DWindow(FCGraph3DWindowVTK* w);
    FCGraph3DWindowVTK* getGraph3DWindow();

protected:
    FCGraph3DWindowVTK* m_graph3DWindow = nullptr;
};
} // namespace FC
#endif
