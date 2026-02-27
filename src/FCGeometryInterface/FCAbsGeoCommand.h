/**
 * @file FCAbsGeoCommand.h
 * @brief 几何命令抽象与命令管理器（移植自 FITKInterfaceGeometry）
 */
#ifndef FCABSGEOCOMMAND_H
#define FCABSGEOCOMMAND_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoEnum.h"
#include "FCGeoCommandProp.h"
#include "FCVirtualTopoManager.h"
#include "FCAbsGeoShapeAgent.h"
#include <FCData/FCType.h>
#include <FCData/FCAbstractNamedDataObject.h>
#include <FCData/FCVariantParams.h>
#include <FCData/FCAbstractDataManager.hpp>
#include <FCData/FCMacros.h>
#include <QList>

namespace FC {

class FCAbsVirtualTopo;

struct FCGEOMETRYINTERFACE_API VirtualShape
{
    VirtualShape() = default;
    VirtualShape(FCID cmdID, int virtualTopoId, int virtualTopoIndex)
        : CmdId(cmdID), VirtualTopoId(virtualTopoId), VirtualTopoIndex(virtualTopoIndex) {}
    VirtualShape(FCID cmdID, int virtualTopoIndex)
        : CmdId(cmdID), VirtualTopoIndex(virtualTopoIndex) {}

    FCID CmdId{ FCID_INVALID };
    int VirtualTopoId{ -1 };
    int VirtualTopoIndex{ -1 };
    FCGeoEnum::VTopoShapeType Type{ FCGeoEnum::VSNone };
    bool isNull() const { return CmdId == FCID_INVALID && VirtualTopoId <= 0 && VirtualTopoIndex < 0; }
    void reset() { CmdId = FCID_INVALID; VirtualTopoId = -1; VirtualTopoIndex = -1; }
    bool operator==(const VirtualShape& other) const {
        return CmdId == other.CmdId && VirtualTopoId == other.VirtualTopoId
            && VirtualTopoIndex == other.VirtualTopoIndex && Type == other.Type;
    }
};

inline bool operator==(const VirtualShape& a, const VirtualShape& b) {
    return a.CmdId == b.CmdId && a.VirtualTopoId == b.VirtualTopoId
        && a.VirtualTopoIndex == b.VirtualTopoIndex && a.Type == b.Type;
}

class FCGEOMETRYINTERFACE_API FCAbsGeoCommand : public FCAbstractNamedDataObject,
    public FCVariantParams
{
    FC_CLASS(FC, FCAbsGeoCommand);
    Q_OBJECT
public:
    explicit FCAbsGeoCommand();
    ~FCAbsGeoCommand() override = 0;

    virtual FCGeoEnum::FCGeometryComType getGeometryCommandType() = 0;
    FCGeoCommandProp* getCommandProp() { return _prop; }
    virtual bool update() = 0;
    virtual bool undo();
    virtual bool redo();
    void setCommandStatus(FCGeoEnum::FCGeoStatus status);
    FCGeoEnum::FCGeoStatus getCommandStatus();
    FCAbsGeoShapeAgent* getShapeAgent();
    template<class T> T* getTShapeAgent() { return dynamic_cast<T*>(_shapeAgent); }
    FCVirtualTopoManager* getVirtualTopoManager();
    FCAbsVirtualTopo* getVirtualTopo(FCGeoEnum::VTopoShapeType type, int id);
    template<typename T> T* getShapeT(FCGeoEnum::VTopoShapeType type, int id);
    template<typename T> T* getShapeT(int id);
    /** 按虚拓扑类型与列表索引获取形状（供 OCC 等实现使用） */
    template<typename T> T* getShapeTopoByIndexT(FCGeoEnum::VTopoShapeType type, int index);
    void printLog(QString msg, int type = 1);
    /** 从部件中移除后的处理逻辑（由部件调用） */
    virtual void removeFromPart(FCAbsGeoCommand* part);
    /** 被引用次数（撤销列表用，接口层可恒为 0） */
    virtual int getReferencedCmdCount() const { return 0; }
    /** 在 GUI 中是否有效（撤销列表用，接口层可恒为 true） */
    virtual bool getDataValidInGUI() const { return true; }

protected:
    FCAbsGeoShapeAgent* _shapeAgent{};
    FCGeoEnum::FCGeoStatus _status{ FCGeoEnum::FGSNormal };
    FCGeoCommandProp* _prop{};
};

template<typename T>
inline T* FCAbsGeoCommand::getShapeT(FCGeoEnum::VTopoShapeType type, int id)
{
    FCAbsVirtualTopo* virShape = getVirtualTopo(type, id);
    return virShape ? virShape->getShapeT<T>() : nullptr;
}

template<typename T>
inline T* FCAbsGeoCommand::getShapeT(int id)
{
    FCAbsGeoShapeAgent* agent = getShapeAgent();
    if (!agent) return nullptr;
    FCVirtualTopoManager* mgr = agent->getVirtualTopoManager();
    if (!mgr) return nullptr;
    for (int t = static_cast<int>(FCGeoEnum::VSPoint); t < static_cast<int>(FCGeoEnum::VSAssembly); ++t) {
        FCShapeVirtualTopoManager* sm = mgr->getShapeVirtualTopoManager(static_cast<FCGeoEnum::VTopoShapeType>(t));
        if (!sm) continue;
        FCAbsVirtualTopo* virShape = sm->getDataByID(id);
        if (virShape) return virShape->getShapeT<T>();
    }
    return nullptr;
}

template<typename T>
inline T* FCAbsGeoCommand::getShapeTopoByIndexT(FCGeoEnum::VTopoShapeType type, int index)
{
    FCVirtualTopoManager* mgr = getVirtualTopoManager();
    if (!mgr) return nullptr;
    FCShapeVirtualTopoManager* sm = mgr->getShapeVirtualTopoManager(type);
    if (!sm) return nullptr;
    FCAbsVirtualTopo* vtopo = sm->getDataByIndex(index);
    return vtopo ? vtopo->getShapeT<T>() : nullptr;
}

class FCGEOMETRYINTERFACE_API FCGeoCommandManager : public FCAbstractDataManager<FCAbsGeoCommand>
{
public:
    explicit FCGeoCommandManager() = default;
    ~FCGeoCommandManager() override = default;
    virtual QList<FCAbsGeoCommand*> getRootCommands();
    FCAbsGeoCommand* getDataByID(FCID id);
};

} // namespace FC

Q_DECLARE_METATYPE(FC::VirtualShape)

#endif // FCABSGEOCOMMAND_H
