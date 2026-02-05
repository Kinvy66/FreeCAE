/**
 * @file FCCoordinateSystem.h
 * @brief 坐标系定义（移植自 FITKInterfaceModel）
 */
#ifndef FCCOORDINATESYSTEM_H
#define FCCOORDINATESYSTEM_H

#include "FCModelInterfaceAPI.h"
#include <FCData/FCAbstractNamedDataObject.h>
#include <FCData/FCAbstractDataManager.hpp>
#include <FCData/FCMacros.h>

namespace FC {

class FCPoint;

class FCMODELINTERFACE_API FCCoordinateSystem : public FCAbstractNamedDataObject
{
    FC_CLASS(FC, FCCoordinateSystem);
    Q_OBJECT
public:
    enum FCSysType
    {
        FCSCartesian,
        FCSCylinder,
        FCSSphere,
    };
    Q_ENUM(FCSysType);

public:
    explicit FCCoordinateSystem(FCSysType sysType);
    ~FCCoordinateSystem() override;

    FCSysType getSysType();
    virtual void setOrigin(double x, double y, double z);
    virtual void setPoint1(double x, double y, double z);
    virtual void setPoint2(double x, double y, double z);
    virtual void getOrigin(double* origin);
    virtual void getDirection1(double* dir);
    virtual void getDirection2(double* dir, bool vertialToD1 = true);
    virtual void getDirection3(double* dir);
    virtual void get3DirectionAtPoint(const double* pxyz, double* dirs);
    bool copy(FCAbstractDataObject* obj) override;

protected:
    FCSysType _sysType{ FCSCartesian };
    FCPoint* _originPt{};
    FCPoint* _xPt{};
    FCPoint* _yPt{};
};

class FCMODELINTERFACE_API FCCoordinateSystemManager :
    public FCAbstractDataManager<FCCoordinateSystem>
{
public:
    explicit FCCoordinateSystemManager() = default;
    virtual ~FCCoordinateSystemManager() = default;

    template<class T>
    T* getCoordinateSystemT(int id)
    {
        return dynamic_cast<T*>(getDataByID(id));
    }
    QList<FCCoordinateSystem*> getCoordinateSystems(FCCoordinateSystem::FCSysType type);
};

} // namespace FC

#endif // FCCOORDINATESYSTEM_H
