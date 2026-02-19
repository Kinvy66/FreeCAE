/**
 * @file FCAppController.cpp
 * @brief 控制层负责逻辑的对接
 * @date 2025-10-29
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAppController.h"

// Qt
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QStandardPaths>
#include <QFontComboBox>
#include <QComboBox>
#include <QInputDialog>
#include <QMenu>
#include <QApplication>
#include <QActionGroup>
#include <QDebug>

// API
#include "AppMainWindow.h"
#include "FCAppCore.h"
#include "FCAppRibbonArea.h"
#include "FCAppDockingArea.h"
#include "FCAppCommand.h"
#include "FCAppActions.h"
#include "FCActionEventHandler.h"
// #include "FCAppDataManager.h"
#include "FCProjectInterface.h"
// Qt-Advanced-Docking-System
#include "DockManager.h"
#include "DockAreaWidget.h"
// Widget
// #include "FCSettingParametersWidget.h"
// #include "FCGraphicOperateWidget.h"
// #include "FCModelBuilderWidget.h"
// sub module

#include "FCOperatorRepo.h"
#include <FCMeshGenGmsh/FCGmshMeshGenInterface.h>
#include <FCMeshGenInterface/FCMeshGenInterface.h>
#include <FCMeshGenInterface/FCAbstractMesherDriver.h>
#include <FCMeshGenInterface/FCAbstractMeshProcessor.h>
#include <FCMeshInterface/FCUnstructuredMeshVTK.h>
#include <FCRenderWindowVTK/FCGraphObjectVTK.h>

#include "FCActionCreateCubeOperator.h"

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkNew.h>
#include <vtkProperty.h>

#include <QDir>
#include <QFileInfo>
#include <QSharedPointer>
#include <QTemporaryDir>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include "FCRenderWidget.h"
#include "FCGraph3DWindowVTK.h"
#include "FCGraphObjectVTK.h"
#include "FCOCCGeoCompRegister.h"
#include <FCGeometryCommand/FCAbstractOCCModel.h>
#include <TopoDS_Shape.hxx>
#include <FCVTKGraphAdaptor/FCVTKViewAdaptorModelCmd.h>
#include <FCVTKGraphAdaptor/FCVTKGraphObject3D.h>

#ifndef FCAPPRIBBONAREA_WINDOW_NAME
#define FCAPPRIBBONAREA_WINDOW_NAME QCoreApplication::translate("FCAppController", "FC", nullptr)
#endif

#define FCAPPCONTROLLER_PASS()                                                                                         \
QMessageBox::                                                                                                      \
    warning(app(),                                                                                                 \
            QCoreApplication::translate("FCAppRibbonArea", "warning", nullptr),                                    \
            QCoreApplication::translate("FCAppRibbonArea",                                                         \
                                        "The current function is not implemented, only the UI is reserved, "       \
                                        "please pay attention: https://github.com/Kinvy66/freecaeworkbench",         \
                                        nullptr))

// 快速链接信号槽
#define FCAPPCONTROLLER_ACTION_BIND(actionname, functionname)                                                          \
    connect(actionname, &QAction::triggered, this, &FCAppController::functionname)
    
    namespace FC 
{
FCAppController::FCAppController(QObject* par) : QObject(par)
{
}

FCAppController::~FCAppController()
{
}

/**
* @brief 设置AppMainWindow
* @param mainWindow
* @return 返回自身引用,方便链式调用
*/
FCAppController &FCAppController::setAppMainWindow(AppMainWindow *mainWindow)
{
    mMainWindow = mainWindow;
    return (*this);
}

/**
* @brief 设置core
* @param core
* @return
*/
FCAppController &FCAppController::setAppCore(FCAppCore *core)
{
    mCore    = core;
    mProject = mCore->getProjectInterface();
    return (*this);
}

/**
* @brief 设置ribbon
* @param ribbon
* @return 返回自身引用,方便链式调用
*/
FCAppController &FCAppController::setAppRibbonArea(FCAppRibbonArea *ribbon)
{
    mRibbon = ribbon;
    return (*this);
}

/**
* @brief 设置dock
* @param dock
* @return 返回自身引用,方便链式调用
*/
FCAppController &FCAppController::setAppDockingArea(FCAppDockingArea *dock)
{
    mDock = dock;
    
    return (*this);
}

/**
* @brief 设置AppCommand
* @param cmd
* @return 返回自身引用,方便链式调用
*/
FCAppController &FCAppController::setAppCommand(FCAppCommand *cmd)
{
    mCommand = cmd;
    return (*this);
}

/**
* @brief 设置AppActions
* @param act
* @return 返回自身引用,方便链式调用
*/
FCAppController &FCAppController::setAppActions(FCAppActions *act)
{
    mActions = act;
    return (*this);
}

