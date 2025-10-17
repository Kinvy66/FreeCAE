#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"


namespace Ui {
class AppWidget;
}

class AppWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppWidget(QWidget *parent = nullptr);
    ~AppWidget();

private:
    Ui::AppWidget *ui;
    
    ads::CDockManager* DockManager;
    ads::CDockAreaWidget* StatusDockArea;
    ads::CDockWidget* TimelineDockWidget;
};

#endif // APPWIDGET_H
