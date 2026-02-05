#include "FCGraphRender.h"
#include "FCGraphObjectVTK.h"
#include "FCGraph3DWindowVTK.h"
#include "FCGraphObjManager.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorObserver.h>
#include <vtkCubeAxesActor.h>
#include <vtkCamera.h>
#include <cmath>

namespace FC
{
FCGraphRender::FCGraphRender(FCGraph3DWindowVTK* gw, vtkRenderer* ren)
    : mGraphWidget(gw)
{
    if (!mGraphWidget)
        return;
    mObjectManager = new FCGraphObjManager();

    if (ren)
        mRenderer = ren;
    else
        mRenderer = vtkRenderer::New();

    mRenderer->SetBackground2(250/255.0, 251/255.0, 254/255.0);
    mRenderer->SetBackground(230/255.0, 242/255.0, 255/255.0);
    mRenderer->SetGradientBackground(true);
    mRenderer->InteractiveOn();

    vtkRenderWindow* renderWin = mGraphWidget->getVTKRenderWindow();
    if (renderWin)
        renderWin->AddRenderer(mRenderer);
}

FCGraphRender::~FCGraphRender()
{
    clear();
    if (mObjectManager)
        delete mObjectManager;
    if (mRenderer)
        mRenderer->Delete();
}

void FCGraphRender::addObject(FCGraphObjectVTK* object)
{
    if (!object)
        return;
    mObjectManager->appendGraphObj(object);
    object->setGraphWidget(mGraphWidget);

    const int nact = object->getActorCount();
    if (!mRenderer)
        return;

    vtkCamera* c = mRenderer->GetActiveCamera();
    for (int i = 0; i < nact; i++)
    {
        vtkProp* act = object->getActor(i);
        if (!act)
            continue;
        vtkCubeAxesActor* cubeAxes = vtkCubeAxesActor::SafeDownCast(act);
        if (cubeAxes)
            cubeAxes->SetCamera(c);
        mRenderer->AddViewProp(act);
    }
}

void FCGraphRender::removeObject(FCGraphObjectVTK* gobj)
{
    if (!gobj || !mObjectManager->isContains(gobj))
        return;
    gobj->setGraphWidget(nullptr);
    const int nact = gobj->getActorCount();
    if (mRenderer)
    {
        for (int i = 0; i < nact; i++)
            mRenderer->RemoveViewProp(gobj->getActor(i));
        int nWidget = gobj->getWidgetCount();
        for (int i = 0; i < nWidget; i++)
        {
            vtkInteractorObserver* widget = gobj->getWidget(i);
            if (widget)
            {
                widget->SetDefaultRenderer(nullptr);
                widget->SetInteractor(nullptr);
                widget->SetEnabled(false);
            }
        }
    }
    mObjectManager->removeGraphObj(gobj);
}

vtkRenderer* FCGraphRender::getRenderer()
{
    return mRenderer;
}

FCGraphObjManager* FCGraphRender::getGraphObjManager()
{
    return mObjectManager;
}

double FCGraphRender::getActorBounds(double* bound)
{
    bool isEmpty = true;
    const int ngobj = mObjectManager->getGraphObjCount();
    for (int i = 0; i < ngobj; ++i)
    {
        FCGraphObjectVTK* object = mObjectManager->getGraphObjTAt<FCGraphObjectVTK>(i);
        if (!object)
            continue;
        double b[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
        if (!object->getFixedBounds(b))
            continue;
        if (b[0] < bound[0]) bound[0] = b[0];
        if (b[1] > bound[1]) bound[1] = b[1];
        if (b[2] < bound[2]) bound[2] = b[2];
        if (b[3] > bound[3]) bound[3] = b[3];
        if (b[4] < bound[4]) bound[4] = b[4];
        if (b[5] > bound[5]) bound[5] = b[5];
        isEmpty = false;
    }
    if (isEmpty)
        return -1;
    double dx2 = (bound[1] - bound[0]) * (bound[1] - bound[0]);
    double dy2 = (bound[3] - bound[2]) * (bound[3] - bound[2]);
    double dz2 = (bound[5] - bound[4]) * (bound[5] - bound[4]);
    return std::sqrt(dx2 + dy2 + dz2);
}

int FCGraphRender::getGraphObjectCount()
{
    return mObjectManager ? mObjectManager->getGraphObjCount() : 0;
}

void FCGraphRender::clear()
{
    const int nActor = mObjectManager->getGraphObjCount();
    for (int i = 0; i < nActor; ++i)
    {
        FCGraphObjectVTK* gobj = mObjectManager->getGraphObjTAt<FCGraphObjectVTK>(i);
        if (!gobj)
            continue;
        gobj->setGraphWidget(nullptr);
        const int nact = gobj->getActorCount();
        if (mRenderer)
        {
            for (int j = 0; j < nact; j++)
                mRenderer->RemoveViewProp(gobj->getActor(j));
        }
    }
    mObjectManager->clear();
}

void FCGraphRender::setBackgroundColor(float* rgb1, float* rgb2)
{
    if (!rgb1 || !mRenderer)
        return;
    if (!rgb2)
        rgb2 = rgb1;
    mRenderer->SetGradientBackground(true);
    mRenderer->SetBackground2(rgb1[0], rgb1[1], rgb1[2]);
    mRenderer->SetBackground(rgb2[0], rgb2[1], rgb2[2]);
}
} // namespace FC
