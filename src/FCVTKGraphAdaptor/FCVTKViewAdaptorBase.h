/**
 * @file FCVTKViewAdaptorBase.h
 * @brief 数据 -> VTK 图形对象适配器基类（移植自 FITKFluidVTKGraphAdaptor FITKFluidVTKViewAdaptorBase）
 */
#ifndef FCVTKVIEWADAPTORBASE_H
#define FCVTKVIEWADAPTORBASE_H

#include "FCVTKGraphAdaptorAPI.h"
#include <FCData/FCAbstractDataObject.h>
#include <QVariant>

namespace FC {

class FCVTKGraphObject3D;

class FCVTKGRAPHADAPTOR_API FCVTKViewAdaptorBase
{
public:
    FCVTKViewAdaptorBase() = default;
    virtual ~FCVTKViewAdaptorBase() = default;

    void setDataObject(FCAbstractDataObject* data);
    void setDetails(QVariant details);
    FCVTKGraphObject3D* getOutputData();
    virtual bool update();

protected:
    FCAbstractDataObject* _dataObj = nullptr;
    QVariant m_details;
    FCVTKGraphObject3D* m_outputData = nullptr;
    bool m_needUpdate = false;
};

} // namespace FC

#endif // FCVTKVIEWADAPTORBASE_H
