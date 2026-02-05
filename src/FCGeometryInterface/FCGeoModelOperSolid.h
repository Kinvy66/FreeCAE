/**
 * @file FCGeoModelOperSolid.h
 * @brief 体特征处理命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoModelOperSolid）
 */
#ifndef FCGEOMODELOPERSOLID_H
#define FCGEOMODELOPERSOLID_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoModelSolid.h"
#include <QList>

namespace FC {

/**
 * @brief 倒角
 */
class FCGEOMETRYINTERFACE_API FCGeoModelChamferSolid : public FCGeoModelSolid
{
    FC_CLASS(FC, FCGeoModelChamferSolid);
public:
    FCGeoModelChamferSolid() = default;
    ~FCGeoModelChamferSolid() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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
class FCGEOMETRYINTERFACE_API FCGeoModelFilletSolid : public FCGeoModelSolid
{
    FC_CLASS(FC, FCGeoModelFilletSolid);
public:
    FCGeoModelFilletSolid() = default;
    ~FCGeoModelFilletSolid() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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
class FCGEOMETRYINTERFACE_API FCGeoModelDefeatureSolid : public FCGeoModelSolid
{
    FC_CLASS(FC, FCGeoModelDefeatureSolid);
public:
    FCGeoModelDefeatureSolid() = default;
    ~FCGeoModelDefeatureSolid() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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
class FCGEOMETRYINTERFACE_API FCGeoModelRemoveChamferSolid : public FCGeoModelDefeatureSolid
{
    FC_CLASS(FC, FCGeoModelRemoveChamferSolid);
public:
    FCGeoModelRemoveChamferSolid() = default;
    ~FCGeoModelRemoveChamferSolid() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/**
 * @brief 删除倒圆
 */
class FCGEOMETRYINTERFACE_API FCGeoModelRemoveFilletSolid : public FCGeoModelDefeatureSolid
{
    FC_CLASS(FC, FCGeoModelRemoveFilletSolid);
public:
    FCGeoModelRemoveFilletSolid() = default;
    ~FCGeoModelRemoveFilletSolid() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

/**
 * @brief 填补孔洞
 */
class FCGEOMETRYINTERFACE_API FCGeoModelFillHoleSolid : public FCGeoModelDefeatureSolid
{
    FC_CLASS(FC, FCGeoModelFillHoleSolid);
public:
    FCGeoModelFillHoleSolid() = default;
    ~FCGeoModelFillHoleSolid() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;
};

} // namespace FC

#endif // FCGEOMODELOPERSOLID_H
