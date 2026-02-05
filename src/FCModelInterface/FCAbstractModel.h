/**
 * @file FCAbstractModel.h
 * @brief 模型抽象类声明（移植自 FITKInterfaceModel，依赖 FCData）
 */
#ifndef FCABSTRACTMODEL_H
#define FCABSTRACTMODEL_H

#include "FCModelInterfaceAPI.h"
#include "FCModelEnum.h"
#include <FCData/FCAbstractNamedDataObject.h>
#include <QString>

namespace FC {

class FCComponentManager;  // forward decl to avoid circular include

class FCMODELINTERFACE_API FCAbstractModel : public FCAbstractNamedDataObject
{
public:
    explicit FCAbstractModel() = default;
    virtual ~FCAbstractModel() = 0;

    virtual FCModelEnum::AbsModelType getAbsModelType() = 0;
    virtual FCComponentManager* getComponentManager();
    virtual bool writeToFile(const QString& file);
    virtual FCModelEnum::FITKModelDim getModelDim();
    virtual bool getPointCoor(int pointID, double* coor, int modelIndex = 0);
    virtual void transformPoint(double* iPos, double* oPos);
    virtual void transformDirection(double* iDir, double* oDir);

    static void CopyArray(double* from, double* to, int dim);
};

} // namespace FC

#endif // FCABSTRACTMODEL_H
