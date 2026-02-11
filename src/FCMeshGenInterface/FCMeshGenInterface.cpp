/**
 * @file FCMeshGenInterface.cpp
 * @brief 网格生成接口实现（移植自 FITKInterfaceMeshGen，适配 Gmsh）
 */
#include "FCMeshGenInterface.h"
#include "FCAbstractMeshSizeInfoGenerator.h"
#include "FCAbstractGeometryMeshSizeGenerator.h"
#include "FCAbstractMeshAlgorithmInfoGenerator.h"
#include "FCGlobalMeshSizeInfo.h"
#include "FCGlobalMeshGenerateAlgorithmInfo.h"
#include "FCRegionMeshSizeManager.h"
#include "FCGeometryMeshSize.h"
#include "FCZonePoints.h"
#include "FCRegionGeometryRefine.h"
#include "FCAbstractMesherDriver.h"
#include "FCAbstractMeshProcessor.h"

namespace FC {

QMutex FCMeshGenInterface::_mutex;
FCMeshGenInterface* FCMeshGenInterface::_instance = nullptr;

FCMeshGenInterface* FCMeshGenInterface::instance()
{
    QMutexLocker locker(&_mutex);
    if (!_instance) _instance = new FCMeshGenInterface();
    return _instance;
}

void FCMeshGenInterface::destroyInstance()
{
    QMutexLocker locker(&_mutex);
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

FCMeshGenInterface::FCMeshGenInterface(QObject* parent) : QObject(parent) {}

FCMeshGenInterface::~FCMeshGenInterface()
{
    for (FCMeshGenInterfaceData* d : _interfaceData) delete d;
    _interfaceData.clear();
}

FCMeshGenInterfaceData* FCMeshGenInterface::getValidData(const QString& mesher)
{
    QString key = mesher.isEmpty() ? QStringLiteral("Gmsh") : mesher;
    if (_interfaceData.contains(key)) return _interfaceData[key];
    FCMeshGenInterfaceData* d = new FCMeshGenInterfaceData;
    _interfaceData.insert(key, d);
    return d;
}

FCGlobalMeshSizeInfo* FCMeshGenInterface::getGlobalMeshSizeInfo(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    if (!d) return nullptr;
    if (!d->globalSize && d->meshSizeGenor)
        d->globalSize = d->meshSizeGenor->generateGlobalMeshSizeInfo();
    return d->globalSize;
}

void FCMeshGenInterface::regMeshSizeGenerator(FCAbstractMeshSizeInfoGenerator* gen, const QString& mesher)
{
    FCMeshGenInterfaceData* d = getValidData(mesher);
    if (d) d->meshSizeGenor = gen;
}

FCGlobalMeshGenerateAlgorithmInfo* FCMeshGenInterface::getGlobalMeshGenerateAlgorithmInfo(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    if (!d) return nullptr;
    if (!d->globalMeshAlgorithm && d->meshAlgorithmGenor)
        d->globalMeshAlgorithm = d->meshAlgorithmGenor->generateGlobalMeshAlgorithmInfo();
    return d->globalMeshAlgorithm;
}

void FCMeshGenInterface::regMeshAlgorithmGenerator(FCAbstractMeshAlgorithmInfoGenerator* gen, const QString& mesher)
{
    FCMeshGenInterfaceData* d = getValidData(mesher);
    if (d) d->meshAlgorithmGenor = gen;
}

FCAbstractMeshSizeInfoGenerator* FCMeshGenInterface::getMeshSizeGenerator(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->meshSizeGenor : nullptr;
}

void FCMeshGenInterface::regGeometryMeshSizeGenerator(FCAbstractGeometryMeshSizeGenerator* gen, const QString& mesher)
{
    FCMeshGenInterfaceData* d = getValidData(mesher);
    if (d) d->geoMeshSizeGenor = gen;
}

FCAbstractGeometryMeshSizeGenerator* FCMeshGenInterface::getGeometryMeshSizeGenerator(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->geoMeshSizeGenor : nullptr;
}

void FCMeshGenInterface::regMesherDriver(FCAbstractMesherDriver* driver, const QString& mesher)
{
    FCMeshGenInterfaceData* d = getValidData(mesher);
    if (d) d->mesherDriver = driver;
}

FCAbstractMesherDriver* FCMeshGenInterface::getMesherDriver(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->mesherDriver : nullptr;
}

void FCMeshGenInterface::regMeshProcessor(FCAbstractMeshProcessor* p, const QString& mesher)
{
    FCMeshGenInterfaceData* d = getValidData(mesher);
    if (d) d->meshProcessor = p;
}

FCAbstractMeshProcessor* FCMeshGenInterface::getMeshProcessor(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->meshProcessor : nullptr;
}

FCRegionMeshSizeManager* FCMeshGenInterface::getRegionMeshSizeMgr(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->regionMeshMgr : nullptr;
}

FCGeometryMeshSizeManager* FCMeshGenInterface::getGeometryMeshSizeManager(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->geoMeshSizeMgr : nullptr;
}

FCZonePointManager* FCMeshGenInterface::getZonePointManager(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->zonePointsManager : nullptr;
}

FCRegionGeometryRefineManager* FCMeshGenInterface::getRegionGeometryRefineManager(const QString& mesher)
{
    FCMeshGenInterfaceData* d = mesher.isEmpty() && !_interfaceData.isEmpty()
        ? _interfaceData[_interfaceData.keys().first()] : getValidData(mesher);
    return d ? d->regionGeoRefineMgr : nullptr;
}

void FCMeshGenInterface::removeMeshGenInterfaceData(const QString& mesher)
{
    if (_interfaceData.contains(mesher)) {
        delete _interfaceData[mesher];
        _interfaceData.remove(mesher);
    }
}

QStringList FCMeshGenInterface::getMeshInterfaceNames() const
{
    return _interfaceData.keys();
}

// FCMeshGenInterfaceData
FCMeshGenInterfaceData::FCMeshGenInterfaceData()
{
    regionMeshMgr = new FCRegionMeshSizeManager();
    geoMeshSizeMgr = new FCGeometryMeshSizeManager();
    zonePointsManager = new FCZonePointManager();
    regionGeoRefineMgr = new FCRegionGeometryRefineManager();
}

FCMeshGenInterfaceData::~FCMeshGenInterfaceData()
{
    delete meshSizeGenor; meshSizeGenor = nullptr;
    delete regionMeshMgr; regionMeshMgr = nullptr;
    delete meshAlgorithmGenor; meshAlgorithmGenor = nullptr;
    delete globalMeshAlgorithm; globalMeshAlgorithm = nullptr;
    delete globalSize; globalSize = nullptr;
    delete mesherDriver; mesherDriver = nullptr;
    delete meshProcessor; meshProcessor = nullptr;
    delete geoMeshSizeMgr; geoMeshSizeMgr = nullptr;
    delete geoMeshSizeGenor; geoMeshSizeGenor = nullptr;
    delete zonePointsManager; zonePointsManager = nullptr;
    delete regionGeoRefineMgr; regionGeoRefineMgr = nullptr;
}

} // namespace FC
