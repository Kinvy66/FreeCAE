/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

#ifndef FCOCCMODELSOLID_H
#define FCOCCMODELSOLID_H

#include "FCAbstractOCCModel.h"
#include "FCAbsGeoModelSolid.h"
#include "FCGeometryCommandAPI.h"

namespace OCC
{
    /**
     * @brief  封闭曲面形成的体.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FCGEOMETRYCMD_API FCOCCModelClosedSurfaceSolid :
        public FC::FCAbsGeoModelClosedSurfaceSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FCOCCModelClosedSurfaceSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FCOCCModelClosedSurfaceSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  拉伸曲面OCC类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FCGEOMETRYCMD_API FCOCCModelExtrudeSolid :
        public FC::FCAbsGeoModelExtrudeSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FCOCCModelExtrudeSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FCOCCModelExtrudeSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  旋转曲面OCC类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FCGEOMETRYCMD_API FCOCCModelRevolSolid :
        public FC::FCAbsGeoModelRevolSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FCOCCModelRevolSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FCOCCModelRevolSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  扫略体OCC类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FCGEOMETRYCMD_API FCOCCModelSweepSolid :
        public FC::FCAbsGeoModelSweepSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FCOCCModelSweepSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FCOCCModelSweepSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  多截面扫略抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FCGEOMETRYCMD_API FCOCCModelMultiSectionSolid :
        public FC::FCAbsGeoModelMultiSectionSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FCOCCModelMultiSectionSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FCOCCModelMultiSectionSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
}

#endif
