/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCVirtualTopoCreator.h"
#include <FCGeometryInterface/FCVirtualTopoManager.h>
#include <FCGeometryInterface/FCGeoEnum.h>

#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

namespace OCC {

static FC::FCGeoEnum::VTopoShapeType occToFCVTopoType(TopAbs_ShapeEnum occType)
{
    switch (occType)
    {
    case TopAbs_VERTEX:   return FC::FCGeoEnum::VSPoint;
    case TopAbs_EDGE:     return FC::FCGeoEnum::VSEdge;
    case TopAbs_WIRE:     return FC::FCGeoEnum::VSWire;
    case TopAbs_FACE:     return FC::FCGeoEnum::VSFace;
    case TopAbs_SHELL:    return FC::FCGeoEnum::VSShell;
    case TopAbs_SOLID:    return FC::FCGeoEnum::VSSolid;
    case TopAbs_COMPOUND:
    case TopAbs_COMPSOLID: return FC::FCGeoEnum::VSAssembly;
    default:              return FC::FCGeoEnum::VSNone;
    }
}

// ----- FCOCCTopoShape -----
FCOCCTopoShape::FCOCCTopoShape(const TopoDS_Shape& shape)
{
    _shape = new TopoDS_Shape(shape);
}

FCOCCTopoShape::~FCOCCTopoShape()
{
    if (_shape) { delete _shape; _shape = nullptr; }
}

bool FCOCCTopoShape::isSameShape(void* shape)
{
    if (!_shape || !shape) return false;
    TopoDS_Shape* s = static_cast<TopoDS_Shape*>(shape);
    return s->IsEqual(*_shape);
}

const TopoDS_Shape& FCOCCTopoShape::getTopoShape() const
{
    return *_shape;
}

// ----- FCOCCVirtualTopoCreator -----
void FCOCCVirtualTopoCreator::createOCCTopos(const TopoDS_Shape& shape, FC::FCVirtualTopoManager* topoMgr)
{
    if (!topoMgr || shape.IsNull()) return;

    topoMgr->clear();

    TopAbs_ShapeEnum rootType = shape.ShapeType();
    FC::FCGeoEnum::VTopoShapeType rootVType = occToFCVTopoType(rootType);

    FC::FCAbsVirtualTopo* root = new FC::FCAbsVirtualTopo(rootVType);
    root->setShape(new FCOCCTopoShape(shape));
    root->setIndexLabel(0);
    topoMgr->setRootObj(root);
    topoMgr->appendVirtualTopoObj(root);

    static const TopAbs_ShapeEnum subTypes[] = {
        TopAbs_VERTEX, TopAbs_EDGE, TopAbs_WIRE, TopAbs_FACE, TopAbs_SHELL, TopAbs_SOLID
    };
    for (TopAbs_ShapeEnum st : subTypes)
    {
        FC::FCGeoEnum::VTopoShapeType vt = occToFCVTopoType(st);
        if (vt == FC::FCGeoEnum::VSNone) continue;
        TopTools_IndexedMapOfShape map;
        TopExp::MapShapes(shape, st, map);
        for (int i = 1; i <= map.Extent(); ++i)
        {
            const TopoDS_Shape& sub = map.FindKey(i);
            FC::FCAbsVirtualTopo* topo = new FC::FCAbsVirtualTopo(vt);
            topo->setShape(new FCOCCTopoShape(sub));
            topo->setIndexLabel(i);
            topoMgr->appendVirtualTopoObj(topo, vt);
        }
    }
}

} // namespace OCC
