#ifndef FCCUBEINFOWIDGET_H
#define FCCUBEINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include <QWidget>
#include <QList>

namespace Ui {
class FCCubeInfoWidget;
}

namespace FC {
class FCGeoModelBox;
class FCAbsGeoCommand;
class FCGeometryDAGData;
}

class FCGUIWIDGET_API FCCubeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FCCubeInfoWidget(QWidget *parent = nullptr);
    ~FCCubeInfoWidget();

    /** 根据 boxCmd 将基点与长宽高设置到 UI 对应控件，并保存引用供「构建」按钮使用 */
    void setBoxCommand(FC::FCGeoModelBox* boxCmd);
    /** COMSOL 式：绑定 DAG 节点；displayBoxCmd 用于「构建」后在 VTK 中显示（可为 nullptr） */
    void setDAGNode(FC::FCGeometryDAGData* dagData, int nodeId, FC::FCGeoModelBox* displayBoxCmd = nullptr);

signals:
    /** 构建了单个几何体，供在 VTK 中显示 */
    void geometryBuilt(FC::FCAbsGeoCommand* cmd);
    /** 按 COMSOL 方式构建了几何序列（从第一个到当前选中），供在 VTK 中刷新显示 */
    void geometrySequenceBuilt(const QList<FC::FCAbsGeoCommand*>& cmds);

private slots:
    /** 构建：将当前 UI 参数写回命令并执行 update，再重建几何实体模型 */
    void onBuildClicked();
    /** 全部构建：对命令列表中所有根命令执行 update，再重建几何实体模型 */
    void onBuildAllClicked();

private:
    void rebuildGeometryEntityModel();

    Ui::FCCubeInfoWidget *ui;
    /** 名称输入框是否被用户手动修改过；未修改时构建不更改自动生成的几何体名称 */
    bool m_nameManuallyEdited{ false };
    FC::FCGeoModelBox* m_boxCmd{ nullptr };
    FC::FCGeometryDAGData* m_dagData{ nullptr };
    int m_nodeId{ -1 };
    /** DAG 路径下仅用于 VTK 显示的 Box 命令（不加入命令列表） */
    FC::FCGeoModelBox* m_displayBoxCmd{ nullptr };
};

#endif // FCCUBEINFOWIDGET_H
