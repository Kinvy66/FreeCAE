/**
 * @file FCGeometryEntityBuilder.cpp
 * @brief 几何实体构建器实现
 * @copyright Copyright (c) 2026 Kinvy. All rights reserved.
 */
#include "FCGeometryEntityBuilder.h"
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCGlobalGeoComponent.h>
#include <FCModelInterface/FCAbstractGeoModel.h>
#include <FCModelInterface/FCModelEnum.h>

namespace FC {

FCGeometryEntityBuilder::FCGeometryEntityBuilder(QObject* parent)
    : QObject(parent)
{
}

FCGeometryEntityBuilder::~FCGeometryEntityBuilder() = default;

FCGeometryEntityModel* FCGeometryEntityBuilder::build(FCGeoCommandList* geoCommandList)
{
    if (!geoCommandList) return nullptr;
    FCGlobalGeoComponentManager* compMgr = geoCommandList->getGlobalGeoCompManager();
    return buildFromGlobalGeoComponentManager(compMgr);
}

FCGeometryEntityModel* FCGeometryEntityBuilder::buildFromGlobalGeoComponentManager(FCGlobalGeoComponentManager* compMgr)
{
    if (!compMgr) return nullptr;
    FCGeometryEntityModel* model = new FCGeometryEntityModel(-1);
    model->clear();

    QList<int> domainIds, boundaryIds, edgeIds, pointIds;
    const int n = compMgr->getDataCount();
    for (int i = 0; i < n; ++i) {
        FCGlobalGeoComponent* globalComp = compMgr->getDataByIndex(i);
        if (!globalComp) continue;
        switch (globalComp->getGeoType()) {
        case FCModelEnum::FMSSolid:
            fillFromComponent(globalComp, FCGeometryEntityLevel::Domain, &domainIds, model);
            break;
        case FCModelEnum::FMSSurface:
            fillFromComponent(globalComp, FCGeometryEntityLevel::Boundary, &boundaryIds, model);
            break;
        case FCModelEnum::FMSEdge:
            fillFromComponent(globalComp, FCGeometryEntityLevel::Edge, &edgeIds, model);
            break;
        case FCModelEnum::FMSPoint:
            fillFromComponent(globalComp, FCGeometryEntityLevel::Point, &pointIds, model);
            break;
        default:
            break;
        }
    }
    model->setDomainIds(domainIds);
    model->setBoundaryIds(boundaryIds);
    model->setEdgeIds(edgeIds);
    model->setPointIds(pointIds);
    return model;
}

void FCGeometryEntityBuilder::fillFromComponent(FCGlobalGeoComponent* globalComp,
                                                FCGeometryEntityLevel level,
                                                QList<int>* entityIds,
                                                FCGeometryEntityModel* model)
{
    if (!globalComp || !entityIds || !model) return;
    const int compCount = globalComp->getDataCount();
    for (int j = 0; j < compCount; ++j) {
        FCGeoComponent* comp = globalComp->getDataByIndex(j);
        if (!comp) continue;
        const QList<int> members = comp->getMember();
        for (int idx = 0; idx < members.size(); ++idx) {
            int memberId = members.at(idx);
            int entityId = entityIds->size() + 1;
            entityIds->append(entityId);
            model->setSourceMemberId(level, entityId, memberId);
        }
    }
}

} // namespace FC
