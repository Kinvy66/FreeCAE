/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#include "ui_FCGraph3DWindowVTK.h"
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
QMutex FCGraph3DWindowVTK::_mutex;

FCGraph3DWindowVTK::FCGraph3DWindowVTK(FCGraph3DWindowInitializer* ini)
    : FCAbstractGraph3DWidget(nullptr)
    , _initializer(ini)
{
    if (!_initializer)
        return;

    m_ui = new Ui::FCGraph3DWindowVTK();
    m_ui->setupUi(this);

#if defined(VTK_MAJOR_VERSION) && VTK_MAJOR_VERSION >= 8
    QVTKOpenGLNativeWidget* vtkWidget = new QVTKOpenGLNativeWidget(this);
#if VTK_MAJOR_VERSION >= 9
    m_renderWindow = vtkWidget->renderWindow();
#else
    m_renderWindow = vtkWidget->GetRenderWindow();
#endif
    m_interactor = static_cast<QVTKInteractor*>(m_renderWindow->GetInteractor());
    m_vtkWidget = vtkWidget;
#else
    QVTKWidget2* vtkWidget = new QVTKWidget2(this);
    m_renderWindow = vtkWidget->GetRenderWindow();
    m_interactor = m_renderWindow->GetInteractor();
    m_vtkWidget = vtkWidget;
#endif

    QLayout* lay = layout();
    if (!lay)
    {
        setLayout(new QVBoxLayout);
        lay = layout();
    }
    lay->addWidget(m_vtkWidget);

    m_camera = vtkCamera::New();
#ifdef QT_DEBUG
    m_renderWindow->SetGlobalWarningDisplay(1);
#else
    m_renderWindow->SetGlobalWarningDisplay(0);
#endif

    if (m_interactor)
    {
        m_interactor->SetDesiredUpdateRate(15);
        m_interactor->SetStillUpdateRate(0.5);
    }

    m_actorBound = new FCBoundaryActor;
    int renderNum = _initializer->getLayerCount();
    m_renderWindow->SetNumberOfLayers(renderNum);

    for (int i = 0; i < renderNum; i++)
    {
        FCGraphRender* renderer = new FCGraphRender(this);
        vtkRenderer* ren = renderer->getRenderer();
        if (ren)
            ren->SetLayer(i);
        m_renders.insert(i, renderer);
    }

    for (FCGraphRender* render : m_renders)
    {
        if (render && render->getRenderer())
            render->getRenderer()->SetActiveCamera(m_camera);
    }

    init();
    setFocusPolicy(Qt::ClickFocus);
    setWindowTitle(QString("View Port-%1").arg(getGraphWidgetID()));

    _renderToolBarLayout = new QGridLayout(m_vtkWidget);
    _renderToolBarLayout->setContentsMargins(0, 0, 0, 0);
    _renderToolBar = new QToolBar(m_vtkWidget);
    _renderToolBar->setContentsMargins(0, 0, 0, 0);
    _renderToolBar->setIconSize(QSize(20, 20));
    _renderIconToolBar = new QToolBar(m_vtkWidget);
    _renderIconToolBar->setContentsMargins(0, 0, 0, 0);
    initActions();
    setIsShowActions(true);

    m_timerView.setInterval(m_interval);
    m_timerView.setSingleShot(true);
    connect(&m_timerView, &QTimer::timeout, this, &FCGraph3DWindowVTK::slot_changingView);
    _renderToolBar->raise();
}

FCGraph3DWindowVTK::~FCGraph3DWindowVTK()
{
    if (m_legendScaleActor)
    {
        m_legendScaleActor->Delete();
        m_legendScaleActor = nullptr;
    }
    if (m_ui)
    {
        delete m_ui;
        m_ui = nullptr;
    }
    if (m_camera)
    {
        m_camera->Delete();
        m_camera = nullptr;
    }
    if (m_axesWidget)
    {
        m_axesWidget->Delete();
        m_axesWidget = nullptr;
    }
    if (m_interactionStyle)
    {
        m_interactionStyle->Delete();
        m_interactionStyle = nullptr;
    }
    if (m_actorBound)
    {
        delete m_actorBound;
        m_actorBound = nullptr;
    }
    for (FCGraphRender* re : m_renders)
    {
        if (re)
        {
            delete re;
        }
    }
    m_renders.clear();
}

