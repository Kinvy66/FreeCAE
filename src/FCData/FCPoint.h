/**
 * @file FCPoint.h
 * @brief 三维点与向量（替代 FITKVec3D/FITKPoint，不依赖 FITKCore）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCPOINT_H
#define FCPOINT_H

#include "FCDataAPI.h"
#include "FCAbstractObject.hpp"
#include <QObject>
#include <cmath>

namespace FC {

class FCDATA_API FCPoint : public FCAbstractObject
{
public:
    FCPoint();
    FCPoint(double x, double y, double z);
    FCPoint(double* c);
    FCPoint(const FCPoint& point);

    double x() const;
    double y() const;
    double z() const;
    void getCoor(double* c) const;
    void setCoor(double x, double y, double z);
    void operator=(const FCPoint& p);
    double operator[](int index) const;

protected:
    double _coor[3] = {0, 0, 0};
};

class FCDATA_API FCNode : public QObject, public FCPoint
{
    Q_OBJECT
public:
    using FCPoint::FCPoint;
    FCNode() = default;
    FCNode(int id, double x, double y, double z);
    FCNode(int id, const double* coor);
    ~FCNode() override;

    void setNodeID(int id);
    int getNodeID() const;
    void setTag(unsigned int tag);
    unsigned int getTag() const;
    void setNativeFlag(bool native);
    bool getNativeFlag() const;

signals:
    void nodeDestroiedSignal(FCNode* node);

private:
    int _nodeID = -1;
    bool _nativeFlag = false;
    unsigned int _tag = 0;
};

class FCDATA_API FCVec3 : public FCPoint
{
public:
    using FCPoint::FCPoint;
    FCVec3(const FCPoint& point);
    FCVec3(const FCPoint& endPt, const FCPoint& startPt);
    double getNorm() const;
    void normalize();
};

} // namespace FC

#endif // FCPOINT_H
