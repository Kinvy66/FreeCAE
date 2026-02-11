/**
 * @file FCAbstractRegionMeshSize.cpp
 */
#include "FCAbstractRegionMeshSize.h"

namespace FC {

FCAbstractRegionMeshSize::FCAbstractRegionMeshSize(QObject* parent) : FCGlobalMeshSizeInfo(parent) {}

FCAbstractRegionMeshSize::~FCAbstractRegionMeshSize() {}

void FCAbstractRegionMeshSize::setDivision(int dim, int divNum)
{
    if (dim >= 0 && dim < 3) _division[dim] = divNum;
}

void FCAbstractRegionMeshSize::setGrading(int dim, double grad)
{
    if (dim >= 0 && dim < 3) _grading[dim] = grad;
}

int FCAbstractRegionMeshSize::getDivision(int dim) const
{
    return (dim >= 0 && dim < 3) ? _division[dim] : 0;
}

double FCAbstractRegionMeshSize::getGrading(int dim) const
{
    return (dim >= 0 && dim < 3) ? _grading[dim] : 1.0;
}

void FCAbstractRegionMeshSize::insertBoundary(int faceID, BoundaryType bt)
{
    _boundarys.insert(faceID, bt);
}

FCAbstractRegionMeshSize::BoundaryType FCAbstractRegionMeshSize::getBoundary(int faceID) const
{
    return _boundarys.value(faceID, BTEmpty);
}

void FCAbstractRegionMeshSize::setInternalSize(double v) { _internalSize = v; }
double FCAbstractRegionMeshSize::getInternalSize() const { return _internalSize; }
void FCAbstractRegionMeshSize::setExternalSize(double v) { _externalSize = v; }
double FCAbstractRegionMeshSize::getExternalSize() const { return _externalSize; }
void FCAbstractRegionMeshSize::setThickness(double v) { _thickness = v; }
double FCAbstractRegionMeshSize::getThickness() const { return _thickness; }

} // namespace FC
