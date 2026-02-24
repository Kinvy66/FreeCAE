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
}

class FCGUIWIDGET_API FCCubeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FCCubeInfoWidget(QWidget *parent = nullptr);
    ~FCCubeInfoWidget();

    /** 根据 boxCmd 将基点与长宽高设置到 UI 对应控件，并保存引用供「构建」按钮使用 */
    void setBoxCommand(FC::FCGeoModelBox* boxCmd);

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
    FC::FCGeoModelBox* m_boxCmd{ nullptr };
};

#endif // FCCUBEINFOWIDGET_H
