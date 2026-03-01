#include "FCGraph3DWindowVTK.h"
#include "FCGraph3DWindowInterface.h"
#include "FCGraphRender.h"
#include "FCBoundaryActor.h"
#include "FCGraphObjectVTK.h"
#include "FCGraphOperator.h"
#include "FCGraphInteractionStyle.h"
#include "FCLegendScaleActor.h"
#include "FCGraph3DClipDialog.h"
#include "FCGraphObjManager.h"

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkBMPWriter.h>
#include <vtkLightCollection.h>
#include <vtkLight.h>
#include <vtkMapper.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>

#include <QHideEvent>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QMutexLocker>
#include <QFileDialog>
#include <QFileInfo>
#include <QtSvg/QSvgGenerator>
#include <QPainter>
#include <QDebug>

#if defined(VTK_MAJOR_VERSION) && VTK_MAJOR_VERSION >= 8
#include <QVTKOpenGLNativeWidget.h>
#include <QVTKInteractor.h>
#else
#include <QVTKWidget2.h>
#include <vtkGenericOpenGLRenderWindow.h>
#endif

namespace FC
{
QMutex FCGraph3DWindowVTK::mMutex;

FCGraph3DWindowVTK::FCGraph3DWindowVTK(FCGraph3DWindowInitializer* ini)
    : FCAbstractGraph3DWidget(nullptr)
    , mInitializer(ini)
{
    if (!mInitializer)
        return;

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

#if defined(VTK_MAJOR_VERSION) && VTK_MAJOR_VERSION >= 8
    QVTKOpenGLNativeWidget* vtkWidget = new QVTKOpenGLNativeWidget(this);
#if VTK_MAJOR_VERSION >= 9
    mRenderWindow = vtkWidget->renderWindow();
#else
    mRenderWindow = vtkWidget->GetRenderWindow();
#endif
    mInteractor = static_cast<QVTKInteractor*>(mRenderWindow->GetInteractor());
    mVtkWidget = vtkWidget;
#else
    QVTKWidget2* vtkWidget = new QVTKWidget2(this);
    mRenderWindow = vtkWidget->GetRenderWindow();
    mInteractor = mRenderWindow->GetInteractor();
    mVtkWidget = vtkWidget;
#endif

    mToolbarRowWidget = new QWidget(this);
    mToolbarRowWidget->setStyleSheet("border: none; background-color: rgb(250, 251, 254);");
    mToolbarRowWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    mRenderToolBarLayout = new QGridLayout(mToolbarRowWidget);
    mRenderToolBarLayout->setContentsMargins(2, 2, 2, 2);
    mRenderToolBarLayout->setSpacing(3);
    mRenderToolBar = new QToolBar(mToolbarRowWidget);
    mRenderToolBar->setContentsMargins(0, 0, 0, 0);
    mRenderToolBar->setIconSize(QSize(20, 20));
    mRenderToolBar->setStyleSheet(
        "QToolBar { border: none; background: transparent; }\n"
        "QToolButton { background: transparent; border: none; border-radius: 3px; }\n"
        "QToolButton:hover { background-color: rgba(0, 0, 0, 8%); }\n"
        "QToolButton:pressed { background-color: rgba(0, 0, 0, 15%); }\n"
        "QToolButton:checked { background-color: rgba(0, 0, 0, 12%); }\n"
    );
    mRenderIconToolBar = new QToolBar(mToolbarRowWidget);
    mRenderIconToolBar->setContentsMargins(0, 0, 0, 0);
    mRenderIconToolBar->setStyleSheet("QToolBar { border: none; background: transparent; }");
    
    mRenderToolBar->raise();
    mainLayout->addWidget(mToolbarRowWidget);
    mainLayout->addWidget(mVtkWidget, 1);

    mCamera = vtkCamera::New();
#ifdef QT_DEBUG
    mRenderWindow->SetGlobalWarningDisplay(1);
#else
    mRenderWindow->SetGlobalWarningDisplay(0);
#endif

    if (mInteractor)
    {
        mInteractor->SetDesiredUpdateRate(15);
        mInteractor->SetStillUpdateRate(0.5);
    }

    mActorBound = new FCBoundaryActor;
    int renderNum = mInitializer->getLayerCount();
    mRenderWindow->SetNumberOfLayers(renderNum);

    for (int i = 0; i < renderNum; i++)
    {
        FCGraphRender* renderer = new FCGraphRender(this);
        vtkRenderer* ren = renderer->getRenderer();
        if (ren)
            ren->SetLayer(i);
        mRenders.insert(i, renderer);
    }

    for (FCGraphRender* render : mRenders)
    {
        if (render && render->getRenderer())
            render->getRenderer()->SetActiveCamera(mCamera);
    }

    init();
    setFocusPolicy(Qt::ClickFocus);
    setWindowTitle(QString("View Port-%1").arg(getGraphWidgetID()));

    initActions();
    setIsShowActions(true);

    mTimerView.setInterval(mInterval);
    mTimerView.setSingleShot(true);
    connect(&mTimerView, &QTimer::timeout, this, &FCGraph3DWindowVTK::slot_changingView);
}

FCGraph3DWindowVTK::~FCGraph3DWindowVTK()
{
    if (mLegendScaleActor)
    {
        mLegendScaleActor->Delete();
        mLegendScaleActor = nullptr;
    }
    if (mCamera)
    {
        mCamera->Delete();
        mCamera = nullptr;
    }
    if (mAxesWidget)
    {
        mAxesWidget->Delete();
        mAxesWidget = nullptr;
    }
    if (mInteractionStyle)
    {
        mInteractionStyle->Delete();
        mInteractionStyle = nullptr;
    }
    if (mActorBound)
    {
        delete mActorBound;
        mActorBound = nullptr;
    }
    for (FCGraphRender* re : mRenders)
    {
        if (re)
        {
            delete re;
        }
    }
    mRenders.clear();
}

void FCGraph3DWindowVTK::showEvent(QShowEvent* event)
{
    if (!mInitFlag)
    {
        mInitFlag = true;
        setView(mDefFp, mDefVp, mDefPos);
        if (mAxesWidget)
        {
            mAxesWidget->SetOutlineColor(0.93, 0.57, 0.13);
            mAxesWidget->SetViewport(0, 0, 0.2, 0.2);
            mAxesWidget->SetEnabled(1);
            mAxesWidget->InteractiveOff();
        }
    }
    // 窗口重新显示时恢复 VTK 静止时的刷新率，避免后台长期不刷新导致的内存/显存占用
    if (mInteractor)
        mInteractor->SetStillUpdateRate(0.5);
    QWidget::showEvent(event);
}

void FCGraph3DWindowVTK::hideEvent(QHideEvent* event)
{
    // 窗口隐藏或最小化时停止 VTK 的静止刷新，减少后台定时渲染带来的内存增长
    if (mInteractor)
        mInteractor->SetStillUpdateRate(0);
    QWidget::hideEvent(event);
}

void FCGraph3DWindowVTK::setHiddenLineRemoval(bool flag)
{
    vtkRendererCollection* renderers = mRenderWindow->GetRenderers();
    int n = renderers->GetNumberOfItems();
    for (int i = n - 1; i >= 0; i--)
    {
        vtkRenderer* ren = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(i));
        if (ren)
            ren->SetUseHiddenLineRemoval(flag ? 1 : 0);
    }
}

