/**
 * @file FCHelixInfoWidget.h
 * @brief 螺旋体属性与构建
 */
#ifndef FCHELIXINFOWIDGET_H
#define FCHELIXINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include "IGeometryBuildContent.h"
#include <FCData/FCType.h>
#include <QWidget>

namespace Ui {
class FCHelixInfoWidget;
}

namespace FC {

class FCGeoModelHelix;
class FCAbsGeoCommand;
class FCGeometryDAGData;

class FCGUIWIDGET_API FCHelixInfoWidget : public QWidget, public IGeometryBuildContent
{
    Q_OBJECT

public:
    explicit FCHelixInfoWidget(QWidget* parent = nullptr);
    ~FCHelixInfoWidget() override;

    void setHelixCommand(FCGeoModelHelix* helixCmd);
    void setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelHelix* displayHelixCmd = nullptr);

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

    Ui::FCHelixInfoWidget* ui;
    bool mNameManuallyEdited{ false };
    FCGeoModelHelix* mHelixCmd{ nullptr };
    FCGeometryDAGData* mDagData{ nullptr };
    FCID mNodeId{ FCID_INVALID };
    FCGeoModelHelix* mDisplayHelixCmd{ nullptr };
    bool mOwnDisplayHelixCmd{ false };
};

} // namespace FC

#endif // FCHELIXINFOWIDGET_H
