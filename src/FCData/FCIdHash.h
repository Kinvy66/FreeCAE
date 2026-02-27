/**
 * @file FCIdHash.h
 * @brief qHash(FCID) for use with QHash<FCID, T>
 */
#ifndef FCIDHASH_H
#define FCIDHASH_H

#include "FCType.h"
#include <QHash>

namespace FC {

inline uint qHash(FCID key, uint seed = 0) { return static_cast<uint>(::qHash(static_cast<quint64>(key), seed)); }

} // namespace FC

#endif // FCIDHASH_H
