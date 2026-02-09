/*
 * Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
 * All rights reserved.
 *
 * This file is part of FastCAE and is distributed under the terms of the
 * BSD 3-Clause License. See the LICENSE file in the project root for details.
 */

/**
 * @file FCOCCGeoCompRegister.h
 * @brief OCC 几何命令注册（移植自 FITKGeoCompOCC FITKGeoCompOCCInterface）
 * 调用 registerOCCGeometryCommands() 将 OCC 命令/基准注册到 FCGeoInterfaceFactory。
 */
#ifndef FCOCCGEOCOMPREGISTER_H
#define FCOCCGEOCOMPREGISTER_H

#include "FCGeometryCommandAPI.h"

namespace OCC {

/**
 * @brief 向 FCGeoInterfaceFactory 注册 OCC 几何命令与基准
 * 应用或插件在加载 OCC 几何模块时调用；析构或卸载时调用 FCGeoInterfaceFactory::instance()->clearAllInterface()。
 */
void FCGEOMETRYCMD_API registerOCCGeometryCommands();

} // namespace OCC

#endif // FCOCCGEOCOMPREGISTER_H
