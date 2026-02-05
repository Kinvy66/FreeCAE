/**
 * @file FCGraphRender.h
 * @brief 渲染场景接口声明
 */
#ifndef FCGRAPHRENDER_H
#define FCGRAPHRENDER_H

#include "FCRenderWindowVTKAPI.h"
#include <QObject>
#include <QMutex>

class vtkRenderer;
class vtkRenderWindow;

namespace FC
{
class FCGraphObjectVTK;
class FCGraph3DWindowVTK;
class FCGraphObjManager;

class FCRENDERWINDOWVTK_API FCGraphRender : public QObject
{
    Q_OBJECT
public:
    FCGraphRender(FCGraph3DWindowVTK* graphWin, vtkRenderer* ren = nullptr);
    ~FCGraphRender() override;

    void addObject(FCGraphObjectVTK* object);
    void removeObject(FCGraphObjectVTK* object);
    vtkRenderer* getRenderer();
    FCGraphObjManager* getGraphObjManager();
    double getActorBounds(double* bound);
    int getGraphObjectCount();
    void clear();
    void setBackgroundColor(float* rgb1, float* rgb2 = nullptr);

private:
    FCGraph3DWindowVTK* mGraphWidget = nullptr;
    vtkRenderer* mRenderer = nullptr;
    FCGraphObjManager* mObjectManager = nullptr;
};
} // namespace FC
#endif
