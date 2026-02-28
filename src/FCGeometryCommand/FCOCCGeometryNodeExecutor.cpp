/**
 * @file FCOCCGeometryNodeExecutor.cpp
 * @brief OCC 几何节点执行器实现：节点 ID = 命令 ID，从 FCDataRepo 取命令执行
 */
#include "FCOCCGeometryNodeExecutor.h"
#include "FCAbstractOCCModel.h"
#include <FCData/FCDataRepo.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCAbsGeoModelCylinder.h>
#include <FCGeometryInterface/FCAbsGeoModelSphere.h>
#include <FCGeometryInterface/FCAbsGeoOperNBodyUnite.h>
#include <FCGeometryInterface/FCAbsGeoOperBool.h>
#include <TopoDS_Shape.hxx>
#include <QMetaType>

namespace OCC {

Q_DECLARE_METATYPE(TopoDS_Shape)

namespace {
struct RegisterTopoDSShapeMetaType {
    RegisterTopoDSShapeMetaType() { qRegisterMetaType<TopoDS_Shape>(); }
} s_registerTopoDSShape;
}

static TopoDS_Shape shapeFromNodeResult(const QVariant& v)
{
    if (!v.isValid()) return TopoDS_Shape();
    if (v.userType() == qMetaTypeId<TopoDS_Shape>())
        return v.value<TopoDS_Shape>();
    return TopoDS_Shape();
}

static QVariant variantFromShape(const TopoDS_Shape& s)
{
    if (s.IsNull()) return QVariant();
    return QVariant::fromValue(s);
}

bool FCOCCGeometryNodeExecutor::supports(FC::FCGeoOpType type) const
{
    switch (type) {
    case FC::FCGeoOpType::Block:
    case FC::FCGeoOpType::Cylinder:
    case FC::FCGeoOpType::Sphere:
    case FC::FCGeoOpType::Cone:
    case FC::FCGeoOpType::Torus:
    case FC::FCGeoOpType::Helix:
    case FC::FCGeoOpType::Union:
    case FC::FCGeoOpType::Difference:
    case FC::FCGeoOpType::Intersection:
        return true;
    default:
        return false;
    }
}

QVariant FCOCCGeometryNodeExecutor::execute(const FC::FCGeoNode& node,
                                            const QHash<FCID, QVariant>& nodeResults,
                                            FC::FCSelectionRule*) const
{
    FC::FCDataRepo* repo = FC::FCDataRepo::instance();
    FC::FCAbsGeoCommand* cmd = repo->getDataAs<FC::FCAbsGeoCommand>(node.id);
    if (!cmd) return QVariant();

    FCAbstractOCCModel* occModel = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occModel) return QVariant();

    switch (node.type) {
    case FC::FCGeoOpType::Block:
    case FC::FCGeoOpType::Cylinder:
    case FC::FCGeoOpType::Sphere:
    case FC::FCGeoOpType::Cone:
    case FC::FCGeoOpType::Torus:
    case FC::FCGeoOpType::Helix: {
        if (!cmd->update()) return QVariant();
        TopoDS_Shape* sh = occModel->getShape();
        if (!sh || sh->IsNull()) return QVariant();
        return variantFromShape(*sh);
    }
    case FC::FCGeoOpType::Union: {
        FC::FCGeoOperNBodyUnite* uniteCmd = dynamic_cast<FC::FCGeoOperNBodyUnite*>(cmd);
        if (!uniteCmd || node.inputs.size() < 2) return QVariant();
        uniteCmd->clearShapes();
        for (int inId : node.inputs) {
            if (nodeResults.contains(inId))
                uniteCmd->addShape(FC::VirtualShape(inId, -1, 0));
        }
        if (!cmd->update()) return QVariant();
        TopoDS_Shape* sh = occModel->getShape();
        if (!sh || sh->IsNull()) return QVariant();
        return variantFromShape(*sh);
    }
    case FC::FCGeoOpType::Difference:
    case FC::FCGeoOpType::Intersection: {
        FC::FCGeoOperBool* boolCmd = dynamic_cast<FC::FCGeoOperBool*>(cmd);
        if (!boolCmd || node.inputs.size() < 2) return QVariant();
        boolCmd->setTarget(FC::VirtualShape(node.inputs[0], -1, 0));
        boolCmd->setTool(FC::VirtualShape(node.inputs[1], -1, 0));
        if (!cmd->update()) return QVariant();
        TopoDS_Shape* sh = occModel->getShape();
        if (!sh || sh->IsNull()) return QVariant();
        return variantFromShape(*sh);
    }
    default:
        return QVariant();
    }
}

} // namespace OCC
