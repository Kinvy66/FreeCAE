/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCModelSketch2D.cpp
 * @brief OCC 2D 草图命令实现（移植自 FITKGeoCompOCC FITKOCCModelSketch2D）
 * 汇总子命令（线、圆、弧等）的边为 Compound，供扫略/多截面等使用。
 */
#include "FCOCCModelSketch2D.h"
#include "FCAbstractOCCModel.h"
#include "FCOCCGeoSketchDeleter.h"
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCData/FCAbstractDataObject.h>
#include <FCModelInterface/FCModelEnum.h>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <BRep_Builder.hxx>

namespace OCC {

FCOCCModelSketch2D::FCOCCModelSketch2D() : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FCOCCModelSketch2D::~FCOCCModelSketch2D() = default;

FC::FCGeoEnum::FCGeometryComType FCOCCModelSketch2D::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTSketch2D;
}

bool FCOCCModelSketch2D::getShape(TopoDS_Shape& oShape)
{
    oShape = TopoDS_Shape();
    QList<TopoDS_Shape> edges;

    const int nSub = getDataCount();
    for (int i = 0; i < nSub; ++i)
    {
        FC::FCAbsGeoCommand* subCmd = getDataByIndex(i);
        if (!subCmd) continue;

        if (subCmd->getGeometryCommandType() == FC::FCGeoEnum::FGTSketchDeleter)
            continue;

        FCAbstractOCCModel* occ = subCmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ || !occ->getShape() || occ->getShape()->IsNull()) continue;

        const int nEdge = occ->getShapeCount(FC::FCModelEnum::FMSEdge);
        for (int j = 0; j < nEdge; ++j)
        {
            TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, j);
            if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) continue;
            edges.append(s);
        }
    }

    if (edges.isEmpty())
        return true;

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    for (const TopoDS_Shape& s : edges)
        builder.Add(compound, s);
    oShape = compound;
    return true;
}

bool FCOCCModelSketch2D::update()
{
    if (!_occShapeAgent) return false;

    const int nSubCmds = getDataCount();
    bool hasChanged = false;
    if (nSubCmds != m_cmdIdShapeHash.size())
        hasChanged = true;
    else
    {
        for (int i = 0; i < nSubCmds; ++i)
        {
            FC::FCAbsGeoCommand* subCmd = getDataByIndex(i);
            if (!subCmd) continue;
            FCAbstractOCCModel* occ = subCmd->getTShapeAgent<FCAbstractOCCModel>();
            if (!occ) continue;
            QVariant state;
            if (!occ->createShapeState(state)) continue;
            const int id = subCmd->getDataObjectID();
            if (m_cmdIdShapeHash.value(id, -1) != state.toInt())
            {
                hasChanged = true;
                break;
            }
        }
    }

    if (!hasChanged && !m_cmdIdShapeHash.isEmpty())
        return true;

    m_cmdIdShapeHash.clear();
    TopoDS_Shape shape;
    getShape(shape);

    if (nSubCmds == 0 || shape.IsNull())
        _occShapeAgent->updateShape(TopoDS_Shape());
    else
        _occShapeAgent->updateShape(shape);

    for (int i = 0; i < nSubCmds; ++i)
    {
        FC::FCAbsGeoCommand* subCmd = getDataByIndex(i);
        if (!subCmd) continue;
        FCAbstractOCCModel* occ = subCmd->getTShapeAgent<FCAbstractOCCModel>();
        if (!occ) continue;
        QVariant state;
        if (occ->createShapeState(state))
            m_cmdIdShapeHash[subCmd->getDataObjectID()] = state.toInt();
    }
    return true;
}

} // namespace OCC
