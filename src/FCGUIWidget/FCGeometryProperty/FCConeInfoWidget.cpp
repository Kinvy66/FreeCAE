#include "FCConeInfoWidget.h"
#include "ui_FCConeInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelCone.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryInterface/FCGeoInterfaceFactory.h>
#include <FCGeometryInterface/FCGeoEnum.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeometryEntityBuilder.h>
#include <FCGeometryEntity/FCGeometryTree.h>
#include <FCGeometryEntity/FCGeometryEntityModel.h>
#include <FCGeometryEntity/FCGeoNode.h>
#include <FCData/FCGlobalData.h>
#include <FCData/FCDataRepo.h>
#include <QLineEdit>

namespace FC {

FCConeInfoWidget::FCConeInfoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FCConeInfoWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, [this]() { mNameManuallyEdited = true; });
    auto syncOnEditFinished = [this]() { syncValuesToModel(); };
    connect(ui->lineEdit_name, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationX, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationY, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationZ, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_axisX, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_axisY, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_axisZ, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_height, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_bottomRadius, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_topRadius, &QLineEdit::editingFinished, this, syncOnEditFinished);
}

FCConeInfoWidget::~FCConeInfoWidget()
{
    if (mOwnDisplayConeCmd && mDisplayConeCmd) {
        delete mDisplayConeCmd;
        mDisplayConeCmd = nullptr;
    }
    delete ui;
}

void FCConeInfoWidget::setConeCommand(FCGeoModelCone* coneCmd)
{
    mDagData = nullptr;
    mNodeId = FCID_INVALID;
    if (mOwnDisplayConeCmd && mDisplayConeCmd) {
        delete mDisplayConeCmd;
        mDisplayConeCmd = nullptr;
        mOwnDisplayConeCmd = false;
    } else {
        mDisplayConeCmd = nullptr;
    }
    mConeCmd = coneCmd;
    if (!coneCmd) return;
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(coneCmd->getDataObjectName());
    ui->lineEdit_name->blockSignals(false);
    double loc[3], axis[3];
    coneCmd->getLocation(loc);
    coneCmd->getAxis(axis);
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_axisX->setText(QString::number(axis[0]));
    ui->lineEdit_axisY->setText(QString::number(axis[1]));
    ui->lineEdit_axisZ->setText(QString::number(axis[2]));
    ui->lineEdit_height->setText(QString::number(coneCmd->getHeight()));
    ui->lineEdit_bottomRadius->setText(QString::number(coneCmd->getBottomRadius()));
    ui->lineEdit_topRadius->setText(QString::number(coneCmd->getTopRadius()));
}

void FCConeInfoWidget::setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelCone* displayConeCmd)
{
    mConeCmd = nullptr;
    mDagData = dagData;
    mNodeId = nodeId;
    if (mOwnDisplayConeCmd && mDisplayConeCmd) {
        delete mDisplayConeCmd;
        mDisplayConeCmd = nullptr;
        mOwnDisplayConeCmd = false;
    }
    if (displayConeCmd) {
        mDisplayConeCmd = displayConeCmd;
        mOwnDisplayConeCmd = false;
    } else {
        mDisplayConeCmd = nullptr;
    }
    if (!dagData || nodeId == FCID_INVALID) return;
    FCGeoNode node = dagData->module()->tree()->node(nodeId);
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(node.name.isEmpty() ? QStringLiteral("Cone_%1").arg(static_cast<qulonglong>(nodeId)) : node.name);
    ui->lineEdit_name->blockSignals(false);
    double loc[3] = {
        node.params.value(QStringLiteral("location_x"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_y"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_z"), 0.0).toDouble()
    };
    double axis[3] = {
        node.params.value(QStringLiteral("axis_x"), 0.0).toDouble(),
        node.params.value(QStringLiteral("axis_y"), 0.0).toDouble(),
        node.params.value(QStringLiteral("axis_z"), 1.0).toDouble()
    };
    double h = node.params.value(QStringLiteral("height"), 100.0).toDouble();
    double r1 = node.params.value(QStringLiteral("bottom_radius"), 50.0).toDouble();
    double r2 = node.params.value(QStringLiteral("top_radius"), 0.0).toDouble();
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_axisX->setText(QString::number(axis[0]));
    ui->lineEdit_axisY->setText(QString::number(axis[1]));
    ui->lineEdit_axisZ->setText(QString::number(axis[2]));
    ui->lineEdit_height->setText(QString::number(h));
    ui->lineEdit_bottomRadius->setText(QString::number(r1));
    ui->lineEdit_topRadius->setText(QString::number(r2));
    if (!mDisplayConeCmd) {
        FCGeoModelCone* existingCmd = FCDataRepo::instance()->getDataAs<FCGeoModelCone>(nodeId);
        if (existingCmd) {
            mDisplayConeCmd = existingCmd;
            mOwnDisplayConeCmd = false;
        } else {
            FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
            if (factory) {
                FCGeoModelCone* cmd = factory->createCommandT<FCGeoModelCone>(FCGeoEnum::FGTCone);
                if (cmd) {
                    cmd->setLocation(loc);
                    cmd->setAxis(axis);
                    cmd->setHeight(h);
                    cmd->setBottomRadius(r1);
                    cmd->setTopRadius(r2);
                    if (cmd->update()) {
                        mDisplayConeCmd = cmd;
                        mOwnDisplayConeCmd = true;
                    } else {
                        delete cmd;
                    }
                }
            }
        }
    }
}

