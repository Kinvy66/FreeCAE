/**
 * @file FCGeoCommandList.cpp
 * @brief 几何命令列表实现
 */
#include "FCGeoCommandList.h"
#include "FCAbsGeoShapeAgent.h"
#include <FCModelInterface/FCAbstractGeoModel.h>

namespace FC {

FCGeoCommandList::FCGeoCommandList()
{
    m_datumMgr = new FCDatumList;
    m_redoList = new FCGeoCommandManager;
    m_globalGeoCompMgr = new FCGlobalGeoComponentManager;
}

FCGeoCommandList::~FCGeoCommandList()
{
    if (m_datumMgr) { delete m_datumMgr; m_datumMgr = nullptr; }
    if (m_globalGeoCompMgr) { delete m_globalGeoCompMgr; m_globalGeoCompMgr = nullptr; }
    if (m_redoList) { delete m_redoList; m_redoList = nullptr; }
}

QString FCGeoCommandList::checkName(const QString& name)
{
    QString newName = FCGeoCommandManager::checkName(name);
    if (m_redoList) newName = m_redoList->checkName(newName);
    return newName;
}

FCDatumList* FCGeoCommandList::getDatumManager()
{
    return m_datumMgr;
}

void FCGeoCommandList::setCurrentSketchID(int sketchDataId)
{
    m_currentSketchID = sketchDataId;
}

FCGeoSketch2D* FCGeoCommandList::getCurrentSketchData()
{
    FCGeoSketch2D* sData = dynamic_cast<FCGeoSketch2D*>(getDataByID(m_currentSketchID));
    if (!sData) m_currentSketchID = -1;
    return sData;
}

FCGlobalGeoComponentManager* FCGeoCommandList::getGlobalGeoCompManager()
{
    return m_globalGeoCompMgr;
}

bool FCGeoCommandList::canUndo()
{
    FCGeoSketch2D* currSketch = getCurrentSketchData();
    if (currSketch) return currSketch->canUndo();
    FCGeoCommandManager* list = getUndoList();
    if (!list) return false;
    return list->getDataCount() != 0;
}

bool FCGeoCommandList::canRedo()
{
    FCGeoSketch2D* currSketch = getCurrentSketchData();
    if (currSketch) return currSketch->canRedo();
    FCGeoCommandManager* list = getRedoList();
    if (!list) return false;
    return list->getDataCount() != 0;
}

bool FCGeoCommandList::undo()
{
    FCGeoSketch2D* currSketch = getCurrentSketchData();
    if (currSketch) return currSketch->undoInternal();
    int nCmd = getDataCount();
    if (nCmd == 0) return false;
    FCAbsGeoCommand* cmd = getDataByIndex(nCmd - 1);
    if (!cmd) return false;
    cmd->undo();
    removeDataObjWithoutRelease(cmd);
    m_redoList->appendDataObj(cmd);
    return true;
}

bool FCGeoCommandList::redo()
{
    FCGeoSketch2D* currSketch = getCurrentSketchData();
    if (currSketch) return currSketch->redoInternal();
    if (!m_redoList) return false;
    int nCmd = m_redoList->getDataCount();
    if (nCmd == 0) return false;
    FCAbsGeoCommand* cmd = m_redoList->getDataByIndex(nCmd - 1);
    if (!cmd) return false;
    cmd->redo();
    m_redoList->removeDataObjWithoutRelease(cmd);
    FCGeoCommandManager::appendDataObj(cmd);
    return true;
}

void FCGeoCommandList::appendDataObj(FCAbstractDataObject* obj)
{
    if (m_redoList) m_redoList->clear();
    if (isContains(dynamic_cast<FCAbsGeoCommand*>(obj))) return;
    FCGeoCommandManager::appendDataObj(obj);
}

void FCGeoCommandList::getBoundaryBox(double* minPt, double* maxPt)
{
    if (!minPt || !maxPt) return;
    minPt[0] = minPt[1] = minPt[2] = 9e66;
    maxPt[0] = maxPt[1] = maxPt[2] = -9e66;
    const int n = getDataCount();
    for (int i = 0; i < n; ++i) {
        FCAbsGeoCommand* c = getDataByIndex(i);
        if (!c) continue;
        FCAbsGeoShapeAgent* m = c->getShapeAgent();
        if (!m) continue;
        double min[3], max[3];
        if (!m->getBoundaryBox(min, max)) continue;
        for (int j = 0; j < 3; ++j) {
            if (min[j] < minPt[j]) minPt[j] = min[j];
            if (max[j] > maxPt[j]) maxPt[j] = max[j];
        }
    }
}

FCGeoCommandManager* FCGeoCommandList::getUndoList()
{
    return this;
}

FCGeoCommandManager* FCGeoCommandList::getRedoList()
{
    return m_redoList;
}

QList<FCAbsGeoCommand*> FCGeoCommandList::getRootCommandList()
{
    QList<FCAbsGeoCommand*> cmdList;
    for (int i = 0; i < getDataCount(); ++i) {
        FCAbsGeoCommand* cmd = getDataByIndex(i);
        if (!cmd || !cmd->getDataValidInGUI()) continue;
        if (cmd->getReferencedCmdCount() == 0) cmdList.append(cmd);
    }
    return cmdList;
}

QList<int> FCGeoCommandList::getGlobalDataIDList()
{
    QList<int> ids;
    ids.append(getDataObjectID());
    return ids;
}

} // namespace FC
