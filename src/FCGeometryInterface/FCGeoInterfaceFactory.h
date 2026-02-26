/**
 * @file FCGeoInterfaceFactory.h
 * @brief 几何接口工厂（移植自 FITKInterfaceGeometry） 
 */
#ifndef FCGEOINTERFACEFACTORY_H
#define FCGEOINTERFACEFACTORY_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCAbsGeoDatum.h"
#include "FCAbsGeomTools.h"
#include "FCGeoEnum.h"
#include <QHash>
#include <QObject>
#include <QMutex>
#include <functional>

namespace FC {

using GeoCommandFuns = std::function<FCAbsGeoCommand*()>;
using GeoDatumFuns = std::function<FCAbsGeoDatum*()>;

class FCGEOMETRYINTERFACE_API FCGeoInterfaceFactory : public QObject
{
    Q_OBJECT
public:
    static FCGeoInterfaceFactory* instance();
    static void destroy();

    void setGeomToolsCreator(FCAbstractGeomToolsCreator* creator);
    FCAbstractGeomToolsCreator* getGeomToolsCreator() const { return _geomToolsCreator; }

    void insertCommandCreateFun(FCGeoEnum::FCGeometryComType t, GeoCommandFuns f);
    void insertDatumCreateFun(FCGeoEnum::FCDatumType t, GeoDatumFuns f);
    FCAbsGeoCommand* createCommand(FCGeoEnum::FCGeometryComType t);
    
    template<class T>
    T* createCommandT(FCGeoEnum::FCGeometryComType t) {
        FCAbsGeoCommand* c = createCommand(t);
        T* ct = dynamic_cast<T*>(c);
        if (!ct && c) delete c;
        return ct;
    }
    
    void clearAllInterface();
    bool getCommandSupported(FCGeoEnum::FCGeometryComType type);
    
    bool getDatumSupported(FCGeoEnum::FCDatumType type);
    
    FCAbsGeoDatum* createDatum(FCGeoEnum::FCDatumType t);
    
    template<class T>
    T* createDatumT(FCGeoEnum::FCDatumType t, bool editable = true) {
        FCAbsGeoDatum* d = createDatum(t);
        if (!d) return nullptr;
        T* dt = dynamic_cast<T*>(d);
        if (!dt) { delete d; return nullptr; }
        dt->setEditable(editable);
        return dt;
    }

private:
    explicit FCGeoInterfaceFactory(QObject* parent = nullptr);
    ~FCGeoInterfaceFactory() override;
    FCGeoInterfaceFactory(const FCGeoInterfaceFactory&) = delete;
    FCGeoInterfaceFactory& operator=(const FCGeoInterfaceFactory&) = delete;

    static QMutex _mutex;
    static FCGeoInterfaceFactory* _instance;
    FCAbstractGeomToolsCreator* _geomToolsCreator{};
    QHash<FCGeoEnum::FCGeometryComType, GeoCommandFuns> _interfaceFuns;
    QHash<FCGeoEnum::FCDatumType, GeoDatumFuns> _interfaceDatumFuns;
};

} // namespace FC

#endif // FCGEOINTERFACEFACTORY_H
