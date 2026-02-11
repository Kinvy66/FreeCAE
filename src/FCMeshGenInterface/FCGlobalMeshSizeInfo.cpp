/**
 * @file FCGlobalMeshSizeInfo.cpp
 * @brief 全局网格尺寸实现（移植自 FITKInterfaceMeshGen FITKGlobalMeshSizeInfo）
 */
#include "FCGlobalMeshSizeInfo.h"

namespace FC {

FCGlobalMeshSizeInfo::FCGlobalMeshSizeInfo(QObject* parent) : QObject(parent) {}

void FCGlobalMeshSizeInfo::setGlobalSize(double s)
{
    if (s >= 0) _globalSize = s;
}

double FCGlobalMeshSizeInfo::getGlobalSize() const { return _globalSize; }

void FCGlobalMeshSizeInfo::setMinSize(double s) { _minSize = s; }
double FCGlobalMeshSizeInfo::getMinSize() const { return _minSize; }

void FCGlobalMeshSizeInfo::setMaxSize(double s) { _maxSize = s; }
double FCGlobalMeshSizeInfo::getMaxSize() const { return _maxSize; }

void FCGlobalMeshSizeInfo::setSizeFactor(double f) { _sizeFactor = f; }
double FCGlobalMeshSizeInfo::getSizeFactor() const { return _sizeFactor; }

} // namespace FC
