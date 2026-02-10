/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "FCOCCSampleCurve.h"
#include <FCData/FCDataRepo.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCModelInterface/FCModelEnum.h>
#include <GCPnts_UniformAbscissa.hxx>
#include <GCPnts_QuasiUniformAbscissa.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <cmath>

namespace OCC {

namespace {
    static std::vector<gp_Pnt> sampleParametric(const Handle(Geom_Curve)& curve, double start, double end, int numPoints)
    {
        std::vector<gp_Pnt> points;
        if (numPoints < 2) return points;
        for (int i = 0; i < numPoints; ++i)
        {
            double u = start + (end - start) * i / (numPoints - 1);
            gp_Pnt point;
            curve->D0(u, point);
            points.push_back(point);
        }
        return points;
    }

    static std::vector<gp_Pnt> sampleArcLength(const Handle(Geom_Curve)& curve, double start, double end, int numPoints)
    {
        std::vector<gp_Pnt> points;
        GeomAdaptor_Curve adaptorCurve(curve, start, end);
        GCPnts_UniformAbscissa uniformAbscissa(adaptorCurve, numPoints);
        if (!uniformAbscissa.IsDone()) return points;
        for (int i = 1; i <= numPoints; ++i)
        {
            double u = uniformAbscissa.Parameter(i);
            gp_Pnt point;
            curve->D0(u, point);
            points.push_back(point);
        }
        return points;
    }

    static std::vector<gp_Pnt> sampleChordLength(const Handle(Geom_Curve)& curve, double start, double end, int numPoints)
    {
        std::vector<gp_Pnt> points;
        GeomAdaptor_Curve adaptorCurve(curve, start, end);
        GCPnts_QuasiUniformAbscissa quasiUniformAbscissa(adaptorCurve, numPoints);
        if (!quasiUniformAbscissa.IsDone()) return points;
        for (int i = 1; i <= numPoints; ++i)
        {
            double u = quasiUniformAbscissa.Parameter(i);
            gp_Pnt point;
            curve->D0(u, point);
            points.push_back(point);
        }
        return points;
    }
} // namespace

FCOCCSampleCurve::FCOCCSampleCurve() : OCCShapeAgent(this)
{
    _shapeAgent = _occShapeAgent;
}

FC::FCGeoEnum::FCGeometryComType FCOCCSampleCurve::getGeometryCommandType()
{
    return FC::FCGeoEnum::FGTSampleCurve;
}

bool FCOCCSampleCurve::update()
{
    FC::VirtualShape vs = getSampleCurve();
    if (vs.isNull()) return false;

    FC::FCAbsGeoCommand* cmd = FC::FCDataRepo::instance()->getDataAs<FC::FCAbsGeoCommand>(vs.CmdId);
    if (!cmd) return false;
    FCAbstractOCCModel* occ = cmd->getTShapeAgent<FCAbstractOCCModel>();
    if (!occ) return false;

    TopoDS_Shape s = occ->getShape(FC::FCModelEnum::FMSEdge, vs.VirtualTopoId);
    if (s.IsNull() || s.ShapeType() != TopAbs_EDGE) return false;

    double start = 0.0, end = 0.0;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(s), start, end);
    if (curve.IsNull() || std::fabs(start - end) < 1e-6) return false;

    int n = getSamplePointCount();
    if (n < 2) return false;

    std::vector<gp_Pnt> sampledPoints;
    switch (getSampleMethod())
    {
    case Parametric:
        sampledPoints = sampleParametric(curve, start, end, n);
        break;
    case ArcLength:
        sampledPoints = sampleArcLength(curve, start, end, n);
        break;
    case ChordLength:
        sampledPoints = sampleChordLength(curve, start, end, n);
        break;
    default:
        return false;
    }

    BRep_Builder builder;
    TopoDS_Compound compound;
    builder.MakeCompound(compound);
    for (const auto& point : sampledPoints)
        builder.Add(compound, BRepBuilderAPI_MakeVertex(point).Shape());

    _occShapeAgent->updateShape(compound);
    return true;
}

} // namespace OCC
