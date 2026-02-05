/**
 * @file FCModelSurface.h
 * @brief 模型表面抽象类型（移植自 FITKInterfaceModel）
 */
#ifndef FCMODELSURFACE_H
#define FCMODELSURFACE_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModelComponent.h"
#include <QList>
#include <QPair>

namespace FC {

class FCAbstractModel;
class FCModelSet;

struct SurfaceAbsSetInfo
{
    FCAbstractModel* _model{};
    FCModelSet* _set{};
    int _surfaceIndex{ -1 };
    double _nodeWidght{ 0 };
};

class FCMODELINTERFACE_API FCModelSurface : public FCAbstractModelComponent
{
    Q_OBJECT
public:
    enum ModelSurfaceType
    {
        MST_None = -1,
        MST_Geom = 0,
        MST_MeshNode,
        MST_MeshEle
    };
    Q_ENUM(ModelSurfaceType);

    explicit FCModelSurface() = default;
    virtual ~FCModelSurface();

    virtual ModelSurfaceType getModelSurfaceType() = 0;
    int getSetCount() const;
    FCModelSet* getSetAt(int index) const;
    bool isInternal() const;
    void setInternal(bool internal);
    FCAbstractModel* getModelAt(int index) const;
    virtual QList<SurfaceAbsSetInfo> getAbsoluteSetInfo();
    virtual void clearSets();

protected:
    int indexOf(int modelID, int setID);
    bool _internal{ false };
    QList<QPair<int, int>> _modelSetIDs;
};

} // namespace FC

#endif // FCMODELSURFACE_H