void FCConeInfoWidget::syncToModel()
{
    syncValuesToModel();
}

FCAbsGeoCommand* FCConeInfoWidget::getCurrentBuildCommand()
{
    return mDisplayConeCmd ? static_cast<FCAbsGeoCommand*>(mDisplayConeCmd) : static_cast<FCAbsGeoCommand*>(mConeCmd);
}

void FCConeInfoWidget::syncValuesToModel()
{
    if (mDagData && mNodeId != FCID_INVALID) {
        FCGeoParamSet params;
        params[QStringLiteral("location_x")] = ui->lineEdit_locationX->text().toDouble();
        params[QStringLiteral("location_y")] = ui->lineEdit_locationY->text().toDouble();
        params[QStringLiteral("location_z")] = ui->lineEdit_locationZ->text().toDouble();
        params[QStringLiteral("axis_x")] = ui->lineEdit_axisX->text().toDouble();
        params[QStringLiteral("axis_y")] = ui->lineEdit_axisY->text().toDouble();
        params[QStringLiteral("axis_z")] = ui->lineEdit_axisZ->text().toDouble();
        params[QStringLiteral("height")] = ui->lineEdit_height->text().toDouble();
        params[QStringLiteral("bottom_radius")] = ui->lineEdit_bottomRadius->text().toDouble();
        params[QStringLiteral("top_radius")] = ui->lineEdit_topRadius->text().toDouble();
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
        if (mDisplayConeCmd) {
            double loc[3] = { params[QStringLiteral("location_x")].toDouble(), params[QStringLiteral("location_y")].toDouble(), params[QStringLiteral("location_z")].toDouble() };
            double axis[3] = { params[QStringLiteral("axis_x")].toDouble(), params[QStringLiteral("axis_y")].toDouble(), params[QStringLiteral("axis_z")].toDouble() };
            mDisplayConeCmd->setLocation(loc);
            mDisplayConeCmd->setAxis(axis);
            mDisplayConeCmd->setHeight(params[QStringLiteral("height")].toDouble());
            mDisplayConeCmd->setBottomRadius(params[QStringLiteral("bottom_radius")].toDouble());
            mDisplayConeCmd->setTopRadius(params[QStringLiteral("top_radius")].toDouble());
            mDisplayConeCmd->update();
        }
        return;
    }
    if (!mConeCmd) return;
    double loc[3] = {
        ui->lineEdit_locationX->text().toDouble(),
        ui->lineEdit_locationY->text().toDouble(),
        ui->lineEdit_locationZ->text().toDouble()
    };
    double axis[3] = {
        ui->lineEdit_axisX->text().toDouble(),
        ui->lineEdit_axisY->text().toDouble(),
        ui->lineEdit_axisZ->text().toDouble()
    };
    if (mNameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty()) {
            mConeCmd->setDataObjectName(newName);
            mNameManuallyEdited = false;
        }
    }
    mConeCmd->setLocation(loc);
    mConeCmd->setAxis(axis);
    mConeCmd->setHeight(ui->lineEdit_height->text().toDouble());
    mConeCmd->setBottomRadius(ui->lineEdit_bottomRadius->text().toDouble());
    mConeCmd->setTopRadius(ui->lineEdit_topRadius->text().toDouble());
    mConeCmd->update();
    rebuildGeometryEntityModel();
}

void FCConeInfoWidget::executeBuild()
{
    if (mDagData && mNodeId != FCID_INVALID) {
        syncValuesToModel();
        if (mDisplayConeCmd) {
            if (mDisplayConeCmd->update())
                emit geometryBuilt(mDisplayConeCmd);
        } else {
            emit geometryBuilt(nullptr);
        }
        return;
    }
    if (!mConeCmd) {
        qWarning() << "FCConeInfoWidget: no cone command or DAG node to build";
        return;
    }
    syncValuesToModel();
    if (!mConeCmd->update()) {
        qWarning() << "FCConeInfoWidget: coneCmd->update() failed";
        return;
    }
    emit geometryBuilt(mConeCmd);
}

void FCConeInfoWidget::executeBuildAll()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    FCGeometryDAGData* dagData = globalData ? globalData->getData<FCGeometryDAGData>(GDTGeom) : mDagData;
    if (!dagData) return;
    dagData->ensureBuild();
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(QList<FCAbsGeoCommand*>());
}

void FCConeInfoWidget::rebuildGeometryEntityModel()
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
        qWarning() << "FCConeInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
}

} // namespace FC
