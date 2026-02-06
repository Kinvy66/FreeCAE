/**
 * @file FCGeoReplaceFaces.h
 * @brief 替换面命令（移植自 FITKInterfaceGeometry FITKAbsGeoReplaceFaces）
 */
#ifndef FCGEOREPLACEFACES_H
#define FCGEOREPLACEFACES_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 替换面
 */
class FCGEOMETRYINTERFACE_API FCGeoReplaceFaces : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoReplaceFaces);
public:
    FCGeoReplaceFaces() = default;
    ~FCGeoReplaceFaces() override = default;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    QList<VirtualShape> faces() const { return m_faces; }
    void setFaces(const QList<VirtualShape>& faces) { m_faces = faces; }
    bool isExtend() const { return m_extendNeighboringFace; }
    void setExtend(bool extend) { m_extendNeighboringFace = extend; }
protected:
    QList<VirtualShape> m_faces;
    bool m_extendNeighboringFace{};
};

} // namespace FC

#endif // FCGEOREPLACEFACES_H