FCAppController &FCAppController::setActionHandler(FCActionEventHandler *handler)
{
    mActionHandler = handler;
    return (*this);        
}

/**
* @brief 设置app数据管理
* @param d
* @return 
*/
FCAppController &FCAppController::setAppDataManager(FCAppDataManager *d)
{
    mDatas = d;
    return (*this);
}

FCAppController &FCAppController::setDataFactory(FCGlobalDataFactory *d)
{
    mDataFactory = d;
    return (*this);
}

/**
* @brief 获取app
* @return
*/
AppMainWindow *FCAppController::app() const
{
    return mMainWindow;
}

/**
* @brief 控制层初始化
*/
void FCAppController::initialize()
{
    // 注册 OCC 几何命令到工厂，否则 createCommandT<FCGeoModelBox>(FGTBox) 等会返回 nullptr
    OCC::registerOCCGeometryCommands();

    // 注册 Gmsh 网格生成引擎
    FCGmshMeshGenInterface::initialize();

    initConnection();
    registeActionsOperator();
}

/**
* @brief action和slot connect
*/
void FCAppController::initConnection()
{
    
    QList<QAction*> actionList = mActions->getAllActions();
    for (QAction* action : actionList) {
        if (action == nullptr)continue;
        connect(action, &QAction::triggered, mActionHandler, &FCActionEventHandler::execOperator);
    }
    
    // 测试使用
    // connect(mActions->actionCreateCube, &QAction::triggered, this, &FCAppController::testCreatorGeometry);
}

/**
 * @brief 注册action的操作器
 * 
 * 注意：使用 Register2FCOperatorRepo 宏的操作器会在静态初始化时自动注册，
 * 这里主要用于确保操作器仓库已初始化，以及可以添加其他手动注册逻辑。
 */
void FCAppController::registeActionsOperator()
{
    // 确保操作器仓库已初始化（通过访问单例）
    FCOPERATORREPO;
    
    // 如果需要手动注册操作器，可以在这里添加：
    FCOPERATORREPO->registerOperatorFunction("actionCreateCUbe",
                                             []() { return new FCActionCreateCubeOperator; });
    
    // 目前 FCActionCreateCubeOperator 已通过 Register2FCOperatorRepo 宏自动注册
    // 注册键为 "actionCreateCUbe"（注意大小写，与 FCAppActions.cpp 中的 objectName 一致）
}


/**
* @brief 设置工程为脏
*
* @note 如果工程状态已经是脏，此函数不会做任何动作也不会触发任何信号
* @param on
*/
void FCAppController::setDirty(bool on)
{
    // if (mProject) {
    //     mProject->setModified(on);
    // }
}

/**
* @brief 工程是否为脏
* @return
*/
bool FCAppController::isDirty() const
{
    // if (mProject) {
    //     return mProject->isDirty();
    // }
    return false;
}

/**
* @brief 更新窗口标题
*/
void FCAppController::updateWindowTitle()
{
    // FCAppProject* project = FC_APP_CORE.getAppProject();
    // if (!project || project->isEmpty()) {
    //     app()->setWindowTitle(makeWindowTitle());
    //     return;
    // }
    // app()->setWindowTitle(makeWindowTitle(project));
}

/**
* @brief 生成窗口标题
* @return
*/
QString FCAppController::makeWindowTitle()
{
    return QString("%1 [*]").arg(FCAPPRIBBONAREA_WINDOW_NAME);
}

/**
* @brief 生成当前项目下的窗口标题
* @return
*/
QString FCAppController::makeWindowTitle(FCProjectInterface *proj)
{
    return QString("%1 [*]").arg(FCAPPRIBBONAREA_WINDOW_NAME);
}

