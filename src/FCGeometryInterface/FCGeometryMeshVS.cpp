#include "FCGeometryMeshVS.h"
#include "FCGeometryMeshEntity.h"

namespace FC {

FCGeometryMeshVS::FCGeometryMeshVS()
    : FCAbstractDataObject(-1)
{}

FCGeometryMeshVS::~FCGeometryMeshVS()
{
    clear();
}

void FCGeometryMeshVS::insertPoint(FCID id, FCGeoMeshVSPt* pt)
{
    if (!pt || id == FCID_INVALID) return;
    if (_points.contains(id))
        delete _points[id];
    _points.insert(id, pt);
}

void FCGeometryMeshVS::insertEdge(FCID id, FCGeoMeshVSEdgeEntity* edge)
{
    if (!edge || id == FCID_INVALID) return;
    if (_edges.contains(id))
        delete _edges[id];
    _edges.insert(id, edge);
}

void FCGeometryMeshVS::insertFace(FCID id, FCGeoMeshVSFaceEntity* face)
{
    if (!face || id == FCID_INVALID) return;
    if (_faces.contains(id))
        delete _faces[id];
    _faces.insert(id, face);
}

const QHash<FCID, FCGeoMeshVSPt*>& FCGeometryMeshVS::getPointItems() const
{
    return _points;
}

const QHash<FCID, FCGeoMeshVSEdgeEntity*>& FCGeometryMeshVS::getEdgeItems() const
{
    return _edges;
}

const QHash<FCID, FCGeoMeshVSFaceEntity*>& FCGeometryMeshVS::getFaceItems() const
{
    return _faces;
}

void FCGeometryMeshVS::clear()
{
    for (FCGeoMeshVSPt* pt : _points)
        delete pt;
    _points.clear();
    for (FCGeoMeshVSEdgeEntity* edge : _edges)
        delete edge;
    _edges.clear();
    for (FCGeoMeshVSFaceEntity* face : _faces)
        delete face;
    _faces.clear();
}

bool FCGeometryMeshVS::isValid() const
{
    return !_points.isEmpty() || !_edges.isEmpty() || !_faces.isEmpty();
}

} // namespace FC
