#ifndef FCCUBEINFOWIDGET_H
#define FCCUBEINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include "IGeometryBuildContent.h"
#include <QWidget>
#include <QList>

namespace Ui {
class FCCubeInfoWidget;
}

namespace FC {

class FCGeoModelBox;
class FCAbsGeoCommand;
class FCGeometryDAGData;

class FCGUIWIDGET_API FCCubeInfoWidget : public QWidget, public IGeometryBuildContent
{
    Q_OBJECT

public:
    explicit FCCubeInfoWidget(QWidget *parent = nullptr);
    ~FCCubeInfoWidget() override;

    /** 根据 boxCmd 将基点与长宽高设置到 UI 对应控件 */
    void setBoxCommand(FCGeoModelBox* boxCmd);
    /** COMSOL 式：绑定 DAG 节点；displayBoxCmd 用于「构建」后在 VTK 中显示（可为 nullptr） */
    void setDAGNode(FCGeometryDAGData* dagData, int nodeId, FCGeoModelBox* displayBoxCmd = nullptr);

    void executeBuild() override;
    void executeBuildAll() override;

signals:
    /** 构建了单个几何体，供在 VTK 中显示 */
    void geometryBuilt(FCAbsGeoCommand* cmd);
    /** 按 COMSOL 方式构建了几何序列（从第一个到当前选中），供在 VTK 中刷新显示 */
    void geometrySequenceBuilt(const QList<FCAbsGeoCommand*>& cmds);

private:
    /** COMSOL 式：将当前 UI 上的属性值立即写回 DAG/Box 内存，无需保存或回车 */
    void syncValuesToModel();
    void rebuildGeometryEntityModel();

    Ui::FCCubeInfoWidget *ui;
    /** 名称输入框是否被用户手动修改过；未修改时构建不更改自动生成的几何体名称 */
    bool m_nameManuallyEdited{ false };
    FCGeoModelBox* m_boxCmd{ nullptr };
    FCGeometryDAGData* m_dagData{ nullptr };
    int m_nodeId{ -1 };
    /** DAG 路径下仅用于 VTK 显示的 Box 命令（不加入命令列表） */
    FCGeoModelBox* m_displayBoxCmd{ nullptr };
    /** 若为 true 表示 m_displayBoxCmd 由本 widget 创建并负责释放 */
    bool m_ownDisplayBoxCmd{ false };
};

} // namespace FC

#endif // FCCUBEINFOWIDGET_H
