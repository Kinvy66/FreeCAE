/**
 * @file FCVTKGraphObject3D.h
 * @brief VTK 3D 图形对象基类（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKGraphObject3D）
 */
#ifndef FCVTKGRAPHOBJECT3D_H
#define FCVTKGRAPHOBJECT3D_H

#include "FCVTKGraphAdaptorAPI.h"
#include "FCVTKCommons.h"
#include <FCRenderWindowVTK/FCGraphObjectVTK.h>
#include <FCData/FCAbstractDataObject.h>
#include <QHash>
#include <QList>
#include <vtkObject.h>

class vtkDataSet;
class vtkProp;

namespace FC {

class FCVTKGRAPHADAPTOR_API FCVTKGraphObject3D : public FCGraphObjectVTK
{
    Q_OBJECT
public:
    explicit FCVTKGraphObject3D(FCAbstractDataObject* dataObj = nullptr);
    ~FCVTKGraphObject3D() override;

    virtual void setViewMode(FCVTKCommons::ShapeMeshViewMode type, bool visible);
    virtual void setTransparent(bool isOn);
    virtual void setColor(QColor color, FCVTKCommons::ShapeType type, int index);
    virtual void setColor(QColor color);
    virtual int getNumberOf(FCVTKCommons::ShapeType type);
    virtual void setPickMode(FCVTKCommons::ShapePickMode mode);
    virtual int getShapeIdByVTKCellId(int vtkCellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType);
    virtual const QVector<int> getVTKCellIdsByShapeId(int shapeId, FCVTKCommons::ShapeAbsEnum topAbsShapeType);
    virtual const QVector<int> getVTKCellIdsByVTKCellId(int cellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType);
    virtual vtkDataSet* getMesh(FCVTKCommons::ShapeType type);
    virtual void setVisible(bool visibility);
    virtual void updateVisibility();
    virtual void update(bool forceUpdate = false) override;
    virtual void highlight(FCVTKCommons::ShapeType type = FCVTKCommons::ShapeTypeNone, QColor color = QColor());
    virtual void disHighlight();
    virtual void advanceHighlight(FCVTKCommons::ShapeType type, QVector<int> indice, QColor color = QColor());
    virtual void disAdvanceHighlight();

    int getDataId() const;
    FCVTKCommons::ShapeInfo getShapeInfo() const;
    bool isHighlighting() const { return m_isHighlighting; }
    bool isAdvancedHighlighting() const { return m_isAdvHighlighting; }

signals:
    void sig_needToBeDeleted();

protected:
    virtual void setRenderLayer(int layer, int layerHighlight = -1);
    virtual bool getDataVisibility() const;
    template <class T>
    void deleteVtkObj(T*& obj) {
        if (obj) { obj->Delete(); obj = nullptr; }
    }
    template <class T> void deleteObj(T*& obj) {
        if (obj) { delete obj; obj = nullptr; }
    }
    template <class T> void resetVtkObj(T* obj) {
        if (obj) obj->Reset();
    }

protected:
    QHash<FCVTKCommons::ShapeMeshViewMode, bool> m_viewModes;
    FCVTKCommons::ShapeInfo m_shapeInfo;
    FCVTKCommons::ShapePickMode m_pickMode = FCVTKCommons::PickNone;
    double m_polygonOffset = 0.;
    int m_layer = 0;
    int m_layerHighlight = 0;
    bool m_isHighlighting = false;
    bool m_isAdvHighlighting = false;
    QList<FCVTKGraphObject3D*> m_addinGraphObjList;
};

} // namespace FC

#endif // FCVTKGRAPHOBJECT3D_H
