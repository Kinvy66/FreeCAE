/**
 * @file FCAbstractGraphObject.h
 * @brief 渲染对象抽象基类
 */
#ifndef FCABSTRACTGRAPEOBJECT_H
#define FCABSTRACTGRAPEOBJECT_H

#include "FCRenderWindowVTKAPI.h"
#include <QObject>

namespace FC
{
class FCGraph3DWindowVTK;

/**
 * @brief 渲染对象抽象基类
 */
class FCRENDERWINDOWVTK_API FCAbstractGraphObject : public QObject
{
    Q_OBJECT
public:
    explicit FCAbstractGraphObject(void* dataObject = nullptr);
    virtual ~FCAbstractGraphObject();

    void setGraphWidget(FCGraph3DWindowVTK* w);
    FCGraph3DWindowVTK* getGraphWidget() const;
    virtual void removeFromGraphWidget();

protected:
    FCGraph3DWindowVTK* mGraphWidget = nullptr;
    void* mDataObject = nullptr;
};
} // namespace FC
#endif
