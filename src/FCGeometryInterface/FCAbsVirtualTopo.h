/**
 * @file FCAbsVirtualTopo.h
 * @brief 几何虚拓扑接口（移植自 FITKInterfaceGeometry）
 */
#ifndef FCABSVIRTUALTOPO_H
#define FCABSVIRTUALTOPO_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoEnum.h"
#include <FCData/FCAbstractDataObject.h>
#include <FCData/FCMacros.h>
#include <QList>

namespace FC {

class FCGEOMETRYINTERFACE_API FCAbsVirtualTopoShape
{
public:
    explicit FCAbsVirtualTopoShape() = default;
    virtual ~FCAbsVirtualTopoShape() = 0;
    virtual bool isSameShape(void* shape) = 0;
};

inline FCAbsVirtualTopoShape::~FCAbsVirtualTopoShape() {}

class FCGEOMETRYINTERFACE_API FCAbsVirtualTopo : public FCAbstractDataObject
{
    FC_CLASS(FC, FCAbsVirtualTopo);
    Q_OBJECT
public:
    explicit FCAbsVirtualTopo(FCGeoEnum::VTopoShapeType type = FCGeoEnum::VSNone);
    ~FCAbsVirtualTopo() override;

    FCGeoEnum::VTopoShapeType getShapeType() const;
    void setShapeType(FCGeoEnum::VTopoShapeType s);
    void addSubTopo(FCAbsVirtualTopo* subTopo);
    void removeSubTopo(FCAbsVirtualTopo* subTopo);
    int getSubTopoCount();
    FCAbsVirtualTopo* getSubTopo(int index);
    int getParentTopoCount();
    FCAbsVirtualTopo* getParentTopo(int index);
    void clearParentTopo();
    void setShape(FCAbsVirtualTopoShape* shape);
    FCAbsVirtualTopoShape* getShape();
    template<class T> T* getShapeT() { return dynamic_cast<T*>(_shape); }
    void setIndexLabel(int index);
    int getIndexLabel() const;
    void setTag(int tag);
    int getTag();

protected:
    FCGeoEnum::VTopoShapeType _shapeType{ FCGeoEnum::VSNone };
    FCAbsVirtualTopoShape* _shape{};
    int _indexLabel{ -1 };
    int _tag{ 0 };
    QList<FCAbsVirtualTopo*> _subTopos;
    QList<FCAbsVirtualTopo*> _parentTopos;
};

} // namespace FC

#endif // FCABSVIRTUALTOPO_H
