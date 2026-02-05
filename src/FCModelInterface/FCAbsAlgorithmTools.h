/**
 * @file FCAbsAlgorithmTools.h
 * @brief 抽象算法工具（移植自 FITKInterfaceModel）
 */
#ifndef FCABSALGORITHMTOOLS_H
#define FCABSALGORITHMTOOLS_H

#include "FCModelInterfaceAPI.h"
#include <FCData/FCAbstractObject.hpp>
#include <QObject>

namespace FC {

class FCMODELINTERFACE_API FCAbstractTransformTool : public QObject, public FCAbstractObject
{
    Q_OBJECT
public:
    explicit FCAbstractTransformTool(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~FCAbstractTransformTool() = default;
};

class FCMODELINTERFACE_API FCAbsAlgorithmToolsCreator : public QObject
{
    Q_OBJECT
public:
    explicit FCAbsAlgorithmToolsCreator(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~FCAbsAlgorithmToolsCreator() = default;
};

} // namespace FC

#endif // FCABSALGORITHMTOOLS_H
