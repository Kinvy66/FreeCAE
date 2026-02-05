/**
 * @file FCAbstractAssInstance.cpp
 * @brief 装配体实例实现（移植自 FITKInterfaceModel）
 */
#include "FCAbstractAssInstance.h"
#include "FCAbstractModel.h"
#include <FCData/FCDataRepo.h>
#include <FCData/FCVec3Alg.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace FC {

FCAbstractAssInstance::FCAbstractAssInstance()
{}

FCAbstractAssInstance::FCAbstractAssInstance(int modelId)
    : _modelID(modelId)
{}

FCAbstractAssInstance::FCAbstractAssInstance(int modelId, double* xyz)
    : _modelID(modelId)
{
    if (xyz) {
        _translation[0] = xyz[0];
        _translation[1] = xyz[1];
        _translation[2] = xyz[2];
    }
}

FCAbstractAssInstance::FCAbstractAssInstance(int modelId, double* pt1, double* pt2, double angle)
    : _modelID(modelId)
    , _rotationAngle(angle * M_PI / 180.0)
{
    if (pt1) { _rotationPt1[0] = pt1[0]; _rotationPt1[1] = pt1[1]; _rotationPt1[2] = pt1[2]; }
    if (pt2) { _rotationPt2[0] = pt2[0]; _rotationPt2[1] = pt2[1]; _rotationPt2[2] = pt2[2]; }
}

FCModelEnum::AbsModelType FCAbstractAssInstance::getAbsModelType()
{
    return FCModelEnum::AssInstance;
}

void FCAbstractAssInstance::setModelID(int id)
{
    _modelID = id;
}

int FCAbstractAssInstance::getModelID() const
{
    return _modelID;
}

FCAbstractModel* FCAbstractAssInstance::getModel()
{
    return FCDataRepo::instance()->getDataAs<FCAbstractModel>(_modelID);
}

void FCAbstractAssInstance::setTranslation(double x, double y, double z)
{
    _translation[0] = x;
    _translation[1] = y;
    _translation[2] = z;
}

void FCAbstractAssInstance::getTranslation(double* xyz) const
{
    if (xyz) {
        xyz[0] = _translation[0];
        xyz[1] = _translation[1];
        xyz[2] = _translation[2];
    }
}

void FCAbstractAssInstance::setRotation(double* pt1, double* pt2, double angle)
{
    if (pt1) { _rotationPt1[0] = pt1[0]; _rotationPt1[1] = pt1[1]; _rotationPt1[2] = pt1[2]; }
    if (pt2) { _rotationPt2[0] = pt2[0]; _rotationPt2[1] = pt2[1]; _rotationPt2[2] = pt2[2]; }
    _rotationAngle = angle * M_PI / 180.0;
}

void FCAbstractAssInstance::getRotation(double* pt1, double* pt2, double* angle) const
{
    if (pt1) { pt1[0] = _rotationPt1[0]; pt1[1] = _rotationPt1[1]; pt1[2] = _rotationPt1[2]; }
    if (pt2) { pt2[0] = _rotationPt2[0]; pt2[1] = _rotationPt2[1]; pt2[2] = _rotationPt2[2]; }
    if (angle) *angle = _rotationAngle * 180.0 / M_PI;
}

void FCAbstractAssInstance::transformPoint(double* iPos, double* oPos)
{
    FCAbstractModel* model = getModel();
    if (!model) {
        for (int i = 0; i < 3; i++) oPos[i] = iPos[i] + _translation[i];
        return;
    }
    double tmp[3];
    model->transformPoint(iPos, tmp);
    // 绕轴(pt1->pt2)旋转：先平移到轴过原点，旋转后再平移回
    double px = tmp[0] - _rotationPt1[0];
    double py = tmp[1] - _rotationPt1[1];
    double pz = tmp[2] - _rotationPt1[2];
    FCPoint p(px, py, pz);
    FCPoint axis(_rotationPt2[0] - _rotationPt1[0], _rotationPt2[1] - _rotationPt1[1], _rotationPt2[2] - _rotationPt1[2]);
    FCPoint r = RotateAroundAxis(p, axis, _rotationAngle);
    oPos[0] = r.x() + _rotationPt1[0] + _translation[0];
    oPos[1] = r.y() + _rotationPt1[1] + _translation[1];
    oPos[2] = r.z() + _rotationPt1[2] + _translation[2];
}

void FCAbstractAssInstance::transformDirection(double* iDir, double* oDir)
{
    FCAbstractModel* model = getModel();
    if (!model) {
        for (int i = 0; i < 3; i++) oDir[i] = iDir[i];
        return;
    }
    model->transformDirection(iDir, oDir);
    FCPoint p(oDir[0], oDir[1], oDir[2]);
    FCPoint axis(_rotationPt2[0] - _rotationPt1[0], _rotationPt2[1] - _rotationPt1[1], _rotationPt2[2] - _rotationPt1[2]);
    FCPoint r = RotateAroundAxis(p, axis, _rotationAngle);
    oDir[0] = r.x();
    oDir[1] = r.y();
    oDir[2] = r.z();
}

bool FCAbstractAssInstance::getPointCoor(int pointID, double* coor, int modelIndex)
{
    Q_UNUSED(modelIndex);
    FCAbstractModel* model = getModel();
    if (!model || !coor) return false;
    if (!model->getPointCoor(pointID, coor, 0)) return false;
    double out[3];
    transformPoint(coor, out);
    coor[0] = out[0]; coor[1] = out[1]; coor[2] = out[2];
    return true;
}

} // namespace FC
