/**
 * @file FCElementFactory.cpp
 * @brief 单元工厂实现（移植自 FITKInterfaceModel）
 */
#include "FCElementFactory.h"
#include "FCElementLine.h"
#include "FCElementTri.h"
#include "FCElementQuad.h"
#include "FCElementTet.h"
#include "FCElementWedge.h"
#include "FCElementHex.h"
#include "FCElementPolygon.h"

namespace FC {

FCAbstractElement* FCElementFactory::createElement(FCModelEnum::FITKEleType t)
{
    switch (t) {
    case FCModelEnum::Line2: return new FCElementLine2;
    case FCModelEnum::Line3: return new FCElementLine3;
    case FCModelEnum::Tri3: return new FCElementTri3;
    case FCModelEnum::Tri6: return new FCElementTri6;
    case FCModelEnum::Quad4: return new FCElementQuad4;
    case FCModelEnum::Quad8: return new FCElementQuad8;
    case FCModelEnum::Tet4: return new FCElementTet4;
    case FCModelEnum::Tet10: return new FCElementTet10;
    case FCModelEnum::Wedge6: return new FCElementWedge6;
    case FCModelEnum::Hex8: return new FCElementHex8;
    case FCModelEnum::Hex20: return new FCElementHex20;
    case FCModelEnum::Polygon: return new FCElementPolygon;
    default: return nullptr;
    }
}

} // namespace FC
