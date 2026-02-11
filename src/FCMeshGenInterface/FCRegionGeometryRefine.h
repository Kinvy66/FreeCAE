/**
 * @file FCRegionGeometryRefine.h
 * @brief 几何区域加密（移植自 FITKInterfaceMeshGen）
 */
#ifndef FCREGIONGEOMETRYREFINE_H
#define FCREGIONGEOMETRYREFINE_H

#include "FCMeshGenInterfaceAPI.h"
#include <QObject>
#include <QList>

namespace FC {

/**
 * @brief 几何区域加密项
 */
class FCMESHGENINTERFACE_API FCRegionGeometryRefine : public QObject
{
    Q_OBJECT
public:
    enum RefineModelType { RefineDistance, RefineInside, RefineOutside };
    Q_ENUM(RefineModelType);

    explicit FCRegionGeometryRefine(QObject* parent = nullptr);
    virtual ~FCRegionGeometryRefine() override = default;

    void setGeometryId(int id);
    int getGeometryId() const { return _geometryId; }
    void setRefineModel(RefineModelType t);
    RefineModelType getRefineModel() const { return _refineModel; }
    void setDistance(double d);
    double getDistance() const { return _distance; }

private:
    int _geometryId{0};
    RefineModelType _refineModel{RefineDistance};
    double _distance{0.1};
};

/**
 * @brief 几何区域加密管理器
 */
class FCMESHGENINTERFACE_API FCRegionGeometryRefineManager : public QObject
{
    Q_OBJECT
public:
    explicit FCRegionGeometryRefineManager(QObject* parent = nullptr);
    virtual ~FCRegionGeometryRefineManager() override;

    void append(FCRegionGeometryRefine* item);
    void clear();
    int count() const { return _list.size(); }
    FCRegionGeometryRefine* at(int index) const;

private:
    QList<FCRegionGeometryRefine*> _list;
};

} // namespace FC

#endif // FCREGIONGEOMETRYREFINE_H
