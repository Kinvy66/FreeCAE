/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/*****************************************************************//**
 * @file        FCOCCModelOperSolid.h
 * @brief       OCC体特征操作数据类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-26
 *********************************************************************/

#ifndef FCOCCMODELOPERSOLID_H
#define FCOCCMODELOPERSOLID_H

#include "FCAbstractOCCModel.h"
#include "FCAbsGeoModelOperSolid.h"
#include "FCGeometryCommandAPI.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC倒角类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-20
     */
    class FCGEOMETRYCMD_API FCOCCModelChamferSolid : public FC::FCAbsGeoModelChamferSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FCOCCModelChamferSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        ~FCOCCModelChamferSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        bool update() override;

    };

    /**
     * @brief       OCC倒圆类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-20
     */
    class FCGEOMETRYCMD_API FCOCCModelFilletSolid : public FC::FCAbsGeoModelFilletSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FCOCCModelFilletSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        ~FCOCCModelFilletSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        bool update() override;

    };

    /**
     * @brief       OCC移除特征工具类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-27
     */
    class FCOCCDefeatureTool
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        FCOCCDefeatureTool() {}

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        ~FCOCCDefeatureTool() {}

        /**
         * @brief       去除特征工具方法。[静态]
         * @param[in]   cmdId：模型数据ID
         * @param[in]   faceIndice：被操作的面数据虚拓扑索引列表
         * @param[out]  outputShape：输出形状[引用]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        static bool Defeature(int cmdId, QList<int> faceIndice, TopoDS_Shape & outputShape);
    };

    /**
     * @brief       OCC移除特征类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-29
     */
    class FCGEOMETRYCMD_API FCOCCModelDefeatureSolid : public FC::FCAbsGeoModelDefeatureSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        FCOCCModelDefeatureSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        ~FCOCCModelDefeatureSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        bool update() override;

    };

    /**
     * @brief       OCC移除倒角类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-27
     */
    class FCGEOMETRYCMD_API FCOCCModelRemoveChamferSolid : public FC::FCAbsGeoModelRemoveChamferSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        FCOCCModelRemoveChamferSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        ~FCOCCModelRemoveChamferSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        bool update() override;

    };

    /**
     * @brief       OCC移除倒圆类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-27
     */
    class FCGEOMETRYCMD_API FCOCCModelRemoveFilletSolid : public FC::FCAbsGeoModelRemoveFilletSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        FCOCCModelRemoveFilletSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        ~FCOCCModelRemoveFilletSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        bool update() override;

    };

    /**
     * @brief       OCC填补孔洞类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-26
     */
    class FCGEOMETRYCMD_API FCOCCModelFillHoleSolid : public FC::FCAbsGeoModelFillHoleSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FCOCCModelFillHoleSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        ~FCOCCModelFillHoleSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        bool update() override;

    };
}

#endif // !FITKOCCMODELOPERSOLID_H
