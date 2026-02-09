/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCAbstractOCCModel.h
 * @brief OCC 模型基类（移植自 FITKGeoCompOCC FITKAbstractOCCModel）
 */
#ifndef FCABSTRACTOCCMODEL_H
#define FCABSTRACTOCCMODEL_H

#include "FCGeometryCommandAPI.h"
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryInterface/FCAbsGeoShapeAgent.h>
#include <FCModelInterface/FCModelEnum.h>
#include <QVariant>

class TopoDS_Shape;

namespace OCC {

class FCGEOMETRYCMD_API FCAbstractOCCModel : public FC::FCAbsGeoShapeAgent
{
public:
    explicit FCAbstractOCCModel(FC::FCAbsGeoCommand* command);
    ~FCAbstractOCCModel() override;

    static int getShapeDim(const TopoDS_Shape& shape);

    TopoDS_Shape* getShape();
    virtual bool writeToFile(const QString& file);

    int getShapeCount(FC::FCModelEnum::FITKModelSetType type) override;
    const TopoDS_Shape getShape(FC::FCModelEnum::FITKModelSetType type, int id);
    FC::FCGeoEnum::FCGeoEngine getGeoEngine() override;
    FC::FCModelEnum::AbsModelType getAbsModelType() override;
    bool getBoundaryBox(double* minPt, double* maxPt) override;

    void updateShape(const TopoDS_Shape& shape, bool buildVTopo = true);
    void buildVirtualTopo(bool keepTopos = false) override;
    void triangulation() override;
    int getDim() override;
    bool createShapeState(QVariant& stateVal) override;
    bool isSameState(QVariant& stateVal1, QVariant& stateVal2) override;

protected:
    TopoDS_Shape* _shape{};
    bool _buildingTopo{ false };
    int _hashCode{ -1 };
};

/**
 * @brief OCC 形状代理混入类（移植自 FITKGeoCompOCC OCCShapeAgent）
 * 各 OCC 命令类继承 FC 接口 + OCCShapeAgent，在构造函数中令 _shapeAgent = _occShapeAgent。
 */
class FCGEOMETRYCMD_API OCCShapeAgent
{
public:
    explicit OCCShapeAgent(FC::FCAbsGeoCommand* c);
    virtual ~OCCShapeAgent() = default;

protected:
    FCAbstractOCCModel* _occShapeAgent{};
};

} // namespace OCC

#endif // FCABSTRACTOCCMODEL_H
