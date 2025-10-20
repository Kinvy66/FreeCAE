/******************************************************************************
 * File     : MainWindow.cpp
 * Brief    : Rrinbon 菜单区和主窗口创建
 * Author   : Kinvy
 * Email    : kinvy66@163.com
 * Date     : 2025-10-19
 * Version  : V1.0.0
 * Detail   :
 * Copyright: Copyright By Kinvy, All Rights Reserved
******************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AppWidget.h"
#include "SARibbonMainWindow.h"

class SARibbonCategory;
class SARibbonActionsManager;

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    // 创建ribbon-application菜单示例
    void createApplicationButton();
    
    // 创建菜单Tab
    void createCategoryHome(SARibbonCategory* page);
    void createCategoryDefine(SARibbonCategory* page);
    void createCategoryGeometry(SARibbonCategory* page);
    void createCategoryMaterial(SARibbonCategory* page);
    void createCategoryPhysic(SARibbonCategory* page);
    void createCategoryGrid(SARibbonCategory* page);
    void createCategoryStudy(SARibbonCategory* page);
    void createCategoryResult(SARibbonCategory* page);
    void createCategoryTool(SARibbonCategory* page);
    
    // 创建ActionsManager，实现actions的管理以及SARibbonBar的自定义
    void createActionsManager();
    
    
    QAction* createAction(const QString& text, const QString& iconurl, const QString& objName);
    QAction* createAction(const QString& text, const QString& iconurl);
    QAction* createAction(const QString& text, const QString& iconurl, QKeySequence shortcut);
    
private Q_SLOTS:
    // void appDev
    
private:
    AppWidget* m_app;
    QMenu* m_MenuApplicationBtn { nullptr };
    SARibbonActionsManager* m_ActionsManager { nullptr };
};
#endif // MAINWINDOW_H
