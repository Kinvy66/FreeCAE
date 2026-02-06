/**
 * @file FCGeoSketch2D.h
 * @brief 2D 草图命令（移植自 FITKInterfaceGeometry FITKAbsGeoSketch2D，接口层不含曲线依赖）
 */
#ifndef FCGEOSKETCH2D_H
#define FCGEOSKETCH2D_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCGeoEnum.h"
#include <FCData/FCMacros.h>
#include <FCData/FCAbstractDataManager.hpp>

namespace FC {

/**
 * @brief 2D 草图：命令 + 子命令容器，平面与尺寸接口
 */
class FCGEOMETRYINTERFACE_API FCGeoSketch2D : public FCAbsGeoCommand, public FCGeoCommandManager
{
    FC_CLASS(FC, FCGeoSketch2D);
    Q_OBJECT
public:
    FCGeoSketch2D();
    ~FCGeoSketch2D() override;

    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    void setPlane(const double* pos, const double* normal, const double* up);
    void getPlane(double* pos, double* normal, double* up) const;
    void setPosition(const double* pos);
    void getPosition(double* pos) const;
    void setNormal(const double* nor);
    void getNormal(double* nor) const;
    void setUp(const double* up);
    void getUp(double* up) const;
    void setDefaultSize(double size) { m_defaultSize = size; }
    double getDefaultSize() const { return m_defaultSize; }
    virtual bool canUndo();
    virtual bool canRedo();
    virtual bool undoInternal();
    virtual bool redoInternal();
protected:
    double m_planePos[3]{ 0., 0., 0. };
    double m_planeNormal[3]{ 0., 0., 1. };
    double m_planeUp[3]{ 0., 1., 0. };
    double m_defaultSize{ 100. };
};

} // namespace FC

#endif // FCGEOSKETCH2D_H
