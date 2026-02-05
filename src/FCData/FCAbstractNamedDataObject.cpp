/**
 * @file FCAbstractNamedDataObject.cpp
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#include "FCAbstractNamedDataObject.h"

namespace FC {

FCAbstractNamedDataObject::FCAbstractNamedDataObject(const QString& name)
    : m_dataObjName(name)
{
}

FCAbstractNamedDataObject::~FCAbstractNamedDataObject() = default;

void FCAbstractNamedDataObject::setDataObjectName(const QString& name)
{
    m_dataObjName = name;
}

QString FCAbstractNamedDataObject::getDataObjectName() const
{
    return m_dataObjName;
}

bool FCAbstractNamedDataObject::copy(FCAbstractDataObject* obj)
{
    auto* nobj = dynamic_cast<FCAbstractNamedDataObject*>(obj);
    if (!nobj) return false;
    m_dataObjName = nobj->getDataObjectName();
    return FCAbstractDataObject::copy(obj);
}

} // namespace FC
