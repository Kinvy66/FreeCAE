/**
 * @file FCGeometryModule.cpp
 * @brief 函数式几何模块实现（COMSOL 式：节点 ID = FCDataRepo 中几何命令 ID）
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryModule.h"
#include "FCGeoOpType.h"
#include <FCData/FCDataRepo.h>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCAbsGeoModelCylinder.h>
#include <FCGeometryInterface/FCAbsGeoModelSphere.h>
#include <FCGeometryInterface/FCAbsGeoOperNBodyUnite.h>
#include <FCGeometryInterface/FCAbsGeoOperBool.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <QScopedPointer>

namespace FC {

static FCGeoEnum::FCGeometryComType toGeometryComType(FCGeoOpType type)
{
    switch (type) {
    case FCGeoOpType::Block:       return FCGeoEnum::FGTBox;
    case FCGeoOpType::Cylinder:    return FCGeoEnum::FGTCylinder;
    case FCGeoOpType::Sphere:      return FCGeoEnum::FGTSphere;
    case FCGeoOpType::Union:       return FCGeoEnum::FGTNBodyUnite;
    case FCGeoOpType::Difference: return FCGeoEnum::FGTBool;
    case FCGeoOpType::Intersection: return FCGeoEnum::FGTBool;
    case FCGeoOpType::Fillet:      return FCGeoEnum::FGTFilletSolid;
    case FCGeoOpType::Chamfer:     return FCGeoEnum::FGTChamferSolid;
    case FCGeoOpType::Import:      return FCGeoEnum::FGTImport;
    default:                       return FCGeoEnum::FGTNone;
    }
}

static void applyParamsToCommand(FCAbsGeoCommand* cmd, FCGeoOpType type, const FCGeoParamSet& params)
{
    if (!cmd) return;
    auto v = [&params](const QString& key, double def = 0) {
        return params.value(key, def).toDouble();
    };
    if (type == FCGeoOpType::Block) {
        FCGeoModelBox* box = dynamic_cast<FCGeoModelBox*>(cmd);
        if (box) {
            double p1[3] = { v(QStringLiteral("point1_x")), v(QStringLiteral("point1_y")), v(QStringLiteral("point1_z")) };
            double len[3] = { v(QStringLiteral("length_x"), 1), v(QStringLiteral("length_y"), 1), v(QStringLiteral("length_z"), 1) };
            box->setPoint1(p1);
            box->setLength(len);
        }
    } else if (type == FCGeoOpType::Cylinder) {
        FCGeoModelCylinder* cyl = dynamic_cast<FCGeoModelCylinder*>(cmd);
        if (cyl) {
            double loc[3] = { v(QStringLiteral("location_x")), v(QStringLiteral("location_y")), v(QStringLiteral("location_z")) };
            double dir[3] = { v(QStringLiteral("direction_x"), 0), v(QStringLiteral("direction_y"), 0), v(QStringLiteral("direction_z"), 1) };
            cyl->setLocation(loc);
            cyl->setDirection(dir);
            cyl->setRadius(v(QStringLiteral("radius"), 1));
            cyl->setLength(v(QStringLiteral("length"), 1));
        }
    } else if (type == FCGeoOpType::Sphere) {
        FCGeoModelSphere* sph = dynamic_cast<FCGeoModelSphere*>(cmd);
        if (sph) {
            double loc[3] = { v(QStringLiteral("location_x")), v(QStringLiteral("location_y")), v(QStringLiteral("location_z")) };
            sph->setLocation(loc);
            sph->setRadius(v(QStringLiteral("radius"), 1));
        }
    }
    // Union/Difference/Intersection inputs are set at build time by executor
}

FCGeometryModule::FCGeometryModule(QObject* parent)
    : QObject(parent)
{
    m_tree.reset(new FCGeometryTree);
    m_engine.reset(new FCGeometryBuildEngine(this));
    m_engine->setTree(m_tree.get());
}

FCGeometryModule::~FCGeometryModule() = default;

void FCGeometryModule::setExecutor(FCGeometryNodeExecutor* executor)
{
    m_engine->setExecutor(executor);
}

int FCGeometryModule::appendNode(FCGeoOpType type, const QList<int>& inputs, const FCGeoParamSet& params, const QString& name)
{
    FCGeoEnum::FCGeometryComType comType = toGeometryComType(type);
    if (comType == FCGeoEnum::FGTNone) return -1;
    FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
    if (!factory || !factory->getCommandSupported(comType)) return -1;

    FCAbsGeoCommand* cmd = factory->createCommand(comType);
    if (!cmd) return -1;

    QString nodeName = name.isEmpty() ? m_tree->checkName(QStringLiteral("Node")) : m_tree->checkName(name);
    cmd->setDataObjectName(nodeName);
    applyParamsToCommand(cmd, type, params);

    if (comType == FCGeoEnum::FGTBool) {
        FCGeoOperBool* boolCmd = dynamic_cast<FCGeoOperBool*>(cmd);
        if (boolCmd) {
            boolCmd->setBoolOperType(type == FCGeoOpType::Difference ? FCGeoOperBool::GBTCut : FCGeoOperBool::GBTCommon);
        }
    }

    int cmdId = cmd->getDataObjectID();
    FCGeoNode node(cmdId, type, nodeName);
    node.inputs = inputs;
    node.params = params;
    m_tree->addNode(node);
    m_lastNodeId = cmdId;
    return cmdId;
}

int FCGeometryModule::addBlock(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Block, QList<int>(), params, name);
}

int FCGeometryModule::addCylinder(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Cylinder, QList<int>(), params, name);
}

int FCGeometryModule::addSphere(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Sphere, QList<int>(), params, name);
}

int FCGeometryModule::addUnion(int a, int b, const QString& name)
{
    return appendNode(FCGeoOpType::Union, QList<int>() << a << b, FCGeoParamSet(), name);
}

int FCGeometryModule::addDifference(int a, int b, const QString& name)
{
    return appendNode(FCGeoOpType::Difference, QList<int>() << a << b, FCGeoParamSet(), name);
}

int FCGeometryModule::addIntersection(int a, int b, const QString& name)
{
    return appendNode(FCGeoOpType::Intersection, QList<int>() << a << b, FCGeoParamSet(), name);
}

int FCGeometryModule::addFillet(int input, FCSelectionRule* rule, const FCGeoParamSet& params, const QString& name)
{
    int id = appendNode(FCGeoOpType::Fillet, QList<int>() << input, params, name);
    if (rule)
        m_engine->setSelectionRuleForNode(id, rule);
    return id;
}

int FCGeometryModule::addImport(const FCGeoParamSet& params, const QString& name)
{
    return appendNode(FCGeoOpType::Import, QList<int>(), params, name);
}

void FCGeometryModule::updateNode(int id, const FCGeoParamSet& params)
{
    if (!m_tree->hasNode(id)) return;
    FCGeoNode node = m_tree->node(id);
    node.params = params;
    m_tree->setNode(id, node);
    FCAbsGeoCommand* cmd = FCDATAREPO->getDataAs<FCAbsGeoCommand>(id);
    if (cmd)
        applyParamsToCommand(cmd, node.type, params);
    m_engine->markDirty(id);
    m_engine->invalidateDownstream(id);
}

void FCGeometryModule::removeNode(int id)
{
    if (!m_tree->hasNode(id)) return;
    m_engine->invalidateDownstream(id);
    m_tree->removeNode(id);
    FCAbsGeoCommand* cmd = FCDATAREPO->getDataAs<FCAbsGeoCommand>(id);
    if (cmd) {
        FCDATAREPO->removeDataObj(cmd);
        delete cmd;
    }
    if (m_lastNodeId == id)
        m_lastNodeId = -1;
}

QVariant FCGeometryModule::build()
{
    return m_engine->build();
}

QVariant FCGeometryModule::buildDirty()
{
    return m_engine->buildDirty();
}

int FCGeometryModule::lastOutputNodeId() const
{
    return m_lastNodeId;
}

} // namespace FC
