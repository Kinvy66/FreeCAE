/**
 * @file FCModelSet.h
 * @brief 集合类（移植自 FITKInterfaceModel）
 */
#ifndef FCMODELSET_H
#define FCMODELSET_H

#include "FCModelInterfaceAPI.h"
#include "FCAbstractModelComponent.h"
#include "FCModelEnum.h"
#include <FCData/FCAbstractDataManager.hpp>
#include <FCData/FCMacros.h>
#include <QList>

namespace FC {

class FCMODELINTERFACE_API FCModelSet : public FCAbstractModelComponent,
    public FCAbstractDataManager<FCModelSet>
{
    FC_CLASS(FC, FCModelSet);

public:
    explicit FCModelSet(FCModelEnum::FITKModelSetType type);
    explicit FCModelSet() = default;
    virtual ~FCModelSet() = default;

    virtual FCModelEnum::FITKModelSetType getModelSetType();
    bool containsType(FCModelEnum::FITKModelSetType t);
    void setModelSetType(FCModelEnum::FITKModelSetType t);
    bool isInternal();
    bool isGenerated();
    void setInternal(bool internal = true);
    void setGenerated(bool generated = true);
    void clearMember();
    void appendMember(const int& mem);
    void setAbsoluteMember(const QList<int>& mem);
    const QList<int>& getMember() const;
    int getMemberCount();
    const QList<int> getAbsoluteMember() const;
    bool isCombination();
    virtual void appendDataObj(FCAbstractDataObject* item) override;
    void getAbsoluteSet(QList<FCModelSet*>& sets, FCModelEnum::FITKModelSetType t);
    void setDataObjectName(const QString& name) override;
    int getFirstMemberID();

protected:
    FCModelEnum::FITKModelSetType _setType{ FCModelEnum::FMSNone };
    QList<int> _setMember;
    bool _internal{ false };
    bool _generated{ false };
};

} // namespace FC

#endif // FCMODELSET_H
