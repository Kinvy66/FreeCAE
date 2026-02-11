/**
 * @file FCAbstractMeshProcessor.cpp
 * @brief 网格后处理器抽象类实现（移植自 FITKInterfaceMeshGen）
 */
#include "FCAbstractMeshProcessor.h"

namespace FC {

FCAbstractMeshProcessor::~FCAbstractMeshProcessor() {}

void FCAbstractMeshProcessor::setArgs(const QStringList& args)
{
    _args = args;
}

void FCAbstractMeshProcessor::insertDataObject(const QString& key, FCAbstractDataObject* value)
{
    _dataObject.insert(key, value);
}

FCAbstractDataObject* FCAbstractMeshProcessor::getDataObject(const QString& key) const
{
    return _dataObject.value(key, nullptr);
}

} // namespace FC
