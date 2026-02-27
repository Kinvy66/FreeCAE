/**
 * @file FCAbsGeoCommand.cpp
 * @brief 几何命令抽象实现（移植自 FITKInterfaceGeometry）
 */
#include "FCAbsGeoCommand.h"
#include "FCAbsGeoShapeAgent.h"

namespace FC {

FCAbsGeoCommand::FCAbsGeoCommand()
    : _prop(new FCGeoCommandProp)
{}

FCAbsGeoCommand::~FCAbsGeoCommand()
{
    if (_prop) { delete _prop; _prop = nullptr; }
}

bool FCAbsGeoCommand::undo()
{
    return false;
}

bool FCAbsGeoCommand::redo()
{
    return false;
}

void FCAbsGeoCommand::setCommandStatus(FCGeoEnum::FCGeoStatus status)
{
    _status = status;
}

FCGeoEnum::FCGeoStatus FCAbsGeoCommand::getCommandStatus()
{
    return _status;
}

FCAbsGeoShapeAgent* FCAbsGeoCommand::getShapeAgent()
{
    return _shapeAgent;
}

FCVirtualTopoManager* FCAbsGeoCommand::getVirtualTopoManager()
{
    FCAbsGeoShapeAgent* agent = getShapeAgent();
    return agent ? agent->getVirtualTopoManager() : nullptr;
}

FCAbsVirtualTopo* FCAbsGeoCommand::getVirtualTopo(FCGeoEnum::VTopoShapeType type, int id)
{
    FCAbsGeoShapeAgent* agent = getShapeAgent();
    if (!agent) return nullptr;
    FCVirtualTopoManager* mgr = agent->getVirtualTopoManager();
    if (!mgr) return nullptr;
    FCShapeVirtualTopoManager* sm = mgr->getShapeVirtualTopoManager(type);
    return sm ? sm->getDataByID(id) : nullptr;
}

void FCAbsGeoCommand::printLog(QString msg, int type)
{
    Q_UNUSED(msg);
    Q_UNUSED(type);
}

void FCAbsGeoCommand::removeFromPart(FCAbsGeoCommand* part)
{
    Q_UNUSED(part);
}

FCAbsGeoCommand* FCGeoCommandManager::getDataByID(FCID id)
{
    FCAbsGeoCommand* cmd = FCAbstractDataManager<FCAbsGeoCommand>::getDataByID(id);
    if (cmd) return cmd;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCGeoCommandManager* sub = dynamic_cast<FCGeoCommandManager*>(getDataByIndex(i));
        if (!sub) continue;
        cmd = sub->getDataByID(id);
        if (cmd) return cmd;
    }
    return nullptr;
}

QList<FCAbsGeoCommand*> FCGeoCommandManager::getRootCommands()
{
    return QList<FCAbsGeoCommand*>();
}

} // namespace FC
