#ifndef FCGUIFRAMEAPI_H
#define FCGUIFRAMEAPI_H
#include <QtCore/QtGlobal>
#include "FCGlobals.h"

#if defined(FCGUIFRAME_BUILDLIB)
#define FCGUIFRAME_API Q_DECL_EXPORT
#else
#ifdef Q_CC_MSVC
#define FCGUIFRAME_API Q_DECL_IMPORT
#else
#define FCGUIFRAME_API Q_DECL_IMPORT
#endif
#endif

#endif // FCGUIFRAMEAPI_H
