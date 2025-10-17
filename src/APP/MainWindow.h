#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AppWidget.h"
#include "SARibbonMainWindow.h"


class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    AppWidget* m_app;
};
#endif // MAINWINDOW_H
