/**
 * @file FCGeometryMeshSize.h
 * @brief 几何网格尺寸（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCGEOMETRYMESHSIZE_H
#define FCGEOMETRYMESHSIZE_H

#include "FCMeshGenInterfaceAPI.h"
#include "FCGlobalMeshSizeInfo.h"
#include <QObject>

namespace FC {

/**
 * @brief 几何划分网格尺寸（边/面等）
 */
class FCMESHGENINTERFACE_API FCGeometryMeshSize : public FCGlobalMeshSizeInfo
{
    Q_OBJECT
public:
    explicit FCGeometryMeshSize(QObject* parent = nullptr);
    virtual ~FCGeometryMeshSize() override = default;

    void setGeoGroupComponentId(int id);
    int getGeoGroupComponentId() const { return _geoGroupComponentId; }

    void setLayerNumber(int n);
    int getLayerNumber() const { return _layerNum; }
    void setExpansionRatio(double r);
    double getExpansionRatio() const { return _expansionRatio; }
    void setLayerThickness(double t);
    double getLayerThickness() const { return _layerThickness; }

private:
    int _geoGroupComponentId{0};
    int _layerNum{5};
    double _expansionRatio{1.25};
    double _layerThickness{0.2};
};

/**
 * @brief 几何网格尺寸管理器
 */
class FCMESHGENINTERFACE_API FCGeometryMeshSizeManager : public QObject
{
    Q_OBJECT
public:
    explicit FCGeometryMeshSizeManager(QObject* parent = nullptr);
    virtual ~FCGeometryMeshSizeManager() override;

    void append(FCGeometryMeshSize* item);
    void clear();
    int count() const { return _list.size(); }
    FCGeometryMeshSize* at(int index) const;

private:
    QList<FCGeometryMeshSize*> _list;
};

} // namespace FC

#endif // FCGEOMETRYMESHSIZE_H
