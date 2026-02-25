/**
 * @file FCBuildAllGeometryOperator.h
 * @brief 构建所有几何体操作器
 * @date 2026-02-25
 * @version V0.0.1
 * @details 
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCBUILDALLGEOMETRYOPERATOR_H
#define FCBUILDALLGEOMETRYOPERATOR_H

#include "FCActionsOperatorAPI.h"
#include "FCActionOperator.h"

namespace FC
{
class FCDockingAreaInterface;
class IGeometryBuildContent;

class FCACTIONSOPERATOR_API FCBuildAllGeometryOperator : public FCActionOperator
{
    Q_OBJECT
public:
    FCBuildAllGeometryOperator();
    bool execGUI() override;
    void setDockingArea(FCDockingAreaInterface* docking) { m_docking = docking; }
    FCDockingAreaInterface* getDockingArea() const;

private:
    static IGeometryBuildContent* findGeometryBuildContent(QWidget* w);
    FCDockingAreaInterface* m_docking{ nullptr };
};
} // namespace FC


#endif // FCBUILDALLGEOMETRYOPERATOR_H
