/**
 * @file FCVTKShapeColorMap.h
 * @brief 模型顶点/边/面的颜色数组管理（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKShapeColorMap）
 */
#ifndef FCVTKSHAPECOLORMAP_H
#define FCVTKSHAPECOLORMAP_H

#include "FCVTKGraphAdaptorAPI.h"
#include <QObject>
#include <QHash>
#include <QList>
#include <QColor>

class vtkDataSet;
class vtkDataArray;
class vtkUnsignedCharArray;

namespace FC {

class FCVTKGRAPHADAPTOR_API FCVTKShapeColorMap : public QObject
{
    Q_OBJECT
public:
    FCVTKShapeColorMap() = default;
    ~FCVTKShapeColorMap() override;

    vtkDataArray* getMappedArray(const QString& name);
    void setInputData(vtkDataSet* dataSet);
    void setCellsColor(const QString& name, const QList<int>& indice, QColor color);
    void setColor(const QString& name, QColor color);
    void update(bool forceUpdate = false);
    bool needUpdate() const { return m_needUpdate; }
    void clear();

private:
    vtkDataSet* m_dataSet = nullptr;
    QHash<QString, vtkUnsignedCharArray*> m_colorArrayHash;
    bool m_needUpdate = false;
};

} // namespace FC

#endif // FCVTKSHAPECOLORMAP_H
