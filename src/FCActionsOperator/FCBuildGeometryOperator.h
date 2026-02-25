/**
 * @file FCBuildGeometryOperator.h
 * @brief 几何体构建操作器
 * @date 2026-02-25
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCBUILDGEOMETRYOPERATOR_H
#define FCBUILDGEOMETRYOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"

namespace FC
{
class FCDockingAreaInterface;
class IGeometryBuildContent;

class FCACTIONSOPERATOR_API FCBuildGeometryOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCBuildGeometryOperator();
    bool execGUI() override;
    /** 无 UI 注入时可由调用方直接设置 Docking（如 FCGeometryPropertyConnector） */
    void setDockingArea(FCDockingAreaInterface* docking) { m_docking = docking; }
    FCDockingAreaInterface* getDockingArea() const;

private:
    static IGeometryBuildContent* findGeometryBuildContent(QWidget* w);
    FCDockingAreaInterface* m_docking{ nullptr };
};
} // namespace FC


#endif // FCBUILDGEOMETRYOPERATOR_H
