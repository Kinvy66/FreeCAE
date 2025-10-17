#include "MainWindow.h"

#if !SARIBBON_USE_3RDPARTY_FRAMELESSHELPER
#include "SAFramelessHelper.h"
#endif

#include "SARibbonApplicationButton.h"
#include "SARibbonBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonCategory.h"
#include <QCheckBox>
#include "SARibbonColorToolButton.h"
#include <QComboBox>
#include "SARibbonCustomizeDialog.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonGallery.h"

#include <spdlog/spdlog.h>
 #include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : SARibbonMainWindow(parent)
{
    this->setWindowTitle(("新能源电池全生命周期数值模拟软件"));
    
     SARibbonBar* ribbon = ribbonBar();
    
    m_app = new AppWidget(this);
    this->setCentralWidget(m_app);
    
    // 普通打印
    spdlog::info("Welcome to info spdlog!");
    
    this->resize(800, 600);   // 窗口大小 800x600
    

    
}

MainWindow::~MainWindow() {}
