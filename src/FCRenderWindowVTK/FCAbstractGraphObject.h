/**
 * @file FCAbstractGraphObject.h
 * @brief 渲染对象抽象基类（仅依赖 Qt，与 FITKAbstractGraphObject 接口一致）
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
    FCGraph3DWindowVTK* m_graphWidget = nullptr;
    void* m_dataObject = nullptr;
};
} // namespace FC
#endif
