/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 */

#include "FCGraphOperator.h"
#include "FCGraph3DWindowVTK.h"

namespace FC
{
FCGraphOperator::FCGraphOperator() = default;

FCGraphOperator::~FCGraphOperator() = default;

void FCGraphOperator::setGraph3DWindow(FCGraph3DWindowVTK* w)
{
    m_graph3DWindow = w;
}

FCGraph3DWindowVTK* FCGraphOperator::getGraph3DWindow()
{
    return m_graph3DWindow;
}
} // namespace FC
