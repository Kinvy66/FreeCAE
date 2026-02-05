/**
 * @file FCAbstractGeoModel.h
 * @brief 抽象几何模型（移植自 FITKInterfaceModel）
 */
#ifndef FCABSTRACTGEOMODEL_H
#define FCABSTRACTGEOMODEL_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModel.h"
#include "FCAbstractModelComponent.h"
#include "FCModelEnum.h"
#include <FCData/FCAbstractDataManager.hpp>
#include <FCData/FCAbstractDataObject.h>
#include <QList>

namespace FC {

class FCGeoComponentManager;
class FCGeoComponent;

class FCMODELINTERFACE_API FCAbstractGeoModel : public FCAbstractModel
{
public:
    explicit FCAbstractGeoModel();
    virtual ~FCAbstractGeoModel() = 0;

    virtual int getShapeCount(FCModelEnum::FITKModelSetType type);
    virtual void getTriangleMeshVS(FCAbstractDataObject* mesh, int id, void* additional = nullptr);
    virtual bool writeSTLFile(QString filePath);
    virtual FCGeoComponentManager* getGeoComponentManager();
    virtual bool getBoundaryBox(double* minPt, double* maxPt);

protected:
    void setAbsDataID(int id) override;
    FCGeoComponentManager* _compManager{};
};

class FCMODELINTERFACE_API FCGeoComponent : public FCAbstractModelComponent
{
public:
    explicit FCGeoComponent(FCModelEnum::FITKModelSetType type = FCModelEnum::FMSSurface);
    virtual ~FCGeoComponent() = default;

    void setMember(QList<int>& member);
    void appendMember(int mem);
    void clear();
    void removeMember(int mem);
    QList<int> getMember() const;
    FCModelEnum::FITKModelSetType getGeoType() const;

private:
    FCModelEnum::FITKModelSetType _type{ FCModelEnum::FMSNone };
    QList<int> _member;
};

class FCMODELINTERFACE_API FCGeoComponentManager : public FCAbstractDataManager<FCGeoComponent>
{
public:
    explicit FCGeoComponentManager() = default;
    virtual ~FCGeoComponentManager() = default;

    void appendDataObj(FCAbstractDataObject* item) override;
    QList<FCGeoComponent*> getGeoComponent(FCModelEnum::FITKModelSetType type = FCModelEnum::FMSSurface);
    QList<int> getAllComponentMember(FCModelEnum::FITKModelSetType type = FCModelEnum::FMSSurface);
};

} // namespace FC

#endif // FCABSTRACTGEOMODEL_H
