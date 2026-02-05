/**
 * @file FCAbstractModelComponent.h
 * @brief 模型组件抽象类（移植自 FITKInterfaceModel）
 */
#ifndef FCABSTRACTMODELCOMPONENT_H
#define FCABSTRACTMODELCOMPONENT_H

#include "FCModelInterfaceAPI.h"
#include "FCModelEnum.h"
#include <FCData/FCAbstractNamedDataObject.h>
#include <QColor>

namespace FC {

class FCAbstractModel;

class FCMODELINTERFACE_API FCAbstractModelComponent : public FCAbstractNamedDataObject
{
public:
    explicit FCAbstractModelComponent();
    virtual ~FCAbstractModelComponent() = 0;

    void setModel(const int& parentID);
    void setModel(FCAbstractModel* model);
    int getAbsModelID() const;
    FCAbstractModel* getAbstractModel();

    template<class T>
    T* getAbstractModelAs()
    {
        return dynamic_cast<T*>(getAbstractModel());
    }

    void setColor(const QColor& color);
    QColor getColor() const;

private:
    int _absModelID{ -1 };
    QColor _color{ Qt::black };
};

} // namespace FC

#endif // FCABSTRACTMODELCOMPONENT_H
