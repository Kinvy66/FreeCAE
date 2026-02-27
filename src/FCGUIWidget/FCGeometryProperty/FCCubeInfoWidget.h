#ifndef FCCUBEINFOWIDGET_H
#define FCCUBEINFOWIDGET_H

#include "../FCGUIWidgetAPI.h"
#include "IGeometryBuildContent.h"
#include <FCData/FCType.h>
#include <QWidget>
#include <QList>

namespace Ui {
class FCCubeInfoWidget;
}

namespace FC {

class FCGeoModelBox;
class FCAbsGeoCommand;
class FCGeometryDAGData;

class FCGUIWIDGET_API FCCubeInfoWidget : public QWidget, public IGeometryBuildContent
{
    Q_OBJECT

public:
    explicit FCCubeInfoWidget(QWidget *parent = nullptr);
    ~FCCubeInfoWidget() override;

  
    /**
     * @brief 根据 boxCmd 将基点与长宽高设置到 UI 对应控件
     * @param boxCmd
     */
    void setBoxCommand(FCGeoModelBox* boxCmd);
    
    /**
     * @brief 绑定 DAG 节点；displayBoxCmd 用于「构建」后在 VTK 中显示
     * @param dagData
     * @param nodeId
     * @param displayBoxCmd
     */
    void setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelBox* displayBoxCmd = nullptr);

    void syncToModel() override;
    FCAbsGeoCommand* getCurrentBuildCommand() override;
    void executeBuild() override;
    void executeBuildAll() override;

signals:

    /**
     * @brief 构建了单个几何体，供在 VTK 中显示
     * @param cmd
     */
    void geometryBuilt(FCAbsGeoCommand* cmd);
    
  
    /**
     * @brief 按 COMSOL 方式构建了几何序列（从第一个到当前选中），供在 VTK 中刷新显示
     * @param cmds
     */
    void geometrySequenceBuilt(const QList<FCAbsGeoCommand*>& cmds);

private:
    
    /**
     * @brief 将当前 UI 上的属性值立即写回内存
     */
    void syncValuesToModel();
    void rebuildGeometryEntityModel();
    
private:

    Ui::FCCubeInfoWidget *ui;
    /** 名称输入框是否被用户手动修改过；未修改时构建不更改自动生成的几何体名称 */
    bool mNameManuallyEdited{ false };
    FCGeoModelBox* mBoxCmd{ nullptr };
    FCGeometryDAGData* mDagData{ nullptr };
    FCID mNodeId{ FCID_INVALID };
    /** DAG 路径下仅用于 VTK 显示的 Box 命令（不加入命令列表） */
    FCGeoModelBox* mDisplayBoxCmd{ nullptr };
    /** 若为 true 表示 m_displayBoxCmd 由本 widget 创建并负责释放 */
    bool mOwnDisplayBoxCmd{ false };
};

} // namespace FC

#endif // FCCUBEINFOWIDGET_H
