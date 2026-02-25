/**
 * @file FCGraphPreprocessOperator.h
 * @brief 前处理可视化事件操作器
 * @date 2026-02-24
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGRAPHPREPROCESSOPERATOR_H
#define FCGRAPHPREPROCESSOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCGraphEventOperator.h"
#include <QList>

namespace FC
{
class FCGraph3DWindowVTK;
class FCAbsGeoCommand;
class FCDockingAreaInterface;
class FCACTIONSOPERATOR_API FCGraphPreprocessOperator : public FCGraphEventOperator
{
    Q_OBJECT
public:
    explicit FCGraphPreprocessOperator();
    ~FCGraphPreprocessOperator();
   
    /**
     * @brief   根据提供的数据对象ID更新对应可视化对象。（没有则创建）[重写]
     * @param   dataObjId：数据对象ID
     * @param   forceUpdate：是否强制刷新[缺省]
     */
    void updateGraph(int dataObjId, bool forceUpdate = false) override;
    
    /**
     * @brief 更新所有可视化对象
     */
    virtual void updateGraph() override;
    
    
    /**
     * @brief   根据提供的数据类型创建可视化对象并执行相关逻辑。（没有则创建）[重写]
     *          （此接口用于非数据类可视化对象预览。）
     * @param   type：数据类型
     * @param   param：可视化对象执行操作参数（默认只进行可视化）[缺省]
     */
    // void updateGraphByType(int type, GraphOperParam param = GraphOperParam()) override;
    
    /**
     * @brief   根据数据对象ID获取模型可视化对象。（没有则不创建）[重写]
     * @param   dataObjId：数据对象ID
     * @return  可视化对象
     */
    FCVTKGraphObject3D* getModelGraphObjectByDataId(int dataObjId) override;
    
    /**
     * @brief   将现有模型全部开启或关闭半透明显示。[重写]
     * @param   flag：是否开启
     */
    void setEnableModelTransparent(bool flag) override;
    
    /**
     * @brief   将现有（边界）网格全部开启或关闭半透明显示。[重写]
     * @param   flag：是否开启
     */
    void setEnableMeshTransparent(bool flag) override;
    
    /**
     * @brief   隐藏或显示模型。[重写]
     * @param   dataObjId：数据对象ID
     * @param   visibility：是否可见
     */
    void setModelVisible(int dataObjId, bool visibility) override;
    
    /**
     * @brief   隐藏或显示（边界）网格。[重写]
     * @param   dataObjId：数据对象ID
     * @param   visibility：是否可见
     */
    void setMeshVisible(int dataObjId, bool visibility) override;

    /**
     * @brief   高亮可视化对象。[重写]
     * @param   dataObjId：数据对象ID
     * @param   info：附加信息[缺省]
     * @param   color：高亮颜色[缺省]
     */
    void highlight(int dataObjId, QVariant info = QVariant(), QColor color = QColor()) override;
    
    /**
     * @brief   根据附加信息高级高亮指定的数据。[重写]
     * @param   dataObjId：数据对象ID
     * @param   indice：附加信息（索引或数据ID）[引用]
     * @param   color：高亮颜色[缺省]
     */
    void advHighlight(int dataObjId, QVector<int> & indice, QColor color = QColor()) override;
    
    /**
     * @brief   取消全部高亮、高级高亮数据的高亮效果。[重写]
     */
    void clearHighlight() override;
 
    /**
     * @brief   刷新渲染窗口。[重写]
     * @param   fitView：是否重置相机
     */
    void reRender(bool fitView = false) override;

    /** 无 UI 注入时可由调用方直接设置 Docking（如 FCGeometryPropertyConnector） */
    void setDockingArea(FCDockingAreaInterface* docking) { m_docking = docking; }

public slots:
    /** 几何体构建后刷新 VTK（单命令） */
    void onGeometryBuilt(FCAbsGeoCommand* cmd);
    /** 几何序列全部构建后刷新 VTK */
    void onGeometrySequenceBuilt(const QList<FCAbsGeoCommand*>& cmds);

private:
    FCGraph3DWindowVTK* getGraphWidget();
    FCDockingAreaInterface* m_docking{ nullptr };
};
} // namespace FC



#endif // FCGRAPHPREPROCESSOPERATOR_H
