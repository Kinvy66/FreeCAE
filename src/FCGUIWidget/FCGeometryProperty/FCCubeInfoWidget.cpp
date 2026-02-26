#include "FCCubeInfoWidget.h"
#include "ui_FCCubeInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeometryEntityBuilder.h>
#include <FCGeometryEntity/FCGeometryTree.h>
#include <FCGeometryEntity/FCGeometryEntityModel.h>
#include <FCGeometryEntity/FCGeoNode.h>
#include <FCData/FCGlobalData.h>
#include <QLineEdit>
#include <QDebug>

namespace FC {

FCCubeInfoWidget::FCCubeInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FCCubeInfoWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, [this]() { mNameManuallyEdited = true; });
    // COMSOL 式：失焦或回车时立即写回内存，无需保存按钮
    auto syncOnEditFinished = [this]() { syncValuesToModel(); };
    connect(ui->lineEdit_name,    &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_pointX,  &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_pointY,  &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_pointZ,  &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_length1, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_length2, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_length3, &QLineEdit::editingFinished, this, syncOnEditFinished);
}

FCCubeInfoWidget::~FCCubeInfoWidget()
{
    if (mOwnDisplayBoxCmd && mDisplayBoxCmd) {
        delete mDisplayBoxCmd;
        mDisplayBoxCmd = nullptr;
    }
    delete ui;
}

void FCCubeInfoWidget::setBoxCommand(FCGeoModelBox* boxCmd)
{
    mDagData = nullptr;
    mNodeId = -1;
    if (mOwnDisplayBoxCmd && mDisplayBoxCmd) {
        delete mDisplayBoxCmd;
        mDisplayBoxCmd = nullptr;
        mOwnDisplayBoxCmd = false;
    } else {
        mDisplayBoxCmd = nullptr;
    }
    mBoxCmd = boxCmd;
    if (!boxCmd) return;
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(boxCmd->getDataObjectName());
    ui->lineEdit_name->blockSignals(false);
    double p[3], len[3];
    boxCmd->getPoint1(p);
    boxCmd->getLength(len);
    ui->lineEdit_pointX->setText(QString::number(p[0]));
    ui->lineEdit_pointY->setText(QString::number(p[1]));
    ui->lineEdit_pointZ->setText(QString::number(p[2]));
    ui->lineEdit_length1->setText(QString::number(len[0]));
    ui->lineEdit_length2->setText(QString::number(len[1]));
    ui->lineEdit_length3->setText(QString::number(len[2]));
}

void FCCubeInfoWidget::setDAGNode(FCGeometryDAGData* dagData, int nodeId, FCGeoModelBox* displayBoxCmd)
{
    mBoxCmd = nullptr;
    mDagData = dagData;
    mNodeId = nodeId;
    if (mOwnDisplayBoxCmd && mDisplayBoxCmd) {
        delete mDisplayBoxCmd;
        mDisplayBoxCmd = nullptr;
        mOwnDisplayBoxCmd = false;
    }
    if (displayBoxCmd) {
        mDisplayBoxCmd = displayBoxCmd;
        mOwnDisplayBoxCmd = false;
    } else {
        mDisplayBoxCmd = nullptr;
    }
    if (!dagData || nodeId < 0) return;
    FCGeoNode node = dagData->module()->tree()->node(nodeId);
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(node.name.isEmpty() ? QStringLiteral("Box_%1").arg(nodeId) : node.name);
    ui->lineEdit_name->blockSignals(false);
    QVariant len = node.params.value(QStringLiteral("length"));
    QVariant w   = node.params.value(QStringLiteral("width"));
    QVariant h   = node.params.value(QStringLiteral("height"));
    double l0 = len.isValid() ? len.toDouble() : 100.0;
    double l1 = w.isValid()   ? w.toDouble()   : 100.0;
    double l2 = h.isValid()   ? h.toDouble()   : 100.0;
    ui->lineEdit_pointX->setText(QStringLiteral("0"));
    ui->lineEdit_pointY->setText(QStringLiteral("0"));
    ui->lineEdit_pointZ->setText(QStringLiteral("0"));
    ui->lineEdit_length1->setText(QString::number(l0));
    ui->lineEdit_length2->setText(QString::number(l1));
    ui->lineEdit_length3->setText(QString::number(l2));
    // 从树选中时未传入 displayBoxCmd，创建用于 VTK 显示的 Box 命令，供「构建」时 emit
    if (!mDisplayBoxCmd) {
        FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
        if (factory) {
            FCGeoModelBox* boxCmd = factory->createCommandT<FCGeoModelBox>(FCGeoEnum::FGTBox);
            if (boxCmd) {
                double p[3] = { 0.0, 0.0, 0.0 };
                double length[3] = { l0, l1, l2 };
                boxCmd->setPoint1(p);
                boxCmd->setLength(length);
                if (boxCmd->update()) {
                    mDisplayBoxCmd = boxCmd;
                    mOwnDisplayBoxCmd = true;
                } else {
                    delete boxCmd;
                }
            }
        }
    }
}

void FCCubeInfoWidget::syncToModel()
{
    syncValuesToModel();
}

FCAbsGeoCommand* FCCubeInfoWidget::getCurrentBuildCommand()
{
    return mDisplayBoxCmd ? static_cast<FCAbsGeoCommand*>(mDisplayBoxCmd) : static_cast<FCAbsGeoCommand*>(mBoxCmd);
}

