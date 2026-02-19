/**
 * @file FCGeometryEntityAPI.h
 * @brief FCGeometryEntity 库导出宏
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#ifndef FCGEOMETRYENTITYAPI_H
#define FCGEOMETRYENTITYAPI_H

#include <QtCore/QtGlobal>

#if defined(FCGEOMETRYENTITY_BUILDLIB)
#  define FCGEOMETRYENTITY_API Q_DECL_EXPORT
#else
#  define FCGEOMETRYENTITY_API Q_DECL_IMPORT
#endif

#endif // FCGEOMETRYENTITYAPI_H
