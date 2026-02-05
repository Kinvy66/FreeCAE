/**
 * @file FCGraph3DWindowVTK.h
 * @brief 三维渲染窗口接口声明
 */
#ifndef FCGRAPH3DWINDOWVTK_H
#define FCGRAPH3DWINDOWVTK_H

#include "FCRenderWindowVTKAPI.h"
#include "FCAbstractGraph3DWidget.h"
#include <QWidget>
#include <QMutex>
#include <QVariant>
#include <QTimer>
#include <QHash>

class vtkRenderWindow;
class vtkCamera;
class vtkRenderWindowInteractor;
class vtkOrientationMarkerWidget;
class vtkActor;
class vtkRenderer;
class QGridLayout;
class QToolBar;

namespace FC
{
class FCGraphRender;
class FCGraphObjectVTK;
class FCGraphInteractionStyle;
class FCBoundaryActor;
class FCLegendScaleActor;
class FCGraph3DWindowInitializer;
}

namespace FC
{
enum class FCGraphWinToolBarPos
{
    FCLeftTop = 1,
    FCLeftCenter,
    FCLeftBottom,
    FCBottomCenter,
    FCRightBottom,
    FCRightCenter,
    FCRightTop,
    FCTopCenter,
    FCTopLeft,
};

enum class FCGraphWinImageType
{
    FC_PNG = 0,
    FC_JPEG,
    FC_TIFF,
    FC_BMP,
    FC_SVG,
};

class FCRENDERWINDOWVTK_API FCGraph3DWindowVTK : public FCAbstractGraph3DWidget
{
    Q_OBJECT
public:
    explicit FCGraph3DWindowVTK(FCGraph3DWindowInitializer* initializer);
    ~FCGraph3DWindowVTK() override;

    void setHiddenLineRemoval(bool flag);
    bool getHiddenLineRemoval();

    void reRender();
    void setAxesEnable(bool state);
    void setViewAnime(double* focalPoint, double* viewUp, double* position);

    void setView(double* focalPoint, double* viewUp, double* position) override;
    void setDefaultView(double* focalPoint, double* viewUp, double* position);
    void setISOView(double* focalPoint, double* viewUp, double* position);
    bool getView(double* pos, double* focual, double* viewup) override;
    void setViewBounds(double* bds) override;

    void fastAddObject(int index, FCGraphObjectVTK* object);
    void addObject(int index, FCGraphObjectVTK* object, bool fitview = true);
    QList<FCGraphObjectVTK*> getAllGraphObj();
    void clear() override;
    void setBackground(double topValue[3], double bottomValue[3]);
    void fitView() override;
    void flush() override;
    void setBackgroundColor(float* rgb1, float* rgb2 = nullptr) override;
    void getBackgroundColor(float* rgb1, float* rgb2 = nullptr) override;
    FCGraphRender* getRenderer(int index);
    int getRenderCount();
    vtkRenderWindow* getVTKRenderWindow();
    vtkCamera* getVTKCamera();
    vtkRenderer* getVTKRenderer(int index = -1);

    void setParallelProjection(bool on = true) override;
    bool getParallelProjection() override;
    void removeGraphObj(FCAbstractGraphObject* gobj) override;
    int getGraphObjCount();
    bool getGraphObjCenter(double* center);

    template<class T>
    void setUserInformation(T info) { mUserInfo = QVariant::fromValue(info); }
    template<class T>
    T getUserInformationAs() { return mUserInfo.value<T>(); }
    void setUserInformation(QVariant info);
    QVariant getUserInformation();

    void saveImage(const QString& image, int w = -1, int h = -1) override;
    void showScaleLegend(bool visibility);
    void addActionsToolBar(QList<QAction*> actions, QAction* beforeAction = nullptr);
    void setToolBarPos(FCGraphWinToolBarPos pos);
    void initRenderIcon(const QPixmap& image, FCGraphWinToolBarPos pos = FCGraphWinToolBarPos::FCRightTop);
    QList<QAction*> getActions();
    void setIsShowActions(bool isShow);
    void setIsShowClipAction(bool isShow);
    void updateBoundary();
    bool getBoundray(double* bds);
    void SetGlobalWarningDisplay(bool isOpen);
    void SetUseDepthPeeling(bool isOpen, int peelMaxNum, int renderIndex = -1);
    FCGraphInteractionStyle* getGraphInteractionStyle();

public slots:
    virtual void slotActionViewFrontEvent();
    virtual void slotActionViewBackEvent();
    virtual void slotActionViewTopEvent();
    virtual void slotActionViewBottomEvent();
    virtual void slotActionViewLeftEvent();
    virtual void slotActionViewRightEvent();
    virtual void slotActionViewIsoEvent();
    virtual void slotActionViewPanEvent();
    virtual void slotActionSaveImageEvent();
    virtual void slotActionViewParallelEvent();
    virtual void slotActionViewPerspectiveEvent();
    virtual void slotActionClipEvent(bool isCreateNewModel = true);

protected:
    virtual void init();
    virtual void initActions();
    void setInteractionStyle(FCGraphInteractionStyle* style);
    void showEvent(QShowEvent* event) override;

private:
    void startChangingView(double* foc, double* up, double* pos);

private slots:
    void slot_changingView();

protected:
    FCGraph3DWindowInitializer* mInitializer = nullptr;
    QWidget* mVtkWidget = nullptr;
    FCLegendScaleActor* mLegendScaleActor = nullptr;
    vtkRenderWindowInteractor* mInteractor = nullptr;
    vtkRenderWindow* mRenderWindow = nullptr;
    vtkCamera* mCamera = nullptr;
    vtkOrientationMarkerWidget* mAxesWidget = nullptr;
    QHash<int, FCGraphRender*> mRenders;
    FCGraphInteractionStyle* mInteractionStyle = nullptr;
    FCBoundaryActor* mActorBound = nullptr;
    QVariant mUserInfo;
    bool mInitFlag = false;
    QGridLayout* mRenderToolBarLayout = nullptr;
    QToolBar* mRenderToolBar = nullptr;
    QToolBar* mRenderIconToolBar = nullptr;
    FCGraphWinToolBarPos mRenderBarStyle = FCGraphWinToolBarPos::FCTopLeft;
    FCGraphWinImageType mImageType = FCGraphWinImageType::FC_PNG;
    double mDefFp[3] = { 0, 0, 0 }, mDefVp[3] = { 0, 1, 0 }, mDefPos[3] = { 1, 1, 1 };
    double mISOFp[3] = { 1, 1, 1 }, mISOVp[3] = { 0, 0, 0 }, mISOPos[3] = { 0, 1, 0 };

private:
    static QMutex mMutex;
    QTimer mTimerView;
    int mViewStep = 50;
    int mCurrStep = 0;
    int mInterval = 10;
    double mTarPos3[3] = { 0., 0., 0. };
    double mTarUp3[3] = { 0., 0., 0. };
    double mTarFoc3[3] = { 0., 0., 0. };
};
} // namespace FC
#endif
