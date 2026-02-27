/**
 * @file FCType.h
 * @brief 数据对象 ID 类型定义（与 COMSOL 风格几何模块兼容）
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCTYPE_H
#define FCTYPE_H
#include <stdint.h>

typedef uint64_t FCIdType;
/** 数据对象 ID 与全局数据仓库一致，使用 FCIdType 以支持唯一 ID 生成器 */
typedef FCIdType FCID;
/** 无效/未设置的数据对象 ID（如无父节点时） */
#define FCID_INVALID ((FCID)-1)

#endif // FCTYPE_H
