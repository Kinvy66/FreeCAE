/**
 * @file FCImportFileThread.h
 * @brief 几何导入文件线程（移植自 FITKInterfaceGeometry FITKImportFileThread）
 */
#ifndef FCIMPORTFILETHREAD_H
#define FCIMPORTFILETHREAD_H

#include "FCGeometryInterfaceAPI.h"
#include "FCGeoEnum.h"
#include <QThread>
#include <QList>

namespace FC {

/**
 * @brief 几何导入后台线程（接口层 run() 为桩，实际 IO 由应用或子类实现）
 */
class FCGEOMETRYINTERFACE_API FCImportFileThread : public QThread
{
    Q_OBJECT
public:
    explicit FCImportFileThread(QObject* parent = nullptr);
    ~FCImportFileThread() override = default;

    void run() override;
    void setFilePath(QString filePath);
    void setImportedMethod(FCGeoEnum::FITKGeometryComType type);
    const QList<int>& newCmdIds() const { return m_newIds; }
    bool isRunning() const { return m_isRunning; }

signals:
    void sig_readFinished(bool status);

protected:
    QString m_filePath;
    FCGeoEnum::FITKGeometryComType m_ioType{ FCGeoEnum::FGTImport };
    QList<int> m_newIds;
    bool m_isRunning{ false };
};

} // namespace FC

#endif // FCIMPORTFILETHREAD_H
