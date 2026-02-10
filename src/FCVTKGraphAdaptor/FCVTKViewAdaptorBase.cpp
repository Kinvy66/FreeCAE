/**
 * @file FCVTKViewAdaptorBase.cpp
 */
#include "FCVTKViewAdaptorBase.h"
#include "FCVTKGraphObject3D.h"

namespace FC {

void FCVTKViewAdaptorBase::setDataObject(FCAbstractDataObject* data)
{
    if (data && data != _dataObj)
        m_needUpdate = true;
    _dataObj = data;
}

void FCVTKViewAdaptorBase::setDetails(QVariant details)
{
    m_details = details;
}

FCVTKGraphObject3D* FCVTKViewAdaptorBase::getOutputData()
{
    return m_outputData;
}

bool FCVTKViewAdaptorBase::update()
{
    return true;
}

} // namespace FC
