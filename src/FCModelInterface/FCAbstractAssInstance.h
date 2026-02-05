/**
 * @file FCAbstractAssInstance.h
 * @brief 抽象装配体实例（移植自 FITKInterfaceModel）
 */
#ifndef FCABSTRACTASSINSTANCE_H
#define FCABSTRACTASSINSTANCE_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModel.h"
#include "FCModelEnum.h"
#include <FCData/FCMacros.h>
#include <QList>

namespace FC {

class FCAbstractModel;

class FCMODELINTERFACE_API FCAbstractAssInstance : public FCAbstractModel
{
    FC_CLASS(FC, FCAbstractAssInstance);
public:
    explicit FCAbstractAssInstance();
    explicit FCAbstractAssInstance(int modelId);
    FCAbstractAssInstance(int modelId, double* xyz);
    FCAbstractAssInstance(int modelId, double* pt1, double* pt2, double angle);
    virtual ~FCAbstractAssInstance() = default;

    virtual FCModelEnum::AbsModelType getAbsModelType() override;
    void setModelID(int id);
    int getModelID() const;
    FCAbstractModel* getModel();

    void setTranslation(double x, double y, double z);
    void getTranslation(double* xyz) const;
    void setRotation(double* pt1, double* pt2, double angle);
    void getRotation(double* pt1, double* pt2, double* angle) const;
    virtual void transformPoint(double* iPos, double* oPos) override;
    virtual void transformDirection(double* iDir, double* oDir) override;
    virtual bool getPointCoor(int pointID, double* coor, int modelIndex = 0) override;

protected:
    int _modelID{ -1 };
    double _translation[3]{ 0, 0, 0 };
    double _rotationPt1[3]{ 0, 0, 0 };
    double _rotationPt2[3]{ 0, 0, 1 };
    double _rotationAngle{ 0 };
};

} // namespace FC

#endif // FCABSTRACTASSINSTANCE_H
