#include "FCCubeInfoWidget.h"
#include "ui_FCCubeInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryEntity/FCGeometryDAGData.h>
#include <FCGeometryEntity/FCGeometryEntityBuilder.h>
#include <FCGeometryEntity/FCGeometryTree.h>
#include <FCGeometryEntity/FCGeometryEntityModel.h>
#include <FCGeometryEntity/FCGeoNode.h>
#include <FCData/FCGlobalData.h>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

FCCubeInfoWidget::FCCubeInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FCCubeInfoWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_build, &QPushButton::clicked, this, &FCCubeInfoWidget::onBuildClicked);
    connect(ui->pushButton_buildAll, &QPushButton::clicked, this, &FCCubeInfoWidget::onBuildAllClicked);
}

FCCubeInfoWidget::~FCCubeInfoWidget()
{
    delete ui;
}

void FCCubeInfoWidget::setBoxCommand(FC::FCGeoModelBox* boxCmd)
{
    m_dagData = nullptr;
    m_nodeId = -1;
    m_displayBoxCmd = nullptr;
    m_boxCmd = boxCmd;
    if (!boxCmd) return;
    ui->lineEdit_name->setText(boxCmd->getDataObjectName());
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

void FCCubeInfoWidget::setDAGNode(FC::FCGeometryDAGData* dagData, int nodeId, FC::FCGeoModelBox* displayBoxCmd)
{
    m_boxCmd = nullptr;
    m_dagData = dagData;
    m_nodeId = nodeId;
    m_displayBoxCmd = displayBoxCmd;
    if (!dagData || nodeId < 0) return;
    FC::FCGeoNode node = dagData->module()->tree()->node(nodeId);
    ui->lineEdit_name->setText(node.name.isEmpty() ? QStringLiteral("Box_%1").arg(nodeId) : node.name);
    QVariant len = node.params.value(QStringLiteral("length"));
    QVariant w   = node.params.value(QStringLiteral("width"));
    QVariant h   = node.params.value(QStringLiteral("height"));
    ui->lineEdit_pointX->setText(QStringLiteral("0"));
    ui->lineEdit_pointY->setText(QStringLiteral("0"));
    ui->lineEdit_pointZ->setText(QStringLiteral("0"));
    ui->lineEdit_length1->setText(len.isValid() ? QString::number(len.toDouble()) : QStringLiteral("100"));
    ui->lineEdit_length2->setText(w.isValid()   ? QString::number(w.toDouble())   : QStringLiteral("100"));
    ui->lineEdit_length3->setText(h.isValid()   ? QString::number(h.toDouble())   : QStringLiteral("100"));
}

void FCCubeInfoWidget::onBuildClicked()
{
    if (m_dagData && m_nodeId >= 0) {
        double len[3] = {
            ui->lineEdit_length1->text().toDouble(),
            ui->lineEdit_length2->text().toDouble(),
            ui->lineEdit_length3->text().toDouble()
        };
        FC::FCGeoParamSet params;
        params[QStringLiteral("length")] = len[0];
        params[QStringLiteral("width")]  = len[1];
        params[QStringLiteral("height")] = len[2];
        QString newName = ui->lineEdit_name->text().trimmed();
        if (!newName.isEmpty()) {
            FC::FCGeometryTree* tree = m_dagData->module()->tree();
            FC::FCGeoNode node = tree->node(m_nodeId);
            node.name = tree->checkName(newName);
            tree->setNode(m_nodeId, node);
            ui->lineEdit_name->setText(node.name);
        }
        m_dagData->module()->updateNode(m_nodeId, params);
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
    QString newName = ui->lineEdit_name->text().trimmed();
    if (!newName.isEmpty())
        m_boxCmd->setDataObjectName(newName);
    m_boxCmd->setPoint1(p);
    m_boxCmd->setLength(len);
    if (!m_boxCmd->update()) {
        qWarning() << "FCCubeInfoWidget: boxCmd->update() failed";
        return;
    }
    rebuildGeometryEntityModel();
    emit geometryBuilt(m_boxCmd);
}

void FCCubeInfoWidget::onBuildAllClicked()
{
    if (m_dagData) {
        m_dagData->ensureBuild();
        rebuildGeometryEntityModel();
        emit geometrySequenceBuilt(QList<FC::FCAbsGeoCommand*>());
        return;
    }
    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    FC::FCGeoCommandList* geoList = globalData ? globalData->getData<FC::FCGeoCommandList>(FC::GDTGeom) : nullptr;
    if (!geoList) return;
    QList<FC::FCAbsGeoCommand*> rootCmds = geoList->getRootCommandList();
    int currentIndex = -1;
    for (int i = 0; i < rootCmds.size(); ++i) {
        if (rootCmds.at(i) == m_boxCmd) {
            currentIndex = i;
            break;
        }
    }
    if (currentIndex < 0)
        currentIndex = rootCmds.size() - 1;
    QList<FC::FCAbsGeoCommand*> builtCmds;
    for (int i = 0; i <= currentIndex && i < rootCmds.size(); ++i) {
        FC::FCAbsGeoCommand* cmd = rootCmds.at(i);
        if (cmd) {
            cmd->update();
            builtCmds.append(cmd);
        }
    }
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(builtCmds);
}

void FCCubeInfoWidget::rebuildGeometryEntityModel()
{
    FC::FCGlobalData* globalData = FC::FCGlobalData::getGlobalData();
    if (!globalData) return;
    FC::FCGeometryEntityModel* entityModel = globalData->getData<FC::FCGeometryEntityModel>(FC::GDTGeomEntity);
    if (!entityModel) return;
    FC::FCGeometryDAGData* dagData = m_dagData ? m_dagData : globalData->getData<FC::FCGeometryDAGData>(FC::GDTGeom);
    if (dagData) {
        dagData->ensureBuild();
        FC::FCGeometryEntityBuilder builder;
        if (!builder.rebuild(dagData->getGlobalGeoCompManager(), entityModel))
            qWarning() << "FCCubeInfoWidget: Failed to rebuild FCGeometryEntityModel from DAG";
        return;
    }
    FC::FCGeoCommandList* geoList = globalData->getData<FC::FCGeoCommandList>(FC::GDTGeom);
    if (!geoList) return;
    FC::FCGeometryEntityBuilder builder;
    if (!builder.rebuild(geoList, entityModel))
        qWarning() << "FCCubeInfoWidget: Failed to rebuild FCGeometryEntityModel";
}
