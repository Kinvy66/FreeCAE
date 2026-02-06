/**
 * @file FCImportFileThread.cpp
 * @brief 几何导入文件线程实现（接口层桩）
 */
#include "FCImportFileThread.h"

namespace FC {

FCImportFileThread::FCImportFileThread(QObject* parent)
    : QThread(parent)
{}

void FCImportFileThread::run()
{
    m_isRunning = true;
    m_newIds.clear();
    if (m_filePath.isEmpty()) {
        m_isRunning = false;
        emit sig_readFinished(false);
        return;
    }
    // 接口层不执行实际 IO，由应用注入或子类重写
    emit sig_readFinished(false);
    m_isRunning = false;
}

void FCImportFileThread::setFilePath(QString filePath)
{
    m_filePath = filePath;
}

void FCImportFileThread::setImportedMethod(FCGeoEnum::FITKGeometryComType type)
{
    m_ioType = type;
}

} // namespace FC
