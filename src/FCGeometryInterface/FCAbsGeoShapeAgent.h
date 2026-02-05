/**
 * @file FCAbsGeoShapeAgent.h
 * @brief 抽象几何形状代理（移植自 FITKInterfaceGeometry）
 */
#ifndef FCABSGEOSHAPEAGENT_H
#define FCABSGEOSHAPEAGENT_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoEnum.h"
#include "FCGeometryMeshVS.h"
#include <FCModelInterface/FCAbstractGeoModel.h>
#include <FCData/FCMacros.h>

namespace FC {

class FCAbsGeoCommand;
class FCVirtualTopoManager;

class FCGEOMETRYINTERFACE_API FCAbsGeoShapeAgent : public FCAbstractGeoModel
{
    FC_CLASS(FC, FCAbsGeoShapeAgent);
public:
    explicit FCAbsGeoShapeAgent(FCAbsGeoCommand* command);
    ~FCAbsGeoShapeAgent() override = 0;

    virtual FCGeoEnum::FITKGeoEngine getGeoEngine() = 0;
    FCAbsGeoCommand* getGeoCommand();
    template<class T> T* getTGeoCommand() { return dynamic_cast<T*>(_command); }
    virtual void buildVirtualTopo(bool keepTopos = false) = 0;
    FCVirtualTopoManager* getVirtualTopoManager();
    virtual void triangulation() = 0;
    void updateVS();
    FCGeometryMeshVS* getMeshVS();
    bool writeSTLFile(QString filePath) override;

protected:
    FCAbsGeoCommand* _command{};
    FCVirtualTopoManager* _vtmanager{};
    FCGeometryMeshVS* _meshVS{};
};

} // namespace FC

#endif // FCABSGEOSHAPEAGENT_H
