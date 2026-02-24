#ifndef FCCUBEINFOWIDGET_H
#define FCCUBEINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include <QWidget>

namespace Ui {
class FCCubeInfoWidget;
}

namespace FC {
class FCGeoModelBox;
}

class FCGUIWIDGET_API FCCubeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FCCubeInfoWidget(QWidget *parent = nullptr);
    ~FCCubeInfoWidget();

    /** 根据 boxCmd 将基点与长宽高设置到 UI 对应控件，并保存引用供「构建」按钮使用 */
    void setBoxCommand(FC::FCGeoModelBox* boxCmd);

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
