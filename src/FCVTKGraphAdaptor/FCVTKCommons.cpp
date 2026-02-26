/**
 * @file FCVTKCommons.cpp
 * @brief FCVTKCommons 实现
 */
#include "FCVTKCommons.h"
#include <cstdlib>
#include <ctime>

namespace FC {

QList<QColor> FCVTKCommons::s_colorList = {
    QColor(69, 139, 116), QColor(245, 245, 220), QColor(187, 58, 58),
    QColor(0, 94, 157), QColor(193, 205, 205), QColor(222, 184, 135),
    QColor(177, 198, 237), QColor(139, 136, 120), QColor(233, 150, 122),
    QColor(189, 183, 107), QColor(139, 10, 80), QColor(49, 58, 151),
    QColor(202, 217, 187), QColor(238, 201, 0), QColor(173, 216, 230),
    QColor(110, 123, 139), QColor(139, 71, 137), QColor(238, 232, 170),
    QColor(184, 206, 198), QColor(184, 184, 219), QColor(206, 164, 107),
    QColor(106, 90, 205), QColor(238, 233, 233), QColor(0, 51, 102)
};

QColor FCVTKCommons::s_geoVertexColor = Qt::blue;
QColor FCVTKCommons::s_geoEdgeColor = Qt::black;
QColor FCVTKCommons::s_geoFaceColor = QColor(255, 255, 255);
QColor FCVTKCommons::s_regionMeshFaceColor = Qt::gray;
QColor FCVTKCommons::s_regionMeshEdgeColor = Qt::yellow;
double FCVTKCommons::s_highlightPointSize = 10.;
double FCVTKCommons::s_highlightLineWidth = 3.;
double FCVTKCommons::s_transparency = 0.6;
double FCVTKCommons::s_polygonOffsetGeom = 0.;
double FCVTKCommons::s_polygonOffsetMesh = 2.;

QColor FCVTKCommons::GetColorByIndex(int index)
{
    return s_colorList[index % s_colorList.count()];
}

QColor FCVTKCommons::GetRandomColor()
{
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    return QColor(r, g, b);
}

void FCVTKCommons::QColorToDouble3(QColor color, double* rgbf)
{
    if (!rgbf) return;
    rgbf[0] = color.redF();
    rgbf[1] = color.greenF();
    rgbf[2] = color.blueF();
}

void FCVTKCommons::QColorToInt3(QColor color, int* rgb)
{
    if (!rgb) return;
    rgb[0] = color.red();
    rgb[1] = color.green();
    rgb[2] = color.blue();
}

} // namespace FC
