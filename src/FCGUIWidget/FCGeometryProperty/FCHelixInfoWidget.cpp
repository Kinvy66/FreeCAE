#include "FCHelixInfoWidget.h"
#include "ui_FCHelixInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelHelix.h>
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

FCHelixInfoWidget::FCHelixInfoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FCHelixInfoWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, [this]() { mNameManuallyEdited = true; });
    auto syncOnEditFinished = [this]() { syncValuesToModel(); };
    connect(ui->lineEdit_name, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationX, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationY, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_locationZ, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_numberOfTurns, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_majorRadius, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_minorRadius, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_axialPitch, &QLineEdit::editingFinished, this, syncOnEditFinished);
    connect(ui->lineEdit_chirality, &QLineEdit::editingFinished, this, syncOnEditFinished);
}

FCHelixInfoWidget::~FCHelixInfoWidget()
{
    if (mOwnDisplayHelixCmd && mDisplayHelixCmd) {
        delete mDisplayHelixCmd;
        mDisplayHelixCmd = nullptr;
    }
    delete ui;
}

void FCHelixInfoWidget::setHelixCommand(FCGeoModelHelix* helixCmd)
{
    mDagData = nullptr;
    mNodeId = FCID_INVALID;
    if (mOwnDisplayHelixCmd && mDisplayHelixCmd) {
        delete mDisplayHelixCmd;
        mDisplayHelixCmd = nullptr;
        mOwnDisplayHelixCmd = false;
    } else {
        mDisplayHelixCmd = nullptr;
    }
    mHelixCmd = helixCmd;
    if (!helixCmd) return;
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(helixCmd->getDataObjectName());
    ui->lineEdit_name->blockSignals(false);
    double loc[3];
    helixCmd->getLocation(loc);
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_numberOfTurns->setText(QString::number(helixCmd->getNumberOfTurns()));
    ui->lineEdit_majorRadius->setText(QString::number(helixCmd->getMajorRadius()));
    ui->lineEdit_minorRadius->setText(QString::number(helixCmd->getMinorRadius()));
    ui->lineEdit_axialPitch->setText(QString::number(helixCmd->getAxialPitch()));
    ui->lineEdit_chirality->setText(QString::number(helixCmd->getChirality()));
}

