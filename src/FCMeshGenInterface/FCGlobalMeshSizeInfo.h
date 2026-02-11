/**
 * @file FCGlobalMeshSizeInfo.h
 * @brief 全局网格尺寸（移植自 FITKInterfaceMeshGen FITKGlobalMeshSizeInfo）
 */
#ifndef FCGLOBALMESHSIZEINFO_H
#define FCGLOBALMESHSIZEINFO_H

#include "FCMeshGenInterfaceAPI.h"
#include <QObject>
#include <FCData/FCVariantParams.h>

namespace FC {

/**
 * @brief 全局网格尺寸
 */
class FCMESHGENINTERFACE_API FCGlobalMeshSizeInfo : public QObject, public FCVariantParams
{
    Q_OBJECT
public:
    explicit FCGlobalMeshSizeInfo(QObject* parent = nullptr);
    virtual ~FCGlobalMeshSizeInfo() override = default;

    void setGlobalSize(double s);
    double getGlobalSize() const;

    void setMinSize(double s);
    double getMinSize() const;

    void setMaxSize(double s);
    double getMaxSize() const;

    void setSizeFactor(double f);
    double getSizeFactor() const;

protected:
    double _globalSize{0};
    double _minSize{1.0};
    double _maxSize{5.0};
    double _sizeFactor{1.0};
};

} // namespace FC

#endif // FCGLOBALMESHSIZEINFO_H
