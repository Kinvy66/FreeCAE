#ifndef FCABSTRACTOPERATOR_H
#define FCABSTRACTOPERATOR_H

#include <QObject>
#include "FCActionsOperatorAPI.h"
#include "FCAbstractObject.hpp"

namespace FC 
{
class FCACTIONSOPERATOR_API FCAbstractOperator : public QObject, public FCAbstractObject
{
    Q_OBJECT
public:
    FCAbstractOperator();
};
} // namespace FC



#endif // FCABSTRACTOPERATOR_H
