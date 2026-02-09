/**
 * @file FCGeoCommandList.h
  * @brief 几何命令列表（移植自 FITKInterfaceGeometry FITKGeoCommandList）
 */
#ifndef FCGEOCOMMANDLIST_H
#define FCGEOCOMMANDLIST_H

#include "FCGeometryInterfaceAPI.h"
#include "FCAbsGeoCommand.h"
#include "FCAbsGeoDatum.h"
#include "FCGlobalGeoComponent.h"
#include "FCAbsGeoSketch2D.h"
#include <FCData/FCAbstractDataObject.h>
#include <QList>

namespace FC {

/**
 * @brief 几何命令列表（撤销列表）：命令管理�?+ 基准/草图/全局分组 + 重做�?
 */
class FCGEOMETRYINTERFACE_API FCGeoCommandList : public FCAbstractDataObject,
    public FCGeoCommandManager
{
    FC_CLASS(FC, FCGeoCommandList);
public:
    explicit FCGeoCommandList();
    virtual ~FCGeoCommandList();

    QString checkName(const QString& name) override;
    FCDatumList* getDatumManager();
    void setCurrentSketchID(int sketchDataId = -1);
    FCGeoSketch2D* getCurrentSketchData();
    FCGlobalGeoComponentManager* getGlobalGeoCompManager();
    virtual bool canUndo();
    virtual bool canRedo();
    virtual bool undo();
    virtual bool redo();
    void appendDataObj(FCAbstractDataObject* obj) override;
    void getBoundaryBox(double* minPt, double* maxPt);
    FCGeoCommandManager* getUndoList();
    FCGeoCommandManager* getRedoList();
    QList<FCAbsGeoCommand*> getRootCommandList();
    QList<int> getGlobalDataIDList();

protected:
    FCGeoCommandManager* m_redoList{ nullptr };
    FCDatumList* m_datumMgr{ nullptr };
    FCGlobalGeoComponentManager* m_globalGeoCompMgr{ nullptr };
    int m_currentSketchID{ -1 };
};

} // namespace FC

#endif // FCGEOCOMMANDLIST_H
