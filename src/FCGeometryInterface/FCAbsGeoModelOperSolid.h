/**
 * @file FCGeoModelOperSolid.h
 * @brief 体特征处理命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelOperSolid）
 */
#ifndef FCGEOMODELOPERSOLID_H
#define FCGEOMODELOPERSOLID_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoModelSolid.h"
#include <QList>

namespace FC {

/**
 * @brief 倒角
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelChamferSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelChamferSolid);
public:
    FCAbsGeoModelChamferSolid() = default;
    ~FCAbsGeoModelChamferSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setInputCmdId(int cmdId) { m_solidCmdId = cmdId; }
    int getInputCmdId() const { return m_solidCmdId; }
    void setEdgeTopos(const QList<VirtualShape>& edgeTopos) { m_edgeVirtualTopos = edgeTopos; }
    QList<VirtualShape>& getEdgeTopos() { return m_edgeVirtualTopos; }
    const QList<VirtualShape>& getEdgeTopos() const { return m_edgeVirtualTopos; }
    void setDistance(double dist) { m_dist = dist; }
    double getDistance() const { return m_dist; }
protected:
    int m_solidCmdId{ -1 };
    QList<VirtualShape> m_edgeVirtualTopos;
    double m_dist{};
};

/**
 * @brief 倒圆
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelFilletSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelFilletSolid);
public:
    FCAbsGeoModelFilletSolid() = default;
    ~FCAbsGeoModelFilletSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setInputCmdId(int cmdId) { m_solidCmdId = cmdId; }
    int getInputCmdId() const { return m_solidCmdId; }
    void setEdgeTopos(const QList<VirtualShape>& edgeTopos) { m_edgeVirtualTopos = edgeTopos; }
    QList<VirtualShape>& getEdgeTopos() { return m_edgeVirtualTopos; }
    const QList<VirtualShape>& getEdgeTopos() const { return m_edgeVirtualTopos; }
    void setRadius(double radius) { m_radius = radius; }
    double getRadius() const { return m_radius; }
protected:
    int m_solidCmdId{ -1 };
    QList<VirtualShape> m_edgeVirtualTopos;
    double m_radius{};
};

/**
 * @brief 特征移除基类
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelDefeatureSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelDefeatureSolid);
public:
    FCAbsGeoModelDefeatureSolid() = default;
    ~FCAbsGeoModelDefeatureSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setInputCmdId(int cmdId) { m_solidCmdId = cmdId; }
    int getInputCmdId() const { return m_solidCmdId; }
    void setFaceTopos(const QList<VirtualShape>& faceTopos) { m_faceVirtualTopos = faceTopos; }
    QList<VirtualShape>& getFaceTopos() { return m_faceVirtualTopos; }
    const QList<VirtualShape>& getFaceTopos() const { return m_faceVirtualTopos; }
protected:
    int m_solidCmdId{ -1 };
    QList<VirtualShape> m_faceVirtualTopos;
};

/**
 * @brief 删除倒角
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelRemoveChamferSolid : public FCAbsGeoModelDefeatureSolid
{
    FC_CLASS(FC, FCAbsGeoModelRemoveChamferSolid);
public:
    FCAbsGeoModelRemoveChamferSolid() = default;
    ~FCAbsGeoModelRemoveChamferSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/**
 * @brief 删除倒圆
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelRemoveFilletSolid : public FCAbsGeoModelDefeatureSolid
{
    FC_CLASS(FC, FCAbsGeoModelRemoveFilletSolid);
public:
    FCAbsGeoModelRemoveFilletSolid() = default;
    ~FCAbsGeoModelRemoveFilletSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/**
 * @brief 填补孔洞
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelFillHoleSolid : public FCAbsGeoModelDefeatureSolid
{
    FC_CLASS(FC, FCAbsGeoModelFillHoleSolid);
public:
    FCAbsGeoModelFillHoleSolid() = default;
    ~FCAbsGeoModelFillHoleSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/**
 * @brief 圆孔
 */
class FCGEOMETRYINTERFACE_API FCAbsGeoModelCircularHoleSolid : public FCAbsGeoModelSolid
{
    FC_CLASS(FC, FCAbsGeoModelCircularHoleSolid);
public:
    FCAbsGeoModelCircularHoleSolid() = default;
    ~FCAbsGeoModelCircularHoleSolid() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setHolePoint(double x, double y, double z);
    void getHolePoint(double& x, double& y, double& z) const;
    void setHolePoint(const double* point);
    void getHolePoint(double* point) const;
    void setFlip(bool flip) { _flip = flip; }
    bool getFlip() const { return _flip; }
    void setDiameter(double diameter) { _diameter = diameter; }
    double getDiameter() const { return _diameter; }
    void setOperFace(const VirtualShape& face) { _operFace = face; }
    VirtualShape getOperFace() const { return _operFace; }
    void setOperEdge1(const VirtualShape& edge) { _operEdge1 = edge; }
    VirtualShape getOperEdge1() const { return _operEdge1; }
    void setDistanceLocateHole1(double d) { _distanceLocateHole1 = d; }
    double getDistanceLocateHole1() const { return _distanceLocateHole1; }
    void setOperEdge2(const VirtualShape& edge) { _operEdge2 = edge; }
    VirtualShape getOperEdge2() const { return _operEdge2; }
    void setDistanceLocateHole2(double d) { _distanceLocateHole2 = d; }
    double getDistanceLocateHole2() const { return _distanceLocateHole2; }
    void setInputCmdId(int cmdId) { _inputCmdId = cmdId; }
    int getInputCmdId() const { return _inputCmdId; }
protected:
    int _inputCmdId{ -1 };
    double _holePoint[3]{};
    bool _flip{ false };
    double _diameter{ 0.6 };
    VirtualShape _operFace{};
    VirtualShape _operEdge1{};
    double _distanceLocateHole1{ 2 };
    VirtualShape _operEdge2{};
    double _distanceLocateHole2{ 2 };
};

} // namespace FC

#endif // FCGEOMODELOPERSOLID_H
