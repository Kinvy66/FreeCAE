/**
 * @file FCAbstractMesherDriver.cpp
 * @brief 网格驱动器抽象类实现（移植自 FITKInterfaceMeshGen）
 */
#include "FCAbstractMesherDriver.h"
#include <FCData/FCAbstractDataObject.h>

namespace FC {

FCAbstractMesherDriver::FCAbstractMesherDriver(QObject* parent) : QObject(parent) {}

FCAbstractMesherDriver::~FCAbstractMesherDriver() {}

void FCAbstractMesherDriver::setDataObject(FCAbstractDataObject* data)
{
    _data = data;
}

void FCAbstractMesherDriver::setArgs(const QStringList& args)
{
    _args = args;
}

void FCAbstractMesherDriver::insertDataObject(const QString& key, FCAbstractDataObject* value)
{
    _dataObject.insert(key, value);
}

} // namespace FC
