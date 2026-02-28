#include "FCTorusInfoWidget.h"
#include "ui_FCTorusInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelTorus.h>
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

FCTorusInfoWidget::FCTorusInfoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FCTorusInfoWidget)
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
    connect(ui->lineEdit_majorRadius, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_minorRadius, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_angle, &QLineEdit::editingFinished, this, syncOnEditFinished);
}

FCTorusInfoWidget::~FCTorusInfoWidget()
{
    if (mOwnDisplayTorusCmd && mDisplayTorusCmd) {
        delete mDisplayTorusCmd;
        mDisplayTorusCmd = nullptr;
    }
    delete ui;
}

void FCTorusInfoWidget::setTorusCommand(FCGeoModelTorus* torusCmd)
{
    mDagData = nullptr;
    mNodeId = FCID_INVALID;
    if (mOwnDisplayTorusCmd && mDisplayTorusCmd) {
        delete mDisplayTorusCmd;
        mDisplayTorusCmd = nullptr;
        mOwnDisplayTorusCmd = false;
    } else {
        mDisplayTorusCmd = nullptr;
    }
    mTorusCmd = torusCmd;
    if (!torusCmd) return;
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(torusCmd->getDataObjectName());
    ui->lineEdit_name->blockSignals(false);
    double loc[3], axis[3];
    torusCmd->getLocation(loc);
    torusCmd->getAxis(axis);
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_axisX->setText(QString::number(axis[0]));
    ui->lineEdit_axisY->setText(QString::number(axis[1]));
    ui->lineEdit_axisZ->setText(QString::number(axis[2]));
    ui->lineEdit_majorRadius->setText(QString::number(torusCmd->getMajorRadius()));
    ui->lineEdit_minorRadius->setText(QString::number(torusCmd->getMinorRadius()));
    ui->lineEdit_angle->setText(QString::number(torusCmd->getAngle()));
}

void FCTorusInfoWidget::setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelTorus* displayTorusCmd)
{
    mTorusCmd = nullptr;
    mDagData = dagData;
    mNodeId = nodeId;
    if (mOwnDisplayTorusCmd && mDisplayTorusCmd) {
        delete mDisplayTorusCmd;
        mDisplayTorusCmd = nullptr;
        mOwnDisplayTorusCmd = false;
    }
    if (displayTorusCmd) {
        mDisplayTorusCmd = displayTorusCmd;
        mOwnDisplayTorusCmd = false;
    } else {
        mDisplayTorusCmd = nullptr;
    }
    if (!dagData || nodeId == FCID_INVALID) return;
    FCGeoNode node = dagData->module()->tree()->node(nodeId);
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(node.name.isEmpty() ? QStringLiteral("Torus_%1").arg(static_cast<qulonglong>(nodeId)) : node.name);
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
    double r1 = node.params.value(QStringLiteral("major_radius"), 50.0).toDouble();
    double r2 = node.params.value(QStringLiteral("minor_radius"), 15.0).toDouble();
    double ang = node.params.value(QStringLiteral("angle"), 360.0).toDouble();
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_axisX->setText(QString::number(axis[0]));
    ui->lineEdit_axisY->setText(QString::number(axis[1]));
    ui->lineEdit_axisZ->setText(QString::number(axis[2]));
    ui->lineEdit_majorRadius->setText(QString::number(r1));
    ui->lineEdit_minorRadius->setText(QString::number(r2));
    ui->lineEdit_angle->setText(QString::number(ang));
    if (!mDisplayTorusCmd) {
        FCGeoModelTorus* existingCmd = FCDataRepo::instance()->getDataAs<FCGeoModelTorus>(nodeId);
        if (existingCmd) {
            mDisplayTorusCmd = existingCmd;
            mOwnDisplayTorusCmd = false;
        } else {
            FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
            if (factory) {
                FCGeoModelTorus* cmd = factory->createCommandT<FCGeoModelTorus>(FCGeoEnum::FGTTorus);
                if (cmd) {
                    cmd->setLocation(loc);
                    cmd->setAxis(axis);
                    cmd->setMajorRadius(r1);
                    cmd->setMinorRadius(r2);
                    cmd->setAngle(ang);
                    if (cmd->update()) {
                        mDisplayTorusCmd = cmd;
                        mOwnDisplayTorusCmd = true;
                    } else {
                        delete cmd;
                    }
                }
            }
        }
    }
}

