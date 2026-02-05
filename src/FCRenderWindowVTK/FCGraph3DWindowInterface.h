/**
 * @file FCGraph3DWindowInterface.h
 * @brief 三维渲染窗口组件接口
 */
#ifndef FCGRAPH3DWINDOWINTERFACE_H
#define FCGRAPH3DWINDOWINTERFACE_H

#include "FCRenderWindowVTKAPI.h"
#include <functional>
#include <QHash>
#include <QVariant>
#include <QString>

#define VTKRENDERLAYER "VTKRENDERLAYER"
#define BackGroundColorTop "BackGroundColorTop"
#define BackGroundColorButtom "BackGroundColorButtom"

namespace FC
{
class FCGraphInteractionStyle;

enum ResolveCoincidentType
{
    RCT_Off = -1,
    RCT_Default = 0,
    RCT_PolygonOffset,
    RCT_ShiftZBuffer
};

/**
 * @brief 渲染窗口信息
 */
class FCRENDERWINDOWVTK_API FCGraph3DWindowInitializer
{
public:
    explicit FCGraph3DWindowInitializer() = default;
    virtual ~FCGraph3DWindowInitializer() = default;

    virtual FCGraphInteractionStyle* getStyle();
    void setLayerCount(int nc);
    int getLayerCount() const;

    QVariant getValue(const QString& key) const;
    void setValue(const QString& key, const QVariant& value);

private:
    int mLayerCount = 3;
    QHash<QString, QVariant> mParams;
};

/**
 * @brief 三维渲染窗口组件接口
 */
class FCRENDERWINDOWVTK_API FCGraph3DWindowInterface
{
public:
    FCGraph3DWindowInterface() = default;
    virtual ~FCGraph3DWindowInterface();

    static QString CheckSystem();

    virtual QWidget* getWidget(int indexPort);
    virtual QString getComponentName();
    void addInitializer(int key, FCGraph3DWindowInitializer* info);
    static void SetResolveCoincidentType(ResolveCoincidentType type);

private:
    QHash<int, FCGraph3DWindowInitializer*> mInitializerList;
};
} // namespace FC
#endif
