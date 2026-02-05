/**
 * @file FCInterfaceAlgorithmFactory.h
 * @brief 算法工具工厂单例（移植自 FITKInterfaceModel，不依赖 DeclSingleton）
 */
#ifndef FCINTERFACEALGORITHMFACTORY_H
#define FCINTERFACEALGORITHMFACTORY_H

#include "FCModelInterfaceAPI.h"
#include <QObject>
#include <QMutex>

namespace FC {

class FCAbsAlgorithmToolsCreator;

class FCMODELINTERFACE_API FCInterfaceAlgorithmFactory : public QObject
{
    Q_OBJECT
public:
    static FCInterfaceAlgorithmFactory* instance();

    void setAlgToolsCreator(FCAbsAlgorithmToolsCreator* creator);
    FCAbsAlgorithmToolsCreator* getAlgToolsCreator();

private:
    FCInterfaceAlgorithmFactory();
    ~FCInterfaceAlgorithmFactory();
    FCInterfaceAlgorithmFactory(const FCInterfaceAlgorithmFactory&) = delete;
    FCInterfaceAlgorithmFactory& operator=(const FCInterfaceAlgorithmFactory&) = delete;

    FCAbsAlgorithmToolsCreator* m_toolsCreator{ nullptr };
    static FCInterfaceAlgorithmFactory* s_instance;
    static QMutex s_mutex;
};

} // namespace FC

#endif // FCINTERFACEALGORITHMFACTORY_H
