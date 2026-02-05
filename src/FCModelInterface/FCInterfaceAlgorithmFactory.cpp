/**
 * @file FCInterfaceAlgorithmFactory.cpp
 * @brief 算法工厂实现（移植自 FITKInterfaceModel）
 */
#include "FCInterfaceAlgorithmFactory.h"
#include "FCAbsAlgorithmTools.h"

namespace FC {

FCInterfaceAlgorithmFactory* FCInterfaceAlgorithmFactory::s_instance = nullptr;
QMutex FCInterfaceAlgorithmFactory::s_mutex;

FCInterfaceAlgorithmFactory* FCInterfaceAlgorithmFactory::instance()
{
    if (!s_instance) {
        QMutexLocker locker(&s_mutex);
        if (!s_instance)
            s_instance = new FCInterfaceAlgorithmFactory();
    }
    return s_instance;
}

FCInterfaceAlgorithmFactory::FCInterfaceAlgorithmFactory()
{}

FCInterfaceAlgorithmFactory::~FCInterfaceAlgorithmFactory()
{
    m_toolsCreator = nullptr;
}

void FCInterfaceAlgorithmFactory::setAlgToolsCreator(FCAbsAlgorithmToolsCreator* creator)
{
    m_toolsCreator = creator;
}

FCAbsAlgorithmToolsCreator* FCInterfaceAlgorithmFactory::getAlgToolsCreator()
{
    return m_toolsCreator;
}

} // namespace FC