bool FCGraph3DWindowVTK::getHiddenLineRemoval()
{
    vtkRendererCollection* renderers = mRenderWindow->GetRenderers();
    if (renderers->GetNumberOfItems() == 0)
        return false;
    vtkRenderer* ren = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(0));
    return ren ? (ren->GetUseHiddenLineRemoval() != 0) : false;
}

void FCGraph3DWindowVTK::reRender()
{
    if (mRenderWindow)
        mRenderWindow->Render();
}

void FCGraph3DWindowVTK::setAxesEnable(bool state)
{
    if (mAxesWidget)
        mAxesWidget->SetEnabled(state ? 1 : 0);
}

void FCGraph3DWindowVTK::setView(double* focalPoint, double* viewUp, double* position)
{
    for (int i = 0; i < 3; i++)
    {
        if (qIsNaN(focalPoint[i]) || qIsNaN(viewUp[i]) || qIsNaN(position[i]))
            return;
    }
    if (!mCamera)
        return;
    mCamera->SetViewUp(viewUp);
    mCamera->SetPosition(position);
    mCamera->SetFocalPoint(focalPoint);
    fitView();
    if (mInteractionStyle)
    {
        mInteractionStyle->Rotate();
        mInteractionStyle->EndRotate();
    }
}

void FCGraph3DWindowVTK::setDefaultView(double* focalPoint, double* viewUp, double* position)
{
    for (int i = 0; i < 3; i++)
    {
        mDefFp[i] = focalPoint[i];
        mDefVp[i] = viewUp[i];
        mDefPos[i] = position[i];
    }
}