void FCTorusInfoWidget::syncToModel()
{
    syncValuesToModel();
}

FCAbsGeoCommand* FCTorusInfoWidget::getCurrentBuildCommand()
{
    return mDisplayTorusCmd ? static_cast<FCAbsGeoCommand*>(mDisplayTorusCmd) : static_cast<FCAbsGeoCommand*>(mTorusCmd);
}

void FCTorusInfoWidget::syncValuesToModel()
{
    if (mDagData && mNodeId != FCID_INVALID) {
        FCGeoParamSet params;
        params[QStringLiteral("location_x")] = ui->lineEdit_locationX->text().toDouble();
        params[QStringLiteral("location_y")] = ui->lineEdit_locationY->text().toDouble();
        params[QStringLiteral("location_z")] = ui->lineEdit_locationZ->text().toDouble();
        params[QStringLiteral("axis_x")] = ui->lineEdit_axisX->text().toDouble();
        params[QStringLiteral("axis_y")] = ui->lineEdit_axisY->text().toDouble();
        params[QStringLiteral("axis_z")] = ui->lineEdit_axisZ->text().toDouble();
        params[QStringLiteral("major_radius")] = ui->lineEdit_majorRadius->text().toDouble();
        params[QStringLiteral("minor_radius")] = ui->lineEdit_minorRadius->text().toDouble();
        params[QStringLiteral("angle")] = ui->lineEdit_angle->text().toDouble();
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
        if (mDisplayTorusCmd) {
            double loc[3] = { params[QStringLiteral("location_x")].toDouble(), params[QStringLiteral("location_y")].toDouble(), params[QStringLiteral("location_z")].toDouble() };
            double axis[3] = { params[QStringLiteral("axis_x")].toDouble(), params[QStringLiteral("axis_y")].toDouble(), params[QStringLiteral("axis_z")].toDouble() };
            mDisplayTorusCmd->setLocation(loc);
            mDisplayTorusCmd->setAxis(axis);
            mDisplayTorusCmd->setMajorRadius(params[QStringLiteral("major_radius")].toDouble());
            mDisplayTorusCmd->setMinorRadius(params[QStringLiteral("minor_radius")].toDouble());
            mDisplayTorusCmd->setAngle(params[QStringLiteral("angle")].toDouble());
            mDisplayTorusCmd->update();
        }
        return;
    }
    if (!mTorusCmd) return;
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
            mTorusCmd->setDataObjectName(newName);
            mNameManuallyEdited = false;
        }
    }
    mTorusCmd->setLocation(loc);
    mTorusCmd->setAxis(axis);
    mTorusCmd->setMajorRadius(ui->lineEdit_majorRadius->text().toDouble());
    mTorusCmd->setMinorRadius(ui->lineEdit_minorRadius->text().toDouble());
    mTorusCmd->setAngle(ui->lineEdit_angle->text().toDouble());
    mTorusCmd->update();
    rebuildGeometryEntityModel();
}

void FCTorusInfoWidget::executeBuild()
{
    if (mDagData && mNodeId != FCID_INVALID) {
        syncValuesToModel();
        if (mDisplayTorusCmd) {
            if (mDisplayTorusCmd->update())
                emit geometryBuilt(mDisplayTorusCmd);
        } else {
            emit geometryBuilt(nullptr);
        }
        return;
    }
    if (!mTorusCmd) {
        qWarning() << "FCTorusInfoWidget: no torus command or DAG node to build";
        return;
    }
    syncValuesToModel();
    if (!mTorusCmd->update()) {
        qWarning() << "FCTorusInfoWidget: torusCmd->update() failed";
        return;
    }
    emit geometryBuilt(mTorusCmd);
}

void FCTorusInfoWidget::executeBuildAll()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    FCGeometryDAGData* dagData = globalData ? globalData->getData<FCGeometryDAGData>(GDTGeom) : mDagData;
    if (!dagData) return;
    dagData->ensureBuild();
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(QList<FCAbsGeoCommand*>());
}

void FCTorusInfoWidget::rebuildGeometryEntityModel()
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
        qWarning() << "FCTorusInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
}

} // namespace FC