void FCCubeInfoWidget::syncValuesToModel()
{
    if (mDagData && mNodeId >= 0) {
        double len[3] = {
            ui->lineEdit_length1->text().toDouble(),
            ui->lineEdit_length2->text().toDouble(),
            ui->lineEdit_length3->text().toDouble()
        };
        FCGeoParamSet params;
        params[QStringLiteral("length")] = len[0];
        params[QStringLiteral("width")]  = len[1];
        params[QStringLiteral("height")] = len[2];
        FCGeometryTree* tree = mDagData->module()->tree();
        FCGeoNode node = tree->node(mNodeId);
        if (mNameManuallyEdited) {
            QString newName = ui->lineEdit_name->text().trimmed();
            if (!newName.isEmpty()) {
                node.name = tree->checkName(newName);
                tree->setNode(mNodeId, node);
                ui->lineEdit_name->blockSignals(true);
                ui->lineEdit_name->setText(node.name);
                ui->lineEdit_name->blockSignals(false);
                mNameManuallyEdited = false;
                mDagData->setDirty(true);
            }
        }
        mDagData->module()->updateNode(mNodeId, params);
        mDagData->setDirty(true);
        mDagData->ensureBuild();
        rebuildGeometryEntityModel();
        if (mDisplayBoxCmd) {
            double p[3] = { 0.0, 0.0, 0.0 };
            mDisplayBoxCmd->setPoint1(p);
            mDisplayBoxCmd->setLength(len);
            mDisplayBoxCmd->update();
        }
        return;
    }
    if (!mBoxCmd) return;
    bool ok = false;
    double p[3] = {
        ui->lineEdit_pointX->text().toDouble(&ok),
        ui->lineEdit_pointY->text().toDouble(&ok),
        ui->lineEdit_pointZ->text().toDouble(&ok)
    };
    double len[3] = {
        ui->lineEdit_length1->text().toDouble(&ok),
        ui->lineEdit_length2->text().toDouble(&ok),
        ui->lineEdit_length3->text().toDouble(&ok)
    };
    if (mNameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty()) {
            mBoxCmd->setDataObjectName(newName);
            mNameManuallyEdited = false;
        }
    }
    mBoxCmd->setPoint1(p);
    mBoxCmd->setLength(len);
    mBoxCmd->update();
    rebuildGeometryEntityModel();
}

void FCCubeInfoWidget::executeBuild()
{
    if (mDagData && mNodeId >= 0) {
        double len[3] = {
            ui->lineEdit_length1->text().toDouble(),
            ui->lineEdit_length2->text().toDouble(),
            ui->lineEdit_length3->text().toDouble()
        };
        FCGeoParamSet params;
        params[QStringLiteral("length")] = len[0];
        params[QStringLiteral("width")]  = len[1];
        params[QStringLiteral("height")] = len[2];
        FCGeometryTree* tree = mDagData->module()->tree();
        FCGeoNode node = tree->node(mNodeId);
        if (mNameManuallyEdited) {
            QString newName = ui->lineEdit_name->text().trimmed();
            if (!newName.isEmpty()) {
                node.name = tree->checkName(newName);
                tree->setNode(mNodeId, node);
                ui->lineEdit_name->blockSignals(true);
                ui->lineEdit_name->setText(node.name);
                ui->lineEdit_name->blockSignals(false);
                mNameManuallyEdited = false;
                mDagData->setDirty(true);
            }
        }
        mDagData->module()->updateNode(mNodeId, params);
        mDagData->setDirty(true);
        mDagData->ensureBuild();
        rebuildGeometryEntityModel();
        // 用显示用 Box 命令驱动 VTK 显示（DAG 执行器未接入时仍能显示）
        if (mDisplayBoxCmd) {
            double p[3] = { 0.0, 0.0, 0.0 };
            mDisplayBoxCmd->setPoint1(p);
            mDisplayBoxCmd->setLength(len);
            if (mDisplayBoxCmd->update())
                emit geometryBuilt(mDisplayBoxCmd);
        } else {
            emit geometryBuilt(nullptr);
        }
        return;
    }
    if (!mBoxCmd) {
        qWarning() << "FCCubeInfoWidget: no box command or DAG node to build";
        return;
    }
    bool ok = false;
    double p[3] = {
        ui->lineEdit_pointX->text().toDouble(&ok),
        ui->lineEdit_pointY->text().toDouble(&ok),
        ui->lineEdit_pointZ->text().toDouble(&ok)
    };
    double len[3] = {
        ui->lineEdit_length1->text().toDouble(&ok),
        ui->lineEdit_length2->text().toDouble(&ok),
        ui->lineEdit_length3->text().toDouble(&ok)
    };
    if (mNameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty())
            mBoxCmd->setDataObjectName(newName);
    }
    mBoxCmd->setPoint1(p);
    mBoxCmd->setLength(len);
    if (!mBoxCmd->update()) {
        qWarning() << "FCCubeInfoWidget: boxCmd->update() failed";
        return;
    }
    rebuildGeometryEntityModel();
    emit geometryBuilt(mBoxCmd);
}

void FCCubeInfoWidget::executeBuildAll()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    FCGeometryDAGData* dagData = globalData ? globalData->getData<FCGeometryDAGData>(GDTGeom) : mDagData;
    if (!dagData) return;
    dagData->ensureBuild();
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(QList<FCAbsGeoCommand*>());
}

void FCCubeInfoWidget::rebuildGeometryEntityModel()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    if (!globalData) return;
    FCGeometryEntityModel* entityModel = globalData->getData<FCGeometryEntityModel>(GDTGeomEntity);
    if (!entityModel) return;
    FCGeometryDAGData* dagData = mDagData ? mDagData : globalData->getData<FCGeometryDAGData>(GDTGeom);
    if (!dagData) return;
    dagData->ensureBuild();
    FCGeometryEntityBuilder builder;
    if (!builder.rebuild(dagData->getGlobalGeoCompManager(), entityModel))
        qWarning() << "FCCubeInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
}

} // namespace FC
