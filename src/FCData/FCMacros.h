/**
 * @file FCMacros.h
 * @brief FCData 核心宏定义
 * @copyright Copyright (c) 2025 Kinvy. All rights reserved.
 */
#ifndef FCMACROS_H
#define FCMACROS_H

#include <QString>

#define FC_CLASS(NS, thisClass) \
public: \
virtual bool isFCClass(const QString& className) const override { return className == #thisClass; } \
virtual QString getClassName() const override { return QString(#NS "::" #thisClass); }

#endif // FCMACROS_H
