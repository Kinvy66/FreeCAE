/**
 * @file FCGeoModelManager.cpp
 * @brief 几何模型管理器实现（移植自 FITKInterfaceGeometry）
 */
#include "FCGeoModelManager.h"

namespace FC {

FCGeoModelManager::FCGeoModelManager()
{}

FCGeoModelManager::~FCGeoModelManager()
{}

void FCGeoModelManager::getBoundaryBox(double* minPt, double* maxPt)
{
    if (!minPt || !maxPt) return;
    minPt[0] = minPt[1] = minPt[2] = 9e66;
    maxPt[0] = maxPt[1] = maxPt[2] = -9e66;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCAbstractGeoModel* m = getDataByIndex(i);
        if (!m) continue;
        double min[3] = { 9e66, 9e66, 9e66 }, max[3] = { -9e66, -9e66, -9e66 };
        if (!m->getBoundaryBox(min, max)) continue;
        for (int j = 0; j < 3; ++j) {
            if (min[j] < minPt[j]) minPt[j] = min[j];
            if (max[j] > maxPt[j]) maxPt[j] = max[j];
        }
    }
}

} // namespace FC
