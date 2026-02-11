/**
 * @file FCRegionMeshSizeBox.cpp
 */
#include "FCRegionMeshSizeBox.h"
#include <cstring>

namespace FC {

FCRegionMeshSizeBox::FCRegionMeshSizeBox(QObject* parent) : FCAbstractRegionMeshSize(parent) {}

void FCRegionMeshSizeBox::setPoint1(const double* p1)
{
    if (p1) std::memcpy(_point1, p1, 3 * sizeof(double));
}

void FCRegionMeshSizeBox::setLength(const double* length)
{
    if (length) std::memcpy(_lengthParam, length, 3 * sizeof(double));
}

void FCRegionMeshSizeBox::getPoint1(double* p1) const
{
    if (p1) std::memcpy(p1, _point1, 3 * sizeof(double));
}

void FCRegionMeshSizeBox::getLength(double* length) const
{
    if (length) std::memcpy(length, _lengthParam, 3 * sizeof(double));
}

} // namespace FC
