/**
 * @file FCVTKCommons.h
 * @brief VTK 图形通用枚举与颜色常量（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKCommons）
 */
#ifndef FCVTKCOMMONS_H
#define FCVTKCOMMONS_H

#include "FCVTKGraphAdaptorAPI.h"
#include <QColor>
#include <QList>

namespace FC {

class FCVTKGRAPHADAPTOR_API FCVTKCommons
{
public:
    enum ShapeAbsEnum
    {
        STA_COMPOUND,
        STA_COMPSOLID,
        STA_SOLID,
        STA_SHELL,
        STA_FACE,
        STA_WIRE,
        STA_EDGE,
        STA_VERTEX,
        STA_SHAPE
    };

    enum ShapeMeshViewMode
    {
        SMVM_Shade = 0,
        SMVM_Wireframe,
        SMVM_Vertex,
    };

    enum ShapePickMode
    {
        PickNone = -1,
        PickVertex = 0,
        PickEdge,
        PickFace,
        PickSolid,
        PickAll
    };

    enum ShapeDataType
    {
        OtherShape = -1,
        ModelShape = 0,
        MeshShape = 1
    };

    enum ShapeType
    {
        ShapeTypeNone = -1,
        ModelVertex = 0,
        ModelEdge,
        ModelFace,
        ModelSolid,
        MeshNode,
        MeshElement,
        Others
    };

    /** 测量工具类型（用于 FCVTKGraphObjectMeasureTools） */
    enum MeasureType
    {
        MT_None = -1,
        MT_Distance = 0,
        MT_Angle
    };

    struct ShapeInfo
    {
        ShapeDataType Type = OtherShape;
        int DataObjId = -1;
    };

    static QColor GetColorByIndex(int index);
    static QColor GetRandomColor();
    static void QColorToDouble3(QColor color, double* rgbf);
    static void QColorToInt3(QColor color, int* rgb);

    static QList<QColor> s_colorList;
    static QColor s_geoVertexColor;
    static QColor s_geoEdgeColor;
    static QColor s_geoFaceColor;
    static QColor s_regionMeshFaceColor;
    static QColor s_regionMeshEdgeColor;
    static double s_highlightPointSize;
    static double s_highlightLineWidth;
    static double s_transparency;
    static double s_polygonOffsetGeom;
    static double s_polygonOffsetMesh;
};

} // namespace FC

#endif // FCVTKCOMMONS_H
