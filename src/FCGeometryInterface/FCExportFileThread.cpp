/**
 * @file FCExportFileThread.cpp
 * @brief 几何导出文件线程实现（接口层桩）
 */
#include "FCExportFileThread.h"

namespace FC {

FCExportFileThread::FCExportFileThread(QObject* parent)
    : QThread(parent)
{}

void FCExportFileThread::run()
{
    if (m_filePath.isEmpty()) {
        m_isRunning = false;
        m_cmdIds.clear();
        emit sig_readFinished(false);
        return;
    }
    m_isRunning = true;
    // 接口层不执行实际 IO，由应用注入或子类重写
    emit sig_readFinished(false);
    m_isRunning = false;
}

void FCExportFileThread::setFilePath(QString filePath)
{
    m_filePath = filePath;
}

void FCExportFileThread::setExportedMethod(FCGeoEnum::FITKGeometryComType type)
{
    m_ioType = type;
}

void FCExportFileThread::addExportCmdId(int cmdId)
{
    m_cmdIds.append(cmdId);
}

} // namespace FC
