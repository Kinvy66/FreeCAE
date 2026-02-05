/**
 * @file FCGraphOperator.h
 * @brief 绘图控制器接口声明
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
    FCGraph3DWindowVTK* mGraph3DWindow = nullptr;
};
} // namespace FC
#endif
