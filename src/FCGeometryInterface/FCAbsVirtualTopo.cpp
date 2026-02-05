/**
 * @file FCAbsVirtualTopo.cpp
 * @brief 几何虚拓扑实现（移植自 FITKInterfaceGeometry）
 */
#include "FCAbsVirtualTopo.h"

namespace FC {

FCAbsVirtualTopo::FCAbsVirtualTopo(FCGeoEnum::VTopoShapeType type)
    : FCAbstractDataObject(-1)
    , _shapeType(type)
{}

FCAbsVirtualTopo::~FCAbsVirtualTopo()
{}

FCGeoEnum::VTopoShapeType FCAbsVirtualTopo::getShapeType() const
{
    return _shapeType;
}

void FCAbsVirtualTopo::setShapeType(FCGeoEnum::VTopoShapeType s)
{
    _shapeType = s;
}

void FCAbsVirtualTopo::addSubTopo(FCAbsVirtualTopo* subTopo)
{
    if (subTopo && !_subTopos.contains(subTopo)) {
        _subTopos.append(subTopo);
        subTopo->_parentTopos.append(this);
    }
}

void FCAbsVirtualTopo::removeSubTopo(FCAbsVirtualTopo* subTopo)
{
    if (!subTopo) return;
    _subTopos.removeOne(subTopo);
    subTopo->_parentTopos.removeOne(this);
}

int FCAbsVirtualTopo::getSubTopoCount()
{
    return _subTopos.size();
}

FCAbsVirtualTopo* FCAbsVirtualTopo::getSubTopo(int index)
{
    if (index < 0 || index >= _subTopos.size()) return nullptr;
    return _subTopos.at(index);
}

int FCAbsVirtualTopo::getParentTopoCount()
{
    return _parentTopos.size();
}

FCAbsVirtualTopo* FCAbsVirtualTopo::getParentTopo(int index)
{
    if (index < 0 || index >= _parentTopos.size()) return nullptr;
    return _parentTopos.at(index);
}

void FCAbsVirtualTopo::clearParentTopo()
{
    _parentTopos.clear();
}

void FCAbsVirtualTopo::setShape(FCAbsVirtualTopoShape* shape)
{
    _shape = shape;
}

FCAbsVirtualTopoShape* FCAbsVirtualTopo::getShape()
{
    return _shape;
}

void FCAbsVirtualTopo::setIndexLabel(int index)
{
    _indexLabel = index;
}

int FCAbsVirtualTopo::getIndexLabel() const
{
    return _indexLabel;
}

void FCAbsVirtualTopo::setTag(int tag)
{
    _tag = tag;
}

int FCAbsVirtualTopo::getTag()
{
    return _tag;
}

} // namespace FC
