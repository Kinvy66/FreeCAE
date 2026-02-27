/**
 * @file FCVTKGraphObject3D.cpp
 * @brief FCVTKGraphObject3D 实现
 */
#include "FCVTKGraphObject3D.h"
#include <FCData/FCAbstractDataObject.h>
#include <vtkObject.h>
#include <vtkProp.h>

namespace FC {

FCVTKGraphObject3D::FCVTKGraphObject3D(FCAbstractDataObject* dataObj)
    : FCGraphObjectVTK(dataObj)
{
    m_viewModes[FCVTKCommons::SMVM_Shade] = true;
    m_viewModes[FCVTKCommons::SMVM_Wireframe] = true;
    m_viewModes[FCVTKCommons::SMVM_Vertex] = true;
    if (dataObj) {
        connect(dataObj, &FCAbstractDataObject::dataObjectDestoried, this, &FCVTKGraphObject3D::sig_needToBeDeleted);
        m_shapeInfo.DataObjId = dataObj->getDataObjectID();
    }
}

FCVTKGraphObject3D::~FCVTKGraphObject3D()
{
    disHighlight();
    for (FCVTKGraphObject3D* o : m_addinGraphObjList)
        delete o;
    m_addinGraphObjList.clear();
}

void FCVTKGraphObject3D::setViewMode(FCVTKCommons::ShapeMeshViewMode type, bool visible)
{
    m_viewModes[type] = visible;
}

void FCVTKGraphObject3D::setTransparent(bool isOn)
{
    Q_UNUSED(isOn);
}

void FCVTKGraphObject3D::setColor(QColor color, FCVTKCommons::ShapeType type, FCID shapeId)
{
    Q_UNUSED(color);
    Q_UNUSED(type);
    Q_UNUSED(shapeId);
}

void FCVTKGraphObject3D::setColor(QColor color)
{
    Q_UNUSED(color);
}

int FCVTKGraphObject3D::getNumberOf(FCVTKCommons::ShapeType type)
{
    Q_UNUSED(type);
    return 0;
}

void FCVTKGraphObject3D::setPickMode(FCVTKCommons::ShapePickMode mode)
{
    m_pickMode = mode;
}

FCID FCVTKGraphObject3D::getShapeIdByVTKCellId(int vtkCellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType)
{
    Q_UNUSED(vtkCellId);
    Q_UNUSED(topAbsShapeType);
    return FCID_INVALID;
}

const QVector<int> FCVTKGraphObject3D::getVTKCellIdsByShapeId(FCID shapeId, FCVTKCommons::ShapeAbsEnum topAbsShapeType)
{
    Q_UNUSED(shapeId);
    Q_UNUSED(topAbsShapeType);
    return QVector<int>();
}

const QVector<int> FCVTKGraphObject3D::getVTKCellIdsByVTKCellId(int cellId, FCVTKCommons::ShapeAbsEnum topAbsShapeType)
{
    Q_UNUSED(cellId);
    Q_UNUSED(topAbsShapeType);
    return QVector<int>();
}

vtkDataSet* FCVTKGraphObject3D::getMesh(FCVTKCommons::ShapeType type)
{
    Q_UNUSED(type);
    return nullptr;
}

void FCVTKGraphObject3D::setVisible(bool visibility)
{
    for (int i = 0; i < getActorCount(); ++i) {
        vtkProp* a = getActor(i);
        if (a) a->SetVisibility(visibility ? 1 : 0);
    }
}

void FCVTKGraphObject3D::updateVisibility()
{
    bool vis = getDataVisibility();
    setVisible(vis);
}

void FCVTKGraphObject3D::update(bool forceUpdate)
{
    Q_UNUSED(forceUpdate);
}

void FCVTKGraphObject3D::highlight(FCVTKCommons::ShapeType type, QColor color)
{
    Q_UNUSED(type);
    Q_UNUSED(color);
    m_isHighlighting = true;
}

void FCVTKGraphObject3D::disHighlight()
{
    m_isHighlighting = false;
}

void FCVTKGraphObject3D::advanceHighlight(FCVTKCommons::ShapeType type, QVector<FCID> indice, QColor color)
{
    Q_UNUSED(type);
    Q_UNUSED(indice);
    Q_UNUSED(color);
    m_isAdvHighlighting = true;
}

void FCVTKGraphObject3D::disAdvanceHighlight()
{
    m_isAdvHighlighting = false;
}

int FCVTKGraphObject3D::getDataId() const
{
    return m_shapeInfo.DataObjId;
}

FCVTKCommons::ShapeInfo FCVTKGraphObject3D::getShapeInfo() const
{
    return m_shapeInfo;
}

void FCVTKGraphObject3D::setRenderLayer(int layer, int layerHighlight)
{
    m_layer = layer;
    m_layerHighlight = layerHighlight >= 0 ? layerHighlight : layer;
}

bool FCVTKGraphObject3D::getDataVisibility() const
{
    FCAbstractDataObject* obj = static_cast<FCAbstractDataObject*>(mDataObject);
    if (!obj) return true;
    QVariant v = obj->getUserData(FCAbstractDataObject::UserRole);
    if (v.isValid() && v.canConvert<bool>())
        return v.toBool();
    return true;
}

} // namespace FC
