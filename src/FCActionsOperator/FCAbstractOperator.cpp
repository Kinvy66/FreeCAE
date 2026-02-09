#include "FCAbstractOperator.h"

namespace FC 
{
FCAbstractOperator::FCAbstractOperator()
    : _emitter(nullptr)
{
}

FCAbstractOperator::~FCAbstractOperator()
{
}

void FCAbstractOperator::setEmitter(QObject* obj)
{
    _emitter = obj;
}

void FCAbstractOperator::setArgs(const QString& argName, QVariant arg)
{
    _operArgs.insert(argName, arg);
}

void FCAbstractOperator::removeArgs(const QString& argName)
{
    _operArgs.remove(argName);
}

bool FCAbstractOperator::hasArgs(const QString& argName)
{
    return _operArgs.contains(argName);
}

void FCAbstractOperator::clearArgs()
{
    _operArgs.clear();
}

} // namespace FC
