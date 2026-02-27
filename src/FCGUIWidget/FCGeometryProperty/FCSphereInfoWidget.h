/**
 * @file FCSphereInfoWidget.h
 * @brief 球体属性与构建（参考 FCCubeInfoWidget，实现 IGeometryBuildContent）
 */
#ifndef FCSPHEREINFOWIDGET_H
#define FCSPHEREINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include "IGeometryBuildContent.h"
#include <FCData/FCType.h>
#include <QWidget>
#include <QList>

namespace Ui {
class FCSphereInfoWidget;
}

namespace FC {

class FCGeoModelSphere;
class FCAbsGeoCommand;
class FCGeometryDAGData;

class FCGUIWIDGET_API FCSphereInfoWidget : public QWidget, public IGeometryBuildContent
{
    Q_OBJECT

public:
    explicit FCSphereInfoWidget(QWidget* parent = nullptr);
    ~FCSphereInfoWidget() override;

    void setSphereCommand(FCGeoModelSphere* sphereCmd);
    void setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelSphere* displaySphereCmd = nullptr);

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

    Ui::FCSphereInfoWidget* ui;
    bool mNameManuallyEdited{ false };
    FCGeoModelSphere* mSphereCmd{ nullptr };
    FCGeometryDAGData* mDagData{ nullptr };
    FCID mNodeId{ FCID_INVALID };
    FCGeoModelSphere* mDisplaySphereCmd{ nullptr };
    bool mOwnDisplaySphereCmd{ false };
};

} // namespace FC

#endif // FCSPHEREINFOWIDGET_H