void FCGraph3DWindowVTK::showEvent(QShowEvent* event)
{
    if (!m_initFlag)
    {
        m_initFlag = true;
        setView(_defFp, _defVp, _defPos);
        if (m_axesWidget)
        {
            m_axesWidget->SetOutlineColor(0.93, 0.57, 0.13);
            m_axesWidget->SetViewport(0, 0, 0.2, 0.2);
            m_axesWidget->SetEnabled(1);
            m_axesWidget->InteractiveOff();
        }
    }
    QWidget::showEvent(event);
}

void FCGraph3DWindowVTK::setHiddenLineRemoval(bool flag)
{
    vtkRendererCollection* renderers = m_renderWindow->GetRenderers();
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
    vtkRendererCollection* renderers = m_renderWindow->GetRenderers();
    if (renderers->GetNumberOfItems() == 0)
        return false;
    vtkRenderer* ren = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(0));
    return ren ? (ren->GetUseHiddenLineRemoval() != 0) : false;
}

void FCGraph3DWindowVTK::reRender()
{
    if (m_renderWindow)
        m_renderWindow->Render();
}

void FCGraph3DWindowVTK::setAxesEnable(bool state)
{
    if (m_axesWidget)
        m_axesWidget->SetEnabled(state ? 1 : 0);
}

void FCGraph3DWindowVTK::setView(double* focalPoint, double* viewUp, double* position)
{
    for (int i = 0; i < 3; i++)
    {
        if (qIsNaN(focalPoint[i]) || qIsNaN(viewUp[i]) || qIsNaN(position[i]))
            return;
    }
    if (!m_camera)
        return;
    m_camera->SetViewUp(viewUp);
    m_camera->SetPosition(position);
    m_camera->SetFocalPoint(focalPoint);
    fitView();
    if (m_interactionStyle)
    {
        m_interactionStyle->Rotate();
        m_interactionStyle->EndRotate();
    }
}

void FCGraph3DWindowVTK::setDefaultView(double* focalPoint, double* viewUp, double* position)
{
    for (int i = 0; i < 3; i++)
    {
        _defFp[i] = focalPoint[i];
        _defVp[i] = viewUp[i];
        _defPos[i] = position[i];
    }
}

void FCGraph3DWindowVTK::setISOView(double* focalPoint, double* viewUp, double* position)
{
    for (int i = 0; i < 3; i++)
    {
        _ISOFp[i] = focalPoint[i];
        _ISOVp[i] = viewUp[i];
        _ISOPos[i] = position[i];
    }
}

bool FCGraph3DWindowVTK::getView(double* pos, double* focual, double* viewup)
{
    if (!m_camera)
        return false;
    m_camera->GetPosition(pos);
    m_camera->GetFocalPoint(focual);
    m_camera->GetViewUp(viewup);
    return true;
}

void FCGraph3DWindowVTK::setViewBounds(double* bds)
{
    if (m_renders.isEmpty())
        return;
    FCGraphRender* fRender = m_renders.value(0);
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
    FCGraphRender* fRender = m_renders.value(index);
    if (!fRender)
        return;
    fRender->addObject(object);
    int nWidget = object->getWidgetCount();
    for (int i = 0; i < nWidget; i++)
    {
        vtkInteractorObserver* widget = object->getWidget(i);
        if (!widget)
            continue;
        if (m_renderWindow)
            widget->SetInteractor(m_renderWindow->GetInteractor());
        FCGraphRender* render = m_renders.value(0);
        if (render && render->getRenderer())
            widget->SetCurrentRenderer(render->getRenderer());
    }
}

void FCGraph3DWindowVTK::addObject(int index, FCGraphObjectVTK* object, bool fitview)
{
    FCGraphRender* r = m_renders.value(index);
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
        if (m_renderWindow)
            widget->SetInteractor(m_renderWindow->GetInteractor());
        FCGraphRender* render = m_renders.value(0);
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
    for (FCGraphRender* render : m_renders)
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
    for (FCGraphRender* render : m_renders)
    {
        if (render)
            render->clear();
    }
}

void FCGraph3DWindowVTK::setBackground(double topValue[3], double bottomValue[3])
{
    for (FCGraphRender* render : m_renders)
    {
        if (render && render->getRenderer())
        {
            render->getRenderer()->SetBackground(topValue);
            render->getRenderer()->SetBackground2(bottomValue);
        }
    }
}

void FCGraph3DWindowVTK::fitView()
{
    double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    bool emptyWindow = true;
    for (FCGraphRender* render : m_renders)
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
    if (m_actorBound)
        m_actorBound->updateBoundary(bound);
    for (FCGraphRender* render : m_renders)
    {
        if (render && render->getRenderer())
            render->getRenderer()->ResetCamera(bound);
    }
    reRender();
}

