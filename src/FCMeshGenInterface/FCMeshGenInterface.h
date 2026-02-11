/**
 * @file FCMeshGenInterface.h
 * @brief 网格生成接口（移植自 FITKInterfaceMeshGen，适配 Gmsh）
 */
#ifndef FCMESHGENINTERFACE_H
#define FCMESHGENINTERFACE_H

#include "FCMeshGenInterfaceAPI.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <QMutex>

namespace FC {

class FCGlobalMeshSizeInfo;
class FCRegionMeshSizeManager;
class FCAbstractMeshSizeInfoGenerator;
class FCAbstractGeometryMeshSizeGenerator;
class FCAbstractMesherDriver;
class FCAbstractMeshProcessor;
class FCGeometryMeshSizeManager;
class FCZonePointManager;
class FCRegionGeometryRefineManager;
class FCGlobalMeshGenerateAlgorithmInfo;
class FCAbstractMeshAlgorithmInfoGenerator;

/**
 * @brief 单引擎网格划分数据
 */
struct FCMESHGENINTERFACE_API FCMeshGenInterfaceData
{
    explicit FCMeshGenInterfaceData();
    virtual ~FCMeshGenInterfaceData();

    FCAbstractMeshSizeInfoGenerator* meshSizeGenor{};
    FCAbstractMeshAlgorithmInfoGenerator* meshAlgorithmGenor{};
    FCAbstractGeometryMeshSizeGenerator* geoMeshSizeGenor{};
    FCAbstractMesherDriver* mesherDriver{};
    FCAbstractMeshProcessor* meshProcessor{};
    FCGlobalMeshSizeInfo* globalSize{};
    FCGlobalMeshGenerateAlgorithmInfo* globalMeshAlgorithm{};
    FCRegionMeshSizeManager* regionMeshMgr{};
    FCGeometryMeshSizeManager* geoMeshSizeMgr{};
    FCZonePointManager* zonePointsManager{};
    FCRegionGeometryRefineManager* regionGeoRefineMgr{};
};

/**
 * @brief 网格生成接口（单例，按网格引擎名称管理参数与驱动器）
 */
class FCMESHGENINTERFACE_API FCMeshGenInterface : public QObject
{
    Q_OBJECT
public:
    static FCMeshGenInterface* instance();
    static void destroyInstance();

    FCGlobalMeshSizeInfo* getGlobalMeshSizeInfo(const QString& mesher = QString());
    void regMeshSizeGenerator(FCAbstractMeshSizeInfoGenerator* gen, const QString& mesher = QString());

    FCGlobalMeshGenerateAlgorithmInfo* getGlobalMeshGenerateAlgorithmInfo(const QString& mesher = QString());
    void regMeshAlgorithmGenerator(FCAbstractMeshAlgorithmInfoGenerator* gen, const QString& mesher = QString());

    FCAbstractMeshSizeInfoGenerator* getMeshSizeGenerator(const QString& mesher = QString());
    void regGeometryMeshSizeGenerator(FCAbstractGeometryMeshSizeGenerator* gen, const QString& mesher = QString());
    FCAbstractGeometryMeshSizeGenerator* getGeometryMeshSizeGenerator(const QString& mesher = QString());

    void regMesherDriver(FCAbstractMesherDriver* driver, const QString& mesher = QString());
    FCAbstractMesherDriver* getMesherDriver(const QString& mesher = QString());

    void regMeshProcessor(FCAbstractMeshProcessor* p, const QString& mesher = QString());
    FCAbstractMeshProcessor* getMeshProcessor(const QString& mesher = QString());

    FCRegionMeshSizeManager* getRegionMeshSizeMgr(const QString& mesher = QString());
    FCGeometryMeshSizeManager* getGeometryMeshSizeManager(const QString& mesher = QString());
    FCZonePointManager* getZonePointManager(const QString& mesher = QString());
    FCRegionGeometryRefineManager* getRegionGeometryRefineManager(const QString& mesher = QString());

    void removeMeshGenInterfaceData(const QString& mesher);
    QStringList getMeshInterfaceNames() const;

private:
    explicit FCMeshGenInterface(QObject* parent = nullptr);
    ~FCMeshGenInterface() override;
    FCMeshGenInterfaceData* getValidData(const QString& mesher);

    QHash<QString, FCMeshGenInterfaceData*> _interfaceData;
    static FCMeshGenInterface* _instance;
    static QMutex _mutex;
};

} // namespace FC

#endif // FCMESHGENINTERFACE_H
