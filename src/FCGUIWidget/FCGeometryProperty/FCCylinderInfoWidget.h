/**
 * @file FCCylinderInfoWidget.h
 * @brief 圆柱属性与构建（参考 FCCubeInfoWidget，实现 IGeometryBuildContent）
 */
#ifndef FCCYLINDERINFOWIDGET_H
#define FCCYLINDERINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include "IGeometryBuildContent.h"
#include <FCData/FCType.h>
#include <QWidget>
#include <QList>

namespace Ui {
class FCCylinderInfoWidget;
}

namespace FC {

class FCGeoModelCylinder;
class FCAbsGeoCommand;
class FCGeometryDAGData;

class FCGUIWIDGET_API FCCylinderInfoWidget : public QWidget, public IGeometryBuildContent
{
    Q_OBJECT

public:
    explicit FCCylinderInfoWidget(QWidget* parent = nullptr);
    ~FCCylinderInfoWidget() override;

    void setCylinderCommand(FCGeoModelCylinder* cylinderCmd);
    void setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelCylinder* displayCylinderCmd = nullptr);

    void syncToModel() override;
    FCAbsGeoCommand* getCurrentBuildCommand() override;
    void executeBuild() override;
    void executeBuildAll() override;

signals:
    void geometryBuilt(FCAbsGeoCommand* cmd);
    void geometrySequenceBuilt(const QList<FCAbsGeoCommand*>& cmds);

private:
    void syncValuesToModel();
    void rebuildGeometryEntityModel();

    Ui::FCCylinderInfoWidget* ui;
    bool mNameManuallyEdited{ false };
    FCGeoModelCylinder* mCylinderCmd{ nullptr };
    FCGeometryDAGData* mDagData{ nullptr };
    FCID mNodeId{ FCID_INVALID };
    FCGeoModelCylinder* mDisplayCylinderCmd{ nullptr };
    bool mOwnDisplayCylinderCmd{ false };
};

} // namespace FC

#endif // FCCYLINDERINFOWIDGET_H
