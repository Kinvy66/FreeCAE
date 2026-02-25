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
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, [this]() { m_nameManuallyEdited = true; });
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
    if (m_ownDisplayBoxCmd && m_displayBoxCmd) {
        delete m_displayBoxCmd;
        m_displayBoxCmd = nullptr;
    }
    delete ui;
}

void FCCubeInfoWidget::setBoxCommand(FCGeoModelBox* boxCmd)
{
    m_dagData = nullptr;
    m_nodeId = -1;
    if (m_ownDisplayBoxCmd && m_displayBoxCmd) {
        delete m_displayBoxCmd;
        m_displayBoxCmd = nullptr;
        m_ownDisplayBoxCmd = false;
    } else {
        m_displayBoxCmd = nullptr;
    }
    m_boxCmd = boxCmd;
    if (!boxCmd) return;
    m_nameManuallyEdited = false;
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
    m_boxCmd = nullptr;
    m_dagData = dagData;
    m_nodeId = nodeId;
    if (m_ownDisplayBoxCmd && m_displayBoxCmd) {
        delete m_displayBoxCmd;
        m_displayBoxCmd = nullptr;
        m_ownDisplayBoxCmd = false;
    }
    if (displayBoxCmd) {
        m_displayBoxCmd = displayBoxCmd;
        m_ownDisplayBoxCmd = false;
    } else {
        m_displayBoxCmd = nullptr;
    }
    if (!dagData || nodeId < 0) return;
    FCGeoNode node = dagData->module()->tree()->node(nodeId);
    m_nameManuallyEdited = false;
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
    if (!m_displayBoxCmd) {
        FCGeoInterfaceFactory* factory = FCGeoInterfaceFactory::instance();
        if (factory) {
            FCGeoModelBox* boxCmd = factory->createCommandT<FCGeoModelBox>(FCGeoEnum::FGTBox);
            if (boxCmd) {
                double p[3] = { 0.0, 0.0, 0.0 };
                double length[3] = { l0, l1, l2 };
                boxCmd->setPoint1(p);
                boxCmd->setLength(length);
                if (boxCmd->update()) {
                    m_displayBoxCmd = boxCmd;
                    m_ownDisplayBoxCmd = true;
                } else {
                    delete boxCmd;
                }
            }
        }
    }
}

void FCCubeInfoWidget::syncValuesToModel()
{
    if (m_dagData && m_nodeId >= 0) {
        double len[3] = {
            ui->lineEdit_length1->text().toDouble(),
            ui->lineEdit_length2->text().toDouble(),
            ui->lineEdit_length3->text().toDouble()
        };
        FCGeoParamSet params;
        params[QStringLiteral("length")] = len[0];
        params[QStringLiteral("width")]  = len[1];
        params[QStringLiteral("height")] = len[2];
        FCGeometryTree* tree = m_dagData->module()->tree();
        FCGeoNode node = tree->node(m_nodeId);
        if (m_nameManuallyEdited) {
            QString newName = ui->lineEdit_name->text().trimmed();
            if (!newName.isEmpty()) {
                node.name = tree->checkName(newName);
                tree->setNode(m_nodeId, node);
                ui->lineEdit_name->blockSignals(true);
                ui->lineEdit_name->setText(node.name);
                ui->lineEdit_name->blockSignals(false);
                m_nameManuallyEdited = false;
                m_dagData->setDirty(true);
            }
        }
        m_dagData->module()->updateNode(m_nodeId, params);
        m_dagData->setDirty(true);
        m_dagData->ensureBuild();
        rebuildGeometryEntityModel();
        if (m_displayBoxCmd) {
            double p[3] = { 0.0, 0.0, 0.0 };
            m_displayBoxCmd->setPoint1(p);
            m_displayBoxCmd->setLength(len);
            m_displayBoxCmd->update();
        }
        return;
    }
    if (!m_boxCmd) return;
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
    if (m_nameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty()) {
            m_boxCmd->setDataObjectName(newName);
            m_nameManuallyEdited = false;
        }
    }
    m_boxCmd->setPoint1(p);
    m_boxCmd->setLength(len);
    m_boxCmd->update();
    rebuildGeometryEntityModel();
}

void FCCubeInfoWidget::executeBuild()
{
    if (m_dagData && m_nodeId >= 0) {
        double len[3] = {
            ui->lineEdit_length1->text().toDouble(),
            ui->lineEdit_length2->text().toDouble(),
            ui->lineEdit_length3->text().toDouble()
        };
        FCGeoParamSet params;
        params[QStringLiteral("length")] = len[0];
        params[QStringLiteral("width")]  = len[1];
        params[QStringLiteral("height")] = len[2];
        FCGeometryTree* tree = m_dagData->module()->tree();
        FCGeoNode node = tree->node(m_nodeId);
        if (m_nameManuallyEdited) {
            QString newName = ui->lineEdit_name->text().trimmed();
            if (!newName.isEmpty()) {
                node.name = tree->checkName(newName);
                tree->setNode(m_nodeId, node);
                ui->lineEdit_name->blockSignals(true);
                ui->lineEdit_name->setText(node.name);
                ui->lineEdit_name->blockSignals(false);
                m_nameManuallyEdited = false;
                m_dagData->setDirty(true);
            }
        }
        m_dagData->module()->updateNode(m_nodeId, params);
        m_dagData->setDirty(true);
        m_dagData->ensureBuild();
        rebuildGeometryEntityModel();
        // 用显示用 Box 命令驱动 VTK 显示（DAG 执行器未接入时仍能显示）
        if (m_displayBoxCmd) {
            double p[3] = { 0.0, 0.0, 0.0 };
            m_displayBoxCmd->setPoint1(p);
            m_displayBoxCmd->setLength(len);
            if (m_displayBoxCmd->update())
                emit geometryBuilt(m_displayBoxCmd);
        } else {
            emit geometryBuilt(nullptr);
        }
        return;
    }
    if (!m_boxCmd) {
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
    if (m_nameManuallyEdited) {
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty())
            m_boxCmd->setDataObjectName(newName);
    }
    m_boxCmd->setPoint1(p);
    m_boxCmd->setLength(len);
    if (!m_boxCmd->update()) {
        qWarning() << "FCCubeInfoWidget: boxCmd->update() failed";
        return;
    }
    rebuildGeometryEntityModel();
    emit geometryBuilt(m_boxCmd);
}

void FCCubeInfoWidget::executeBuildAll()
{
    FCGlobalData* globalData = FCGlobalData::getGlobalData();
    FCGeometryDAGData* dagData = globalData ? globalData->getData<FCGeometryDAGData>(GDTGeom) : m_dagData;
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
    FCGeometryDAGData* dagData = m_dagData ? m_dagData : globalData->getData<FCGeometryDAGData>(GDTGeom);
    if (!dagData) return;
    dagData->ensureBuild();
    FCGeometryEntityBuilder builder;
    if (!builder.rebuild(dagData->getGlobalGeoCompManager(), entityModel))
        qWarning() << "FCCubeInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
}

} // namespace FC
