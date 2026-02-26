#include "FCCylinderInfoWidget.h"
#include "ui_FCCylinderInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelCylinder.h>
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

FCCylinderInfoWidget::FCCylinderInfoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FCCylinderInfoWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, [this]() { mNameManuallyEdited = true; });
    auto syncOnEditFinished = [this]() { syncValuesToModel(); };
    connect(ui->lineEdit_name, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationX, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationY, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationZ, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_directionX, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_directionY, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_directionZ, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_radius, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_length, &QLineEdit::editingFinished, this, syncOnEditFinished);
}

FCCylinderInfoWidget::~FCCylinderInfoWidget()
{
    if (mOwnDisplayCylinderCmd && mDisplayCylinderCmd) {
        delete mDisplayCylinderCmd;
        mDisplayCylinderCmd = nullptr;
    }
    delete ui;
}

void FCCylinderInfoWidget::setCylinderCommand(FCGeoModelCylinder* cylinderCmd)
{
    mDagData = nullptr;
    mNodeId = -1;
    if (mOwnDisplayCylinderCmd && mDisplayCylinderCmd) {
        delete mDisplayCylinderCmd;
        mDisplayCylinderCmd = nullptr;
        mOwnDisplayCylinderCmd = false;
    } else {
        mDisplayCylinderCmd = nullptr;
    }
    mCylinderCmd = cylinderCmd;
    if (!cylinderCmd) return;
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(cylinderCmd->getDataObjectName());
    ui->lineEdit_name->blockSignals(false);
    double loc[3], dir[3];
    cylinderCmd->getLocation(loc);
    cylinderCmd->getDirection(dir);
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_directionX->setText(QString::number(dir[0]));
    ui->lineEdit_directionY->setText(QString::number(dir[1]));
    ui->lineEdit_directionZ->setText(QString::number(dir[2]));
    ui->lineEdit_radius->setText(QString::number(cylinderCmd->getRadius()));
    ui->lineEdit_length->setText(QString::number(cylinderCmd->getLength()));
}

void FCCylinderInfoWidget::setDAGNode(FCGeometryDAGData* dagData, int nodeId, FCGeoModelCylinder* displayCylinderCmd)
{
    mCylinderCmd = nullptr;
    mDagData = dagData;
    mNodeId = nodeId;
    if (mOwnDisplayCylinderCmd && mDisplayCylinderCmd) {
        delete mDisplayCylinderCmd;
        mDisplayCylinderCmd = nullptr;
        mOwnDisplayCylinderCmd = false;
    }
    if (displayCylinderCmd) {
        mDisplayCylinderCmd = displayCylinderCmd;
        mOwnDisplayCylinderCmd = false;
    } else {
        mDisplayCylinderCmd = nullptr;
    }
    if (!dagData || nodeId < 0) return;
    FCGeoNode node = dagData->module()->tree()->node(nodeId);
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(node.name.isEmpty() ? QStringLiteral("Cylinder_%1").arg(nodeId) : node.name);
    ui->lineEdit_name->blockSignals(false);
    double loc[3] = {
        node.params.value(QStringLiteral("location_x"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_y"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_z"), 0.0).toDouble()
    };
    double dir[3] = {
        node.params.value(QStringLiteral("direction_x"), 0.0).toDouble(),
        node.params.value(QStringLiteral("direction_y"), 0.0).toDouble(),
        node.params.value(QStringLiteral("direction_z"), 1.0).toDouble()
    };
    double r = node.params.value(QStringLiteral("radius"), 50.0).toDouble();
    double L = node.params.value(QStringLiteral("length"), 100.0).toDouble();
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_directionX->setText(QString::number(dir[0]));
    ui->lineEdit_directionY->setText(QString::number(dir[1]));
    ui->lineEdit_directionZ->setText(QString::number(dir[2]));
    ui->lineEdit_radius->setText(QString::number(r));
    ui->lineEdit_length->setText(QString::number(L));
    if (!mDisplayCylinderCmd) {
        FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
        if (factory) {
            FCGeoModelCylinder* cmd = factory->createCommandT<FCGeoModelCylinder>(FCGeoEnum::FGTCylinder);
            if (cmd) {
                cmd->setLocation(loc);
                cmd->setDirection(dir);
                cmd->setRadius(r);
                cmd->setLength(L);
                if (cmd->update()) {
                    mDisplayCylinderCmd = cmd;
                    mOwnDisplayCylinderCmd = true;
                } else {
                    delete cmd;
                }
            }
        }
    }
}

