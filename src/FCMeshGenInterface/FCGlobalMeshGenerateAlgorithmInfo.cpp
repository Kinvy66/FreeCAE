/**
 * @file FCGlobalMeshGenerateAlgorithmInfo.cpp
 */
#include "FCGlobalMeshGenerateAlgorithmInfo.h"

namespace FC {

FCGlobalMeshGenerateAlgorithmInfo::FCGlobalMeshGenerateAlgorithmInfo(QObject* parent)
    : QObject(parent) {}

void FCGlobalMeshGenerateAlgorithmInfo::setMeshGenerateDimension(int dim) { _meshDim = dim; }
void FCGlobalMeshGenerateAlgorithmInfo::set2DAlgorithm(Algorithm2D a) { _alg2D = a; }
void FCGlobalMeshGenerateAlgorithmInfo::set3DAlgorithm(Algorithm3D a) { _alg3D = a; }
void FCGlobalMeshGenerateAlgorithmInfo::setElementOrder(int order) { _elementOrder = order; }

} // namespace FC
