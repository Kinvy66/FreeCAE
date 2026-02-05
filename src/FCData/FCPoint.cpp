/**
 * @file FCPoint.cpp
 * @brief FCPoint / FCNode / FCVec3 实现
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCPoint.h"
#include <cmath>
#include <algorithm>

namespace FC {

// --- FCPoint ---
FCPoint::FCPoint() = default;

FCPoint::FCPoint(double x, double y, double z)
{
    _coor[0] = x;
    _coor[1] = y;
    _coor[2] = z;
}

FCPoint::FCPoint(double* c)
{
    if (c) {
        _coor[0] = c[0];
        _coor[1] = c[1];
        _coor[2] = c[2];
    }
}

FCPoint::FCPoint(const double* c)
{
    if (c) {
        _coor[0] = c[0];
        _coor[1] = c[1];
        _coor[2] = c[2];
    }
}

FCPoint::FCPoint(const FCPoint& point)
{
    _coor[0] = point._coor[0];
    _coor[1] = point._coor[1];
    _coor[2] = point._coor[2];
}

double FCPoint::x() const { return _coor[0]; }
double FCPoint::y() const { return _coor[1]; }
double FCPoint::z() const { return _coor[2]; }

void FCPoint::getCoor(double* c) const
{
    if (c) {
        c[0] = _coor[0];
        c[1] = _coor[1];
        c[2] = _coor[2];
    }
}

void FCPoint::setCoor(double x, double y, double z)
{
    _coor[0] = x;
    _coor[1] = y;
    _coor[2] = z;
}

void FCPoint::operator=(const FCPoint& p)
{
    _coor[0] = p._coor[0];
    _coor[1] = p._coor[1];
    _coor[2] = p._coor[2];
}

double FCPoint::operator[](int index) const
{
    if (index >= 0 && index < 3)
        return _coor[index];
    return 0;
}

// --- FCNode ---
FCNode::FCNode(int id, double x, double y, double z)
    : FCPoint(x, y, z)
    , _nodeID(id)
{}

FCNode::FCNode(int id, const double* coor)
    : FCPoint(coor)
    , _nodeID(id)
{}

FCNode::~FCNode()
{
    emit nodeDestroiedSignal(this);
}

void FCNode::setNodeID(int id) { _nodeID = id; }
int FCNode::getNodeID() const { return _nodeID; }
void FCNode::setTag(unsigned int tag) { _tag = tag; }
unsigned int FCNode::getTag() const { return _tag; }
void FCNode::setNativeFlag(bool native) { _nativeFlag = native; }
bool FCNode::getNativeFlag() const { return _nativeFlag; }

// --- FCVec3 ---
FCVec3::FCVec3(const FCPoint& point)
    : FCPoint(point.x(), point.y(), point.z())
{}

FCVec3::FCVec3(const FCPoint& endPt, const FCPoint& startPt)
    : FCPoint(endPt.x() - startPt.x(), endPt.y() - startPt.y(), endPt.z() - startPt.z())
{}

double FCVec3::getNorm() const
{
    return std::sqrt(_coor[0] * _coor[0] + _coor[1] * _coor[1] + _coor[2] * _coor[2]);
}

void FCVec3::normalize()
{
    double n = getNorm();
    if (n > 1e-20) {
        _coor[0] /= n;
        _coor[1] /= n;
        _coor[2] /= n;
    }
}

} // namespace FC
