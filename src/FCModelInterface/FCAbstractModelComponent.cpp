/**
 * @file FCAbstractModelComponent.cpp
 * @brief 模型组件实现（移植自 FITKInterfaceModel）
 */
#include "FCAbstractModelComponent.h"
#include "FCAbstractModel.h"
#include <FCData/FCDataRepo.h>

namespace FC {

static QColor colorFromIndex(unsigned int index)
{
    static const QColor palette[] = {
        QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255),
        QColor(255, 255, 0), QColor(255, 0, 255), QColor(0, 255, 255),
        QColor(128, 0, 0), QColor(0, 128, 0), QColor(0, 0, 128),
        QColor(128, 128, 0), QColor(128, 0, 128), QColor(0, 128, 128),
    };
    return palette[index % (sizeof(palette) / sizeof(palette[0]))];
}

FCAbstractModelComponent::FCAbstractModelComponent()
{
    static unsigned int instanceCount = 0;
    _color = colorFromIndex(instanceCount);
    instanceCount++;
}

FCAbstractModelComponent::~FCAbstractModelComponent()
{}

int FCAbstractModelComponent::getAbsModelID() const
{
    return _absModelID;
}

void FCAbstractModelComponent::setModel(const int& parentID)
{
    auto* model = FCDataRepo::instance()->getDataAs<FCAbstractModel>(parentID);
    if (!model) return;
    _absModelID = parentID;
}

void FCAbstractModelComponent::setModel(FCAbstractModel* model)
{
    if (!model) return;
    _absModelID = model->getDataObjectID();
}

FCAbstractModel* FCAbstractModelComponent::getAbstractModel()
{
    return FCDataRepo::instance()->getDataAs<FCAbstractModel>(_absModelID);
}

void FCAbstractModelComponent::setColor(const QColor& color)
{
    _color = color;
}

QColor FCAbstractModelComponent::getColor() const
{
    return _color;
}

} // namespace FC
