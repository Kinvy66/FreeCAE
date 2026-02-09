#include "FCActionCreateCubeOperator.h"
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCData/FCDataRepo.h>
#include <QDebug>

namespace FC 
{

FCActionCreateCubeOperator::FCActionCreateCubeOperator()
{
}

bool FCActionCreateCubeOperator::execGUI()
{
    // 目前简化实现，直接返回true，后续可以添加对话框等UI交互
    return true;
}

bool FCActionCreateCubeOperator::execProfession()
{
    // 获取几何接口工厂
    FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
    if (!factory) {
        qWarning() << "FCActionCreateCubeOperator: FCGeoInterfaceFactory is null";
        return false;
    }

    // 创建Box几何命令
    FCGeoModelBox* boxCmd = factory->createCommandT<FCGeoModelBox>(FCGeoEnum::FGTBox);
    if (!boxCmd) {
        qWarning() << "FCActionCreateCubeOperator: Failed to create FCGeoModelBox";
        return false;
    }

    // 设置默认参数：基点(0,0,0)，长度(100,100,100)
    double point1[3] = { 0.0, 0.0, 0.0 };
    double length[3] = { 100.0, 100.0, 100.0 };
    boxCmd->setPoint1(point1);
    boxCmd->setLength(length);

    // 设置名称
    QString name = QString("Box_%1").arg(boxCmd->getDataObjectID());
    boxCmd->setName(name);

    // 更新几何
    if (!boxCmd->update()) {
        qWarning() << "FCActionCreateCubeOperator: Failed to update geometry";
        delete boxCmd;
        return false;
    }

    // 获取几何命令列表
    FCGeoCommandList* geoList = FCDATAREPO->getFirstDataByType<FCGeoCommandList>();
    if (!geoList) {
        qWarning() << "FCActionCreateCubeOperator: FCGeoCommandList not found";
        delete boxCmd;
        return false;
    }

    // 添加到几何命令列表
    geoList->appendDataObj(boxCmd);

    qDebug() << "FCActionCreateCubeOperator: Created cube" << name << "with ID" << boxCmd->getDataObjectID();

    // TODO: 更新视图显示
    // 这里可以添加视图更新逻辑，例如：
    // - 触发几何更新信号
    // - 刷新3D视图
    // - 更新树形控件

    return true;
}

} // namespace FC