void FCHelixInfoWidget::setDAGNode(FCGeometryDAGData* dagData, FCID nodeId, FCGeoModelHelix* displayHelixCmd)
{
    mHelixCmd = nullptr;
    mDagData = dagData;
    mNodeId = nodeId;
    if (mOwnDisplayHelixCmd && mDisplayHelixCmd) {
        delete mDisplayHelixCmd;
        mDisplayHelixCmd = nullptr;
        mOwnDisplayHelixCmd = false;
    }
    if (displayHelixCmd) {
        mDisplayHelixCmd = displayHelixCmd;
        mOwnDisplayHelixCmd = false;
    } else {
        mDisplayHelixCmd = nullptr;
    }
    if (!dagData || nodeId == FCID_INVALID) return;
    FCGeoNode node = dagData->module()->tree()->node(nodeId);
    mNameManuallyEdited = false;
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_name->setText(node.name.isEmpty() ? QStringLiteral("Helix_%1").arg(static_cast<qulonglong>(nodeId)) : node.name);
    ui->lineEdit_name->blockSignals(false);
    double loc[3] = {
        node.params.value(QStringLiteral("location_x"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_y"), 0.0).toDouble(),
        node.params.value(QStringLiteral("location_z"), 0.0).toDouble()
    };
    int turns = static_cast<int>(node.params.value(QStringLiteral("number_of_turns"), 5.0).toDouble());
    double r1 = node.params.value(QStringLiteral("major_radius"), 30.0).toDouble();
    double r2 = node.params.value(QStringLiteral("minor_radius"), 5.0).toDouble();
    double pitch = node.params.value(QStringLiteral("axial_pitch"), 20.0).toDouble();
    int chir = static_cast<int>(node.params.value(QStringLiteral("chirality"), 1.0).toDouble());
    ui->lineEdit_locationX->setText(QString::number(loc[0]));
    ui->lineEdit_locationY->setText(QString::number(loc[1]));
    ui->lineEdit_locationZ->setText(QString::number(loc[2]));
    ui->lineEdit_numberOfTurns->setText(QString::number(turns));
    ui->lineEdit_majorRadius->setText(QString::number(r1));
    ui->lineEdit_minorRadius->setText(QString::number(r2));
    ui->lineEdit_axialPitch->setText(QString::number(pitch));
    ui->lineEdit_chirality->setText(QString::number(chir));
    if (!mDisplayHelixCmd) {
        FCGeoModelHelix* existingCmd = FCDataRepo::instance()->getDataAs<FCGeoModelHelix>(nodeId);
        if (existingCmd) {
            mDisplayHelixCmd = existingCmd;
            mOwnDisplayHelixCmd = false;
        } else {
            FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
            if (factory) {
                FCGeoModelHelix* cmd = factory->createCommandT<FCGeoModelHelix>(FCGeoEnum::FGTHelix);
                if (cmd) {
                    cmd->setLocation(loc);
                    cmd->setNumberOfTurns(turns);
                    cmd->setMajorRadius(r1);
                    cmd->setMinorRadius(r2);
                    cmd->setAxialPitch(pitch);
                    cmd->setChirality(chir);
                    if (cmd->update()) {
                        mDisplayHelixCmd = cmd;
                        mOwnDisplayHelixCmd = true;
                    } else {
                        delete cmd;
                    }
                }
            }
        }
    }
}

void FCHelixInfoWidget::syncToModel()
{
    syncValuesToModel();
}

FCAbsGeoCommand* FCHelixInfoWidget::getCurrentBuildCommand()
{
    return mDisplayHelixCmd ? static_cast<FCAbsGeoCommand*>(mDisplayHelixCmd) : static_cast<FCAbsGeoCommand*>(mHelixCmd);
}

void FCHelixInfoWidget::syncValuesToModel()
{
    if (mDagData && mNodeId != FCID_INVALID) {
        FCGeoParamSet params;
        params[QStringLiteral("location_x")] = ui->lineEdit_locationX->text().toDouble();
        params[QStringLiteral("location_y")] = ui->lineEdit_locationY->text().toDouble();
        params[QStringLiteral("location_z")] = ui->lineEdit_locationZ->text().toDouble();
        params[QStringLiteral("number_of_turns")] = ui->lineEdit_numberOfTurns->text().toDouble();
        params[QStringLiteral("major_radius")] = ui->lineEdit_majorRadius->text().toDouble();
        params[QStringLiteral("minor_radius")] = ui->lineEdit_minorRadius->text().toDouble();
        params[QStringLiteral("axial_pitch")] = ui->lineEdit_axialPitch->text().toDouble();
        params[QStringLiteral("chirality")] = ui->lineEdit_chirality->text().toDouble();
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
        if (mDisplayHelixCmd) {
            double loc[3] = { params[QStringLiteral("location_x")].toDouble(), params[QStringLiteral("location_y")].toDouble(), params[QStringLiteral("location_z")].toDouble() };
            mDisplayHelixCmd->setLocation(loc);
            mDisplayHelixCmd->setNumberOfTurns(static_cast<int>(params[QStringLiteral("number_of_turns")].toDouble()));
            mDisplayHelixCmd->setMajorRadius(params[QStringLiteral("major_radius")].toDouble());
            mDisplayHelixCmd->setMinorRadius(params[QStringLiteral("minor_radius")].toDouble());
            mDisplayHelixCmd->setAxialPitch(params[QStringLiteral("axial_pitch")].toDouble());
            mDisplayHelixCmd->setChirality(static_cast<int>(params[QStringLiteral("chirality")].toDouble()));
            mDisplayHelixCmd->update();
        }
        return;
    }
    if (!mHelixCmd) return;
    double loc[3] = {
        ui->lineEdit_locationX->text().toDouble(),
        ui->lineEdit_locationY->text().toDouble(),
        ui->lineEdit_locationZ->text().toDouble()
    };
    if (mNameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty()) {
            mHelixCmd->setDataObjectName(newName);
            mNameManuallyEdited = false;
        }
    }
    mHelixCmd->setLocation(loc);
    mHelixCmd->setNumberOfTurns(ui->lineEdit_numberOfTurns->text().toInt());
    mHelixCmd->setMajorRadius(ui->lineEdit_majorRadius->text().toDouble());
    mHelixCmd->setMinorRadius(ui->lineEdit_minorRadius->text().toDouble());
    mHelixCmd->setAxialPitch(ui->lineEdit_axialPitch->text().toDouble());
    mHelixCmd->setChirality(ui->lineEdit_chirality->text().toInt());
    mHelixCmd->update();
    rebuildGeometryEntityModel();
}

void FCHelixInfoWidget::executeBuild()
{
    if (mDagData && mNodeId != FCID_INVALID) {
        syncValuesToModel();
        if (mDisplayHelixCmd) {
            if (mDisplayHelixCmd->update())
                emit geometryBuilt(mDisplayHelixCmd);
        } else {
            emit geometryBuilt(nullptr);
        }
        return;
    }
    if (!mHelixCmd) {
        qWarning() << "FCHelixInfoWidget: no helix command or DAG node to build";
        return;
    }
    syncValuesToModel();
    if (!mHelixCmd->update()) {
        qWarning() << "FCHelixInfoWidget: helixCmd->update() failed";
        return;
    }
    emit geometryBuilt(mHelixCmd);
}

void FCHelixInfoWidget::executeBuildAll()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    FCGeometryDAGData* dagData = globalData ? globalData->getData<FCGeometryDAGData>(GDTGeom) : mDagData;
    if (!dagData) return;
    dagData->ensureBuild();
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(QList<FCAbsGeoCommand*>());
}

void FCHelixInfoWidget::rebuildGeometryEntityModel()
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
        qWarning() << "FCHelixInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
}

} // namespace FC