void FCAppController::testCreatorGeometry()
{
    // ---------- 1. OCC 几何创建并三角化 ----------
    FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
    if (!factory) {
        qWarning() << "testCreatorGeometry: FCGeoInterfaceFactory is null";
        return;
    }

    FCGeoModelBox* boxCmd = factory->createCommandT<FCGeoModelBox>(FCGeoEnum::FGTBox);
    if (!boxCmd) {
        qWarning() << "testCreatorGeometry: Failed to create FCGeoModelBox";
        return;
    }

    double point1[3] = { 0.0, 0.0, 0.0 };
    double length[3] = { 100.0, 100.0, 100.0 };
    boxCmd->setPoint1(point1);
    boxCmd->setLength(length);
    if (!boxCmd->update()) {
        qWarning() << "testCreatorGeometry: boxCmd->update() failed (OCC triangulation)";
        delete boxCmd;
        return;
    }

    // ---------- 2. 导出几何供 Gmsh 使用 ----------
    OCC::FCAbstractOCCModel* occModel = boxCmd->getTShapeAgent<OCC::FCAbstractOCCModel>();
    if (!occModel || !occModel->getShape() || occModel->getShape()->IsNull()) {
        qWarning() << "testCreatorGeometry: OCC model invalid, skip mesh gen";
        delete boxCmd;
        return;
    }

    QSharedPointer<QTemporaryDir> tmpDir(new QTemporaryDir);
    if (!tmpDir->isValid()) {
        qWarning() << "testCreatorGeometry: Cannot create temp dir";
        delete boxCmd;
        return;
    }
    QString workDir = tmpDir->path();
    QString geoFile = workDir + "/box.stl";  // STL 无需 Gmsh 编译 OpenCASCADE
    if (!occModel->writeToFile(geoFile)) {
        qWarning() << "testCreatorGeometry: Failed to export geometry to STEP";
        delete boxCmd;
        return;
    }

    // ---------- 3. Gmsh 网格生成（异步）----------
    FCMeshGenInterface* meshIface = FCMeshGenInterface::instance();
    FCAbstractMesherDriver* driver = meshIface ? meshIface->getMesherDriver("Gmsh") : nullptr;
    if (!driver) {
        qWarning() << "testCreatorGeometry: Gmsh driver not found";
        delete boxCmd;
        return;
    }

    QString meshFile = workDir + "/mesh.msh";
    driver->setValue("WorkDir", workDir);
    driver->setValue("GeometryFile", geoFile);
    driver->setValue("OutputMeshFile", meshFile);

    FCUnstructuredMeshVTK* meshVTK = new FCUnstructuredMeshVTK;

    // mesherFinished 后读取网格并显示（tmpDir 需被 lambda 持有，避免临时目录提前被删）
    connect(driver, &FCAbstractMesherDriver::mesherFinished, this, [this, driver, meshVTK, workDir, meshFile, tmpDir]() {
        disconnect(driver, &FCAbstractMesherDriver::mesherFinished, this, nullptr);

        FCAbstractMeshProcessor* proc = FCMeshGenInterface::instance()->getMeshProcessor("Gmsh");
        if (!proc || !QFileInfo::exists(meshFile)) {
            qWarning() << "testCreatorGeometry: mesh file not found or processor null";
            delete meshVTK;
            return;
        }
        proc->setValue("WorkDir", workDir);
        proc->setValue("OutputMeshFile", meshFile);
        proc->insertDataObject("Mesh", meshVTK);
        proc->start();

        if (meshVTK->getNumberOfCells() == 0) {
            qWarning() << "testCreatorGeometry: mesh is empty";
            delete meshVTK;
            return;
        }

        // 创建网格显示对象
        vtkUnstructuredGrid* grid = meshVTK->getGrid();
        if (!grid) {
            delete meshVTK;
            return;
        }
        vtkNew<vtkDataSetSurfaceFilter> surfaceFilter;
        surfaceFilter->SetInputData(grid);
        surfaceFilter->Update();
        vtkNew<vtkDataSetMapper> mapper;
        mapper->SetInputConnection(surfaceFilter->GetOutputPort());
        mapper->ScalarVisibilityOff();
        vtkNew<vtkActor> meshActor;
        meshActor->SetMapper(mapper);
        meshActor->GetProperty()->SetColor(0.8, 0.4, 0.2);  // 橙黄色区分几何
        meshActor->GetProperty()->SetEdgeVisibility(true);
        meshActor->GetProperty()->SetLineWidth(0.5);

        FCGraphObjectVTK* meshGraphObj = new FCGraphObjectVTK(meshVTK);
        meshGraphObj->addActor(meshActor);

        if (!mDock) { delete meshGraphObj; delete meshVTK; return; }
        FCRenderWidget* rw = mDock->getGraphicOperateWidget();
        if (!rw) { delete meshGraphObj; delete meshVTK; return; }
        FCGraph3DWindowVTK* graphWin = rw->getGraph3DWindow();
        if (!graphWin) { delete meshGraphObj; delete meshVTK; return; }

        graphWin->addObject(1, meshGraphObj, true);
        graphWin->reRender();
    });

    // ---------- 4. 先显示几何体 ----------
    FCVTKViewAdaptorModelCmd adaptor;
    adaptor.setDataObject(boxCmd);
    if (adaptor.update()) {
        FCVTKGraphObject3D* graphObj = adaptor.getOutputData();
        if (graphObj && graphObj->getActorCount() > 0 && mDock) {
            FCRenderWidget* rw = mDock->getGraphicOperateWidget();
            if (rw) {
                FCGraph3DWindowVTK* graphWin = rw->getGraph3DWindow();
                if (graphWin) {
                    graphWin->addObject(0, graphObj, true);
                }
            }
        }
    }

    driver->startMesher();
}


void FCAppController::onFocusedDockWidgetChanged(ads::CDockWidget *old, ads::CDockWidget *now)
{
    FCAPPCONTROLLER_PASS();
}

    
} // namespace FC


