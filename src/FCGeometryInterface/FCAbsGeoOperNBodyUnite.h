/**
 * @file FCGeoOperNBodyUnite.h
 * @brief 多实体布尔求和命令（移植自 FITKInterfaceGeometry FITKAbsGeoOperNBodyUnite）
 */
#ifndef FCGEOOPERNBODYUNITE_H
#define FCGEOOPERNBODYUNITE_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <QList>

namespace FC {

/**
 * @brief 多实体布尔求和命令
 */
class FCGEOMETRYINTERFACE_API FCGeoOperNBodyUnite : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoOperNBodyUnite);
    Q_OBJECT
public:
    explicit FCGeoOperNBodyUnite() = default;
    ~FCGeoOperNBodyUnite() override = default;

    FCGeoEnum::FCGeometryComType getGeometryCommandType() override;
    bool update() override;

    void addShape(const VirtualShape& shape);
    const QList<VirtualShape>& getShapes() const { return m_shapes; }
    void clearShapes();

protected:
    QList<VirtualShape> m_shapes;
};

} // namespace FC

#endif // FCGEOOPERNBODYUNITE_H
