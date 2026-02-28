/**
 * @file FCConeInfoWidget.h
 * @brief 圆锥/圆台属性与构建
 */
#ifndef FCCONEINFOWIDGET_H
#define FCCONEINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include "IGeometryBuildContent.h"
#include <FCData/FCType.h>
#include <QWidget>

namespace Ui {
class FCConeInfoWidget;
}

namespace FC {

class FCGeoModelCone;
class FCAbsGeoCommand;
class FCGeometryDAGData;

class FCGUIWIDGET_API FCConeInfoWidget : public QWidget, public IGeometryBuildContent
{
    Q_OBJECT

public:
    explicit FCConeInfoWidget(QWidget* parent = nullptr);
    ~FCConeInfoWidget() override;

    void setConeCommand(FCGeoModelCone* coneCmd);
    void setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelCone* displayConeCmd = nullptr);

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

    Ui::FCConeInfoWidget* ui;
    bool mNameManuallyEdited{ false };
    FCGeoModelCone* mConeCmd{ nullptr };
    FCGeometryDAGData* mDagData{ nullptr };
    FCID mNodeId{ FCID_INVALID };
    FCGeoModelCone* mDisplayConeCmd{ nullptr };
    bool mOwnDisplayConeCmd{ false };
};

} // namespace FC

#endif // FCCONEINFOWIDGET_H
