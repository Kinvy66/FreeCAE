/**
 * @file FCExportFileThread.h
 * @brief 几何导出文件线程（移植自 FITKInterfaceGeometry FITKExportFileThread）
 */
#ifndef FCEXPORTFILETHREAD_H
#define FCEXPORTFILETHREAD_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoEnum.h"
#include <QThread>
#include <QList>

namespace FC {

/**
 * @brief 几何导出后台线程（接口层 run() 为桩，实际 IO 由应用或子类实现）
 */
class FCGEOMETRYINTERFACE_API FCExportFileThread : public QThread
{
    Q_OBJECT
public:
    explicit FCExportFileThread(QObject* parent = nullptr);
    ~FCExportFileThread() override = default;

    void run() override;
    void setFilePath(QString filePath);
    void setExportedMethod(FCGeoEnum::FITKGeometryComType type);
    void addExportCmdId(int cmdId);
    bool isRunning() const { return m_isRunning; }

signals:
    void sig_readFinished(bool status);

protected:
    QString m_filePath;
    FCGeoEnum::FITKGeometryComType m_ioType{ FCGeoEnum::FGTExport };
    QList<int> m_cmdIds;
    bool m_isRunning{ false };
};

} // namespace FC

#endif // FCEXPORTFILETHREAD_H
