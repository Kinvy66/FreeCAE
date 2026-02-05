/**
 * @file FCGraphObjectVTK.h
 * @brief 渲染对象接口声明
 */
#ifndef FCGRAPHOBJECTVTK_H
#define FCGRAPHOBJECTVTK_H

#include "FCRenderWindowVTKAPI.h"
#include "FCAbstractGraphObject.h"
#include <QObject>

class vtkProp;
class vtkPlane;
class vtkInteractorObserver;

namespace FC
{
/**
 * @brief 渲染对象接口声明
 */
class FCRENDERWINDOWVTK_API FCGraphObjectVTK : public FCAbstractGraphObject
{
    Q_OBJECT
public:
    enum GraphClipType
    {
        NoneType = -1,
        Clip = 0,
        Cut,
        ExtractGeometry
    };

    explicit FCGraphObjectVTK(void* dataObject = nullptr);
    ~FCGraphObjectVTK() override;

    virtual void setClipType(GraphClipType type);
    virtual void setCustomClipPlane(double* org, double* nor);

    virtual void addActor(vtkProp* actor);
    vtkProp* getActor(int index);
    virtual int getActorCount();

    virtual void addWidget(vtkInteractorObserver* widget);
    vtkInteractorObserver* getWidget(int index);
    int getWidgetCount();

    bool hasFixedBounds();
    virtual bool getFixedBounds(double* bounds);
    bool getActorBounds(vtkProp* prop, double* bounds, bool ignoreVisibility = false);
    bool getActorsBounds(QList<vtkProp*> props, double* bounds);

protected:
    QList<vtkProp*> mActorList;
    QList<vtkInteractorObserver*> mWidgetList;
    bool mHasFixedBounds = true;
    double mPolygonOffset = 0.;
    vtkPlane* mPlaneFunction = nullptr;
    bool mIsOpenClip = false;
    vtkPlane* mClipPlane = nullptr;
};
} // namespace FC
#endif
