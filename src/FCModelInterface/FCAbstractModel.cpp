/**
 * @file FCAbstractModel.cpp
 * @brief 模型抽象类实现（移植自 FITKInterfaceModel）
 */
#include "FCAbstractModel.h"
#include "FCComponentManager.h"

namespace FC {

FCAbstractModel::~FCAbstractModel()
{}

FCModelEnum::AbsModelType FCAbstractModel::getAbsModelType()
{
    return FCModelEnum::AMTNone;
}

FCComponentManager* FCAbstractModel::getComponentManager()
{
    return nullptr;
}

bool FCAbstractModel::writeToFile(const QString& file)
{
    Q_UNUSED(file);
    return false;
}

FCModelEnum::FITKModelDim FCAbstractModel::getModelDim()
{
    return FCModelEnum::FMDNone;
}

bool FCAbstractModel::getPointCoor(int pointID, double* coor, int modelIndex)
{
    Q_UNUSED(pointID);
    Q_UNUSED(coor);
    Q_UNUSED(modelIndex);
    return false;
}

void FCAbstractModel::transformPoint(double* iPos, double* oPos)
{
    for (int i = 0; i < 3; i++)
        oPos[i] = iPos[i];
}

void FCAbstractModel::transformDirection(double* iDir, double* oDir)
{
    for (int i = 0; i < 3; i++)
        oDir[i] = iDir[i];
}

void FCAbstractModel::CopyArray(double* from, double* to, int dim)
{
    for (int i = 0; i < dim; ++i)
        to[i] = from[i];
}

} // namespace FC