void FCGraph3DWindowVTK::setISOView(double* focalPoint, double* viewUp, double* position)
{
    for (int i = 0; i < 3; i++)
    {
        mISOFp[i] = focalPoint[i];
        mISOVp[i] = viewUp[i];
        mISOPos[i] = position[i];
    }
}

bool FCGraph3DWindowVTK::getView(double* pos, double* focual, double* viewup)
{
    if (!mCamera)
        return false;
    mCamera->GetPosition(pos);
    mCamera->GetFocalPoint(focual);
    mCamera->GetViewUp(viewup);
    return true;
}

void FCGraph3DWindowVTK::setViewBounds(double* bds)
{
    if (mRenders.isEmpty())
        return;
    FCGraphRender* fRender = mRenders.value(0);
    if (!fRender)
        return;
    vtkRenderer* renderer = fRender->getRenderer();
    if (!renderer)
        return;
    renderer->ResetCamera(bds);
    reRender();
}

void FCGraph3DWindowVTK::fastAddObject(int index, FCGraphObjectVTK* object)
{
    FCGraphRender* fRender = mRenders.value(index);
    if (!fRender)
        return;
    fRender->addObject(object);
    int nWidget = object->getWidgetCount();
    for (int i = 0; i < nWidget; i++)
    {
        vtkInteractorObserver* widget = object->getWidget(i);
        if (!widget)
            continue;
        if (mRenderWindow)
            widget->SetInteractor(mRenderWindow->GetInteractor());
        FCGraphRender* render = mRenders.value(0);
        if (render && render->getRenderer())
            widget->SetCurrentRenderer(render->getRenderer());
    }
}

void FCGraph3DWindowVTK::addObject(int index, FCGraphObjectVTK* object, bool fitview)
{
    FCGraphRender* r = mRenders.value(index);
    if (!r)
        return;
    int graphCount = getGraphObjCount();
    r->addObject(object);

    int nWidget = object->getWidgetCount();
    for (int i = 0; i < nWidget; i++)
    {
        vtkInteractorObserver* widget = object->getWidget(i);
        if (!widget)
            continue;
        if (mRenderWindow)
            widget->SetInteractor(mRenderWindow->GetInteractor());
        FCGraphRender* render = mRenders.value(0);
        if (render && render->getRenderer())
            widget->SetCurrentRenderer(render->getRenderer());
    }
    if (fitview && graphCount == 0)
        fitView();
    else
        updateBoundary();
}

QList<FCGraphObjectVTK*> FCGraph3DWindowVTK::getAllGraphObj()
{
    QList<FCGraphObjectVTK*> objs;
    for (FCGraphRender* render : mRenders)
    {
        if (!render)
            continue;
        FCGraphObjManager* objManager = render->getGraphObjManager();
        if (!objManager)
            continue;
        int objCount = objManager->getGraphObjCount();
        for (int i = 0; i < objCount; i++)
            objs.append(render->getGraphObjManager()->getGraphObjAt(i));
    }
    return objs;
}

void FCGraph3DWindowVTK::clear()
{
    for (FCGraphRender* render : mRenders)
    {
        if (render)
            render->clear();
    }
}