void FCCylinderInfoWidget::syncToModel()
{
    syncValuesToModel();
}

FCAbsGeoCommand* FCCylinderInfoWidget::getCurrentBuildCommand()
{
    return mDisplayCylinderCmd ? static_cast<FCAbsGeoCommand*>(mDisplayCylinderCmd) : static_cast<FCAbsGeoCommand*>(mCylinderCmd);
}

void FCCylinderInfoWidget::syncValuesToModel()
{
    if (mDagData && mNodeId >= 0) {
        FCGeoParamSet params;
        params[QStringLiteral("location_x")] = ui->lineEdit_locationX->text().toDouble();
        params[QStringLiteral("location_y")] = ui->lineEdit_locationY->text().toDouble();
        params[QStringLiteral("location_z")] = ui->lineEdit_locationZ->text().toDouble();
        params[QStringLiteral("direction_x")] = ui->lineEdit_directionX->text().toDouble();
        params[QStringLiteral("direction_y")] = ui->lineEdit_directionY->text().toDouble();
        params[QStringLiteral("direction_z")] = ui->lineEdit_directionZ->text().toDouble();
        params[QStringLiteral("radius")] = ui->lineEdit_radius->text().toDouble();
        params[QStringLiteral("length")] = ui->lineEdit_length->text().toDouble();
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
        if (mDisplayCylinderCmd) {
            double loc[3] = { params[QStringLiteral("location_x")].toDouble(), params[QStringLiteral("location_y")].toDouble(), params[QStringLiteral("location_z")].toDouble() };
            double dir[3] = { params[QStringLiteral("direction_x")].toDouble(), params[QStringLiteral("direction_y")].toDouble(), params[QStringLiteral("direction_z")].toDouble() };
            mDisplayCylinderCmd->setLocation(loc);
            mDisplayCylinderCmd->setDirection(dir);
            mDisplayCylinderCmd->setRadius(params[QStringLiteral("radius")].toDouble());
            mDisplayCylinderCmd->setLength(params[QStringLiteral("length")].toDouble());
            mDisplayCylinderCmd->update();
        }
        return;
    }
    if (!mCylinderCmd) return;
    double loc[3] = {
        ui->lineEdit_locationX->text().toDouble(),
        ui->lineEdit_locationY->text().toDouble(),
        ui->lineEdit_locationZ->text().toDouble()
    };
    double dir[3] = {
        ui->lineEdit_directionX->text().toDouble(),
        ui->lineEdit_directionY->text().toDouble(),
        ui->lineEdit_directionZ->text().toDouble()
    };
    double r = ui->lineEdit_radius->text().toDouble();
    double L = ui->lineEdit_length->text().toDouble();
    if (mNameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty()) {
            mCylinderCmd->setDataObjectName(newName);
            mNameManuallyEdited = false;
        }
    }
    mCylinderCmd->setLocation(loc);
    mCylinderCmd->setDirection(dir);
    mCylinderCmd->setRadius(r);
    mCylinderCmd->setLength(L);
    mCylinderCmd->update();
    rebuildGeometryEntityModel();
}

void FCCylinderInfoWidget::executeBuild()
{
    if (mDagData && mNodeId >= 0) {
        syncValuesToModel();
        if (mDisplayCylinderCmd) {
            if (mDisplayCylinderCmd->update())
                emit geometryBuilt(mDisplayCylinderCmd);
        } else {
            emit geometryBuilt(nullptr);
        }
        return;
    }
    if (!mCylinderCmd) {
        qWarning() << "FCCylinderInfoWidget: no cylinder command or DAG node to build";
        return;
    }
    syncValuesToModel();
    if (!mCylinderCmd->update()) {
        qWarning() << "FCCylinderInfoWidget: cylinderCmd->update() failed";
        return;
    }
    emit geometryBuilt(mCylinderCmd);
}

void FCCylinderInfoWidget::executeBuildAll()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    FCGeometryDAGData* dagData = globalData ? globalData->getData<FCGeometryDAGData>(GDTGeom) : mDagData;
    if (!dagData) return;
    dagData->ensureBuild();
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(QList<FCAbsGeoCommand*>());
}

void FCCylinderInfoWidget::rebuildGeometryEntityModel()
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
        qWarning() << "FCCylinderInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
}

} // namespace FC
