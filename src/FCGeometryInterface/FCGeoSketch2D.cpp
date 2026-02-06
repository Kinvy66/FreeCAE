/**
 * @file FCGeoSketch2D.cpp
 * @brief 2D 草图命令实现（接口层）
 */
#include "FCGeoSketch2D.h"
#include <FCModelInterface/FCAbstractModel.h>

namespace FC {

FCGeoSketch2D::FCGeoSketch2D()
{
}

FCGeoSketch2D::~FCGeoSketch2D()
{
}

FCGeoEnum::FITKGeometryComType FCGeoSketch2D::getGeometryCommandType()
{
    return FCGeoEnum::FGTSketch2D;
}

bool FCGeoSketch2D::update()
{
    return true;
}

void FCGeoSketch2D::setPlane(const double* pos, const double* normal, const double* up)
{
    if (pos) FCAbstractModel::CopyArray(const_cast<double*>(pos), m_planePos, 3);
    if (normal) FCAbstractModel::CopyArray(const_cast<double*>(normal), m_planeNormal, 3);
    if (up) FCAbstractModel::CopyArray(const_cast<double*>(up), m_planeUp, 3);
}

void FCGeoSketch2D::getPlane(double* pos, double* normal, double* up) const
{
    if (pos) FCAbstractModel::CopyArray(const_cast<double*>(m_planePos), pos, 3);
    if (normal) FCAbstractModel::CopyArray(const_cast<double*>(m_planeNormal), normal, 3);
    if (up) FCAbstractModel::CopyArray(const_cast<double*>(m_planeUp), up, 3);
}

void FCGeoSketch2D::setPosition(const double* pos)
{
    if (pos) FCAbstractModel::CopyArray(const_cast<double*>(pos), m_planePos, 3);
}

void FCGeoSketch2D::getPosition(double* pos) const
{
    if (pos) FCAbstractModel::CopyArray(const_cast<double*>(m_planePos), pos, 3);
}

void FCGeoSketch2D::setNormal(const double* nor)
{
    if (nor) FCAbstractModel::CopyArray(const_cast<double*>(nor), m_planeNormal, 3);
}

void FCGeoSketch2D::getNormal(double* nor) const
{
    if (nor) FCAbstractModel::CopyArray(const_cast<double*>(m_planeNormal), nor, 3);
}

void FCGeoSketch2D::setUp(const double* up)
{
    if (up) FCAbstractModel::CopyArray(const_cast<double*>(up), m_planeUp, 3);
}

void FCGeoSketch2D::getUp(double* up) const
{
    if (up) FCAbstractModel::CopyArray(const_cast<double*>(m_planeUp), up, 3);
}

bool FCGeoSketch2D::canUndo()
{
    return getDataCount() > 0;
}

bool FCGeoSketch2D::canRedo()
{
    return false;
}

bool FCGeoSketch2D::undoInternal()
{
    return false;
}

bool FCGeoSketch2D::redoInternal()
{
    return false;
}

} // namespace FC
