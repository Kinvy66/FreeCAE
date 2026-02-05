/**
 * @file FCGraph3DClipDialog.h
 * @brief 三维渲染窗口裁切对话框
 */
#ifndef FCGRAPH3DCLIPDIALOG_H
#define FCGRAPH3DCLIPDIALOG_H

#include "FCRenderWindowVTKAPI.h"
#include <QDialog>

namespace FC
{
class FCGraph3DWindowVTK;

class FCRENDERWINDOWVTK_API FCGraph3DClipDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FCGraph3DClipDialog(FCGraph3DWindowVTK* graph3DWindow, bool clipCreateNewModel, QWidget* parent = nullptr);
    ~FCGraph3DClipDialog() override;
    void init();

protected:
    void showEvent(QShowEvent* event) override;

private:
    FCGraph3DWindowVTK* mGraph3DWindow = nullptr;
    bool mClipCreateNewModel = true;
};
} // namespace FC
#endif