void FCGraph3DWindowVTK::flush()
{
    if (m_renderWindow)
        m_renderWindow->Render();
}

void FCGraph3DWindowVTK::setBackgroundColor(float* rgb1, float* rgb2)
{
    if (!rgb1)
        return;
    if (!rgb2)
        rgb2 = rgb1;
    for (FCGraphRender* render : m_renders)
    {
        if (render)
            render->setBackgroundColor(rgb1, rgb2);
    }
}

void FCGraph3DWindowVTK::getBackgroundColor(float* rgb1, float* rgb2)
{
    if (!rgb1 || m_renders.isEmpty())
        return;
    FCGraphRender* r = m_renders.value(0);
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
    return m_renders.value(index);
}

int FCGraph3DWindowVTK::getRenderCount()
{
    return m_renders.size();
}

vtkRenderWindow* FCGraph3DWindowVTK::getVTKRenderWindow()
{
    return m_renderWindow;
}

vtkCamera* FCGraph3DWindowVTK::getVTKCamera()
{
    return m_camera;
}

vtkRenderer* FCGraph3DWindowVTK::getVTKRenderer(int index)
{
    if (index < 0)
        index = m_renders.size() - 1;
    FCGraphRender* r = m_renders.value(index);
    return r ? r->getRenderer() : nullptr;
}

void FCGraph3DWindowVTK::setParallelProjection(bool on)
{
    if (m_camera)
        m_camera->SetParallelProjection(on ? 1 : 0);
}

bool FCGraph3DWindowVTK::getParallelProjection()
{
    return m_camera && m_camera->GetParallelProjection();
}

void FCGraph3DWindowVTK::removeGraphObj(FCAbstractGraphObject* gobj)
{
    FCGraphObjectVTK* objvtk = dynamic_cast<FCGraphObjectVTK*>(gobj);
    if (!objvtk)
        return;
    for (FCGraphRender* render : m_renders)
    {
        if (render)
            render->removeObject(objvtk);
    }
}

int FCGraph3DWindowVTK::getGraphObjCount()
{
    int c = 0;
    for (FCGraphRender* render : m_renders)
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
    for (FCGraphRender* render : m_renders)
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
    if (image.isEmpty() || !m_renderWindow)
        return;
    vtkWindowToImageFilter* imageFilter = vtkWindowToImageFilter::New();
    imageFilter->SetInput(m_renderWindow);

    switch (_imageType)
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
    if (m_renderWindow)
        m_renderWindow->Finalize();
    qDebug().noquote() << QString("Viewports printed to \"%1\"").arg(image);
}

void FCGraph3DWindowVTK::showScaleLegend(bool visibility)
{
    if (m_legendScaleActor)
        m_legendScaleActor->SetVisibility(visibility ? 1 : 0);
}

void FCGraph3DWindowVTK::addActionsToolBar(QList<QAction*> actions, QAction* beforeAction)
{
    QMutexLocker locker(&_mutex);
    if (!_renderToolBarLayout || !_renderToolBar)
        return;
    _renderToolBar->insertActions(beforeAction, actions);
    setToolBarPos(_renderBarStyle);
    QList<QToolButton*> buttons = _renderToolBar->findChildren<QToolButton*>();
    for (QToolButton* b : buttons)
    {
        if (b)
        {
            b->setStyleSheet("background: transparent;");
            b->setToolTipDuration(1);
        }
    }
}

void FCGraph3DWindowVTK::setToolBarPos(FCGraphWinToolBarPos pos)
{
    if (!_renderToolBar || !_renderToolBarLayout)
        return;
    _renderBarStyle = pos;
    switch (pos)
    {
    case FCGraphWinToolBarPos::FCLeftTop:
        _renderToolBar->setOrientation(Qt::Vertical);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCLeftCenter:
        _renderToolBar->setOrientation(Qt::Vertical);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
        break;
    case FCGraphWinToolBarPos::FCLeftBottom:
        _renderToolBar->setOrientation(Qt::Vertical);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCBottomCenter:
        _renderToolBar->setOrientation(Qt::Horizontal);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignHCenter | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightBottom:
        _renderToolBar->setOrientation(Qt::Vertical);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignRight | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightCenter:
        _renderToolBar->setOrientation(Qt::Vertical);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
        break;
    case FCGraphWinToolBarPos::FCRightTop:
        _renderToolBar->setOrientation(Qt::Vertical);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignRight | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCTopCenter:
        _renderToolBar->setOrientation(Qt::Horizontal);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignHCenter | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCTopLeft:
        _renderToolBar->setOrientation(Qt::Horizontal);
        _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop);
        break;
    }
}

