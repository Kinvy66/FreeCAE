/**
 * @file FCVTKShapeColorMap.cpp
 * @brief FCVTKShapeColorMap 实现
 */
#include "FCVTKShapeColorMap.h"
#include <vtkDataSet.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>

namespace FC {

FCVTKShapeColorMap::~FCVTKShapeColorMap()
{
    clear();
    m_dataSet = nullptr;
}

void FCVTKShapeColorMap::clear()
{
    for (vtkUnsignedCharArray* arr : m_colorArrayHash)
        if (arr) arr->Delete();
    m_colorArrayHash.clear();
}

vtkDataArray* FCVTKShapeColorMap::getMappedArray(const QString& name)
{
    return m_colorArrayHash.value(name, nullptr);
}

void FCVTKShapeColorMap::setInputData(vtkDataSet* dataSet)
{
    m_dataSet = dataSet;
}

void FCVTKShapeColorMap::setCellsColor(const QString& name, const QList<int>& indice, QColor color)
{
    if (name.isEmpty() || !m_dataSet) return;

    vtkUnsignedCharArray* colorArray = m_colorArrayHash.value(name, nullptr);
    if (!colorArray) {
        colorArray = vtkUnsignedCharArray::New();
        colorArray->SetName(name.toUtf8().constData());
        colorArray->SetNumberOfComponents(3);
        m_colorArrayHash.insert(name, colorArray);
        m_dataSet->GetCellData()->AddArray(colorArray);
        m_needUpdate = true;
    }

    int nCell = m_dataSet->GetNumberOfCells();
    if (colorArray->GetNumberOfTuples() != nCell) {
        colorArray->SetNumberOfTuples(nCell);
        m_needUpdate = true;
    }

    int r = color.red(), g = color.green(), b = color.blue();
    for (int id : indice) {
        if (!m_needUpdate && (colorArray->GetComponent(id, 0) != r ||
            colorArray->GetComponent(id, 1) != g || colorArray->GetComponent(id, 2) != b))
            m_needUpdate = true;
        colorArray->SetComponent(id, 0, r);
        colorArray->SetComponent(id, 1, g);
        colorArray->SetComponent(id, 2, b);
    }
}

void FCVTKShapeColorMap::setColor(const QString& name, QColor color)
{
    if (name.isEmpty() || !m_dataSet) return;

    vtkUnsignedCharArray* colorArray = m_colorArrayHash.value(name, nullptr);
    if (!colorArray) {
        colorArray = vtkUnsignedCharArray::New();
        colorArray->SetName(name.toUtf8().constData());
        colorArray->SetNumberOfComponents(3);
        m_colorArrayHash.insert(name, colorArray);
        m_dataSet->GetCellData()->AddArray(colorArray);
    }

    int nCell = m_dataSet->GetNumberOfCells();
    if (colorArray->GetNumberOfTuples() != nCell)
        colorArray->SetNumberOfTuples(nCell);
    colorArray->FillComponent(0, color.red());
    colorArray->FillComponent(1, color.green());
    colorArray->FillComponent(2, color.blue());
    m_needUpdate = true;
}

void FCVTKShapeColorMap::update(bool forceUpdate)
{
    if (!m_dataSet) return;
    if (forceUpdate || m_needUpdate)
        m_dataSet->Modified();
    m_needUpdate = false;
}

} // namespace FC
