/**
 * @file FCGeometryDAGData.cpp
 * @brief COMSOL 式函数式几何数据实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryDAGData.h"
#include <FCGeometryInterface/FCGlobalGeoComponent.h>
#include <QMetaObject>

namespace FC {

FCGeometryDAGData::FCGeometryDAGData(int parentDataID)
    : FCAbstractDataObject(parentDataID)
{
    m_module.reset(new FCGeometryModule(this));
    m_compMgr.reset(new FCGlobalGeoComponentManager);
    connect(m_module->engine(), &FCGeometryBuildEngine::buildFinished,
            this, &FCGeometryDAGData::onBuildFinished);
}

FCGeometryDAGData::~FCGeometryDAGData() = default;

void FCGeometryDAGData::setExecutor(FCGeometryNodeExecutor* executor)
{
    if (m_module)
        m_module->setExecutor(executor);
}

void FCGeometryDAGData::ensureBuild()
{
    if (!m_module || !m_module->engine() || !m_module->engine()->executor())
        return;
    if (m_module->tree() && m_module->tree()->nodeCount() > 0)
        m_module->build();
}

void FCGeometryDAGData::onBuildFinished()
{
    if (!m_compMgr || !m_buildResultFiller)
        return;
    QVariant result = m_module->engine()->nodeResult(m_module->lastOutputNodeId());
    if (result.isValid())
        m_buildResultFiller(result, m_compMgr.get());
}

QString FCGeometryDAGData::serialize(int /*label*/)
{
    // TODO: 序列化 DAG（FCGeometryTree 节点列表）用于存盘
    return QString();
}

bool FCGeometryDAGData::deserialize(const QString& /*text*/, int /*label*/)
{
    // TODO: 反序列化 DAG
    return true;
}

bool FCGeometryDAGData::copy(FCAbstractDataObject* obj)
{
    FCGeometryDAGData* other = dynamic_cast<FCGeometryDAGData*>(obj);
    if (!other)
        return FCAbstractDataObject::copy(obj);
    // 浅拷贝：仅拷贝数据对象基类信息；DAG 内容可后续扩展
    return FCAbstractDataObject::copy(obj);
}

} // namespace FC