void FCGraph3DWindowVTK::initRenderIcon(const QPixmap& image, FCGraphWinToolBarPos pos)
{
    QMutexLocker locker(&_mutex);
    if (!_renderIconToolBar || !_renderToolBarLayout)
        return;
    _renderIconToolBar->clear();
    QLabel* label = new QLabel(m_vtkWidget);
    label->setPixmap(image);
    _renderIconToolBar->addWidget(label);
    switch (pos)
    {
    case FCGraphWinToolBarPos::FCLeftTop:
        _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop);
        break;
    case FCGraphWinToolBarPos::FCLeftBottom:
        _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightBottom:
        _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignRight | Qt::AlignBottom);
        break;
    case FCGraphWinToolBarPos::FCRightTop:
        _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignRight | Qt::AlignTop);
        break;
    default:
        break;
    }
}

QList<QAction*> FCGraph3DWindowVTK::getActions()
{
    return _renderToolBar ? _renderToolBar->actions() : QList<QAction*>();
}

void FCGraph3DWindowVTK::setIsShowActions(bool isShow)
{
    if (_renderToolBar)
        _renderToolBar->setVisible(isShow);
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
        if (m_actorBound)
            m_actorBound->updateBoundary(bound);
    }
}

bool FCGraph3DWindowVTK::getBoundray(double* bds)
{
    double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };
    bool hasAny = false;
    for (FCGraphRender* render : m_renders)
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
    if (m_renderWindow)
        m_renderWindow->SetGlobalWarningDisplay(isOpen ? 1 : 0);
}

void FCGraph3DWindowVTK::SetUseDepthPeeling(bool isOpen, int peelMaxNum, int renderIndex)
{
    Q_UNUSED(isOpen);
    Q_UNUSED(peelMaxNum);
    Q_UNUSED(renderIndex);
}

FCGraphInteractionStyle* FCGraph3DWindowVTK::getGraphInteractionStyle()
{
    return m_interactionStyle;
}

void FCGraph3DWindowVTK::setUserInformation(QVariant info)
{
    m_userInfo = info;
}

QVariant FCGraph3DWindowVTK::getUserInformation()
{
    return m_userInfo;
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
    setView(_ISOFp, _ISOVp, _ISOPos);
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
        _imageType = FCGraphWinImageType::FC_PNG;
    else if (suffix == "jpeg" || suffix == "jpg")
        _imageType = FCGraphWinImageType::FC_JPEG;
    else if (suffix == "tiff" || suffix == "tif")
        _imageType = FCGraphWinImageType::FC_TIFF;
    else if (suffix == "bmp")
        _imageType = FCGraphWinImageType::FC_BMP;
    else if (suffix == "svg")
        _imageType = FCGraphWinImageType::FC_SVG;
    else
        _imageType = FCGraphWinImageType::FC_PNG;
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
    if (m_timerView.isActive())
        m_timerView.stop();
    m_currStep = 0;
    for (int i = 0; i < 3; i++)
    {
        m_tarFoc3[i] = foc[i];
        m_tarUp3[i] = up[i];
        m_tarPos3[i] = pos[i];
    }
    m_timerView.start();
}

void FCGraph3DWindowVTK::slot_changingView()
{
    m_currStep++;
    if (m_currStep >= m_viewStep)
    {
        if (m_camera)
        {
            m_camera->SetViewUp(m_tarUp3);
            m_camera->SetPosition(m_tarPos3);
            m_camera->SetFocalPoint(m_tarFoc3);
        }
        return;
    }
    if (!m_camera)
        return;
    double* currFoc = m_camera->GetFocalPoint();
    double* currUp = m_camera->GetViewUp();
    double* currPos = m_camera->GetPosition();
    int steps = m_viewStep - m_currStep;
    double deltaFoc3[3], deltaUp3[3], deltaPos3[3];
    for (int i = 0; i < 3; i++)
    {
        deltaFoc3[i] = (m_tarFoc3[i] - currFoc[i]) / steps;
        deltaUp3[i] = (m_tarUp3[i] - currUp[i]) / steps;
        deltaPos3[i] = (m_tarPos3[i] - currPos[i]) / steps;
    }
    m_camera->SetViewUp(currUp[0] + deltaUp3[0], currUp[1] + deltaUp3[1], currUp[2] + deltaUp3[2]);
    m_camera->SetPosition(currPos[0] + deltaPos3[0], currPos[1] + deltaPos3[1], currPos[2] + deltaPos3[2]);
    m_camera->SetFocalPoint(currFoc[0] + deltaFoc3[0], currFoc[1] + deltaFoc3[1], currFoc[2] + deltaFoc3[2]);
    reRender();
    m_timerView.start();
}

