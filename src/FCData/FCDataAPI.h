/**
 * @file FCDataAPI.h
 * @brief FCData 库导出宏
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCDATAAPI_H
#define FCDATAAPI_H

#include <QtCore/QtGlobal>

#if defined(FCDATA_BUILD)
#  define FCDATA_API Q_DECL_EXPORT
#else
#  define FCDATA_API Q_DECL_IMPORT
#endif

#endif // FCDATAAPI_H
