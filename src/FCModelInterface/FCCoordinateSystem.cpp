/**
 * @file FCCoordinateSystem.cpp
 * @brief 坐标系实现（移植自 FITKInterfaceModel）
 */
#include "FCCoordinateSystem.h"
#include <FCData/FCPoint.h>
#include <FCData/FCVec3Alg.h>

namespace FC {

FCCoordinateSystem::FCCoordinateSystem(FCSysType sysType)
    : _sysType(sysType)
{
    _originPt = new FCPoint(0, 0, 0);
    _xPt = new FCPoint(1, 0, 0);
    _yPt = new FCPoint(0, 1, 0);
}

FCCoordinateSystem::~FCCoordinateSystem()
{
    if (_originPt) delete _originPt;
    if (_xPt) delete _xPt;
    if (_yPt) delete _yPt;
}

FCCoordinateSystem::FCSysType FCCoordinateSystem::getSysType()
{
    return _sysType;
}

void FCCoordinateSystem::setOrigin(double x, double y, double z)
{
    if (_originPt) _originPt->setCoor(x, y, z);
}

void FCCoordinateSystem::setPoint1(double x, double y, double z)
{
    if (_xPt) _xPt->setCoor(x, y, z);
}

void FCCoordinateSystem::setPoint2(double x, double y, double z)
{
    if (_yPt) _yPt->setCoor(x, y, z);
}

void FCCoordinateSystem::getOrigin(double* origin)
{
    if (_originPt && origin) _originPt->getCoor(origin);
}

void FCCoordinateSystem::getDirection1(double* dir)
{
    FCVec3 d1(Subtract(*_xPt, *_originPt));
    d1.normalize();
    if (dir) d1.getCoor(dir);
}

void FCCoordinateSystem::getDirection2(double* dir, bool v)
{
    if (!v) {
        FCVec3 d1(Subtract(*_yPt, *_originPt));
        d1.normalize();
        if (dir) d1.getCoor(dir);
    } else {
        FCVec3 d1(Subtract(*_xPt, *_originPt));
        FCVec3 d2(Subtract(*_yPt, *_originPt));
        FCVec3 d3(CrossProduct(d1, d2));
        FCVec3 d(CrossProduct(d3, d1));
        d.normalize();
        if (dir) d.getCoor(dir);
    }
}

void FCCoordinateSystem::getDirection3(double* dir)
{
    FCVec3 d1(Subtract(*_xPt, *_originPt));
    FCVec3 d2(Subtract(*_yPt, *_originPt));
    FCVec3 d3(CrossProduct(d1, d2));
    d3.normalize();
    if (dir) d3.getCoor(dir);
}

void FCCoordinateSystem::get3DirectionAtPoint(const double* pxyz, double* dirs)
{
    if (!pxyz || !dirs) return;
    double d1[3] = {0, 0, 0}, d2[3] = {0, 0, 0}, d3[3] = {0, 0, 0};
    getDirection1(d1);
    getDirection2(d2);
    getDirection3(d3);
    for (int i = 0; i < 3; ++i) dirs[i] = d1[i];
    for (int i = 0; i < 3; ++i) dirs[i + 3] = d2[i];
    for (int i = 0; i < 3; ++i) dirs[i + 6] = d3[i];
}

bool FCCoordinateSystem::copy(FCAbstractDataObject* obj)
{
    FCCoordinateSystem* csys = dynamic_cast<FCCoordinateSystem*>(obj);
    if (!csys) return false;
    double ori[3] = {0, 0, 0};
    double pt[3] = {0, 0, 0};
    csys->getOrigin(ori);
    setOrigin(ori[0], ori[1], ori[2]);
    csys->getDirection1(pt);
    setPoint1(ori[0] + pt[0], ori[1] + pt[1], ori[2] + pt[2]);
    csys->getDirection2(pt, false);
    setPoint2(ori[0] + pt[0], ori[1] + pt[1], ori[2] + pt[2]);
    return FCAbstractNamedDataObject::copy(obj);
}

QList<FCCoordinateSystem*> FCCoordinateSystemManager::getCoordinateSystems(FCCoordinateSystem::FCSysType type)
{
    QList<FCCoordinateSystem*> sysList;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCCoordinateSystem* sys = getDataByIndex(i);
        if (!sys) continue;
        if (type == sys->getSysType())
            sysList.append(sys);
    }
    return sysList;
}

} // namespace FC
