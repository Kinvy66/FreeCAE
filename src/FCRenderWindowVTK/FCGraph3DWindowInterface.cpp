#include "FCGraph3DWindowInterface.h"
#include "FCGraph3DWindowVTK.h"
#include "FCGraphInteractionStyle.h"
#include <vtkMapper.h>
#include <QWidget>
#include <QColor>

namespace FC
{
QString FCGraph3DWindowInterface::CheckSystem()
{
    return QString();
}

FCGraph3DWindowInterface::~FCGraph3DWindowInterface()
{
    QList<FCGraph3DWindowInitializer*> inis = mInitializerList.values();
    for (auto iner : inis)
    {
        if (iner)
            delete iner;
    }
    mInitializerList.clear();
}

QWidget* FCGraph3DWindowInterface::getWidget(int indexPort)
{
    FCGraph3DWindowInitializer* initer = mInitializerList.value(indexPort);
    if (!initer)
        initer = new FCGraph3DWindowInitializer;
    FCGraph3DWindowVTK* w = new FCGraph3DWindowVTK(initer);

    QVariant vtc = initer->getValue(BackGroundColorTop);
    QVariant vbc = initer->getValue(BackGroundColorButtom);
    if (vtc.isValid() && vbc.isValid())
    {
        QColor c = vtc.value<QColor>();
        float t[3] = { c.redF(), c.greenF(), c.blueF() };
        c = vbc.value<QColor>();
        float b[3] = { c.redF(), c.greenF(), c.blueF() };
        if (w)
            w->setBackgroundColor(t, b);
    }
    return w;
}

QString FCGraph3DWindowInterface::getComponentName()
{
    return QStringLiteral("Graph3DWindowVTK");
}

void FCGraph3DWindowInterface::addInitializer(int key, FCGraph3DWindowInitializer* info)
{
    if (mInitializerList.contains(key))
    {
        FCGraph3DWindowInitializer* iner = mInitializerList.value(key);
        if (iner)
            delete iner;
    }
    mInitializerList[key] = info;
}

FCGraphInteractionStyle* FCGraph3DWindowInitializer::getStyle()
{
    return FCGraphInteractionStyle::New();
}

void FCGraph3DWindowInitializer::setLayerCount(int nc)
{
    mLayerCount = nc;
}

int FCGraph3DWindowInitializer::getLayerCount() const
{
    return mLayerCount;
}

QVariant FCGraph3DWindowInitializer::getValue(const QString& key) const
{
    return mParams.value(key);
}

void FCGraph3DWindowInitializer::setValue(const QString& key, const QVariant& value)
{
    mParams[key] = value;
}

void FCGraph3DWindowInterface::SetResolveCoincidentType(ResolveCoincidentType type)
{
    switch (type)
    {
    case RCT_Off:
        vtkMapper::SetResolveCoincidentTopologyToOff();
        break;
    case RCT_Default:
        vtkMapper::SetResolveCoincidentTopologyToDefault();
        break;
    case RCT_PolygonOffset:
        vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();
        break;
    case RCT_ShiftZBuffer:
        vtkMapper::SetResolveCoincidentTopologyToShiftZBuffer();
        break;
    default:
        break;
    }
}
} // namespace FC
