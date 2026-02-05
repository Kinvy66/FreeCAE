/**
 * @file FCAbstractGraph3DWidget.h
 * @brief 三维图形窗口抽象基类
 */
#ifndef FCABSTRACTGRAPH3DWIDGET_H
#define FCABSTRACTGRAPH3DWIDGET_H

#include "FCRenderWindowVTKAPI.h"
#include <QWidget>

namespace FC
{
class FCAbstractGraphObject;

class FCRENDERWINDOWVTK_API FCAbstractGraph3DWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FCAbstractGraph3DWidget(QWidget* parent = nullptr);
    ~FCAbstractGraph3DWidget() override;

    virtual void setView(double* focalPoint, double* viewUp, double* position) = 0;
    virtual bool getView(double* pos, double* focal, double* viewup) = 0;
    virtual void setViewBounds(double* bds) = 0;
    virtual void clear() = 0;
    virtual void fitView() = 0;
    virtual void flush() = 0;
    virtual void setBackgroundColor(float* rgb1, float* rgb2 = nullptr) = 0;
    virtual void getBackgroundColor(float* rgb1, float* rgb2 = nullptr) = 0;
    virtual void removeGraphObj(FCAbstractGraphObject* gobj) = 0;
    virtual void saveImage(const QString& image, int w = -1, int h = -1) = 0;
    virtual void setParallelProjection(bool on = true) = 0;
    virtual bool getParallelProjection() = 0;

    int getGraphWidgetID() const { return m_graphWidgetID; }
    void setGraphWidgetID(int id) { m_graphWidgetID = id; }

private:
    int m_graphWidgetID = 0;
};
} // namespace FC
#endif