void FCGraph3DWindowVTK::init()
{
    FCGraphRender* lastRender = getRenderer(m_renders.count() - 1);
    if (_initializer && _initializer->getLayerCount() > 0 && lastRender && m_actorBound)
    {
        vtkActor* actor = m_actorBound->getActor();
        if (actor)
            lastRender->getRenderer()->AddViewProp(actor);
    }
    if (lastRender && lastRender->getRenderer())
    {
        m_legendScaleActor = FCLegendScaleActor::New();
        m_legendScaleActor->SetVisibility(0);
        lastRender->getRenderer()->AddViewProp(m_legendScaleActor);
    }
    vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
    m_axesWidget = vtkOrientationMarkerWidget::New();
    m_axesWidget->SetInteractor(m_interactor);
    m_axesWidget->SetOrientationMarker(axesActor);
    FCGraphRender* r0 = getRenderer(0);
    if (r0 && r0->getRenderer())
        m_axesWidget->SetDefaultRenderer(r0->getRenderer());

    for (FCGraphRender* fRender : m_renders)
    {
        if (!fRender || !fRender->getRenderer())
            continue;
        vtkRenderer* renderer = fRender->getRenderer();
        renderer->RemoveAllLights();
        vtkLight* light1 = renderer->MakeLight();
        light1->SetLightTypeToHeadlight();
        renderer->AddLight(light1);
    }
    if (_initializer)
        setInteractionStyle(_initializer->getStyle());
}

void FCGraph3DWindowVTK::initActions()
{
    QList<QAction*> actions;
    QAction* a = nullptr;
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewFront");
    a->setIcon(QIcon(":/icon/icoR_viewFront.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewFrontEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewBack");
    a->setIcon(QIcon(":/icon/icoR_viewBack.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewBackEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewTop");
    a->setIcon(QIcon(":/icon/icoR_viewTop.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewTopEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewBottom");
    a->setIcon(QIcon(":/icon/icoR_viewBottom.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewBottomEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewLeft");
    a->setIcon(QIcon(":/icon/icoR_viewLeft.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewLeftEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewRight");
    a->setIcon(QIcon(":/icon/icoR_viewRight.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewRightEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewIso");
    a->setIcon(QIcon(":/icon/icoR_viewIso.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewIsoEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewAuto");
    a->setIcon(QIcon(":/icon/icoR_autofit.png"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewPanEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionSaveImage");
    a->setIcon(QIcon(":/icon/icoR_saveimage.png"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionSaveImageEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewParallel");
    a->setIcon(QIcon(":/icon/icoR_viewParallel.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewParallelEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionViewPerspective");
    a->setIcon(QIcon(":/icon/icoR_viewPerspective.svg"));
    connect(a, &QAction::triggered, this, &FCGraph3DWindowVTK::slotActionViewPerspectiveEvent);
    actions.append(a);
    a = new QAction(m_vtkWidget);
    a->setObjectName("actionRenderClip");
    a->setIcon(QIcon(":/icon/icoR_clip.png"));
    a->setVisible(false);
    connect(a, &QAction::triggered, this, [this]() { slotActionClipEvent(); });
    actions.append(a);
    addActionsToolBar(actions);
}

void FCGraph3DWindowVTK::setInteractionStyle(FCGraphInteractionStyle* style)
{
    if (m_interactionStyle)
    {
        m_interactionStyle->Delete();
        m_interactionStyle = nullptr;
    }
    m_interactionStyle = style;
    if (m_interactionStyle)
    {
        m_interactionStyle->setGraph3DWin(this);
        m_interactionStyle->setInteractor(m_interactor);
        if (m_renders.count() > 0)
        {
            FCGraphRender* r = m_renders.value(0);
            if (r && r->getRenderer())
                m_interactionStyle->setAreaPickRenderer(r->getRenderer());
        }
    }
}
} // namespace FC
