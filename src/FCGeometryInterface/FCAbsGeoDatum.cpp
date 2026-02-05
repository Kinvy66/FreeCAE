/**
 * @file FCAbsGeoDatum.cpp
 * @brief 基准元素抽象实现（移植自 FITKInterfaceGeometry）
 */
#include "FCAbsGeoDatum.h"

namespace FC {

void FCAbsGeoDatum::setEditable(bool flag)
{
    _editable = flag;
}

bool FCAbsGeoDatum::editable()
{
    return _editable;
}

void FCAbsGeoDatum::setPosition(double* pos)
{
    if (!pos) return;
    _pos[0] = pos[0]; _pos[1] = pos[1]; _pos[2] = pos[2];
}

void FCAbsGeoDatum::getPosition(double* pos)
{
    if (!pos) return;
    pos[0] = _pos[0]; pos[1] = _pos[1]; pos[2] = _pos[2];
}

FCGeoEnum::FITKGeometryComType FCAbsGeoDatum::getGeometryCommandType()
{
    return FCGeoEnum::FGTDatum;
}

void FCAbsGeoDatum::printLog(QString msg, int type)
{
    FCAbsGeoCommand::printLog(msg, type);
}

FCGeoEnum::FITKDatumType FCAbsGeoDatumPoint::getDatumType()
{
    return FCGeoEnum::FDTPoint;
}

FCGeoEnum::FITKDatumType FCAbsGeoDatumLine::getDatumType()
{
    return FCGeoEnum::FDTLine;
}

void FCAbsGeoDatumLine::setPosition2(const double* pos)
{
    if (!pos) return;
    _pos2[0] = pos[0]; _pos2[1] = pos[1]; _pos2[2] = pos[2];
}

void FCAbsGeoDatumLine::getPosition2(double* pos) const
{
    if (!pos) return;
    pos[0] = _pos2[0]; pos[1] = _pos2[1]; pos[2] = _pos2[2];
}

FCGeoEnum::FITKDatumType FCAbsGeoDatumPlane::getDatumType()
{
    return FCGeoEnum::FDTPlane;
}

void FCAbsGeoDatumPlane::setNormal(const double* nor)
{
    if (!nor) return;
    _nor[0] = nor[0]; _nor[1] = nor[1]; _nor[2] = nor[2];
}

void FCAbsGeoDatumPlane::getNormal(double* nor) const
{
    if (!nor) return;
    nor[0] = _nor[0]; nor[1] = _nor[1]; nor[2] = _nor[2];
}

void FCAbsGeoDatumPlane::setUp(const double* up)
{
    if (!up) return;
    _up[0] = up[0]; _up[1] = up[1]; _up[2] = up[2];
}

void FCAbsGeoDatumPlane::getUp(double* up) const
{
    if (!up) return;
    up[0] = _up[0]; up[1] = _up[1]; up[2] = _up[2];
}

} // namespace FC