void FCGraph3DWindowVTK::setBackground(double topValue[3], double bottomValue[3])
{
    for (FCGraphRender* render : mRenders)
    {
        if (render && render->getRenderer())
        {
            render->getRenderer()->SetBackground(topValue);
            render->getRenderer()->SetBackground2(bottomValue);
        }
    }
    if (mToolbarRowWidget && topValue)
    {
        int r = qBound(0, static_cast<int>(topValue[0] * 255), 255);
        int g = qBound(0, static_cast<int>(topValue[1] * 255), 255);
        int b = qBound(0, static_cast<int>(topValue[2] * 255), 255);
        mToolbarRowWidget->setStyleSheet(QString("border: none; background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));
    }
}

void FCGraph3DWindowVTK::fitView()
{
    double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    bool emptyWindow = true;
    for (FCGraphRender* render : mRenders)
    {
        if (!render)
            continue;
        double length = render->getActorBounds(bound);
        if (length > 0)
            emptyWindow = false;
    }
    if (emptyWindow)
    {
        bound[0] = bound[2] = bound[4] = -1;
        bound[1] = bound[3] = bound[5] = 1;
    }
    if (mActorBound)
        mActorBound->updateBoundary(bound);
    for (FCGraphRender* render : mRenders)
    {
        if (render && render->getRenderer())
            render->getRenderer()->ResetCamera(bound);
    }
    reRender();
}

void FCGraph3DWindowVTK::flush()
{
    if (mRenderWindow)
        mRenderWindow->Render();
}

void FCGraph3DWindowVTK::setBackgroundColor(float* rgb1, float* rgb2)
{
    if (!rgb1)
        return;
    if (!rgb2)
        rgb2 = rgb1;
    for (FCGraphRender* render : mRenders)
    {
        if (render)
            render->setBackgroundColor(rgb1, rgb2);
    }
    if (mToolbarRowWidget)
    {
        int r = qBound(0, static_cast<int>(rgb1[0] * 255), 255);
        int g = qBound(0, static_cast<int>(rgb1[1] * 255), 255);
        int b = qBound(0, static_cast<int>(rgb1[2] * 255), 255);
        mToolbarRowWidget->setStyleSheet(QString("border: none; background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));
    }
}

void FCGraph3DWindowVTK::getBackgroundColor(float* rgb1, float* rgb2)
{
    if (!rgb1 || mRenders.isEmpty())
        return;
    FCGraphRender* r = mRenders.value(0);
    if (!r || !r->getRenderer())
        return;
    double* bg = r->getRenderer()->GetBackground();
    double* bg2 = r->getRenderer()->GetBackground2();
    rgb1[0] = static_cast<float>(bg[0]);
    rgb1[1] = static_cast<float>(bg[1]);
    rgb1[2] = static_cast<float>(bg[2]);
    if (rgb2 && bg2)
    {
        rgb2[0] = static_cast<float>(bg2[0]);
        rgb2[1] = static_cast<float>(bg2[1]);
        rgb2[2] = static_cast<float>(bg2[2]);
    }
}

FCGraphRender* FCGraph3DWindowVTK::getRenderer(int index)
{
    return mRenders.value(index);
}

int FCGraph3DWindowVTK::getRenderCount()
{
    return mRenders.size();
}

vtkRenderWindow* FCGraph3DWindowVTK::getVTKRenderWindow()
{
    return mRenderWindow;
}

vtkCamera* FCGraph3DWindowVTK::getVTKCamera()
{
    return mCamera;
}

vtkRenderer* FCGraph3DWindowVTK::getVTKRenderer(int index)
{
    if (index < 0)
        index = mRenders.size() - 1;
    FCGraphRender* r = mRenders.value(index);
    return r ? r->getRenderer() : nullptr;
}

void FCGraph3DWindowVTK::setParallelProjection(bool on)
{
    if (mCamera)
        mCamera->SetParallelProjection(on ? 1 : 0);
}

bool FCGraph3DWindowVTK::getParallelProjection()
{
    return mCamera && mCamera->GetParallelProjection();
}

void FCGraph3DWindowVTK::removeGraphObj(FCAbstractGraphObject* gobj)
{
    FCGraphObjectVTK* objvtk = dynamic_cast<FCGraphObjectVTK*>(gobj);
    if (!objvtk)
        return;
    for (FCGraphRender* render : mRenders)
    {
        if (render)
            render->removeObject(objvtk);
    }
}

int FCGraph3DWindowVTK::getGraphObjCount()
{
    int c = 0;
    for (FCGraphRender* render : mRenders)
    {
        if (render)
            c += render->getGraphObjectCount();
    }
    return c;
}

bool FCGraph3DWindowVTK::getGraphObjCenter(double* center)
{
    double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    bool emptyWindow = true;
    for (FCGraphRender* render : mRenders)
    {
        if (!render)
            continue;
        if (render->getActorBounds(bound) > 0)
            emptyWindow = false;
    }
    if (emptyWindow)
    {
        bound[0] = bound[2] = bound[4] = -1;
        bound[1] = bound[3] = bound[5] = 1;
    }
    center[0] = (bound[0] + bound[1]) / 2.0;
    center[1] = (bound[2] + bound[3]) / 2.0;
    center[2] = (bound[4] + bound[5]) / 2.0;
    return true;
}

void FCGraph3DWindowVTK::saveImage(const QString& image, int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    if (image.isEmpty() || !mRenderWindow)
        return;
    vtkWindowToImageFilter* imageFilter = vtkWindowToImageFilter::New();
    imageFilter->SetInput(mRenderWindow);

    switch (mImageType)
    {
    case FCGraphWinImageType::FC_PNG:
    {
        vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
        writer->SetFileName(image.toLocal8Bit().constData());
        writer->SetInputConnection(imageFilter->GetOutputPort());
        writer->Write();
        break;
    }
    case FCGraphWinImageType::FC_JPEG:
    {
        vtkSmartPointer<vtkJPEGWriter> writer = vtkSmartPointer<vtkJPEGWriter>::New();
        writer->SetFileName(image.toLocal8Bit().constData());
        writer->SetInputConnection(imageFilter->GetOutputPort());
        writer->Write();
        break;
    }
    case FCGraphWinImageType::FC_TIFF:
    {
        vtkSmartPointer<vtkTIFFWriter> writer = vtkSmartPointer<vtkTIFFWriter>::New();
        writer->SetFileName(image.toLocal8Bit().constData());
        writer->SetInputConnection(imageFilter->GetOutputPort());
        writer->Write();
        break;
    }
    case FCGraphWinImageType::FC_BMP:
    {
        vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
        writer->SetFileName(image.toLocal8Bit().constData());
        writer->SetInputConnection(imageFilter->GetOutputPort());
        writer->Write();
        break;
    }
    case FCGraphWinImageType::FC_SVG:
    {
        QSvgGenerator svgGenerator;
        svgGenerator.setFileName(image);
        svgGenerator.setSize(size());
        svgGenerator.setViewBox(QRect(0, 0, size().width(), size().height()));
        QPainter painter;
        painter.begin(&svgGenerator);
        render(&painter);
        painter.end();
        break;
    }
    }
    imageFilter->Delete();
    if (mRenderWindow)
        mRenderWindow->Finalize();
    qDebug().noquote() << QString("Viewports printed to \"%1\"").arg(image);
}

void FCGraph3DWindowVTK::showScaleLegend(bool visibility)
{
    if (mLegendScaleActor)
        mLegendScaleActor->SetVisibility(visibility ? 1 : 0);
}

void FCGraph3DWindowVTK::addActionsToolBar(QList<QAction*> actions, QAction* beforeAction)
{
    QMutexLocker locker(&mMutex);
    if (!mRenderToolBarLayout || !mRenderToolBar)
        return;
    mRenderToolBar->insertActions(beforeAction, actions);
    setToolBarPos(mRenderBarStyle);
    QList<QToolButton*> buttons = mRenderToolBar->findChildren<QToolButton*>();
    for (QToolButton* b : buttons)
    {
        if (b)
            b->setToolTipDuration(-1);  // 悬停提示由 QAction::toolTip 提供，-1 表示使用系统默认延迟
    }
}

void FCGraph3DWindowVTK::setToolBarPos(FCGraphWinToolBarPos pos)
{
    if (!mRenderToolBar || !mRenderToolBarLayout)
        return;
    mRenderBarStyle = pos;
    switch (pos)
    {
    case FCGraphWinToolBarPos::FCLeftTop:
        mRenderToolBar->setOrientation(Qt::Vertical);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCLeftCenter:
        mRenderToolBar->setOrientation(Qt::Vertical);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
        break;
    case FCGraphWinToolBarPos::FCLeftBottom:
        mRenderToolBar->setOrientation(Qt::Vertical);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCBottomCenter:
        mRenderToolBar->setOrientation(Qt::Horizontal);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignHCenter | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightBottom:
        mRenderToolBar->setOrientation(Qt::Vertical);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignRight | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightCenter:
        mRenderToolBar->setOrientation(Qt::Vertical);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
        break;
    case FCGraphWinToolBarPos::FCRightTop:
        mRenderToolBar->setOrientation(Qt::Vertical);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignRight | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCTopCenter:
        mRenderToolBar->setOrientation(Qt::Horizontal);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignHCenter | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCTopLeft:
        mRenderToolBar->setOrientation(Qt::Horizontal);
        mRenderToolBarLayout->addWidget(mRenderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop);
        break;
    }
}

void FCGraph3DWindowVTK::initRenderIcon(const QPixmap& image, FCGraphWinToolBarPos pos)
{
    QMutexLocker locker(&mMutex);
    if (!mRenderIconToolBar || !mRenderToolBarLayout)
        return;
    mRenderIconToolBar->clear();
    QLabel* label = new QLabel(mToolbarRowWidget);
    label->setPixmap(image);
    mRenderIconToolBar->addWidget(label);
    switch (pos)
    {
    case FCGraphWinToolBarPos::FCLeftTop:
        mRenderToolBarLayout->addWidget(mRenderIconToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCLeftBottom:
        mRenderToolBarLayout->addWidget(mRenderIconToolBar, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightBottom:
        mRenderToolBarLayout->addWidget(mRenderIconToolBar, 0, 0, Qt::AlignRight | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightTop:
        mRenderToolBarLayout->addWidget(mRenderIconToolBar, 0, 0, Qt::AlignRight | Qt::AlignTop);
        break;
    default:
        break;
    }
}

QList<QAction*> FCGraph3DWindowVTK::getActions()
{
    return mRenderToolBar ? mRenderToolBar->actions() : QList<QAction*>();
}

void FCGraph3DWindowVTK::setIsShowActions(bool isShow)
{
    if (mRenderToolBar)
        mRenderToolBar->setVisible(isShow);
}

void FCGraph3DWindowVTK::setIsShowClipAction(bool isShow)
{
    QList<QAction*> acts = getActions();
    for (QAction* a : acts)
    {
        if (a && a->objectName() == "actionRenderClip")
        {
            a->setVisible(isShow);
            break;
        }
    }
}

void FCGraph3DWindowVTK::updateBoundary()
{
    double bound[6];
    if (getBoundray(bound))
    {
        if (mActorBound)
            mActorBound->updateBoundary(bound);
    }
}

bool FCGraph3DWindowVTK::getBoundray(double* bds)
{
    double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    bool hasAny = false;
    for (FCGraphRender* render : mRenders)
    {
        if (!render)
            continue;
        if (render->getActorBounds(bound) > 0)
            hasAny = true;
    }
    if (!hasAny)
        return false;
    for (int i = 0; i < 6; i++)
        bds[i] = bound[i];
    return true;
}

void FCGraph3DWindowVTK::SetGlobalWarningDisplay(bool isOpen)
{
    if (mRenderWindow)
        mRenderWindow->SetGlobalWarningDisplay(isOpen ? 1 : 0);
}

void FCGraph3DWindowVTK::SetUseDepthPeeling(bool isOpen, int peelMaxNum, int renderIndex)
{
    Q_UNUSED(isOpen);
    Q_UNUSED(peelMaxNum);
    Q_UNUSED(renderIndex);
}

FCGraphInteractionStyle* FCGraph3DWindowVTK::getGraphInteractionStyle()
{
    return mInteractionStyle;
}

void FCGraph3DWindowVTK::setUserInformation(QVariant info)
{
    mUserInfo = info;
}

QVariant FCGraph3DWindowVTK::getUserInformation()
{
    return mUserInfo;
}

void FCGraph3DWindowVTK::slotActionViewFrontEvent()
{
    double position[3] = { 0, 0, 1 };
    double focalPoint[3] = { 0, 0, 0 };
    double viewUp[3] = { 0, 1, 0 };
    setView(focalPoint, viewUp, position);
}

void FCGraph3DWindowVTK::slotActionViewBackEvent()
{
    double position[3] = { 0, 0, -1 };
    double focalPoint[3] = { 0, 0, 0 };
    double viewUp[3] = { 0, 1, 0 };
    setView(focalPoint, viewUp, position);
}

void FCGraph3DWindowVTK::slotActionViewTopEvent()
{
    double position[3] = { 0, 1, 0 };
    double focalPoint[3] = { 0, 0, 0 };
    double viewUp[3] = { 0, 0, -1 };
    setView(focalPoint, viewUp, position);
}

void FCGraph3DWindowVTK::slotActionViewBottomEvent()
{
    double position[3] = { 0, -1, 0 };
    double focalPoint[3] = { 0, 0, 0 };
    double viewUp[3] = { 0, 0, 1 };
    setView(focalPoint, viewUp, position);
}

void FCGraph3DWindowVTK::slotActionViewLeftEvent()
{
    double position[3] = { -1, 0, 0 };
    double focalPoint[3] = { 0, 0, 0 };
    double viewUp[3] = { 0, 1, 0 };
    setView(focalPoint, viewUp, position);
}

void FCGraph3DWindowVTK::slotActionViewRightEvent()
{
    double position[3] = { 1, 0, 0 };
    double focalPoint[3] = { 0, 0, 0 };
    double viewUp[3] = { 0, 1, 0 };
    setView(focalPoint, viewUp, position);
}

void FCGraph3DWindowVTK::slotActionViewIsoEvent()
{
    setView(mISOFp, mISOVp, mISOPos);
}

void FCGraph3DWindowVTK::slotActionViewPanEvent()
{
    fitView();
}

void FCGraph3DWindowVTK::slotActionSaveImageEvent()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "",
        tr("PNG (*.png);;JPEG (*.jpeg);;TIFF (*.tiff);;BMP (*.bmp);;SVG (*.svg)"));
    if (filePath.isEmpty())
        return;
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();
    if (suffix == "png")
        mImageType = FCGraphWinImageType::FC_PNG;
    else if (suffix == "jpeg" || suffix == "jpg")
        mImageType = FCGraphWinImageType::FC_JPEG;
    else if (suffix == "tiff" || suffix == "tif")
        mImageType = FCGraphWinImageType::FC_TIFF;
    else if (suffix == "bmp")
        mImageType = FCGraphWinImageType::FC_BMP;
    else if (suffix == "svg")
        mImageType = FCGraphWinImageType::FC_SVG;
    else
        mImageType = FCGraphWinImageType::FC_PNG;
    saveImage(filePath);
}

void FCGraph3DWindowVTK::slotActionViewParallelEvent()
{
    setParallelProjection(true);
}

void FCGraph3DWindowVTK::slotActionViewPerspectiveEvent()
{
    setParallelProjection(false);
}

void FCGraph3DWindowVTK::slotActionClipEvent(bool isCreateNewModel)
{
    FCGraph3DClipDialog* dialog = new FCGraph3DClipDialog(this, isCreateNewModel);
    dialog->show();
}

void FCGraph3DWindowVTK::slotActionTransparentEvent(bool checked)
{
    for (FCGraphObjectVTK* obj : getAllGraphObj())
        obj->setTransparent(checked);
    reRender();
}

void FCGraph3DWindowVTK::setViewAnime(double* focalPoint, double* viewUp, double* position)
{
    for (int i = 0; i < 3; i++)
    {
        if (qIsNaN(focalPoint[i]) || qIsNaN(viewUp[i]) || qIsNaN(position[i]))
            return;
    }
    startChangingView(focalPoint, viewUp, position);
}

void FCGraph3DWindowVTK::startChangingView(double* foc, double* up, double* pos)
{
    if (mTimerView.isActive())
        mTimerView.stop();
    mCurrStep = 0;
    for (int i = 0; i < 3; i++)
    {
        mTarFoc3[i] = foc[i];
        mTarUp3[i] = up[i];
        mTarPos3[i] = pos[i];
    }
    mTimerView.start();
}

void FCGraph3DWindowVTK::slot_changingView()
{
    mCurrStep++;
    if (mCurrStep >= mViewStep)
    {
        if (mCamera)
        {
            mCamera->SetViewUp(mTarUp3);
            mCamera->SetPosition(mTarPos3);
            mCamera->SetFocalPoint(mTarFoc3);
        }
        return;
    }
    if (!mCamera)
        return;
    double* currFoc = mCamera->GetFocalPoint();
    double* currUp = mCamera->GetViewUp();
    double* currPos = mCamera->GetPosition();
    int steps = mViewStep - mCurrStep;
    double deltaFoc3[3], deltaUp3[3], deltaPos3[3];
    for (int i = 0; i < 3; i++)
    {
        deltaFoc3[i] = (mTarFoc3[i] - currFoc[i]) / steps;
        deltaUp3[i] = (mTarUp3[i] - currUp[i]) / steps;
        deltaPos3[i] = (mTarPos3[i] - currPos[i]) / steps;
    }
    mCamera->SetViewUp(currUp[0] + deltaUp3[0], currUp[1] + deltaUp3[1], currUp[2] + deltaUp3[2]);
    mCamera->SetPosition(currPos[0] + deltaPos3[0], currPos[1] + deltaPos3[1], currPos[2] + deltaPos3[2]);
    mCamera->SetFocalPoint(currFoc[0] + deltaFoc3[0], currFoc[1] + deltaFoc3[1], currFoc[2] + deltaFoc3[2]);
    reRender();
    mTimerView.start();
}

void FCGraph3DWindowVTK::init()
{
    FCGraphRender* lastRender = getRenderer(mRenders.count() - 1);
    if (mInitializer && mInitializer->getLayerCount() > 0 && lastRender && mActorBound)
    {
        vtkActor* actor = mActorBound->getActor();
        if (actor)
            lastRender->getRenderer()->AddViewProp(actor);
    }
    if (lastRender && lastRender->getRenderer())
    {
        mLegendScaleActor = FCLegendScaleActor::New();
        mLegendScaleActor->SetVisibility(0);
        lastRender->getRenderer()->AddViewProp(mLegendScaleActor);
    }
    vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
    mAxesWidget = vtkOrientationMarkerWidget::New();
    mAxesWidget->SetInteractor(mInteractor);
    mAxesWidget->SetOrientationMarker(axesActor);
    FCGraphRender* r0 = getRenderer(0);
    if (r0 && r0->getRenderer())
        mAxesWidget->SetDefaultRenderer(r0->getRenderer());

    for (FCGraphRender* fRender : mRenders)
    {
        if (!fRender || !fRender->getRenderer())
            continue;
        vtkRenderer* renderer = fRender->getRenderer();
        renderer->RemoveAllLights();
        vtkLight* light1 = renderer->MakeLight();
        light1->SetLightTypeToHeadlight();
        renderer->AddLight(light1);
    }
    if (mInitializer)
        setInteractionStyle(mInitializer->getStyle());
}

void FCGraph3DWindowVTK::initActions()
{
    QList<QAction*> actions;
    QAction* a = nullptr;
    a = new QAction(this);
    a->setObjectName("actionViewFront");
    a->setIcon(QIcon(":/icon/icoR_viewFront.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewFrontEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewBack");
    a->setIcon(QIcon(":/icon/icoR_viewBack.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewBackEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewTop");
    a->setIcon(QIcon(":/icon/icoR_viewTop.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewTopEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewBottom");
    a->setIcon(QIcon(":/icon/icoR_viewBottom.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewBottomEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewLeft");
    a->setIcon(QIcon(":/icon/icoR_viewLeft.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewLeftEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewRight");
    a->setIcon(QIcon(":/icon/icoR_viewRight.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewRightEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewIso");
    a->setIcon(QIcon(":/icon/icoR_viewIso.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewIsoEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewAuto");
    a->setIcon(QIcon(":/icon/icoR_autofit.png"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewPanEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionSaveImage");
    a->setIcon(QIcon(":/icon/icoR_saveimage.png"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionSaveImageEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewParallel");
    a->setIcon(QIcon(":/icon/icoR_viewParallel.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewParallelEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionViewPerspective");
    a->setIcon(QIcon(":/icon/icoR_viewPerspective.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewPerspectiveEvent);
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionRenderClip");
    a->setIcon(QIcon(":/icon/icoR_clip.png"));
    a->setVisible(false);
    connect(a, &QAction::triggered, this, [this]() { slotActionClipEvent(); });
    actions.append(a);
    a = new QAction(this);
    a->setObjectName("actionGeometryTransparent");
    a->setIcon(QIcon(":/icon/transparent.svg"));
    a->setCheckable(true);
    a->setChecked(true);  // 默认与创建时透明度一致（s_transparency），避免锯齿且只有两种状态
    a->setToolTip(tr("几何体透明显示"));
    connect(a, &QAction::toggled, this, &FCGraph3DWindowVTK::slotActionTransparentEvent);
    actions.append(a);
    addActionsToolBar(actions);
}

void FCGraph3DWindowVTK::setInteractionStyle(FCGraphInteractionStyle* style)
{
    if (mInteractionStyle)
    {
        mInteractionStyle->Delete();
        mInteractionStyle = nullptr;
    }
    mInteractionStyle = style;
    if (mInteractionStyle)
    {
        mInteractionStyle->setGraph3DWin(this);
        mInteractionStyle->setInteractor(mInteractor);
        if (mRenders.count() > 0)
        {
            FCGraphRender* r = mRenders.value(0);
            if (r && r->getRenderer())
                mInteractionStyle->setAreaPickRenderer(r->getRenderer());
        }
    }
}
} // namespace FC
