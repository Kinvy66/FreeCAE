#include "FCSphereInfoWidget.h"
#include "ui_FCSphereInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelSphere.h>
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

FCSphereInfoWidget::FCSphereInfoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FCSphereInfoWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, [this]() { mNameManuallyEdited = true; });
    auto syncOnEditFinished = [this]() { syncValuesToModel(); };
    connect(ui->lineEdit_name, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationX, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationY, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationZ, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_radius, &QLineEdit::editingFinished, this, syncOnEditFinished);
}

FCSphereInfoWidget::~FCSphereInfoWidget()
{
    if (mOwnDisplaySphereCmd && mDisplaySphereCmd) {
        delete mDisplaySphereCmd;
        mDisplaySphereCmd = nullptr;
    }
    delete ui;
}

void FCSphereInfoWidget::setSphereCommand(FCGeoModelSphere* sphereCmd)
{
    mDagData = nullptr;
    mNodeId = -1;
    if (mOwnDisplaySphereCmd && mDisplaySphereCmd) {
        delete mDisplaySphereCmd;
        mDisplaySphereCmd = nullptr;
        mOwnDisplaySphereCmd = false;
    } else {
        mDisplaySphereCmd = nullptr;
    }
    mSphereCmd = sphereCmd;
    if (!sphereCmd) return;
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(sphereCmd->getDataObjectName());
    ui->lineEdit_name->blockSignals(false);
    double loc[3];
    sphereCmd->getLocation(loc);
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_radius->setText(QString::number(sphereCmd->getRadius()));
}

void FCSphereInfoWidget::setDAGNode(FCGeometryDAGData* dagData, int nodeId, FCGeoModelSphere* displaySphereCmd)
{
    mSphereCmd = nullptr;
    mDagData = dagData;
    mNodeId = nodeId;
    if (mOwnDisplaySphereCmd && mDisplaySphereCmd) {
        delete mDisplaySphereCmd;
        mDisplaySphereCmd = nullptr;
        mOwnDisplaySphereCmd = false;
    }
    if (displaySphereCmd) {
        mDisplaySphereCmd = displaySphereCmd;
        mOwnDisplaySphereCmd = false;
    } else {
        mDisplaySphereCmd = nullptr;
    }
    if (!dagData || nodeId < 0) return;
    FCGeoNode node = dagData->module()->tree()->node(nodeId);
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(node.name.isEmpty() ? QStringLiteral("Sphere_%1").arg(nodeId) : node.name);
    ui->lineEdit_name->blockSignals(false);
    double loc[3] = {
        node.params.value(QStringLiteral("location_x"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_y"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_z"), 0.0).toDouble()
    };
    double r = node.params.value(QStringLiteral("radius"), 50.0).toDouble();
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_radius->setText(QString::number(r));
    if (!mDisplaySphereCmd) {
        FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
        if (factory) {
            FCGeoModelSphere* cmd = factory->createCommandT<FCGeoModelSphere>(FCGeoEnum::FGTSphere);
            if (cmd) {
                cmd->setLocation(loc);
                cmd->setRadius(r);
                if (cmd->update()) {
                    mDisplaySphereCmd = cmd;
                    mOwnDisplaySphereCmd = true;
                } else {
                    delete cmd;
                }
            }
        }
    }
}

void FCSphereInfoWidget::syncToModel()
{
    syncValuesToModel();
}

FCAbsGeoCommand* FCSphereInfoWidget::getCurrentBuildCommand()
{
    return mDisplaySphereCmd ? static_cast<FCAbsGeoCommand*>(mDisplaySphereCmd) : static_cast<FCAbsGeoCommand*>(mSphereCmd);
}

void FCSphereInfoWidget::syncValuesToModel()
{
    if (mDagData && mNodeId >= 0) {
        FCGeoParamSet params;
        params[QStringLiteral("location_x")] = ui->lineEdit_locationX->text().toDouble();
        params[QStringLiteral("location_y")] = ui->lineEdit_locationY->text().toDouble();
        params[QStringLiteral("location_z")] = ui->lineEdit_locationZ->text().toDouble();
        params[QStringLiteral("radius")] = ui->lineEdit_radius->text().toDouble();
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
        if (mDisplaySphereCmd) {
            double loc[3] = {
                params[QStringLiteral("location_x")].toDouble(),
                params[QStringLiteral("location_y")].toDouble(),
                params[QStringLiteral("location_z")].toDouble()
            };
            mDisplaySphereCmd->setLocation(loc);
            mDisplaySphereCmd->setRadius(params[QStringLiteral("radius")].toDouble());
            mDisplaySphereCmd->update();
        }
        return;
    }
    if (!mSphereCmd) return;
    double loc[3] = {
        ui->lineEdit_locationX->text().toDouble(),
        ui->lineEdit_locationY->text().toDouble(),
        ui->lineEdit_locationZ->text().toDouble()
    };
    double r = ui->lineEdit_radius->text().toDouble();
    if (mNameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty()) {
            mSphereCmd->setDataObjectName(newName);
            mNameManuallyEdited = false;
        }
    }
    mSphereCmd->setLocation(loc);
    mSphereCmd->setRadius(r);
    mSphereCmd->update();
    rebuildGeometryEntityModel();
}

void FCSphereInfoWidget::executeBuild()
{
    if (mDagData && mNodeId >= 0) {
        syncValuesToModel();
        if (mDisplaySphereCmd) {
            if (mDisplaySphereCmd->update())
                emit geometryBuilt(mDisplaySphereCmd);
        } else {
            emit geometryBuilt(nullptr);
        }
        return;
    }
    if (!mSphereCmd) {
        qWarning() << "FCSphereInfoWidget: no sphere command or DAG node to build";
        return;
    }
    syncValuesToModel();
    if (!mSphereCmd->update()) {
        qWarning() << "FCSphereInfoWidget: sphereCmd->update() failed";
        return;
    }
    emit geometryBuilt(mSphereCmd);
}

void FCSphereInfoWidget::executeBuildAll()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    FCGeometryDAGData* dagData = globalData ? globalData->getData<FCGeometryDAGData>(GDTGeom) : mDagData;
    if (!dagData) return;
    dagData->ensureBuild();
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(QList<FCAbsGeoCommand*>());
}

void FCSphereInfoWidget::rebuildGeometryEntityModel()
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
        qWarning() << "FCSphereInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
}

} // namespace FC
