/**
 * @file FCGeoInterfaceFactory.cpp
 * @brief 几何命令/基准创建工厂实现（接口层）
 */
#include "FCGeoInterfaceFactory.h"
#include <QMutexLocker>

namespace FC {

QMutex FCGeoInterfaceFactory::_mutex;
FCGeoInterfaceFactory* FCGeoInterfaceFactory::_instance = nullptr;

FCGeoInterfaceFactory* FCGeoInterfaceFactory::instance()
{
    QMutexLocker locker(&_mutex);
    if (!_instance)
        _instance = new FCGeoInterfaceFactory;
    return _instance;
}

void FCGeoInterfaceFactory::destroy()
{
    QMutexLocker locker(&_mutex);
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

FCGeoInterfaceFactory::FCGeoInterfaceFactory(QObject* parent)
    : QObject(parent)
{}

FCGeoInterfaceFactory::~FCGeoInterfaceFactory()
{
    clearAllInterface();
}

void FCGeoInterfaceFactory::insertCommandCreateFun(FCGeoEnum::FITKGeometryComType t, GeoCommandFuns f)
{
    if (f) _interfaceFuns.insert(t, f);
}

void FCGeoInterfaceFactory::setGeomToolsCreator(FCAbstractGeomToolsCreator* creator)
{
    _geomToolsCreator = creator;
}

void FCGeoInterfaceFactory::insertDatumCreateFun(FCGeoEnum::FITKDatumType t, GeoDatumFuns f)
{
    if (f) _interfaceDatumFuns.insert(t, f);
}

FCAbsGeoCommand* FCGeoInterfaceFactory::createCommand(FCGeoEnum::FITKGeometryComType t)
{
    if (!_interfaceFuns.contains(t)) return nullptr;
    GeoCommandFuns fun = _interfaceFuns.value(t);
    if (!fun) return nullptr;
    return fun();
}

void FCGeoInterfaceFactory::clearAllInterface()
{
    _geomToolsCreator = nullptr;
    _interfaceFuns.clear();
    _interfaceDatumFuns.clear();
}

bool FCGeoInterfaceFactory::getCommandSupported(FCGeoEnum::FITKGeometryComType type)
{
    return _interfaceFuns.contains(type);
}

bool FCGeoInterfaceFactory::getDatumSupported(FCGeoEnum::FITKDatumType type)
{
    return _interfaceDatumFuns.contains(type);
}

FCAbsGeoDatum* FCGeoInterfaceFactory::createDatum(FCGeoEnum::FITKDatumType t)
{
    if (!_interfaceDatumFuns.contains(t)) return nullptr;
    GeoDatumFuns fun = _interfaceDatumFuns.value(t);
    if (!fun) return nullptr;
    return fun();
}

} // namespace FC
