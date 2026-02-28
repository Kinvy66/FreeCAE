/**
 * @file FCTorusInfoWidget.h
 * @brief 圆环属性与构建
 */
#ifndef FCTORUSINFOWIDGET_H
#define FCTORUSINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include "IGeometryBuildContent.h"
#include <FCData/FCType.h>
#include <QWidget>

namespace Ui {
class FCTorusInfoWidget;
}

namespace FC {

class FCGeoModelTorus;
class FCAbsGeoCommand;
class FCGeometryDAGData;

class FCGUIWIDGET_API FCTorusInfoWidget : public QWidget, public IGeometryBuildContent
{
    Q_OBJECT

public:
    explicit FCTorusInfoWidget(QWidget* parent = nullptr);
    ~FCTorusInfoWidget() override;

    void setTorusCommand(FCGeoModelTorus* torusCmd);
    void setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelTorus* displayTorusCmd = nullptr);

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

    Ui::FCTorusInfoWidget* ui;
    bool mNameManuallyEdited{ false };
    FCGeoModelTorus* mTorusCmd{ nullptr };
    FCGeometryDAGData* mDagData{ nullptr };
    FCID mNodeId{ FCID_INVALID };
    FCGeoModelTorus* mDisplayTorusCmd{ nullptr };
    bool mOwnDisplayTorusCmd{ false };
};

} // namespace FC

#endif // FCTORUSINFOWIDGET_H
