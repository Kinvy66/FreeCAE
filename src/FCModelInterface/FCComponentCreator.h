/**
 * @file FCComponentCreator.h
 * @brief 模型部件创建器（移植自 FITKInterfaceModel）
 */
#ifndef FCCOMPONENTCREATOR_H
#define FCCOMPONENTCREATOR_H

#include "FCModelInterfaceAPI.h"
#include <FCData/FCAbstractObject.hpp>
#include <QHash>

namespace FC {

class FCComponentManager;
class FCModelSet;
class FCMeshSurface;

class FCMODELINTERFACE_API FCComponentCreator : public FCAbstractObject
{
public:
    explicit FCComponentCreator(FCComponentManager* manager);
    virtual ~FCComponentCreator() = default;

    void setName(const QString& name);
    void setInternal(bool internal);

    FCModelSet* createNodeSet(const int& parentID, const QList<int>& member, bool addCompManager = true);
    FCModelSet* createEleSet(const int& parentID, const QList<int>& member, bool addCompManager = true);
    FCModelSet* createGeometryVertexSet(const int& parentID, const QList<int>& member, bool addCompManager = true);
    FCModelSet* createGeometryEdgeSet(const int& parentID, const QList<int>& member, bool addCompManager = true);
    FCModelSet* createGeometryFaceSet(const int& parentID, const QList<int>& member, bool addCompManager = true);
    FCModelSet* createGeometrySolidSet(const int& parentID, const QList<int>& member, bool addCompManager = true);

    FCModelSet* editNodeSet(const int& parentID, const QList<int>& member, int originId);
    FCModelSet* editEleSet(const int& parentID, const QList<int>& member, int originId);

    FCMeshSurface* createEleSurface(QHash<FCModelSet*, int>& setSurfaceIndex);
    FCMeshSurface* editEleSurface(const QHash<FCModelSet*, int>& setSurfaceIndex, int originId);
    FCMeshSurface* createNodeSurface(QHash<FCModelSet*, int>& setSurfaceIndex);
    FCMeshSurface* editNodeSurface(const QHash<FCModelSet*, int>& setSurfaceIndex, int originId);

private:
    FCModelSet* createSet(const int& parentID, const QList<int>& member, bool addCompManager);

    FCComponentManager* _compManager{};
    bool _internal{ true };
    QString _name;
};

} // namespace FC

#endif // FCCOMPONENTCREATOR_H
