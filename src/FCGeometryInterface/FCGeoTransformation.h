/**
 * @file FCGeoTransformation.h
 * @brief 几何变换命令接口（移植自 FITKInterfaceGeometry FITKAbsGeoTransformation）
 */
#ifndef FCGEOTRANSFORMATION_H
#define FCGEOTRANSFORMATION_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include <array>

namespace FC {

/**
 * @brief 变换基类（抽象）
 */
class FCGEOMETRYINTERFACE_API FCGeoTransformation : public FCAbsGeoCommand
{
    FC_CLASS(FC, FCGeoTransformation);
public:
    FCGeoTransformation() = default;
    ~FCGeoTransformation() override = default;

    VirtualShape sourceShape() const { return m_sourceShape; }
    void setSourceShape(const VirtualShape& shape) { m_sourceShape = shape; }
    bool isCopy() const { return m_isCopy; }
    void setCopy(bool copy) { m_isCopy = copy; }

protected:
    VirtualShape m_sourceShape{};
    bool m_isCopy{ false };
};

/**
 * @brief 平移变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelTransform : public FCGeoTransformation
{
    FC_CLASS(FC, FCGeoModelTransform);
public:
    FCGeoModelTransform() = default;
    ~FCGeoModelTransform() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    std::array<double, 3> vector() const { return m_vector; }
    void setVector(std::array<double, 3> vec) { m_vector = vec; }
    void setVector(double x, double y, double z) { m_vector = { x, y, z }; }
protected:
    std::array<double, 3> m_vector{};
};

/**
 * @brief 通过两个点平移变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelTransformByTwoPoints : public FCGeoModelTransform
{
    FC_CLASS(FC, FCGeoModelTransformByTwoPoints);
public:
    FCGeoModelTransformByTwoPoints() = default;
    ~FCGeoModelTransformByTwoPoints() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    std::array<double, 3> startPoint() const { return m_startPoint; }
    void setStartPoint(std::array<double, 3> vec) { m_startPoint = vec; }
    void setStartPoint(double x, double y, double z) { m_startPoint = { x, y, z }; }
    std::array<double, 3> endPoint() const { return m_endPoint; }
    void setEndPoint(std::array<double, 3> vec) { m_endPoint = vec; }
    void setEndPoint(double x, double y, double z) { m_endPoint = { x, y, z }; }
protected:
    std::array<double, 3> m_startPoint{};
    std::array<double, 3> m_endPoint{};
};

/**
 * @brief 通过方向和距离平移变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelTransformByDirAndDis : public FCGeoModelTransform
{
    FC_CLASS(FC, FCGeoModelTransformByDirAndDis);
public:
    FCGeoModelTransformByDirAndDis() = default;
    ~FCGeoModelTransformByDirAndDis() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    std::array<double, 3> direction() const { return m_direction; }
    void setDirection(std::array<double, 3> dir) { m_direction = dir; }
    void setDirection(double x, double y, double z) { m_direction = { x, y, z }; }
    double distance() const { return m_distance; }
    void setDistance(double dis) { m_distance = dis; }
protected:
    std::array<double, 3> m_direction{};
    double m_distance{};
};

/**
 * @brief 旋转变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelRotate : public FCGeoTransformation
{
    FC_CLASS(FC, FCGeoModelRotate);
public:
    FCGeoModelRotate() = default;
    ~FCGeoModelRotate() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    std::array<double, 3> axisStartPoint() const { return m_axisStartPoint; }
    void setAxisStartPoint(std::array<double, 3> xyz) { m_axisStartPoint = xyz; }
    void setAxisStartPoint(double x, double y, double z) { m_axisStartPoint = { x, y, z }; }
    std::array<double, 3> axisEndPoint() const { return m_axisEndPoint; }
    void setAxisEndPoint(std::array<double, 3> xyz) { m_axisEndPoint = xyz; }
    void setAxisEndPoint(double x, double y, double z) { m_axisEndPoint = { x, y, z }; }
    double degree() const { return m_angle / 3.14159265358979323846 * 180; }
    void setDegree(double deg) { m_angle = deg / 180 * 3.14159265358979323846; }
    double radian() const { return m_angle; }
    void setRadian(double rad) { m_angle = rad; }
protected:
    std::array<double, 3> m_axisStartPoint{};
    std::array<double, 3> m_axisEndPoint{};
    double m_angle{};
};

/**
 * @brief 缩放变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelScale : public FCGeoTransformation
{
    FC_CLASS(FC, FCGeoModelScale);
public:
    FCGeoModelScale() = default;
    ~FCGeoModelScale() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    std::array<double, 3> factors() const { return m_factors; }
    void setFactors(std::array<double, 3> factors) { m_factors = factors; }
    void setFactors(double x, double y, double z) { m_factors = { x, y, z }; }
    std::array<double, 3> basePoint() const { return m_basePoint; }
    void setBasePoint(std::array<double, 3> point) { m_basePoint = point; }
    void setBasePoint(double x, double y, double z) { m_basePoint = { x, y, z }; }
protected:
    std::array<double, 3> m_factors{};
    std::array<double, 3> m_basePoint{ { 0, 0, 0 } };
};

/**
 * @brief 镜像变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelMirror : public FCGeoTransformation
{
    FC_CLASS(FC, FCGeoModelMirror);
public:
    FCGeoModelMirror() = default;
    ~FCGeoModelMirror() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape reference() const { return m_reference; }
    void setReference(const VirtualShape& ref) { m_reference = ref; }
protected:
    VirtualShape m_reference{};
};

/**
 * @brief 阵列变换基类
 */
class FCGEOMETRYINTERFACE_API FCGeoModelPattern : public FCGeoTransformation
{
    FC_CLASS(FC, FCGeoModelPattern);
    Q_OBJECT
public:
    FCGeoModelPattern() = default;
    ~FCGeoModelPattern() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    enum Definition {
        InstanceAndSpacing = 0,
        InstanceAndTotal = 1,
        SpacingAndTotal = 2
    };
    Q_ENUM(Definition)

