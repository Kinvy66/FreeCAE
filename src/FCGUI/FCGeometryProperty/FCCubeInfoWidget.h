#ifndef FCCUBEINFOWIDGET_H
#define FCCUBEINFOWIDGET_H

#include "../FCGUIAPI.h"
#include <QWidget>

namespace Ui {
class FCCubeInfoWidget;
}

namespace FC {
class FCGeoModelBox;
}

class FCGUI_API FCCubeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FCCubeInfoWidget(QWidget *parent = nullptr);
    ~FCCubeInfoWidget();

    /** 根据 boxCmd 将基点与长宽高设置到 UI 对应控件 */
    void setBoxCommand(FC::FCGeoModelBox* boxCmd);

private:
    Ui::FCCubeInfoWidget *ui;
};

#endif // FCCUBEINFOWIDGET_H