    Definition firstDefinition() const { return m_firstDefinition; }
    void setFirstDefinition(Definition def) { m_firstDefinition = def; }
    Definition secondDefinition() const { return m_secondDefinition; }
    void setSecondDefinition(Definition def) { m_secondDefinition = def; }
protected:
    Definition m_firstDefinition{};
    Definition m_secondDefinition{};
};

/**
 * @brief 矩形阵列变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelRectangularPattern : public FCGeoModelPattern
{
    FC_CLASS(FC, FCGeoModelRectangularPattern);
public:
    FCGeoModelRectangularPattern() = default;
    ~FCGeoModelRectangularPattern() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    std::array<double, 3> firstDirection() const { return m_firstDirection; }
    void setFirstDirection(std::array<double, 3> dir) { m_firstDirection = dir; }
    void setFirstDirection(double x, double y, double z) { m_firstDirection = { x, y, z }; }
    int firstCount() const { return m_firstCount; }
    void setFirstCount(int count) { m_firstCount = count; }
    double firstSpacing() const { return m_firstSpacing; }
    void setFirstSpacing(double spacing) { m_firstSpacing = spacing; }
    double firstTotal() const { return m_firstTotal; }
    void setFirstTotal(double total) { m_firstTotal = total; }

    std::array<double, 3> secondDirection() const { return m_secondDirection; }
    void setSecondDirection(std::array<double, 3> dir) { m_secondDirection = dir; }
    void setSecondDirection(double x, double y, double z) { m_secondDirection = { x, y, z }; }
    int secondCount() const { return m_secondCount; }
    void setSecondCount(int count) { m_secondCount = count; }
    double secondSpacing() const { return m_secondSpacing; }
    void setSecondSpacing(double spacing) { m_secondSpacing = spacing; }
    double secondTotal() const { return m_secondTotal; }
    void setSecondTotal(double total) { m_secondTotal = total; }
protected:
    std::array<double, 3> m_firstDirection{ { 1, 0, 0 } };
    int m_firstCount{ 2 };
    double m_firstSpacing{ 20 };
    double m_firstTotal{ 20 };
    std::array<double, 3> m_secondDirection{ { 1, 0, 0 } };
    int m_secondCount{ 1 };
    double m_secondSpacing{ 20 };
    double m_secondTotal{ 20 };
};

/**
 * @brief 环形阵列变换
 */
class FCGEOMETRYINTERFACE_API FCGeoModelCircularPattern : public FCGeoModelPattern
{
    FC_CLASS(FC, FCGeoModelCircularPattern);
public:
    FCGeoModelCircularPattern() = default;
    ~FCGeoModelCircularPattern() override = default;
    FCGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    bool update() override;

    VirtualShape axis() const { return m_axisShape; }
    void setAxis(const VirtualShape& axis) { m_axisShape = axis; }
    bool isReverse() const { return m_isReverse; }
    void setReverse(bool rev) { m_isReverse = rev; }
    int axisCount() const { return m_axisCount; }
    void setAxisCount(int count) { m_axisCount = count; }
    double axisSpacing() const { return m_axisSpacingDegree; }
    void setAxisSpacing(double spacing) { m_axisSpacingDegree = spacing; }
    double axisTotal() const { return m_axisTotalDegree; }
    void setAxisTotal(double total) { m_axisTotalDegree = total; }

    int crownCount() const { return m_crownCount; }
    void setCrownCount(int count) { m_crownCount = count; }
    double crownSpacing() const { return m_crownSpacing; }
    void setCrownSpacing(double spacing) { m_crownSpacing = spacing; }
    double crownTotal() const { return m_crownTotal; }
    void setCrownTotal(double total) { m_crownTotal = total; }
protected:
    VirtualShape m_axisShape{};
    bool m_isReverse{ false };
    int m_axisCount{ 2 };
    double m_axisSpacingDegree{ 45 };
    double m_axisTotalDegree{ 45 };
    int m_crownCount{ 1 };
    double m_crownSpacing{ 20 };
    double m_crownTotal{ 20 };
};

} // namespace FC

#endif // FCGEOTRANSFORMATION_H
